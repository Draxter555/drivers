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

# ip link
```
> ip link

1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN mode DEFAULT group default qlen 1000
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
2: enp0s3: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc fq_codel state UP mode DEFAULT group default qlen 1000
    link/ether 08:00:27:76:c4:55 brd ff:ff:ff:ff:ff:ff
13: enp0s0: <BROADCAST,MULTICAST> mtu 1500 qdisc noop state DOWN mode DEFAULT group default qlen 1000
    link/ether 4a:5a:65:d6:f5:e9 brd ff:ff:ff:ff:ff:ff
14: enp0s1f0: <BROADCAST,MULTICAST> mtu 1500 qdisc noop state DOWN mode DEFAULT group default qlen 1000
    link/ether 3a:63:ca:07:4e:28 brd ff:ff:ff:ff:ff:ff
```

# Запуск приложения

```
> gcc -o app app.c
> sudo ./app 

RAW HEX: 00 00 00 00 00 00 00 00 00 00 00 00 4e 20 3a f8 1f 26 6f 20 77 6f 72 6c
ASCII: \x00 \x00 \x00 \x00 \x00 \x00 \x00 \x00 \x00 \x00 \x00 \x00 N   : \xf8 \x1f & h e l l o   w o r l d
```


