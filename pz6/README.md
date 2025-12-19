При привязке к реальному PCI-устройству ядро Linux автоматически переименовывает сетевой интерфейс в *enp0s3*.
Ядро сначала создаёт интерфейс с именем *my_pci_net*, но затем переименовывает его в стандартное
---


# 1. Сборка модуля и згрузка драйвера
```
> make
> make app
> sudo insmod main.ko

*Выгружаем старый драйвер*
> sudo modprobe -r e1000

PCI устройство найдено! Vendor: 0x8086, Device: 0x100E
MAC адрес прочитан: 00:11:22:33:44:55
сетевой интерфейс my_pci_net создан
my_pci_net_driver 0000:00:03.0 enp0s3: renamed from my_pci_net
enp0s3 поднят
```

# 2. Поднятие интерфейса
```
> ip link

1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN mode DEFAULT group default qlen 1000
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
13: enp0s3: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc fq_codel state UP mode DEFAULT group default qlen 1000
    link/ether 08:00:27:76:c4:55 brd ff:ff:ff:ff:ff:ff
```

# 3. Проверка отправки
```
> sudo ping -I enp0s3 -c 3 8.8.8.8

> dmesg | tail

[ 6202.258769] пакет пойман! длина: 333
[ 6203.484940] пакет пойман! длина: 70
[ 6206.234916] пакет пойман! длина: 333
[ 6207.234894] пакет пойман! длина: 70
[ 6209.702071] пакет пойман! длина: 333
[ 6214.707525] пакет пойман! длина: 70
[ 6217.032834] пакет пойман! длина: 333
```


