#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>

// имя интерфейса — можно любое придумать, но чтобы не пересекалось
#define DEV_NAME "mynet0"

// мак-адрес — просто захардкодил
static unsigned char my_mac[ETH_ALEN] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55};

// функция вызывается, когда ядро пытается "отправить" пакет через наш интерфейс
// на самом деле мы его не отправляем, а просто выводим в лог, что поймали
static netdev_tx_t my_start_xmit(struct sk_buff *skb, struct net_device *dev)
{
    printk(KERN_INFO "ура, пакет пришёл! длина: %d байт\n", skb->len);
    dev_kfree_skb(skb); // освобождаем пакет, чтобы не утекала память
    return NETDEV_TX_OK; // говорим, что всё ок
}

// функция при "поднятии" интерфейса (ip link set up)
static int my_open(struct net_device *dev)
{
    printk(KERN_INFO "интерфейс %s подняли\n", dev->name);
    netif_start_queue(dev); // разрешаем ядру слать пакеты в нас
    return 0;
}

// функция при "опускании" интерфейса (ip link set down)
static int my_stop(struct net_device *dev)
{
    printk(KERN_INFO "интерфейс %s опустили\n", dev->name);
    netif_stop_queue(dev); // запрещаем слать пакеты
    return 0;
}

// структура с функциями, которые ядро будет вызывать
static const struct net_device_ops my_netdev_ops = {
    .ndo_open = my_open,
    .ndo_stop = my_stop,
    .ndo_start_xmit = my_start_xmit,
};

// функция настройки интерфейса — вызывается при создании
static void my_setup(struct net_device *dev)
{
    ether_setup(dev); // стандартная настройка для ethernet
    dev->netdev_ops = &my_netdev_ops; // привязываем наши функции
    memcpy(dev->dev_addr, my_mac, ETH_ALEN); // ставим мак
    dev->flags |= IFF_NOARP; // говорим, что arp не нужен (это виртуальное устройство)
}

// при загрузке модуля
static int __init my_init(void)
{
    struct net_device *dev;

    // создаём устройство
    dev = alloc_netdev(0, DEV_NAME, NET_NAME_UNKNOWN, my_setup);
    if (!dev) {
        printk(KERN_ERR "не получилось создать устройство\n");
        return -ENOMEM;
    }

    // регистрируем его в системе
    if (register_netdev(dev)) {
        printk(KERN_ERR "не получилось зарегистрировать устройство\n");
        free_netdev(dev);
        return -ENODEV;
    }

    printk(KERN_INFO "драйвер загружен, интерфейс: %s\n", dev->name);
    return 0;
}

// при выгрузке модуля
static void __exit my_exit(void)
{
    struct net_device *dev = dev_get_by_name(&init_net, DEV_NAME);
    if (dev) {
        unregister_netdev(dev);
        free_netdev(dev);
        printk(KERN_INFO "драйвер выгружен\n");
    }
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("student");
MODULE_DESCRIPTION("простой сетевой драйвер для лабы");
