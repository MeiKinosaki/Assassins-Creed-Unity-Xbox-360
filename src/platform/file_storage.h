#pragma once

#include "platform_services.h"
#include "../save/save_data_codec.h"

#include <cstdio>
#include <fstream>
#include <limits>
#include <vector>

namespace acunity360 {

// Desktop/reference backend for integration tests. This is not an Xbox 360
// save-game implementation; the console must provide its own IStorage backend.
class FileStorage : public IStorage {
public:
    bool readAll(const std::string& path, std::vector<std::uint8_t>& out) override {
        if (path.empty()) return false;
        std::ifstream file(path.c_str(), std::ios::binary | std::ios::ate);
        if (!file) return false;

        const std::ifstream::pos_type end = file.tellg();
        if (end < std::ifstream::pos_type(0)) return false;
        const std::uint64_t size = static_cast<std::uint64_t>(end);
        if (size > SaveDataCodec::kMaxEncodedSize ||
            size > static_cast<std::uint64_t>((std::numeric_limits<std::size_t>::max)())) {
            return false;
        }

        std::vector<std::uint8_t> candidate(static_cast<std::size_t>(size));
        file.seekg(0, std::ios::beg);
        if (!file) return false;
        if (!candidate.empty()) {
            file.read(reinterpret_cast<char*>(&candidate[0]),
                      static_cast<std::streamsize>(candidate.size()));
            if (!file || file.gcount() != static_cast<std::streamsize>(candidate.size())) {
                return false;
            }
        }
        out.swap(candidate);
        return true;
    }

    bool writeAll(const std::string& path, const std::vector<std::uint8_t>& data) override {
        if (path.empty() || data.size() > SaveDataCodec::kMaxEncodedSize) return false;

        // Stage the full payload before replacing the destination. Filesystems
        // with atomic rename replacement avoid exposing a partial save.
        const std::string temporary_path = path + ".tmp";
        {
            std::ofstream file(temporary_path.c_str(), std::ios::binary | std::ios::trunc);
            if (!file) return false;
            if (!data.empty()) {
                file.write(reinterpret_cast<const char*>(&data[0]),
                           static_cast<std::streamsize>(data.size()));
            }
            file.flush();
            if (!file) {
                file.close();
                std::remove(temporary_path.c_str());
                return false;
            }
            file.close();
            if (!file) {
                std::remove(temporary_path.c_str());
                return false;
            }
        }

        if (std::rename(temporary_path.c_str(), path.c_str()) != 0) {
            std::remove(temporary_path.c_str());
            return false;
        }
        return true;
    }
};

} // namespace acunity360
