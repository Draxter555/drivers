# Вставка модуля и чтение MAC, реализовано в main.c
```
> make
> sudo insmod main.ko
> sudo dmesg | tail

[ 3827.091759] LAB5: probe success
[ 3827.094595] LAB5: probe entered
[ 3827.095090] LAB5: MAC 46:ae:bb:7b:31:96
[ 3827.095898] LAB5: probe success
[ 3827.111804] lab5_pci_net 0000:00:00.0 enp0s0: renamed from eth0
[ 3827.119512] lab5_pci_net 0000:00:01.0 enp0s1f0: renamed from eth1
```
