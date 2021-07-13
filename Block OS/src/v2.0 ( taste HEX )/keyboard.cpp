
#include "keyboard.h"

KeyboardDriver::KeyboardDriver(InterruptManager* manager) 
:InterruptHandler(0x21, manager),
dataport(0x60),
commandport(0x64)
{
    while(commandport.Read() & 0x1)
        dataport.Read();
    commandport.Write(0xAE); 
    commandport.Write(0x20); 
    uint8_t status = (dataport.Read() | 1) & ~0x10;
    commandport.Write(0x60); 
    dataport.Write(status);
    
    dataport.Write(0xF4);
}

KeyboardDriver::~KeyboardDriver()
{
}

void printf(char*);

uint32_t KeyboardDriver::HandleInterrupt(uint32_t esp)
{
    uint8_t key = dataport.Read();
    static bool SHIFT=0;
    
        switch(key)
        {
            default:
                char* msg = "0x00 ";
                char* hex = "0123456789ABCDEF";
                msg[2] = hex[(key >> 4) & 0x0F];
                msg[3] = hex[key & 0x0F];
                printf(msg); 
        }
    return esp;
}
