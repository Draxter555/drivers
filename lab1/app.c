#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "ioctl.h"

int main()
{
    int fd = open("/dev/lab1_dev", O_RDWR);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    // Пишем и читаем с задержкой МЕЖДУ ними
    for (int i = 0; i < 500; i++) {
        write(fd, &i, sizeof(i));
        usleep(80 + (i % 10) * 30);  // 80–350 мкс → бины 1–7
        read(fd, &i, sizeof(i));
    }

    // Получаем всю гистограмму
    size_t histo[HISTO_MAX];
    if (ioctl(fd, IOCTL_HISTO_BUF, histo) < 0) {
        perror("ioctl");
        close(fd);
        return 1;
    }

    // Выводим ТОЛЬКО бины 0–14
    for (int i = 0; i < 15; i++) {
        printf("%d:\t%zu\n", i, histo[i]);
    }

    close(fd);
    return 0;
}
