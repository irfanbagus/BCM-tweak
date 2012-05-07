
KERNEL_SRC_PATH=$(PWD)/../common

obj-m += bcm-tweak.o

all:
	make -C $(KERNEL_SRC_PATH) M=$(PWD) modules

clean:
	make -C $(KERNEL_SRC_PATH) M=$(PWD) clean
