# 1. Сборка модуля
## Makefile:
```
obj-m += main.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

```

## Логи:
```
make -C /lib/modules/5.15.0-163-generic/build M=/home/drivers/drivers/pz2 modules
make[1]: Entering directory '/usr/src/linux-headers-5.15.0-163-generic'
  CC [M]  /home/drivers/drivers/pz2/main.o
  MODPOST /home/drivers/drivers/pz2/Module.symvers
  CC [M]  /home/drivers/drivers/pz2/main.mod.o
  LD [M]  /home/drivers/drivers/pz2/main.ko
  BTF [M] /home/drivers/drivers/pz2/main.ko
make[1]: Leaving directory '/usr/src/linux-headers-5.15.0-163-generic'
```
# 2. Вставка модуля insmod, dmesg
```
drivers@drivers:~drivers/pz2$ sudo insmod main.ko
```
