obj-m += driver.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) CFLAGS_MODULE="-fPIC" modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
