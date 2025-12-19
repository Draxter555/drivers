#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <time.h>

#include "ioctl.h"

int main()
{
    int fd = open("/lab1", O_RDWR);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    const int N = 1000;
    for (int i = 0; i < N; i++) {
        int val = i;
        write(fd, &val, sizeof(val));
        read(fd, &val, sizeof(val));
        // небольшая задержка, чтобы были заметные задержки
        usleep(100 + (i % 50) * 10); // 100–600 мкс
    }

    // Получаем длину гистограммы
    size_t histo_len;
    if (ioctl(fd, IOCTL_HISTO_LEN, &histo_len) < 0) {
        perror("ioctl LEN");
        close(fd);
        return 1;
    }

    // Получаем буфер
    size_t *histo = calloc(HISTO_MAX, sizeof(size_t));
    if (!histo) {
        perror("calloc");
        close(fd);
        return 1;
    }

    if (ioctl(fd, IOCTL_HISTO_BUF, histo) < 0) {
        perror("ioctl BUF");
        free(histo);
        close(fd);
        return 1;
    }

    printf("Гистограмма задержек между write и read (бин = 50 мкс):\n");
    for (size_t i = 0; i < histo_len && i < 20; i++) { // выводим первые 20 бинов
        if (histo[i] > 0) {
            printf("[%3zu–%3zu) мкс: %zu\n", i*50, (i+1)*50, histo[i]);
        }
    }

    free(histo);
    close(fd);
    return 0;
}
