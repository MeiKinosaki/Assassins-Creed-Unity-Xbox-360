#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>
#include "../gameplay/equipment_system.h"

namespace acunity360 {

struct SaveItem {
    std::uint32_t item_id;
    std::uint32_t quantity;
};

struct SaveSnapshot {
    std::uint32_t objective_id;
    std::uint32_t objective_progress;
    std::uint32_t objective_required;
    std::vector<SaveItem> items;
    EquipmentLoadout equipment;

    SaveSnapshot() : objective_id(0), objective_progress(0), objective_required(0), items(), equipment() {}
};

class SaveDataCodec {
public:
    static const std::uint32_t kMagic = 0x30365541u;
    static const std::uint16_t kVersion = 2u;
    static const std::size_t kMaxItems = 256u;

    static bool encode(const SaveSnapshot& snapshot, std::vector<std::uint8_t>& out) {
        if (snapshot.items.size() > kMaxItems) return false;
        if ((snapshot.objective_id == 0u) != (snapshot.objective_required == 0u)) return false;
        if (snapshot.objective_progress > snapshot.objective_required) return false;
        for (std::size_t i = 0; i < snapshot.items.size(); ++i) {
            if (snapshot.items[i].item_id == 0u || snapshot.items[i].quantity == 0u) return false;
        }

        out.clear();
        put32(out, kMagic);
        put16(out, kVersion);
        put16(out, static_cast<std::uint16_t>(snapshot.items.size()));
        put32(out, snapshot.objective_id);
        put32(out, snapshot.objective_progress);
        put32(out, snapshot.objective_required);
        put32(out, snapshot.equipment.main_hand);
        put32(out, snapshot.equipment.tool);
        put32(out, snapshot.equipment.outfit);
        for (std::size_t i = 0; i < snapshot.items.size(); ++i) {
            put32(out, snapshot.items[i].item_id);
            put32(out, snapshot.items[i].quantity);
        }
        put32(out, checksum(out));
        return true;
    }

    static bool decode(const std::vector<std::uint8_t>& bytes, SaveSnapshot& out) {
        const std::size_t header_size = 4u + 2u + 2u + 4u + 4u + 4u + 4u + 4u + 4u;
        if (bytes.size() < header_size + 4u) return false;
        if (read32(bytes, 0u) != kMagic || read16(bytes, 4u) != kVersion) return false;
        const std::uint16_t count = read16(bytes, 6u);
        if (count > kMaxItems) return false;
        const std::size_t expected = header_size + static_cast<std::size_t>(count) * 8u + 4u;
        if (bytes.size() != expected) return false;
        if (read32(bytes, expected - 4u) != checksumRange(bytes, expected - 4u)) return false;

        SaveSnapshot candidate;
        candidate.objective_id = read32(bytes, 8u);
        candidate.objective_progress = read32(bytes, 12u);
        candidate.objective_required = read32(bytes, 16u);
        candidate.equipment.main_hand = read32(bytes, 20u);
        candidate.equipment.tool = read32(bytes, 24u);
        candidate.equipment.outfit = read32(bytes, 28u);
        if ((candidate.objective_id == 0u) != (candidate.objective_required == 0u)) return false;
        if (candidate.objective_progress > candidate.objective_required) return false;

        for (std::size_t i = 0; i < count; ++i) {
            const std::size_t offset = header_size + i * 8u;
            SaveItem item = { read32(bytes, offset), read32(bytes, offset + 4u) };
            if (item.item_id == 0u || item.quantity == 0u) return false;
            candidate.items.push_back(item);
        }
        out = candidate;
        return true;
    }

private:
    static void put16(std::vector<std::uint8_t>& out, std::uint16_t v) {
        out.push_back(static_cast<std::uint8_t>(v & 0xffu));
        out.push_back(static_cast<std::uint8_t>((v >> 8) & 0xffu));
    }
    static void put32(std::vector<std::uint8_t>& out, std::uint32_t v) {
        out.push_back(static_cast<std::uint8_t>(v & 0xffu));
        out.push_back(static_cast<std::uint8_t>((v >> 8) & 0xffu));
        out.push_back(static_cast<std::uint8_t>((v >> 16) & 0xffu));
        out.push_back(static_cast<std::uint8_t>((v >> 24) & 0xffu));
    }
    static std::uint16_t read16(const std::vector<std::uint8_t>& b, std::size_t p) {
        return static_cast<std::uint16_t>(b[p]) |
            static_cast<std::uint16_t>(static_cast<std::uint16_t>(b[p + 1u]) << 8);
    }
    static std::uint32_t read32(const std::vector<std::uint8_t>& b, std::size_t p) {
        return static_cast<std::uint32_t>(b[p]) |
            (static_cast<std::uint32_t>(b[p + 1u]) << 8) |
            (static_cast<std::uint32_t>(b[p + 2u]) << 16) |
            (static_cast<std::uint32_t>(b[p + 3u]) << 24);
    }
    static std::uint32_t checksumRange(const std::vector<std::uint8_t>& b, std::size_t length) {
        std::uint32_t hash = 2166136261u;
        for (std::size_t i = 0; i < length; ++i) {
            hash ^= b[i];
            hash *= 16777619u;
        }
        return hash;
    }
    static std::uint32_t checksum(const std::vector<std::uint8_t>& b) {
        return checksumRange(b, b.size());
    }
};

} // namespace acunity360
