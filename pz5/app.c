#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    unsigned char buf[24];
    int fd = open("/dev/rawdemo", O_RDONLY);
    if(fd < 0) { perror("open"); return 1; }
    read(fd, buf, sizeof(buf));
    close(fd);

    printf("RAW HEX:");
    for(int i=0;i<24;i++)
        printf(" %02x", buf[i]);
    printf("\nASCII:");
    for(int i=0;i<24;i++)
        if(buf[i]>=32 && buf[i]<=126)
            printf(" %c", buf[i]);
        else
            printf(" \\x%02x", buf[i]);
    printf("\n");
    return 0;
}
