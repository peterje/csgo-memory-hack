//
// Created by Peter on 5/3/2021.
//

#ifndef HACK_LOCAL_PLAYER_H
#define HACK_LOCAL_PLAYER_H
#include "entity.h"
#include "algorithm"

class local_player : public entity {
public:
    local_player(uintptr_t state_offset, uintptr_t player_offset, memory_manager *mem);
    vector3 view_angles{};
    void set_pitch(float new_pitch);
    void set_yaw(float new_yaw);
    void update() override;

    uintptr_t local_state_address;


    void aim_at(const entity *target);

    float angle_distance(const entity &target) const;

    vector3 get_required_angles(const entity &target) const;

    void aim_at(const entity &target);
};


#endif //HACK_LOCAL_PLAYER_H
