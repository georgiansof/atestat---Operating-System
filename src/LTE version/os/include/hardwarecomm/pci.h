#ifndef __OS__HARDWARECOMM_PCI_H
#define __OS__HARDWARECOMM_PCI_H

#include <hardwarecomm/port.h>
#include <drivers/driver.h>
#include <common/types.h>
#include <hardwarecomm/interrupts.h>
namespace os
{
	namespace hardwarecomm
	{
		
		enum BaseAdressRegisterType
		{
			MemoryMapping = 0,
			InputOutput = 1
		};
		
		class BaseAdressRegister 
		{
		public:
			bool prefetchable;
			os::common::uint8_t* address;
			os::common::uint32_t size;
			BaseAdressRegisterType type;
		};
		
		class PeripherialComponentInterconnectDeviceDescriptor
		{
		public:
			os::common::uint32_t portBase;
			os::common::uint32_t interrupt;
			
			os::common::uint16_t bus;
			os::common::uint16_t device;
			os::common::uint16_t function;
			
			os::common::uint16_t vendor_id;
			os::common::uint16_t device_id;
			
			os::common::uint8_t class_id;
			os::common::uint8_t subclass_id;
			os::common::uint8_t interface_id;
			
			os::common::uint8_t revision;
			
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
			
			os::common::uint32_t Read(os::common::uint16_t bus, os::common::uint16_t device, os::common::uint16_t function, os::common::uint32_t registeroffset);
			void Write(os::common::uint16_t bus, os::common::uint16_t device, os::common::uint16_t function, os::common::uint32_t registeroffset, os::common::uint32_t value);
			bool DeviceHasFunctions(os::common::uint16_t bus, os::common::uint16_t device);
			
			void SelectDrivers(os::drivers::DriverManager* driverManager, os::hardwarecomm::InterruptManager* interrupts);
			PeripherialComponentInterconnectDeviceDescriptor GetDeviceDescriptor(os::common::uint16_t bus, os::common::uint16_t device, os::common::uint16_t function);
			BaseAdressRegister GetBaseAddressRegister(os::common::uint16_t bus, os::common::uint16_t device, os::common::uint16_t function,os::common::uint16_t bar);
		};
	}
}
#endif
