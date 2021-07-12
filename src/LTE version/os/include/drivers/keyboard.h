#ifndef __OS__DRIVERS__KEYBOARD_H
#define __OS__DRIVERS__KEYBOARD_H

#include <common/types.h>
#include <hardwarecomm/interrupts.h>
#include <drivers/driver.h>
#include <hardwarecomm/port.h>

namespace os
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

        class KeyboardDriver : public os::hardwarecomm::InterruptHandler,public Driver
        {
            os::hardwarecomm::Port8Bit dataport;
            os::hardwarecomm::Port8Bit commandport;
            
            KeyboardEventHandler* handler;
        public:
            KeyboardDriver(os::hardwarecomm::InterruptManager* manager, KeyboardEventHandler *handler);
            ~KeyboardDriver();
            virtual os::common::uint32_t HandleInterrupt(os::common::uint32_t esp);
            virtual void Activate();
        };
    }
}

#endif
