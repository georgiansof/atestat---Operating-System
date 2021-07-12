#include <hardwarecomm/pci.h>
using namespace os::common;
using namespace os::drivers;
using namespace os::hardwarecomm;

void printf(char*);
void printfHex(uint8_t);

PeripherialComponentInterconnectDeviceDescriptor::PeripherialComponentInterconnectDeviceDescriptor()
{
	
}
PeripherialComponentInterconnectDeviceDescriptor::~PeripherialComponentInterconnectDeviceDescriptor()
{
	
}

PeripherialComponentInterconnectController::PeripherialComponentInterconnectController()
: dataPort(0xCFC),
  commandPort(0xCF8)
{
}

PeripherialComponentInterconnectController::~PeripherialComponentInterconnectController()
{
}

uint32_t PeripherialComponentInterconnectController::Read(uint16_t bus, uint16_t device, uint16_t function, uint32_t registeroffset)
{
	uint32_t id = 
		0x1 << 31
		| ((bus & 0xFF) << 16)
		| ((device & 0x1F) << 11)
		| ((function & 0x07) << 8)
		| (registeroffset & 0xFC);
	commandPort.Write(id);
	uint32_t result = dataPort.Read();
	return result >> (8* (registeroffset % 4));
}

void PeripherialComponentInterconnectController::Write(uint16_t bus, uint16_t device, uint16_t function, uint32_t registeroffset, uint32_t value)
{
		uint32_t id = 
		0x1 << 31
		| ((bus & 0xFF) << 16)
		| ((device & 0x1F) << 11)
		| ((function & 0x07) << 8)
		| (registeroffset & 0xFC);
		commandPort.Write(id);
		dataPort.Write(value);
}

bool PeripherialComponentInterconnectController::DeviceHasFunctions(common::uint16_t bus, common::uint16_t device)
{return Read(bus, device, 0, 0x0E) & (1<<7);}

void PeripherialComponentInterconnectController::SelectDrivers(os::drivers::DriverManager* driverManager)
{
	for(int bus = 0; bus < 8; ++bus)
	{
		for(int device = 0; device < 32 ; ++device)
		{
			int numFunctions = DeviceHasFunctions(bus, device) ? 8 : 1; 
			for(int function = 0; function < numFunctions ; ++ function)
			{
				PeripherialComponentInterconnectDeviceDescriptor dev = GetDeviceDescriptor(bus, device, function);
				
				if(dev.vendor_id == 0x0000 || dev.vendor_id == 0xFFFF) 
					continue;
				
				printf("PCI BUS ");
				printfHex(bus & 0xFF);
				
				printf(", DEVICE ");
				printfHex(device & 0xFF);
				
				printf(", FUNCTION ");
				printfHex(function & 0xFF);
				
				printf(", VENDOR ");
				printfHex((dev.vendor_id & 0xFF00) >> 8);
				printfHex(dev.vendor_id & 0xFF);
				
				printf(", DEVICE_ID ");
				printfHex((dev.device_id & 0xFF00) >> 8);
				printfHex(dev.device_id & 0xFF);
				printf("\n");
			}
		}
	}
}

PeripherialComponentInterconnectDeviceDescriptor PeripherialComponentInterconnectController::GetDeviceDescriptor(common::uint16_t bus, common::uint16_t device, common::uint16_t function)
{
	PeripherialComponentInterconnectDeviceDescriptor result;
	
	result.bus = bus;
	result.device = device;
	result.function = function;
	
	result.vendor_id = Read(bus, device, function, 0x00);
	result.device_id = Read(bus, device, function, 0x02);
	
	result.class_id = Read(bus, device, function, 0x0B);
	result.subclass_id = Read(bus, device, function, 0x0A);
	result.interface_id = Read(bus, device, function, 0x09);
	
	result.revision = Read(bus, device, function, 0x08);
	result.interrupt = Read(bus, device, function, 0x3C);
	
	return result;
}
