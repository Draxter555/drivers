obj-m += main.o

ccflags-y := -fno-pie
KBUILD_CFLAGS += -fno-pie
KBUILD_LDFLAGS += -fno-pie

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
