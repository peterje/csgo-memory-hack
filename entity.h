//
// Created by Peter on 5/3/2021.
//

#ifndef HACK_ENTITY_H
#define HACK_ENTITY_H
enum Bone{
    HEAD=8,
    FOOT=2
};
#include "memory_manager.h"
#include "csgo.h"
#include "vector3.h"

#include <cstdint>

class entity {
private:
    int32_t flags;
protected:
    memory_manager* mem;
public:
    int32_t glow_index;
    int32_t health{};
    int32_t team{};
    vector3 view_offset{};
    vector3 origin{};
    vector3 get_bone(Bone bone) const;
    double distance_to(entity other) const;
    void glow(uintptr_t glow_object_address, float r, float g, float b);
    bool operator==(const entity& other) const;
    bool operator!=(const entity& other) const;
    entity(uintptr_t offset, memory_manager* mem);
    entity();

    virtual void update();

    bool on_ground() const;

    uintptr_t offset;
    float aim_punch;
    bool spotted;

    void draw_cham(uintptr_t glow_object_address, float r, float g, float b);

    void set_spotted();
};



#endif //HACK_ENTITY_H
