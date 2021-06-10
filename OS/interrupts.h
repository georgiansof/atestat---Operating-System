#ifndef _INTERRUPTS_H
#define _INTERRUPTS_H

#include "types.h"
#include "port.h"
#include "gdt.h"

class InterruptManager
{
protected:
    struct GateDescriptor
    {
        uint16_t handlerAddressLowBits;
        uint16_t gdt_codeSegmentSelector;
        uint8_t reserved;
        uint8_t access;
        uint16_t handlerAddressHighBits;
    } __attribute__((packed));
    
    static GateDescriptor interruptDescriptorTable[256];
    
    struct InterruptDescriptorTablePointer
    {
        uint16_t size;
        uint32_t base;
    } __attribute__((packed));
    
    static void SetInterruptDescriptorTableEntry(
        uint8_t interruptNumber,
        uint16_t codeSegmentSelectorOffset,
        void (*handler)(),
        uint8_t DescriptorPrivillegeLevel,
        uint8_t DescriptorType
    );
public:
    
    InterruptManager(GlobalDescriptorTable* gdt);
    ~InterruptManager();
    
    void Activate();
    
    static uint32_t handleInterrupt(uint8_t interruptNumber, uint32_t esp); // esp = stack pointer, registru din asm
    
    static void IgnoreInterruptRequest();
    static void HandleInterruptRequest0x00(); /// timer
    static void HandleInterruptRequest0x01(); /// tastatura
};

#endif
