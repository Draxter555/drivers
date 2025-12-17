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



# 3. Реализовать ioctl для сброса содержимого буффера и получения информации чист ли буфер.



# 4. Написать пользовательское приложение для проверки



# 5. Проверка блокирующей операции read()


