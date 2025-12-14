# 1. Написать программу для чтения данных с устройства /dev/zero:
```
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(void) {
    char buffer[100];
    ssize_t bytes_read;

    int fd = open("/dev/zero", O_RDONLY);
    if (fd < 0) {
        perror("open");
        return EXIT_FAILURE;
    }

    bytes_read = read(fd, buffer, sizeof(buffer));
    if (bytes_read < 0) {
        perror("read");
        close(fd);
        return EXIT_FAILURE;
    }

    close(fd);
    return EXIT_SUCCESS;
}
```
