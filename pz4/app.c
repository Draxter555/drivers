// test_app.c
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>

#define IOCTL_RESET  _IO('q', 1)
#define IOCTL_IS_EMPTY _IOR('q', 2, int)

int main() {
    int fd = open("/dev/simple_dev", O_RDWR);
    if(fd < 0){ perror("open"); return 1; }

    char buf[100];
    write(fd, "hello", 5);
    read(fd, buf, 5);
    buf[5] = 0;
    printf("Read: %s\n", buf);

    int empty;
    ioctl(fd, IOCTL_IS_EMPTY, &empty);
    printf("Buffer empty? %d\n", empty);

    ioctl(fd, IOCTL_RESET, 0);
    ioctl(fd, IOCTL_IS_EMPTY, &empty);
    printf("After reset, buffer empty? %d\n", empty);

    close(fd);
    return 0;
}
