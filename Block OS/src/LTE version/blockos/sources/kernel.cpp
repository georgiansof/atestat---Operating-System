#include <common/types.h>
#include <gdt.h>
#include <memorymanagement.h>
#include <hardwarecomm/interrupts.h>
#include <hardwarecomm/pci.h>
#include <drivers/driver.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>
#include <drivers/vga.h>
#include <gui/desktop.h>
#include <gui/window.h>
#include <multitasking.h>

#define NULL 0
#define null 0

#define GRAPHICSMODE

using namespace blockos;
using namespace blockos::common;
using namespace blockos::hardwarecomm;
using namespace blockos::drivers;
using namespace blockos::gui;


void Swap(int x,int y)
{
    int aux=x;
    x=y;
    y=aux;
}

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

void printf(int32_t nr)
{
    char str[12];
    int strlen=-1;
    while(nr)
    {
        str[++strlen]=(nr%10)+'0';
        nr/=10;
    }
    for(int i=0;i<(strlen+1)/2;++i)
        Swap(str[i],str[strlen-i]);
    printf(str);
}

void printfHex(uint8_t key)
{
    char* msg = "00";
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
    void OnMouseMove(int xoffset, int yoffset)
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

void taskA()
{
    while(true)
        printf("A");
}

void taskB()
{
    while(true)
        printf("B");
}



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
    printf("Successfully booted BlockOS!\n\n");
    
    GlobalDescriptorTable gdt;

    uint32_t* memupper = (uint32_t*)(((size_t)multiboot_structure) + 8);
    size_t heap = 10*1024*1024;
    MemoryManager memoryManager(heap, (*memupper)*1024 - heap - 10*1024);

    printf("heap: 0x");
    printfHex((heap >> 24) & 0xFF);
    printfHex((heap >> 16) & 0xFF);
    printfHex((heap >> 8) & 0xFF);
    printfHex(heap & 0xFF);
    
    void* allocated = memoryManager.malloc(1024);
    printf("\nallocated: 0x");
    printfHex(((size_t)allocated >> 24) & 0xFF);
    printfHex(((size_t)allocated >> 16) & 0xFF);
    printfHex(((size_t)allocated >> 8) & 0xFF);
    printfHex((size_t)allocated & 0xFF);
    printf("\n");

    /************** Multitasking **************/
    TaskManager taskManager;
    /*Task task1(&gdt, taskA);
    Task task2(&gdt, taskB);
    taskManager.AddTask(&task1);
    taskManager.AddTask(&task2);*/
    printf("\nMultithreading module initialized\n");
    /**************************************/

    InterruptManager interrupts(0x20, &gdt, &taskManager);
    #ifdef GRAPHICSMODE
    Desktop desktop(320,200,0x00,0x00,0xA8);
    #endif
    printf("Initializing drivers...\n");
    
    DriverManager drvManager;
    

        #ifdef GRAPHICSMODE
        KeyboardDriver keyboard(&interrupts, &desktop);
        #else 
        PrintfKeyboardEventHandler kbhandler;
        KeyboardDriver keyboard(&interrupts, &kbhandler);
        #endif
        drvManager.AddDriver(&keyboard);
        #ifdef GRAPHICSMODE
        MouseDriver mouse(&interrupts, &desktop);
        #else
        MouseToConsole mousehandler;
        MouseDriver mouse(&interrupts, &mousehandler);
        #endif
        drvManager.AddDriver(&mouse);
        
		PeripherialComponentInterconnectController PCIcontroller;
		PCIcontroller.SelectDrivers(&drvManager, &interrupts);
		
        VideoGraphicsArray vga;

        drvManager.ActivateAll();
    
    printf("All set!\n");
    /************** VIDEO MODE INIT ************/
    #ifdef GRAPHICSMODE
    vga.SetMode(320,200,8);

    Window win1(&desktop, 10,10,20,20, 0xA8, 0x00, 0x00);
    desktop.AddChild(&win1);
    Window win2(&desktop, 40,15,30,30, 0x00, 0xA8, 0x00);
    desktop.AddChild(&win2);
    #endif

    interrupts.Activate();

    while(open)
    {
        #ifdef GRAPHICSMODE
        desktop.Draw(&vga);
        desktop.Update(&vga);
        #endif
    }
}
