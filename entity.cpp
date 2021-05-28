//
// Created by Peter on 5/3/2021.
//

#include "entity.h"


entity::entity(uintptr_t offset, memory_manager* mem) {
    this->offset = offset;
    this->mem = mem;
}

bool entity::operator==(const entity& other) const{
    return this->offset == other.offset;
}


void entity::update() {
    this->health = mem->read<int32_t>(offset + hazedumper::netvars::m_iHealth);
    this->team = mem->read<int32_t>(offset + hazedumper::netvars::m_iTeamNum);
    this->view_offset = mem->read<vector3>(offset + hazedumper::netvars::m_vecViewOffset);
    this->origin = mem->read<vector3>(offset + hazedumper::netvars::m_vecOrigin);
    this->glow_index = mem->read<int32_t>(offset + hazedumper::netvars::m_iGlowIndex);
    this->flags = mem->read<int32_t>(offset + hazedumper::netvars::m_fFlags);
    this->aim_punch = mem->read<float>(offset + hazedumper::netvars::m_aimPunchAngle);
    this->spotted = mem->read<bool>(offset + hazedumper::netvars::m_bSpotted);
}

void entity::set_spotted(){
    mem->write<bool>(offset + hazedumper::netvars::m_bSpotted, true);
}

bool entity::on_ground() const{
    return this->flags & 1;
}
entity::entity() {
    this->offset = 0x0;
    this->mem = nullptr;
}

double entity::distance_to(entity other) const {
    return this->origin.distance(other.origin);
}

bool entity::operator!=(const entity &other) const {
    return this->offset != other.offset;
}

void entity::glow(uintptr_t glow_object_address, float r, float g, float b) {
    uintptr_t glow_color_address = glow_object_address + this->glow_index * 0x38;
    mem->write<float>(glow_color_address + 0x4, r);
    mem->write<float>(glow_color_address + 0x8, g);
    mem->write<float>(glow_color_address + 0xC, b);
    mem->write<float>(glow_color_address + 0x10, 2);
    // glow when hidden
    mem->write<bool>(glow_color_address + 0x24, true);
    // glow when visible
    mem->write<bool>(glow_color_address + 0x25, true);
}

void entity::draw_cham(uintptr_t glow_object_address, float r, float g, float b){
    uintptr_t glow_color_address = glow_object_address + this->glow_index * 0x38;
    mem->write<float>(glow_color_address + 0x4, r);
    mem->write<float>(glow_color_address + 0x8, g);
    mem->write<float>(glow_color_address + 0xC, b);
    mem->write<float>(glow_color_address + 0x10, 0.5);
    int render_r = 255;
    int render_g = 255;
    int render_b = 255;
    if(this->spotted) {
        render_r = r*100;
        render_g = g*100;
        render_b = b*100;

    }
    mem->write<int>(this->offset + hazedumper::netvars::m_clrRender + 0x0, render_r);
    mem->write<int>(this->offset + hazedumper::netvars::m_clrRender + 0x1, render_g);
    mem->write<int>(this->offset + hazedumper::netvars::m_clrRender + 0x2, render_b);

    mem->write<bool>(glow_color_address + 0x24, true);
    mem->write<bool>(glow_color_address + 0x25, true);

}

vector3 entity::get_bone(Bone bone) const{
    auto bone_base = mem->read<uintptr_t>(offset + hazedumper::netvars::m_dwBoneMatrix);
    auto x = mem->read<float>(bone_base + 0x30 * bone + 0x0C);
    auto y = mem->read<float>( bone_base+ 0x30 * bone + 0x1C);
    auto z = mem->read<float>( bone_base+ 0x30 * bone + 0x2C);
    return vector3{x,y,z};
}