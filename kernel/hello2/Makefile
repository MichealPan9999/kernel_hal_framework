obj-m +=hello2.o

#generate the path
CURRENT_PATH:= $(shell pwd)

#the current kernel version number
LINUX_KERNEL:=$(shell uname -r)

#the absolute path
LINUX_KERNEL_PATH:=usr/src/linux-headers-$(LINUX_KERNEL)

#compile object
all:
	make -C $(LINUX_KERNEL_PATH) M=(CURRENT_PATH) modules

#clean content under current path
clean:
	make -c $(LINUX_KERNEL_PATH) m=(CURRENT_PATH) clean
