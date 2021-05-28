//
// Created by Peter on 5/1/2021.
//

#ifndef HACK_MEMORY_MANAGER_H
#define HACK_MEMORY_MANAGER_H
#include <Windows.h>
#include <cstdio>
#include <iostream>
#include <TlHelp32.h>
#include <exception>
#include <memoryapi.h>
#include <processthreadsapi.h>


class memory_manager {
public:
    memory_manager(char* process_name, char* client_module, char* engine_module);
    uintptr_t get_client() const;
    template<typename T>
    T read(uintptr_t base_address) {
        T buffer;
        if(!ReadProcessMemory(this->process_handle, (LPCVOID) base_address, &buffer, sizeof(T), NULL)){
            std::cout << std::system_category().message(GetLastError());
            throw std::exception("RPM Failure!");
        }
        return buffer;
    }

    template<typename T>
    T write(uintptr_t base_address, T buffer) {
        if(!WriteProcessMemory(this->process_handle, (LPVOID) base_address, &buffer, sizeof(T), NULL)){
            std::cout << std::system_category().message(GetLastError());
            throw std::exception("WPM Failure!");
        }
        return buffer;
    }

    const PROCESSENTRY32 &getProcess() const;

    void setProcess(const PROCESSENTRY32 &process);

    const MODULEENTRY32 &getModule() const;

    void setModule(const MODULEENTRY32 &module);

    uintptr_t get_engine() const;

private:
    PROCESSENTRY32 process{};
    MODULEENTRY32 client{};
    HANDLE process_handle;
    PROCESSENTRY32 get_process(char* process_name);
    static MODULEENTRY32 get_module(char* module_name, LPPROCESSENTRY32 module);

    MODULEENTRY32 engine;

};


#endif //HACK_MEMORY_MANAGER_H
