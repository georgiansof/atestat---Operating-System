#include <common/types.h>
#include <gdt.h>
#include <hardwarecomm/interrupts.h>
#include <hardwarecomm/pci.h>
#include <drivers/driver.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>

using namespace os;
using namespace os::common;
using namespace os::hardwarecomm;
using namespace os::drivers;

void printf(char* str)
{
    static volatile uint16_t* VideoMemory = (uint16_t*)0xb8000; 
    static uint8_t x=0, y=0;
    
    for(int i=0;str[i] != '\0'; ++i)
    {
        
        switch(str[i])
        {
            case '\n':
                ++y,x=0;
                break;

            default:
            VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | str[i];
            ++x;
        }
        
        if(x >= 80)
        {
            ++y;
            x=0;
        }
        
        if(y >= 25)
        {
            for(y=0;y<25;++y)
                for(x=0;x<80;++x)
                    VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | ' ';
                    
            x=0;
            y=0;
        }
    }
    
}

void printfHex(uint8_t key)
{
    char* msg = "0x00";
    char* hex = "0123456789ABCDEF";
    msg[0] = hex[(key >> 4) & 0x0F];
    msg[1] = hex[key & 0x0F];
    printf(msg); 
}

class PrintfKeyboardEventHandler : public KeyboardEventHandler
{
public:
    void OnKeyDown(char c)
    {
        char* str = " ";
        str[0]=c;
        printf(str);
    }
};

class MouseToConsole : public MouseEventHandler
{
    int8_t x,y;
public:
    MouseToConsole()
    {
        uint16_t* VideoMemory = (uint16_t*)0xb8000;
        x=40;
        y=12;
        VideoMemory[80*y+x] = ((VideoMemory[80*y+x] & 0xF000) >> 4) 
                            | ((VideoMemory[80*y+x] & 0x0F00) << 4) 
                            | ((VideoMemory[80*y+x] & 0x00FF)); 
    }
    void OnMouseMovement(int xoffset, int yoffset)
    {
        static volatile uint16_t* VideoMemory = (uint16_t*)0xb8000; 
        
        VideoMemory[80*y+x] = ((VideoMemory[80*y+x] & 0xF000) >> 4) 
                            | ((VideoMemory[80*y+x] & 0x0F00) << 4) 
                            | ((VideoMemory[80*y+x] & 0x00FF)); 
        
        x += xoffset;
        if(x < 0) x = 0;
        if(x >=80) x=79;
        
        y += yoffset;
        if(y < 0) y=0;
        if(y>=25) y=24;
        
        VideoMemory[80*y+x] = ((VideoMemory[80*y+x] & 0xF000) >> 4) 
                            | ((VideoMemory[80*y+x] & 0x0F00) << 4) 
                            | ((VideoMemory[80*y+x] & 0x00FF)); 
    }
};

typedef void (*constructor)();
extern "C" constructor *start_ctors;
extern "C" constructor *end_ctors;

extern "C" void callConstructors()
{
    for(constructor *i= start_ctors; i!=end_ctors; ++i)
        (*i)();
}

extern "C" void kernelMain(void* multiboot_structure, uint32_t /*multiboot_magic*/)
{
    bool open=1;
    printf("Successfully booted kernel v2.0!\n\n"); 
    
    GlobalDescriptorTable gdt; 
    InterruptManager interrupts(0x20, &gdt);
    
    printf("Initializing drivers...\n");
    
    DriverManager drvManager;
    
        PrintfKeyboardEventHandler kbhandler;
        KeyboardDriver keyboard(&interrupts, &kbhandler);
        drvManager.AddDriver(&keyboard);

        MouseToConsole mousehandler;
        MouseDriver mouse(&interrupts, &mousehandler);
        drvManager.AddDriver(&mouse);
        
		PeripherialComponentInterconnectController PCIcontroller;
		PCIcontroller.SelectDrivers(&drvManager);
		
        drvManager.ActivateAll();
    
    interrupts.Activate();
    printf("All set!\n");
    while(open);
}
