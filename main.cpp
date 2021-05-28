#include <cstdio>
#include <string>
#include "vector"
#include "memory_manager.h"
#include "csgo.h"
#include "entity.h"
#include "local_player.h"

int main(int argc, char* argv[]) {
    char process_name[] = "csgo.exe";
    char client_lib[] = "client.dll";
    char engine_lib[] = "engine.dll";
    bool do_glow = true;
    bool do_aim = true;
    memory_manager mem(process_name, client_lib, engine_lib);
    auto local_player_address = mem.read<std::uintptr_t>(mem.get_client() + hazedumper::signatures::dwLocalPlayer);
    auto local_state_address = mem.read<std::uintptr_t>(mem.get_engine() + hazedumper::signatures::dwClientState);
    local_player lp = local_player(local_state_address, local_player_address, &mem);
    entity remote_players[64];
    while (true) {
        if (GetAsyncKeyState(VK_SPACE) && lp.on_ground()) {
            mem.write<DWORD>(mem.get_client() + hazedumper::signatures::dwForceJump, 6);
        }
        auto glow_object_address = mem.read<std::uintptr_t>( mem.get_client() + hazedumper::signatures::dwGlowObjectManager);
        lp.update();
        entity *closest_player = &lp;
        double min_distance = 1000000000;
        int numPlayer =0;
        for (int i = 0; i < 64; i++) {
            entity &player = remote_players[i];
            auto remote_player_address = mem.read<uintptr_t>( mem.get_client() + hazedumper::signatures::dwEntityList + i * 0x10);
            if (!remote_player_address) continue;
            numPlayer++;
            player = entity(remote_player_address, &mem);
            player.update();
            player.set_spotted();
            if(do_glow && player.team != lp.team){
                player.glow(glow_object_address, 2, 0, 2);
            }
            float dist = lp.angle_distance(player);
            if (player.team != lp.team && player.health > 0 && player != lp &&
                dist < min_distance && dist < 2) {
                min_distance = dist;
                closest_player = &player;
            }
        }
        if(GetAsyncKeyState(VK_F7)){
            std::cout << "Glow ON" << std::endl;
            do_glow = true;
        }
        if(GetAsyncKeyState(VK_F8)){
            std::cout << "Glow OFF" << std::endl;
            do_glow = false;
        }
        if(GetAsyncKeyState(VK_F9)){
            std::cout << "Aim ON" << std::endl;
            do_aim = true;
        }
        if(GetAsyncKeyState(VK_F10)){
            std::cout << "Aim OFF" << std::endl;
            do_aim = false;
        }
        if (do_aim && GetAsyncKeyState(VK_MENU) && GetKeyState(VK_LBUTTON) < 0 && closest_player != &lp) {
            float old_yaw = lp.view_angles.y;
            float old_pitch = lp.view_angles.z;
            vector3 new_angles = lp.get_required_angles(*closest_player);
            float yaw = new_angles.y;
            float pitch = new_angles.x;
            if (GetAsyncKeyState(VK_CONTROL)) {
                pitch -= 2.0 * lp.aim_punch;
            }
            lp.set_pitch(pitch);
            lp.set_yaw(yaw);
            mem.write<DWORD>(mem.get_client() + hazedumper::signatures::dwForceAttack, 6);
//            Sleep(3);
//            lp.set_pitch(old_pitch);
//            lp.set_yaw(old_yaw);
        }


    }
    return 0;
}