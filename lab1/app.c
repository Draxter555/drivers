#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <time.h>

#include "ioctl.h"

int main()
{
    int fd = open("/dev/lab1_dev", O_RDWR);
    if (fd < 0) {
        perror("open /dev/lab1_dev");
        return 1;
    }

    const int N = 1000;
    for (int i = 0; i < N; i++) {
        int val = i;
        if (write(fd, &val, sizeof(val)) != sizeof(val)) {
            perror("write");
            break;
        }
        if (read(fd, &val, sizeof(val)) != sizeof(val)) {
            perror("read");
            break;
        }
        usleep(100 + (i % 50) * 10); // задержка 100–600 мкс
    }

    size_t histo_len;
    if (ioctl(fd, IOCTL_HISTO_LEN, &histo_len) < 0) {
        perror("ioctl HISTO_LEN");
        close(fd);
        return 1;
    }

    size_t *histo = calloc(HISTO_MAX, sizeof(size_t));
    if (!histo) {
        perror("calloc");
        close(fd);
        return 1;
    }

    if (ioctl(fd, IOCTL_HISTO_BUF, histo) < 0) {
        perror("ioctl HISTO_BUF");
        free(histo);
        close(fd);
        return 1;
    }

    printf("Гистограмма задержек между write и read (бин = 50 мкс):\n");
    for (size_t i = 0; i < histo_len && i < 30; i++) {
        if (histo[i] > 0) {
            printf("[%3zu–%3zu) мкс: %zu\n", i * 50, (i + 1) * 50, histo[i]);
        }
    }

    free(histo);
    close(fd);
    return 0;
}
