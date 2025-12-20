# Задание
1. Написать символьный драйвер с open, close, read, write, ioctl
  write записывает int во внутренний буфер
  read читает int из внутреннего буфера

3. Написать тестовое приложение
  Приложение должно:
    - В цикле вызывать write и read
    - Выводить гистограмму с помощью ioctl
  
# Компиляция драйвера и загрузка ядра
```
> make
> sudo insmod main.ko
```

# ПРИЛОЖЕНИЕ:

Открывает символьное устройство /dev/lab1_dev в режиме чтения и записи.
---

В цикле 1000 раз выполняет:
```
write(fd, &value, sizeof(value));
read(fd, &value, sizeof(value));
```
То есть записывает целое число в устройство и тут же читает его обратно.

После цикла запрашивает гистограмму через ioctl
---
```
ioctl(fd, IOCTL_HISTO_BUF, histogram_buffer);
```


## ВЫзов тесового приложения
```
> make app
> ./app

dev_write_single: 0
dev_read_single: 0
/
/
/
dev_write_single: 999
dev_read_single: 999
Histogram length: 70
0:	500
1:	0
2:	0
3:	1
4:	4
5:	1
6:	1
7:	0
8:	0
9:	0
10:	0
11:	0
12:	0
13:	0
14:	1

```
