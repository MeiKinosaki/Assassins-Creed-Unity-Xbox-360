#include "../src/platform/file_storage.h"
#include "../src/save/save_storage_service.h"

#include <cassert>
#include <cstdio>
#include <fstream>
#include <string>
#include <vector>

int main() {
    const std::string path = "acunity360_file_storage_test.sav";
    acunity360::FileStorage storage;

    acunity360::SaveSnapshot source;
    source.objective_id = 3u;
    source.objective_progress = 1u;
    source.objective_required = 4u;
    source.items.push_back(acunity360::SaveItem{11u, 2u});

    assert(acunity360::SaveStorageService::save(source, storage, path));
    acunity360::SaveSnapshot loaded;
    loaded.objective_id = 99u;
    assert(acunity360::SaveStorageService::load(loaded, storage, path));
    assert(loaded.objective_id == 3u);
    assert(loaded.objective_progress == 1u);
    assert(loaded.items.size() == 1u && loaded.items[0].quantity == 2u);

    const std::vector<std::uint8_t> oversized(
        acunity360::SaveDataCodec::kMaxEncodedSize + 1u, 1u);
    assert(!storage.writeAll(path, oversized));
    assert(acunity360::SaveStorageService::load(loaded, storage, path));
    assert(loaded.objective_id == 3u);

    std::vector<std::uint8_t> unchanged(1u, 77u);
    assert(!storage.readAll("", unchanged));
    assert(unchanged.size() == 1u && unchanged[0] == 77u);

    {
        std::ofstream file(path.c_str(), std::ios::binary | std::ios::trunc);
        assert(file);
        const std::vector<std::uint8_t> invalid(3u, 0u);
        file.write(reinterpret_cast<const char*>(&invalid[0]),
                   static_cast<std::streamsize>(invalid.size()));
    }
    assert(!acunity360::SaveStorageService::load(loaded, storage, path));
    assert(loaded.objective_id == 3u);

    {
        std::ofstream file(path.c_str(), std::ios::binary | std::ios::trunc);
        assert(file);
        const std::vector<std::uint8_t> too_large(
            acunity360::SaveDataCodec::kMaxEncodedSize + 1u, 2u);
        file.write(reinterpret_cast<const char*>(&too_large[0]),
                   static_cast<std::streamsize>(too_large.size()));
    }
    unchanged.assign(1u, 77u);
    assert(!storage.readAll(path, unchanged));
    assert(unchanged.size() == 1u && unchanged[0] == 77u);

    assert(std::remove(path.c_str()) == 0);
    assert(!storage.readAll(path, unchanged));
    return 0;
}
