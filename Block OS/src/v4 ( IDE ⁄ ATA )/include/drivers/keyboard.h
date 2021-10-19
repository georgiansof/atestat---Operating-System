#ifndef __BLOCKOS__DRIVERS__KEYBOARD_H
#define __BLOCKOS__DRIVERS__KEYBOARD_H

#include <common/types.h>
#include <hardwarecomm/interrupts.h>
#include <drivers/driver.h>
#include <hardwarecomm/port.h>

namespace blockos
{
    namespace drivers 
    {
        class KeyboardEventHandler
        {
        public:
            KeyboardEventHandler();
            virtual void OnKeyDown(char);
            virtual void OnKeyUp(char);
        };

        class KeyboardDriver : public blockos::hardwarecomm::InterruptHandler,public Driver
        {
            blockos::hardwarecomm::Port8Bit dataport;
            blockos::hardwarecomm::Port8Bit commandport;
            
            KeyboardEventHandler* handler;
        public:
            KeyboardDriver(blockos::hardwarecomm::InterruptManager* manager, KeyboardEventHandler *handler);
            ~KeyboardDriver();
            virtual blockos::common::uint32_t HandleInterrupt(blockos::common::uint32_t esp);
            virtual void Activate();
        };
    }
}

#endif
