#ifndef __BLOCKOS__HARDWARECOMM__INTERRUPTS_H
#define __BLOCKOS__HARDWARECOMM__INTERRUPTS_H

#include <multitasking.h>
#include <common/types.h>
#include <hardwarecomm/port.h>
#include <gdt.h>

namespace blockos
{
    namespace hardwarecomm
    {
        class InterruptManager;

        class InterruptHandler
        {
        protected:
            blockos::common::uint8_t InterruptNumber;
            InterruptManager* interruptManager;
            InterruptHandler(InterruptManager* interruptManager, blockos::common::uint8_t InterruptNumber);
            ~InterruptHandler();
        public:
            virtual blockos::common::uint32_t HandleInterrupt(blockos::common::uint32_t esp);
        };

        class InterruptManager
        {
            friend class InterruptHandler;
        protected:
            
            static InterruptManager* ActiveInterruptManager;
            InterruptHandler* handlers[256];
            TaskManager *taskManager;

            struct GateDescriptor
            {
                blockos::common::uint16_t handlerAddressLowBits;
                blockos::common::uint16_t gdt_codeSegmentSelector;
                blockos::common::uint8_t reserved;
                blockos::common::uint8_t access;
                blockos::common::uint16_t handlerAddressHighBits;
            } __attribute__((packed));
            
            static GateDescriptor interruptDescriptorTable[256];
            
            struct InterruptDescriptorTablePointer
            {
                blockos::common::uint16_t size;
                blockos::common::uint32_t base;
            } __attribute__((packed));
            
            blockos::common::uint16_t hardwareInterruptOffset;
            static void SetInterruptDescriptorTableEntry(
                blockos::common::uint8_t interrupt,
                blockos::common::uint16_t codeSegmentSelectorOffset,
                void (*handler)(),
                blockos::common::uint8_t DescriptorPrivillegeLevel,
                blockos::common::uint8_t DescriptorType
            );
            
            static void InterruptIgnore();
            static void HandleInterruptRequest0x00(); /// timer
            static void HandleInterruptRequest0x01(); /// tastatura
            static void HandleInterruptRequest0x0C(); /// mouse
            static void HandleInterruptRequest0x02();
            static void HandleInterruptRequest0x03();
            static void HandleInterruptRequest0x04();
            static void HandleInterruptRequest0x05();
            static void HandleInterruptRequest0x06();
            static void HandleInterruptRequest0x07();
            static void HandleInterruptRequest0x08();
            static void HandleInterruptRequest0x09();
            static void HandleInterruptRequest0x0A();
            static void HandleInterruptRequest0x0B();
            static void HandleInterruptRequest0x0D();
            static void HandleInterruptRequest0x0E();
            static void HandleInterruptRequest0x0F();
            static void HandleInterruptRequest0x31();
            
            static void HandleException0x00();
            static void HandleException0x01();
            static void HandleException0x02();
            static void HandleException0x03();
            static void HandleException0x04();
            static void HandleException0x05();
            static void HandleException0x06();
            static void HandleException0x07();
            static void HandleException0x08();
            static void HandleException0x09();
            static void HandleException0x0A();
            static void HandleException0x0B();
            static void HandleException0x0C();
            static void HandleException0x0D();
            static void HandleException0x0E();
            static void HandleException0x0F();
            static void HandleException0x10();
            static void HandleException0x11();
            static void HandleException0x12();
            static void HandleException0x13();
            
            static blockos::common::uint32_t HandleInterrupt(blockos::common::uint8_t interrupt, blockos::common::uint32_t esp); // esp = stack pointer, registru din asm
            blockos::common::uint32_t DoHandleInterrupt(blockos::common::uint8_t interrupt, blockos::common::uint32_t esp);
            
            Port8BitSlow programmableInterruptControllerMasterCommandPort;
            Port8BitSlow programmableInterruptControllerMasterDataPort;
            Port8BitSlow programmableInterruptControllerSlaveCommandPort;
            Port8BitSlow programmableInterruptControllerSlaveDataPort;
            
        public:
            
            InterruptManager(blockos::common::uint16_t hardwareInterruptOffset, blockos::GlobalDescriptorTable* globalDescriptorTable, blockos::TaskManager* taskManager);
            ~InterruptManager();
            blockos::common::uint16_t HardwareInterruptOffset();
            void Activate();
            void Deactivate();
        };
    }
}
#endif
