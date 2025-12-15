#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    int fd = open("/dev/main", O_RDWR);
    if(fd < 0) {
        perror("open");
        return 1;
    }
    printf("Device opened successfully\n");
    close(fd);
    return 0;
}
