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
# 2. Вставка модуля insmod, lsmod, dmesg
```
drivers@drivers:~drivers/pz2$ sudo insmod main.ko

               10:17:54.514793 main     Package type: LINUX_64BITS_GENERIC
[   22.746282] 10:17:54.592172 main     7.1.8 r168469 started. Verbose level = 0
[   22.771399] 10:17:54.618793 main     vbglR3GuestCtrlDetectPeekGetCancelSupport: Supported (#1)
[   22.852701] 10:17:54.700103 automount vbsvcAutomounterMountIt: Successfully mounted 'shared' on '/media/sf_shared'
[   75.701364] loop6: detected capacity change from 0 to 8
[ 2922.186945] Module loaded
```

# 3. Удаление модуля rmmod
```
drivers@drivers:~drivers/pz2$ sudo rmmod main.ko

[   22.746282] 10:17:54.592172 main     7.1.8 r168469 started. Verbose level = 0
[   22.771399] 10:17:54.618793 main     vbglR3GuestCtrlDetectPeekGetCancelSupport: Supported (#1)
[   22.852701] 10:17:54.700103 automount vbsvcAutomounterMountIt: Successfully mounted 'shared' on '/media/sf_shared'
[   75.701364] loop6: detected capacity change from 0 to 8
[ 2922.186945] Module loaded
[ 3102.849632] Module removed
```
