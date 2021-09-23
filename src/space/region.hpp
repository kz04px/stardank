#ifndef space_region_hpp
#define space_region_hpp

#include <vector>

namespace space {

struct System {
    enum class Status
    {
        Available = 0,
        Owned,
    };

    Status status = Status::Available;
    float x = 0.0f;
    float y = 0.0f;
    int owner = 0;
    float star_field_density = 0.0f;
};

struct Region {
    float age = 0.0f;
    std::vector<System> systems = {};
};

}  // namespace space

#endif
