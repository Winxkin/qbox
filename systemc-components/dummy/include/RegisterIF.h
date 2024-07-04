/*
 * Copyright (c) 2022 GreenSocs
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * The common design for register interface
 * Author: Huan Nguyen-Duy
 * date: 01/07/2024
 */
#ifndef _REGISTERIF_H
#define _REGISTERIF_H
#include <unordered_map>
#include <map>
#include <string>
#include <cstdint>
#include <stdexcept>
#include <memory>
#include <stdio.h>
#include <iostream>

class Register {
public:

    Register() {};
    Register(std::string name, uint64_t address, uint32_t init)
        : name(name), address(address), value(init) {};

    ~Register() {};

    uint32_t get_value() const { return value; };
    void set_value(uint32_t new_value) { value = new_value; };
    uint64_t get_address() { return address; };
    std::string get_name() { return name;};

    Register& operator=(uint32_t new_value) {
        value = new_value;
        return *this;
    }

private:
    std::string name;
    uint64_t address;
    uint32_t value;
};

// Class to manage a collection of registers
class RegisterInterface {
public:
    void add_register(std::string name, uint64_t address, uint32_t init) {
        registers.emplace(name, Register(name, address, init));
        printf("[Adding new register]   Register name: [%s], address [%lX], initialize value [%X] \n", registers[name].get_name().c_str(), registers[name].get_address(), registers[name].get_value());
    };

    Register& operator[](std::string name) {
        if (registers.find(name) == registers.end()) {
            throw std::runtime_error("Register not found");
        }
        return registers[name];
    };


    void update_register(uint64_t address, uint32_t value) {
        for (auto& reg : registers) {
            if (reg.second.get_address() == address) {
                reg.second.set_value(value);
                printf("Register %s change to value -> [%X] \n", reg.second.get_name().c_str(), reg.second.get_value() );
                return;
            }
        }
        throw std::runtime_error("Register with the given address not found");
    };

    void dump_registers()
    {
        for (auto& reg : registers) 
        {
            printf("[dump]  Register %s, address: [%lX] value: [%X] \n", reg.second.get_name().c_str(), reg.second.get_address() ,reg.second.get_value());
        }
        
    };

public:
    std::map<std::string, Register> registers;
};



#endif