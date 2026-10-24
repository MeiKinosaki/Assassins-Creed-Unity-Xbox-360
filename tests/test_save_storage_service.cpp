#include "../src/save/save_storage_service.h"
#include <cassert>
#include <map>
#include <string>
#include <vector>

namespace {
class MemoryStorage : public acunity360::IStorage {
public:
    std::map<std::string, std::vector<std::uint8_t> > files;
    bool fail_read = false;
    bool fail_write = false;

    bool readAll(const std::string& path, std::vector<std::uint8_t>& out) override {
        if (fail_read) return false;
        std::map<std::string, std::vector<std::uint8_t> >::const_iterator it = files.find(path);
        if (it == files.end()) return false;
        out = it->second;
        return true;
    }
    bool writeAll(const std::string& path, const std::vector<std::uint8_t>& data) override {
        if (fail_write) return false;
        files[path] = data;
        return true;
    }
};
}

int main() {
    MemoryStorage storage;
    acunity360::SaveSnapshot source;
    source.objective_id = 17u;
    source.objective_progress = 2u;
    source.objective_required = 5u;
    source.items.push_back(acunity360::SaveItem{42u, 3u});

    assert(!acunity360::SaveStorageService::save(source, storage, ""));
    assert(acunity360::SaveStorageService::save(source, storage, "slot1"));

    acunity360::SaveSnapshot loaded;
    loaded.objective_id = 99u;
    assert(acunity360::SaveStorageService::load(loaded, storage, "slot1"));
    assert(loaded.objective_id == 17u);
    assert(loaded.objective_progress == 2u);
    assert(loaded.items.size() == 1u);
    assert(loaded.items[0].item_id == 42u);

    acunity360::SaveSnapshot preserved;
    preserved.objective_id = 88u;
    assert(!acunity360::SaveStorageService::load(preserved, storage, "missing"));
    assert(preserved.objective_id == 88u);

    storage.files["broken"] = std::vector<std::uint8_t>{1u, 2u, 3u};
    assert(!acunity360::SaveStorageService::load(preserved, storage, "broken"));
    assert(preserved.objective_id == 88u);

    storage.fail_write = true;
    assert(!acunity360::SaveStorageService::save(source, storage, "slot2"));
    storage.fail_write = false;
    storage.fail_read = true;
    assert(!acunity360::SaveStorageService::load(preserved, storage, "slot1"));
    return 0;
}
