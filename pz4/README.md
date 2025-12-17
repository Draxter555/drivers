# 1. Создать символьный драйвер, который будет автоматически создавать спец. файл устройства
Запуск драйвера
```
> sudo insmod driver.ko
> dmesg | taal

[   27.927368] 17:35:39.722611 automount vbsvcAutomounterMountIt: Successfully mounted 'shared' on '/media/sf_shared'
[   78.470079] loop6: detected capacity change from 0 to 8
[  153.041027] module: x86/modules: Skipping invalid relocation target, existing value is nonzero for type 1, loc 000000001419dd79, val ffffffffc0c2301d
[  453.450619] module: x86/modules: Skipping invalid relocation target, existing value is nonzero for type 1, loc 000000001419dd79, val ffffffffc0c2301d
[  683.408439] loop6: detected capacity change from 0 to 130608
[ 1208.435174] module: x86/modules: Skipping invalid relocation target, existing value is nonzero for type 1, loc 0000000014c11036, val ffffffffc0c2701d
[ 1872.497199] Module loaded
[ 2393.942025] Module removed
[ 2395.911513] Driver loaded, major=237
[ 2438.382155] PZ4 driver loaded
```


# 2. Реализовать чтение и запись в глобальный буфер
## read
static ssize_t read_f(struct file *f, char __user *u, size_t l, loff_t *off)
{
    if (wait_event_interruptible(read_queue, buf_len>0)) return -ERESTARTSYS;
    mutex_lock(&buf_mutex);
    if (l>buf_len) l=buf_len;
    if (copy_to_user(u, buf, l)) l=-EFAULT;
    else buf_len=0;
    mutex_unlock(&buf_mutex);
    return l;
}

// Ждёт данные, если буфер пуст (wait_event_interruptible)
// Копирует данные в user-space (copy_to_user)
// После чтения очищает буфер (buf_len=0)

## write 
```
static ssize_t write_f(struct file *f, const char __user *u, size_t l, loff_t *off)
{
    if (l>BUF_SIZE) l=BUF_SIZE;
    mutex_lock(&buf_mutex);
    if (copy_from_user(buf, u, l)) l=-EFAULT;
    else buf_len=l;
    mutex_unlock(&buf_mutex);
    wake_up_interruptible(&read_queue);
    return l;
}

// Копирует данные из user-space (copy_from_user)
// Обновляет buf_len
// Пробуждает процессы, ожидающие read() (wake_up_interruptible)
```


# 3. Реализовать ioctl для сброса содержимого буффера и получения информации чист ли буфер.



# 4. Написать пользовательское приложение для проверки



# 5. Проверка блокирующей операции read()



