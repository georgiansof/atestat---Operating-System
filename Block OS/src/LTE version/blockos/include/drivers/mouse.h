#ifndef __BLOCKOS__DRIVERS__MOUSE_H
#define __BLOCKOS__DRIVERS__MOUSE_H

#include <common/types.h>
#include <hardwarecomm/port.h>
#include <drivers/driver.h>
#include <hardwarecomm/interrupts.h>

namespace blockos
{
    namespace drivers
    {
                
        class MouseEventHandler
        {
        public:
            MouseEventHandler();
            
            virtual void OnActivate();
            virtual void OnClickDown(blockos::common::uint8_t button);
            virtual void OnClickUp(blockos::common::uint8_t button);
            virtual void OnMouseMovement(int x, int y);
        };
        
        class MouseDriver : public blockos::hardwarecomm::InterruptHandler, public Driver
        {
            blockos::hardwarecomm::Port8Bit dataport;
            blockos::hardwarecomm::Port8Bit commandport;
            
            blockos::common::uint8_t buffer[3];
            blockos::common::uint8_t offset;
            blockos::common::uint8_t buttons;
            
            MouseEventHandler* handler;
        public:
            MouseDriver(blockos::hardwarecomm::InterruptManager* manager,MouseEventHandler* handler);
            ~MouseDriver();
            virtual blockos::common::uint32_t HandleInterrupt(blockos::common::uint32_t esp);
            virtual void Activate();
        };
    }
}
#endif
