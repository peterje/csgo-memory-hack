//
// Created by Peter on 5/1/2021.
//

#include "memory_manager.h"
memory_manager::memory_manager(char *process_name, char* client_module, char* engine_module) {
    try{
        this->process = get_process(process_name);
        this->client = get_module(client_module, &this->process);
        this->engine = get_module(engine_module, &this->process);
        this->process_handle =  OpenProcess(PROCESS_ALL_ACCESS, false, this->process.th32ProcessID);
    } catch (const std::exception &e) {
        printf("%s\n", e.what());
        exit(EXIT_FAILURE);
    }
}
PROCESSENTRY32 memory_manager::get_process(char *process_name) {
    PROCESSENTRY32 process;
    HANDLE process_snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if(process_snapshot == INVALID_HANDLE_VALUE){
        throw std::exception("Unable to create a snapshot of processes!");
    }
    process.dwSize = sizeof(process);
    if(!Process32First(process_snapshot, &process)){
        /* There are no processes running on the system...somehow*/
        CloseHandle(process_snapshot);
        throw std::exception("No processes to enumerate!");
    }
    while(strcmp(process.szExeFile, process_name) != 0){
        if(Process32Next(process_snapshot, &process) == 0){
            /* There are no more process to check...
             * and we did not find the right one!*/
            CloseHandle(process_snapshot);
            throw std::exception("Requested process not found!");
        }
    }
    printf("FOUND: %s\n", process.szExeFile);
    /* If we made it this far we should have the correct process*/
    CloseHandle(process_snapshot);
    return process;
}

MODULEENTRY32 memory_manager::get_module(char *module_name, LPPROCESSENTRY32 process) {
    MODULEENTRY32 module;
    HANDLE module_snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, process->th32ProcessID);
    if(module_snapshot == INVALID_HANDLE_VALUE){
        std::cout << GetLastError() << std::endl;
        throw std::exception("Unable to create a snapshot of modules!");
    }
    module.dwSize = sizeof(MODULEENTRY32);
    if(!Module32First(module_snapshot, &module)){
        /* There are no modules running in the process...somehow*/
        CloseHandle(module_snapshot);
        throw std::exception("No processes to enumerate!");
    }
    while(strcmp(module.szModule, module_name) != 0){
        if(Module32Next(module_snapshot, &module) == 0){
            /* There are no more modules to check...
             * and we did not find the right one!*/
            CloseHandle(module_snapshot);
            throw std::exception("Requested module not found!");
        }
    }
    /* If we made it this far we should have the correct modules*/
    printf("FOUND: %s\n", module.szModule);
    CloseHandle(module_snapshot);
    return module;
}


const PROCESSENTRY32 &memory_manager::getProcess() const {
    return process;
}

void memory_manager::setProcess(const PROCESSENTRY32 &process) {
    memory_manager::process = process;
}

const MODULEENTRY32 &memory_manager::getModule() const {
    return client;
}

void memory_manager::setModule(const MODULEENTRY32 &module) {
    memory_manager::client = module;
}

uintptr_t memory_manager::get_client() const {
    return (uintptr_t)this->client.modBaseAddr;
}

uintptr_t memory_manager::get_engine() const {
    return (uintptr_t)this->engine.modBaseAddr;
}
