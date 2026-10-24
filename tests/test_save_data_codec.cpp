#include "../src/save/save_data_codec.h"
#include <cassert>
#include <vector>

int main() {
    acunity360::SaveSnapshot source;
    source.objective_id = 12u;
    source.objective_progress = 2u;
    source.objective_required = 5u;
    source.items.push_back(acunity360::SaveItem{7u, 4u});
    source.items.push_back(acunity360::SaveItem{9u, 1u});
    source.equipment.main_hand = 101u;
    source.equipment.tool = 202u;
    source.equipment.outfit = 303u;

    std::vector<std::uint8_t> bytes;
    assert(acunity360::SaveDataCodec::encode(source, bytes));
    assert(!bytes.empty());

    acunity360::SaveSnapshot decoded;
    assert(acunity360::SaveDataCodec::decode(bytes, decoded));
    assert(decoded.objective_id == 12u);
    assert(decoded.objective_progress == 2u);
    assert(decoded.objective_required == 5u);
    assert(decoded.items.size() == 2u);
    assert(decoded.items[0].item_id == 7u && decoded.items[0].quantity == 4u);
    assert(decoded.items[1].item_id == 9u && decoded.items[1].quantity == 1u);
    assert(decoded.equipment.main_hand == 101u);
    assert(decoded.equipment.tool == 202u);
    assert(decoded.equipment.outfit == 303u);

    std::vector<std::uint8_t> corrupt = bytes;
    corrupt[8] ^= 1u;
    const std::uint32_t preserved_objective_id = decoded.objective_id;
    const std::size_t preserved_item_count = decoded.items.size();
    assert(!acunity360::SaveDataCodec::decode(corrupt, decoded));
    assert(decoded.objective_id == preserved_objective_id);
    assert(decoded.items.size() == preserved_item_count);

    std::vector<std::uint8_t> truncated = bytes;
    truncated.pop_back();
    assert(!acunity360::SaveDataCodec::decode(truncated, decoded));

    acunity360::SaveSnapshot invalid;
    invalid.items.push_back(acunity360::SaveItem{0u, 1u});
    assert(!acunity360::SaveDataCodec::encode(invalid, bytes));

    invalid = acunity360::SaveSnapshot();
    invalid.objective_id = 1u;
    invalid.objective_required = 0u;
    assert(!acunity360::SaveDataCodec::encode(invalid, bytes));

    invalid = acunity360::SaveSnapshot();
    invalid.items.push_back(acunity360::SaveItem{7u, 1u});
    invalid.items.push_back(acunity360::SaveItem{7u, 2u});
    assert(!acunity360::SaveDataCodec::encode(invalid, bytes));

    std::vector<std::uint8_t> empty;
    assert(!acunity360::SaveDataCodec::decode(empty, decoded));
    std::vector<std::uint8_t> oversized(
        acunity360::SaveDataCodec::kMaxEncodedSize + 1u, 0u);
    assert(!acunity360::SaveDataCodec::decode(oversized, decoded));
    return 0;
}
