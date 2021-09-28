GCCPARAMS = -m32 -Iinclude -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings
ASPARAMS = --32
LDPARAMS = -melf_i386

objects = obj/loader.o \
	  obj/gdt.o \
	  obj/memorymanagement.o \
	  obj/drivers/driver.o \
	  obj/hardwarecomm/port.o \
	  obj/hardwarecomm/interruptstubs.o \
	  obj/hardwarecomm/interrupts.o \
	  obj/multitasking.o \
	  obj/hardwarecomm/pci.o \
	  obj/drivers/keyboard.o \
	  obj/drivers/mouse.o \
	  obj/drivers/vga.o \
	  obj/drivers/ata.o \
	  obj/gui/widget.o \
	  obj/gui/window.o \
	  obj/gui/desktop.o \
	  obj/kernel.o \

run: mykernel.iso
	cp mykernel.iso ../../iso_output/
	(killall VirtualBoxVM && sleep 1) || true
	VirtualBoxVM --startvm "hddtest" &

	  
obj/%.o: sources/%.cpp
	mkdir -p $(@D)
	gcc $(GCCPARAMS) -c -o $@ $<

obj/%.o: sources/%.s
	mkdir -p $(@D)
	as $(ASPARAMS) -o $@ $<
	
mykernel.bin: linker.ld $(objects)
	ld $(LDPARAMS) -T $< -o $@ $(objects) 

mykernel.iso: mykernel.bin
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp mykernel.bin iso/boot/mykernel.bin
	echo 'set timeout=3'  >> iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo 'menuentry "Kernel" {' >> iso/boot/grub/grub.cfg
	echo '	multiboot /boot/mykernel.bin' >> iso/boot/grub/grub.cfg
	echo '	boot' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=mykernel.iso iso
	rm -rf iso

entry:
	echo 'menuentry "Kernel" {' >> /boot/grub/grub.cfg
	echo '  multiboot /boot/mykernel.bin' >> /boot/grub/grub.cfg
	echo '  boot' >> /boot/grub/grub.cfg
	echo '}' >> /boot/grub/grub.cfg

install: mykernel.bin
	sudo cp $< /boot/mykernel.bin
	
.PHONY: clean exec
clean:
	rm -rf iso obj mykernel.bin mykernel.iso ../../iso_output/mykernel.iso

exec:
	make clean
	make run

