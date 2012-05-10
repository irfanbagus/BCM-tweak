
KERNEL_SRC_PATH=$(PWD)/../common
TOOLCHAIN=/opt/arm-2009q3/bin/arm-none-eabi-

obj-m += bcm-tweak.o cpufreq_interactive.o

all:
	make -C $(KERNEL_SRC_PATH) M=$(PWD) CROSS_COMPILE=$(TOOLCHAIN) modules
	$(TOOLCHAIN)strip --strip-debug *.ko
clean:
	make -C $(KERNEL_SRC_PATH) M=$(PWD) clean
