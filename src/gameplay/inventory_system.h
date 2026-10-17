#pragma once

#include <cstdint>
#include <vector>

namespace acunity360 {

struct InventoryStack {
    std::uint32_t item_id;
    std::uint32_t quantity;
};

class InventorySystem {
public:
    explicit InventorySystem(std::uint32_t capacity)
        : capacity_(capacity) {}

    bool add(std::uint32_t item_id, std::uint32_t quantity) {
        if (item_id == 0 || quantity == 0) return false;

        for (std::vector<InventoryStack>::iterator it = stacks_.begin();
             it != stacks_.end(); ++it) {
            if (it->item_id == item_id) {
                if (quantity > UINT32_MAX - it->quantity) return false;
                it->quantity += quantity;
                return true;
            }
        }

        if (stacks_.size() >= capacity_) return false;
        InventoryStack stack = { item_id, quantity };
        stacks_.push_back(stack);
        return true;
    }

    bool remove(std::uint32_t item_id, std::uint32_t quantity) {
        if (item_id == 0 || quantity == 0) return false;
        for (std::vector<InventoryStack>::iterator it = stacks_.begin();
             it != stacks_.end(); ++it) {
            if (it->item_id != item_id) continue;
            if (it->quantity < quantity) return false;
            it->quantity -= quantity;
            if (it->quantity == 0) stacks_.erase(it);
            return true;
        }
        return false;
    }

    std::uint32_t count(std::uint32_t item_id) const {
        for (std::vector<InventoryStack>::const_iterator it = stacks_.begin();
             it != stacks_.end(); ++it) {
            if (it->item_id == item_id) return it->quantity;
        }
        return 0;
    }

    std::uint32_t capacity() const { return capacity_; }
    std::size_t occupiedSlots() const { return stacks_.size(); }
    const std::vector<InventoryStack>& stacks() const { return stacks_; }

private:
    std::uint32_t capacity_;
    std::vector<InventoryStack> stacks_;
};

} // namespace acunity360
