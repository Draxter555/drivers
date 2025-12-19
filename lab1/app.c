#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "ioctl.h"

int main()
{
    int fd = open("/dev/my_char_dev", O_RDWR);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    // Цикл write + read
    for (int i = 0; i < 10; i++) {
        if (write(fd, &i, sizeof(i)) != sizeof(i)) {
            perror("write");
            break;
        }
        int val;
        if (read(fd, &val, sizeof(val)) != sizeof(val)) {
            perror("read");
            break;
        }
        printf("записали %d, прочитали %d\n", i, val);
    }

    // Читаем гистограмму (счётчик вызовов read)
    int histo;
    if (ioctl(fd, IOCTL_HISTO, &histo) < 0) {
        perror("ioctl");
    } else {
        printf("\nГистограмма (количество вызовов read): %d\n", histo);
        // Выводим "гистограмму" как звёздочки
        for (int i = 0; i < histo && i < 50; i++) {
            printf("*");
        }
        printf("\n");
    }

    close(fd);
    return 0;
}
