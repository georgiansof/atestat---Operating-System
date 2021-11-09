#include <syscalls.h>

using namespace blockos;
using namespace blockos::hardwarecomm;
using namespace blockos::common;

SyscallHandler::SyscallHandler(InterruptManager *interruptManager, uint8_t InterruptNumber)
: InterruptHandler(interruptManager, InterruptNumber + interruptManager->HardwareInterruptOffset())
{

}
SyscallHandler::~SyscallHandler()
{

}

void printf(const char*);

uint32_t SyscallHandler::HandleInterrupt(uint32_t esp) //TODO https://web.archive.org/web/20200620014648/ http://syscalls.kernelgrok.com/
{
    CPUState* cpu = (CPUState*)esp;

    switch(cpu->eax)
    {
        case 4:
            printf((const char*)cpu->ebx);
            break;
        break;

        default:
        break;
    }

    return esp;
}
