obj-m += main.o

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

app:
	gcc -o app app.c

install: all
	sudo insmod main.ko

uninstall:
	sudo rmmod main

.PHONY: all clean app install uninstall
