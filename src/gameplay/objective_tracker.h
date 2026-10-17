#pragma once

#include <cstdint>

namespace acunity360 {

struct ObjectiveProgress {
    std::uint32_t objective_id;
    std::uint32_t current;
    std::uint32_t required;
    bool completed;
};

class ObjectiveTracker {
public:
    ObjectiveTracker() : progress_{0u, 0u, 0u, false}, configured_(false) {}

    bool configure(std::uint32_t objective_id, std::uint32_t required) {
        if (objective_id == 0u || required == 0u) return false;
        progress_ = ObjectiveProgress{objective_id, 0u, required, false};
        configured_ = true;
        return true;
    }

    bool advance(std::uint32_t objective_id, std::uint32_t amount) {
        if (!configured_ || progress_.completed || objective_id != progress_.objective_id || amount == 0u) {
            return false;
        }
        const std::uint32_t remaining = progress_.required - progress_.current;
        progress_.current += amount > remaining ? remaining : amount;
        progress_.completed = progress_.current >= progress_.required;
        return true;
    }

    const ObjectiveProgress& current() const { return progress_; }
    bool configured() const { return configured_; }

private:
    ObjectiveProgress progress_;
    bool configured_;
};

} // namespace acunity360
