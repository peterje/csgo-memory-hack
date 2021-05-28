//
// Created by Peter on 5/3/2021.
//

#include "local_player.h"

local_player::local_player(uintptr_t state_offset, uintptr_t player_offset, memory_manager* mem) : entity(player_offset,mem){
    this->local_state_address = state_offset;
}

void local_player::aim_at(const entity& target) {
    vector3 required_angles = get_required_angles(target);
    std::cout << std::endl;
    std::cout << required_angles.x << std::endl;
    std::cout << required_angles.y << std::endl;
    std::cout << std::endl;
    set_pitch(required_angles.x);
    set_yaw(required_angles.y);
}

vector3 local_player::get_required_angles(const entity& target) const{
    vector3 my_head = target.get_bone(HEAD);
    vector3 player_head = this->origin + this->view_offset;
    vector3 delta = my_head - player_head;
    float height_diff = delta.z;
    float required_yaw = atan2(delta.y, delta.x) * (180.0f / 3.14159265);
    float required_pitch = -asin(height_diff / delta.magnitude()) * (180.0f / 3.14159265);
    return vector3{required_pitch, required_yaw, 0};
}

float local_player::angle_distance(const entity& target) const {
    vector3 required_angles = get_required_angles(target);
    float current_pitch = this->view_angles.x;
    float current_yaw = this->view_angles.y;
    return sqrtf(pow(required_angles.x - current_pitch, 2) + pow(required_angles.y - current_yaw, 2));
}
void local_player::update() {
    entity::update();
    this->view_angles = mem->read<vector3>(this->local_state_address + hazedumper::signatures::dwClientState_ViewAngles);
}

void local_player::set_pitch(float new_pitch) {
    if(new_pitch > 89 || new_pitch < -89){
        return;
    }
    mem->write<float>(this->local_state_address + hazedumper::signatures::dwClientState_ViewAngles, new_pitch);
}

void local_player::set_yaw(float new_yaw) {
    if(new_yaw > 180 || new_yaw < -180){
        return;
    }
    mem->write<float>(this->local_state_address + hazedumper::signatures::dwClientState_ViewAngles + 0x4, new_yaw);
}

