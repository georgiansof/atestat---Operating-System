GPPPARAMS = -m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings
ASPARAMS = --32
LDPARAMS = -melf_i386

objects = loader.o gdt.o port.o interruptstubs.o interrupts.o keyboard.o kernel.o

%.o: %.cpp
	g++ $(GPPPARAMS) -o $@ -c $<

%.o: %.s
	as $(ASPARAMS) -o $@ $<
	
mykernel.bin: linker.ld $(objects)
	ld $(LDPARAMS) -T $< -o $@ $(objects) 

entry:
	echo 'menuentry "Kernel" {' >> /boot/grub/grub.cfg
	echo '  multiboot /boot/mykernel.bin' >> /boot/grub/grub.cfg
	echo '  boot' >> /boot/grub/grub.cfg
	echo '}' >> /boot/grub/grub.cfg

install: mykernel.bin
	sudo cp $< /boot/mykernel.bin

mykernel.iso: mykernel.bin
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp $< iso/boot/
	echo 'set timeout=3' >> iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo 'menuentry "Kernel" {' >> iso/boot/grub/grub.cfg
	echo '	multiboot /boot/mykernel.bin' >> iso/boot/grub/grub.cfg
	echo '	boot' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=$@ iso
	rm -rf iso
	
run: mykernel.iso
	cp mykernel.iso ../../iso_output/
	rm -f $(objects) mykernel.bin mykernel.iso
	(killall VirtualBox && sleep 1) || true
	VirtualBoxVM --startvm "atestat" &

.PHONY: clean exec
clean:
	rm -f $(objects) mykernel.bin mykernel.iso ../../iso_output/mykernel.iso
exec:
	make clean
	make run
