#pragma once

#include "save_data_codec.h"
#include "../platform/platform_services.h"

#include <cstdint>
#include <string>
#include <vector>

namespace acunity360 {

// Storage bridge for the portable save codec. Concrete IStorage implementations
// must provide the real platform filesystem or console save-data integration.
class SaveStorageService {
public:
    static bool save(const SaveSnapshot& snapshot, IStorage& storage,
                     const std::string& path) {
        if (path.empty()) return false;
        std::vector<std::uint8_t> bytes;
        if (!SaveDataCodec::encode(snapshot, bytes)) return false;
        return storage.writeAll(path, bytes);
    }

    static bool load(SaveSnapshot& out, IStorage& storage,
                     const std::string& path) {
        if (path.empty()) return false;
        std::vector<std::uint8_t> bytes;
        if (!storage.readAll(path, bytes)) return false;
        SaveSnapshot candidate;
        if (!SaveDataCodec::decode(bytes, candidate)) return false;
        out = candidate;
        return true;
    }
};

} // namespace acunity360
