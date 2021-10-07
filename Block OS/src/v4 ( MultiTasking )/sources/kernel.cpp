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

//#define GRAPHICSMODE

#ifdef GRAPHICSMODE
//
#define BLACK 0x00 
#define BROWN 0x30
#define GREY 0x38 
#define SILVER 0x07 
#define WHITE 0x3F

#define VERY_LIGHT_BLUE 0x1F 
#define LIGHT_CYAN 0x3B 
#define CYAN 0x1B 
#define AQUAMARINE_BLUE 0x23 
#define LIGHT_BLUE 0x0B 
#define LIGHT_BLUE1 0x2B 
#define COSMIC_BLUE 0x19 
#define BLUE 0x09 
#define ROYAL_BLUE 0x01
#define GREYED_BLUE 0x11 
#define DARK_BLUE 0x08 

#define VERNYL 0x0A

#define TURQUOISE 0x03 
#define TURQUOISE1 0x33
#define GREEN_TURQUOISE 0x13 

#define VERY_LIGHT_GREEN 0x17 
#define LIGHT_GREEN 0x12
#define LIGHT_GREEN1 0x32
#define LIME 0x16 
#define YELLOW_LIME 0x1E 
#define GREEN_LIME 0x1A 
#define GREEN_LIME1 0x3A 
#define GRASS_GREEN 0x22 
#define GREEN 0x02
#define GREEN1 0x2A
#define DEEP_SEA_GREEN 0x18
#define DARK_GREEN 0x10 

#define RED 0x04 
#define CARMIN_RED 0x0C
#define DARK_RED 0x20 
#define STRONG_RED 0x24 
#define LIGHT_CARMIN_RED 0x2C
#define BRICK_RED 0x3C 

#define MAGENTA 0x05 
#define DARK_MAGENTA 0x28 
#define STRONG_MAGENTA 0x2D 
#define SKIN_PINK 0x27 
#define LIGHT_PINK 0x2F 
#define PINK 0x3D 
#define PINK1 0x35 
#define STRONG_PINK 0x25 
#define DARK_PINK 0x15 

#define LIGHT_PURPLE 0x0F
#define GREYED_PURPLE 0x31 
#define PINK_PURPLE 0x0D 
#define LIGHT_PINK_PURPLE 0x1D 
#define PURPLE 0x29
#define COSMIC_PURPLE 0x39 
#define DARK_PURPLE 0x21 

#define LIGHT_YELLOW 0x37 
#define LIGHT_YELLOW1 0x3E 
#define YELLOW 0x36 
#define GREYED_YELLOW 0x0E 
#define DARK_YELLOW 0x06 

#define LIGHT_ORANGE 0x26 
#define SKIN_ORANGE 0x2E 
#define ORANGE 0x34 
#define BRICK_ORANGE 0x1C 
#define DARK_ORANGE 0x14 
//
#endif

using namespace blockos;
using namespace blockos::common;
using namespace blockos::hardwarecomm;
using namespace blockos::drivers;
using namespace blockos::gui;


template <typename type> void Swap(type &x,type &y)
{
    type aux=x;
    x=y;
    y=aux;
}

void printf(char* str)
{
    #ifndef GRAPHICSMODE
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
    #endif
}

void printf(int32_t nr)
{
    #ifndef GRAPHICSMODE
    char str[12];
    int strlen=-1;
    while(nr)
    {
        str[++strlen]=(nr%10)+'0';
        nr/=10;
    }
    for(int i=0;i<(strlen+1)/2;++i)
        Swap(str[i],str[strlen-i]);
    str[strlen+1]=NULL;
    printf(str);
    #endif
}

void printfHex(uint8_t key)
{
    #ifndef GRAPHICSMODE
    char* msg = "00";
    char* hex = "0123456789ABCDEF";
    msg[0] = hex[(key >> 4) & 0x0F];
    msg[1] = hex[key & 0x0F];
    printf(msg); 
    #endif
}

#ifndef GRAPHICSMODE
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
    void OnMouseDown(uint8_t button)
    {
        if(button==1) printf("LMB_DOWN");
        if(button==2) printf("RMB_DOWN");
        if(button==3) printf("WMB_DOWN");
    }
    void OnMouseUp(uint8_t button)
    {
        if(button==1) printf("LMB_UP");
        if(button==2) printf("RMB_UP");
        if(button==3) printf("WMB_UP");
    }
};
#endif

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
#ifdef GRAPHICSMODE
    VideoGraphicsArray vga;
    desktop.gc=&vga;
#endif

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
    Task task1(&gdt, taskA);
    Task task2(&gdt, taskB);
    taskManager.AddTask(&task1);
    taskManager.AddTask(&task2);
    while(open)
    {
#ifdef GRAPHICSMODE
        desktop.Draw();
#endif
    }
}
