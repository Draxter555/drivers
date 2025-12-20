#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "ioctl.h"

// Чтение одного значения из устройства
static void dev_read_single(const int fd)
{
    int value = 0;
    ssize_t read_len = read(fd, &value, sizeof(value));

    if (read_len < 0) {
        printf("dev_read_single: read failed: %ld\n", read_len);
        return;
    }

    printf("dev_read_single: %d\n", value);
}

// Запись одного значения в устройство
static void dev_write_single(const int fd, const int value)
{
    ssize_t wrote_len = write(fd, &value, sizeof(value));

    if (wrote_len < 0) {
        printf("dev_write_single: write failed: %ld\n", wrote_len);
        return;
    }

    printf("dev_write_single: %d\n", value);
}

int main()
{
    const char dev_path[] = "/dev/lab1_dev";

    int fd_r = open(dev_path, O_RDONLY);
    if (fd_r < 0) {
        printf("Failed to open %s for reading: %d\n", dev_path, fd_r);
        return fd_r;
    }

    int fd_w = open(dev_path, O_WRONLY);
    if (fd_w < 0) {
        printf("Failed to open %s for writing: %d\n", dev_path, fd_w);
        close(fd_r);
        return fd_w;
    }

    // Генерация нагрузки: задержка МЕЖДУ write и read!
    for (size_t i = 0; i < 1000; i++) {
        dev_write_single(fd_w, (int)i);
        usleep(100 + (i % 50) * 10);  // 100–590 мкс между write и read
        dev_read_single(fd_r);
    }

    // Получение длины гистограммы
    size_t histo_len = 0;
    if (ioctl(fd_r, IOCTL_HISTO_LEN, &histo_len) < 0) {
        printf("Failed to get histogram length\n");
        close(fd_r);
        close(fd_w);
        return -1;
    }

    printf("Histogram length: %zu\n", histo_len);

    // Получение буфера гистограммы
    size_t *histo_buf = malloc(HISTO_MAX * sizeof(size_t));
    if (!histo_buf) {
        printf("malloc failed\n");
        close(fd_r);
        close(fd_w);
        return -1;
    }

    if (ioctl(fd_r, IOCTL_HISTO_BUF, histo_buf) < 0) {
        printf("Failed to get histogram buffer\n");
        free(histo_buf);
        close(fd_r);
        close(fd_w);
        return -1;
    }

    // Выводим только первые 20 ненулевых бинов (или меньше)
    size_t printed = 0;
    for (size_t i = 0; i < histo_len && printed < 20; i++) {
        if (histo_buf[i] > 0) {
            printf("%zu:\t%zu\n", i, histo_buf[i]);
            printed++;
        }
    }

    free(histo_buf);
    close(fd_r);
    close(fd_w);
    return 0;
}
