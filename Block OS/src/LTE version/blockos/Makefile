GCCPARAMS = -m32 -Iinclude -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings
ASPARAMS = --32
LDPARAMS = -melf_i386

objects = obj/loader.o \
	  obj/gdt.o \
	  obj/drivers/driver.o \
	  obj/hardwarecomm/port.o \
	  obj/hardwarecomm/interruptstubs.o \
	  obj/hardwarecomm/interrupts.o \
	  obj/hardwarecomm/pci.o \
	  obj/drivers/keyboard.o \
	  obj/drivers/mouse.o \
	  obj/kernel.o \

run: mykernel.iso
	cp mykernel.iso ../../../iso_output/
	(killall VirtualBox && sleep 1) || true
	VirtualBoxVM --startvm "atestat" &

	  
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
	echo 'set timeout=3'  > iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo 'menuentry "Kernel" {' >> iso/boot/grub/grub.cfg
	echo '	multiboot /boot/mykernel.bin' >> iso/boot/grub/grub.cfg
	echo '	boot' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=mykernel.iso iso
	rm -rf iso
	
install: mykernel.bin
	sudo cp $< /boot/mykernel.bin
	
.PHONY: clean
clean:
	rm -rf iso obj mykernel.bin mykernel.iso ../../../iso_output/mykernel.iso
