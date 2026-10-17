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

    std::vector<std::uint8_t> corrupt = bytes;
    corrupt[8] ^= 1u;
    assert(!acunity360::SaveDataCodec::decode(corrupt, decoded));

    std::vector<std::uint8_t> truncated = bytes;
    truncated.pop_back();
    assert(!acunity360::SaveDataCodec::decode(truncated, decoded));

    acunity360::SaveSnapshot invalid;
    invalid.items.push_back(acunity360::SaveItem{0u, 1u});
    assert(!acunity360::SaveDataCodec::encode(invalid, bytes));
    return 0;
}
