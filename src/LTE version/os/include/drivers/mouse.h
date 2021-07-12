#ifndef __OS__DRIVERS__MOUSE_H
#define __OS__DRIVERS__MOUSE_H

#include <common/types.h>
#include <hardwarecomm/port.h>
#include <drivers/driver.h>
#include <hardwarecomm/interrupts.h>

namespace os
{
    namespace drivers
    {
                
        class MouseEventHandler
        {
        public:
            MouseEventHandler();
            
            virtual void OnActivate();
            virtual void OnClickDown(os::common::uint8_t button);
            virtual void OnClickUp(os::common::uint8_t button);
            virtual void OnMouseMovement(int x, int y);
        };
        
        class MouseDriver : public os::hardwarecomm::InterruptHandler, public Driver
        {
            os::hardwarecomm::Port8Bit dataport;
            os::hardwarecomm::Port8Bit commandport;
            
            os::common::uint8_t buffer[3];
            os::common::uint8_t offset;
            os::common::uint8_t buttons;
            
            MouseEventHandler* handler;
        public:
            MouseDriver(os::hardwarecomm::InterruptManager* manager,MouseEventHandler* handler);
            ~MouseDriver();
            virtual os::common::uint32_t HandleInterrupt(os::common::uint32_t esp);
            virtual void Activate();
        };
    }
}
#endif
