#ifndef __BLOCKOS__SYSCALLS_H
#define __BLOCKOS__SYSCALLS_H

#include <hardwarecomm/interrupts.h>
#include <common/types.h>
#include <multitasking.h>

namespace blockos
{
    class SyscallHandler : public hardwarecomm::InterruptHandler
    {
    public:
        SyscallHandler(hardwarecomm::InterruptManager *interruptManager, common::uint8_t InterruptNumber);
        ~SyscallHandler();
        virtual common::uint32_t HandleInterrupt(common::uint32_t esp);
    };

}

#endif