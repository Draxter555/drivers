# Задание
1. Собрать загрузочный образ для платы **BeagleBone Black** с использованием Buildroot.  
2. Записать образ на SD-карту.  
3. Загрузить плату и убедиться, что система работает.

### 1. Скачивание исходного кода Buildroot
```
git clone https://git.buildroot.net/buildroot
cd buildroot
git checkout 2022.11
```


### 2. Сборка
Предварительно установил:
```
sudo apt install libncurses5-dev libncursesw5-dev
```
```
> make menuconfig
```

<img width="931" height="593" alt="image" src="https://github.com/user-attachments/assets/10213083-078d-4fa6-9fd2-1f7ed836bbc5" />
