obj-m += main2.o

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

install: all
	sudo insmod main.ko

uninstall:
	sudo rmmod main

# Компиляция пользовательского приложения
app: app.c
	gcc -o app app.c

# Удобный таргет для перезагрузки
reload: uninstall install

.PHONY: all clean install uninstall app reload
