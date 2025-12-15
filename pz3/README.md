# 1. Скомпилировать символьный драйвер
makefile:
```
obj-m += main.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
```
Логи:
```
make -C /lib/modules/5.15.0-163-generic/build M=/home/drivers/drivers/pz3 modules
make[1]: Entering directory '/usr/src/linux-headers-5.15.0-163-generic'
  CC [M]  /home/drivers/drivers/pz3/main.o
  MODPOST /home/drivers/drivers/pz3/Module.symvers
  CC [M]  /home/drivers/drivers/pz3/main.mod.o
  LD [M]  /home/drivers/drivers/pz3/main.ko
  BTF [M] /home/drivers/drivers/pz3/main.ko
Skipping BTF generation for /home/drivers/drivers/pz3/main.ko due to unavailability of vmlinux
make[1]: Leaving directory '/usr/src/linux-headers-5.15.0-163-generic'
```
# 2. Вставить в ядро

# 3. Создать специальный файл устройства

# 4. Написать приложение для открытия специального файлового устройства
