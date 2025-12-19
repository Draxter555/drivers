#include <linux/module.h>
#include <linux/pci.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>

// Имя сетевого интерфейса
#define DEV_NAME "my_pci_net"

// Псевдо-MAC адрес
static unsigned char fake_mac[ETH_ALEN] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55};

#define VENDOR_ID 0x8086   // Intel
#define DEVICE_ID 0x100E   // e1000 (пример)

// Структура для хранения данных устройства
struct my_pci_dev {
    struct net_device *netdev;
    void __iomem *mmio_base; // условно — базовый адрес памяти (в реальном драйвере)
};

// Функция отправки пакета
static netdev_tx_t my_start_xmit(struct sk_buff *skb, struct net_device *dev)
{
    printk(KERN_INFO "пакет пойман! длина: %d\n", skb->len);
    dev_kfree_skb(skb);
    return NETDEV_TX_OK;
}

// Открытие интерфейса
static int my_open(struct net_device *dev)
{
    printk(KERN_INFO "%s поднят\n", dev->name);
    netif_start_queue(dev);
    return 0;
}

// Закрытие интерфейса
static int my_stop(struct net_device *dev)
{
    printk(KERN_INFO "%s опущен\n", dev->name);
    netif_stop_queue(dev);
    return 0;
}

// Операции сетевого устройства
static const struct net_device_ops my_netdev_ops = {
    .ndo_open = my_open,
    .ndo_stop = my_stop,
    .ndo_start_xmit = my_start_xmit,
};

// Функция настройки сетевого устройства
static void my_setup(struct net_device *dev)
{
    ether_setup(dev);
    dev->netdev_ops = &my_netdev_ops;
    dev->flags |= IFF_NOARP;
}

// Функция probe
static int my_pci_probe(struct pci_dev *pdev, const struct pci_device_id *id)
{
    struct my_pci_dev *priv;
    struct net_device *dev;
    int err;

    printk(KERN_INFO "PCI устройство найдено! Vendor: 0x%04X, Device: 0x%04X\n",
           pdev->vendor, pdev->device);

    // Выделяем структуру для нашего устройства
    priv = kzalloc(sizeof(*priv), GFP_KERNEL);
    if (!priv)
        return -ENOMEM;

    // Создание сетевого устройства
    dev = alloc_netdev(0, DEV_NAME, NET_NAME_UNKNOWN, my_setup);
    if (!dev) {
        kfree(priv);
        return -ENOMEM;
    }

    // Сохраняем указатель на устройство
    priv->netdev = dev;
    SET_NETDEV_DEV(dev, &pdev->dev); // связываем с PCI-устройством

    // Имитируем чтение MAC-адреса из "регистров"
    // В реальном драйвере здесь было бы чтение из BAR
    memcpy(dev->dev_addr, fake_mac, ETH_ALEN);
    printk(KERN_INFO "MAC адрес прочитан: %pM\n", dev->dev_addr);

    // Регистрируем сетевое устройство
    err = register_netdev(dev);
    if (err) {
        printk(KERN_ERR "не удалось зарегистрировать устройство\n");
        free_netdev(dev);
        kfree(priv);
        return err;
    }

    // Сохраняем данные в PCI-устройстве
    pci_set_drvdata(pdev, priv);

    printk(KERN_INFO "сетевой интерфейс %s создан\n", dev->name);
    return 0;
}

// Функция remove
static void my_pci_remove(struct pci_dev *pdev)
{
    struct my_pci_dev *priv = pci_get_drvdata(pdev);
    struct net_device *dev = priv->netdev;

    unregister_netdev(dev);
    free_netdev(dev);
    kfree(priv);

    printk(KERN_INFO "устройство удалено\n");
}

// Таблица совместимых PCI-устройств
static const struct pci_device_id my_pci_ids[] = {
    { PCI_DEVICE(VENDOR_ID, DEVICE_ID) }, // наш "виртуальный" адаптер
    { 0, } // конец таблицы
};
MODULE_DEVICE_TABLE(pci, my_pci_ids);

// Структура драйвера PCI
static struct pci_driver my_pci_driver = {
    .name = "my_pci_net_driver",
    .id_table = my_pci_ids,
    .probe = my_pci_probe,
    .remove = my_pci_remove,
};

// Инициализация модуля
static int __init my_init(void)
{
    int err;

    err = pci_register_driver(&my_pci_driver);
    if (err) {
        printk(KERN_ERR "не удалось зарегистрировать PCI-драйвер\n");
        return err;
    }

    printk(KERN_INFO "PCI-драйвер загружен\n");
    return 0;
}

// Выгрузка модуля
static void __exit my_exit(void)
{
    pci_unregister_driver(&my_pci_driver);
    printk(KERN_INFO "PCI-драйвер выгружен\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("student");
MODULE_DESCRIPTION("PCI сетевой драйвер для лабы");
