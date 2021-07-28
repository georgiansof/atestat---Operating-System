#ifndef __BLOCKOS__HARDWARECOMM_PCI_H
#define __BLOCKOS__HARDWARECOMM_PCI_H

#include <hardwarecomm/port.h>
#include <drivers/driver.h>
#include <common/types.h>
#include <hardwarecomm/interrupts.h>
#include <memorymanagement.h>

namespace blockos
{
	namespace hardwarecomm
	{
		
		enum BaseAdressRegisterType
		{
			MemoryMapping = 0,
			InputOutput = 1
		};
		
		class BaseAddressRegister
		{
		public:
			bool prefetchable;
			blockos::common::uint8_t* address;
			blockos::common::uint32_t size;
			BaseAdressRegisterType type;
		};
		
		class PeripherialComponentInterconnectDeviceDescriptor
		{
		public:
			blockos::common::uint32_t portBase;
			blockos::common::uint32_t interrupt;
			
			blockos::common::uint16_t bus;
			blockos::common::uint16_t device;
			blockos::common::uint16_t function;
			
			blockos::common::uint16_t vendor_id;
			blockos::common::uint16_t device_id;
			
			blockos::common::uint8_t class_id;
			blockos::common::uint8_t subclass_id;
			blockos::common::uint8_t interface_id;
			
			blockos::common::uint8_t revision;
			
			PeripherialComponentInterconnectDeviceDescriptor();
			~PeripherialComponentInterconnectDeviceDescriptor();
		};

		class PeripherialComponentInterconnectController /// PCI controller
		{
			Port32Bit dataPort;
			Port32Bit commandPort;
		public:
			PeripherialComponentInterconnectController();
			~PeripherialComponentInterconnectController();
			
			blockos::common::uint32_t Read(blockos::common::uint16_t bus, blockos::common::uint16_t device, blockos::common::uint16_t function, blockos::common::uint32_t registeroffset);
			void Write(blockos::common::uint16_t bus, blockos::common::uint16_t device, blockos::common::uint16_t function, blockos::common::uint32_t registeroffset, blockos::common::uint32_t value);
			bool DeviceHasFunctions(blockos::common::uint16_t bus, blockos::common::uint16_t device);
			
			void SelectDrivers(blockos::drivers::DriverManager* driverManager, blockos::hardwarecomm::InterruptManager* interrupts);
            blockos::drivers::Driver* GetDriver(PeripherialComponentInterconnectDeviceDescriptor dev, blockos::hardwarecomm::InterruptManager* interrupts);
			PeripherialComponentInterconnectDeviceDescriptor GetDeviceDescriptor(blockos::common::uint16_t bus, blockos::common::uint16_t device, blockos::common::uint16_t function);
			BaseAddressRegister GetBaseAddressRegister(blockos::common::uint16_t bus, blockos::common::uint16_t device, blockos::common::uint16_t function,blockos::common::uint16_t bar);
		};
	}
}
#endif
