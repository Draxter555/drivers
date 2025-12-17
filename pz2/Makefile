obj-m += main.o

ccflags-y := \
    -fno-pie \
    -fno-sanitize=all \
    -fno-stack-protector \
    -fno-omit-frame-pointer

KBUILD_CFLAGS += \
    -fno-sanitize=all \
    -fno-stack-protector

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
