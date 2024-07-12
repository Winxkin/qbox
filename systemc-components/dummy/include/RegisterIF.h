/*
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

    using Callback = std::function<void(const std::string&, uint32_t)>;

    Register() {};
    Register(std::string name, uint64_t address, uint32_t init)
        : name(name), address(address), value(init) {};

    ~Register() {};

    uint32_t get_value() const { return value; };
    void set_value(uint32_t new_value) { value = new_value; };
    uint64_t get_address() { return address; };
    std::string get_name() { return name; };

    Register& operator=(uint32_t new_value) {
        value = new_value;
        if (callback) {
            callback(name, value);
        }
        return *this;
    }

    void set_callback(Callback cb) {
        callback = cb;
    }

private:
    std::string name;
    uint64_t address;
    uint32_t value;
    Callback callback;
};

// Class to manage a collection of registers
class RegisterInterface {
public:
    void add_register(std::string name, uint64_t address, uint32_t init) {
        registers.emplace(name, Register(name, address, init));
        std::cout << "[Adding new register]   Register name: [" << registers[name].get_name()
            << "], address [" << std::hex << registers[name].get_address()
            << "], initialize value [" << std::hex << registers[name].get_value() << "]" << std::endl;
    };

    // The operator to get register by name for example this->reg[name]
    Register& operator[](std::string name) {
        if (registers.find(name) == registers.end()) {
            throw std::runtime_error("Register not found");
        }
        return registers[name];
    };

    // The operator to get register by address for example this->reg[addr]
    Register& operator[](uint64_t address)
    {
        for (auto& reg : registers) {
            if (reg.second.get_address() == address) {
                return registers[reg.first];
            }

        }
        throw std::runtime_error("Register with the given address not found");
    }


    void update_register(uint64_t address, uint32_t value) {
        for (auto& reg : registers) {
            if (reg.second.get_address() == address) {
                std::cout << "Register " << reg.second.get_name()
                    << " changed to value -> [0x" << std::hex << value << "]"
                    << std::dec << std::endl;
                reg.second = value;
                return;
            }
        }
        throw std::runtime_error("Register with the given address not found");
    };

    void dump_registers()
    {
        for (auto& reg : registers)
        {
            std::cout << "[dump]  Register " << reg.second.get_name()
                << ", address: [" << std::hex << reg.second.get_address()
                << "] value: [" << std::hex << reg.second.get_value() << "]" << std::endl;
        }

    };

    void set_register_callback(const std::string& name, Register::Callback cb) {
        if (registers.find(name) == registers.end()) {
            throw std::runtime_error("Register not found");
        }
        registers[name].set_callback(cb);
    }

public:
    std::map<std::string, Register> registers;
};



#endif