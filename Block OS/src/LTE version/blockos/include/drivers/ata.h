#ifndef __BLOCKOS__DRIVERS__ATA_H
#define __BLOCKOS__DRIVERS__ATA_H

#include <common/types.h>
#include <hardwarecomm/interrupts.h>
#include <hardwarecomm/port.h>

namespace blockos
{
    namespace drivers
    {
        class AdvancedTechnologyAttachment
        {
        protected:
            hardwarecomm::Port16Bit dataPort;
            hardwarecomm::Port8Bit errorPort;
            hardwarecomm::Port8Bit sectorCountPort;
            hardwarecomm::Port8Bit lbaLowPort;
            hardwarecomm::Port8Bit lbaMidPort;
            hardwarecomm::Port8Bit lbaHiPort;
            hardwarecomm::Port8Bit devicePort;
            hardwarecomm::Port8Bit commandPort;
            hardwarecomm::Port8Bit controlPort;
            
            bool master;
        public:
            AdvancedTechnologyAttachment(common::uint16_t portBase, bool master);
            ~AdvancedTechnologyAttachment();

            void Identify();
            void Read28(common::uint32_t sector, common::uint8_t *data, int count = 512);
            void Write28(common::uint32_t sector, common::uint8_t *data, int count);
            void Flush();

        };
    }
}

#endif