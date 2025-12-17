#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define IOCTL_CLEAR _IO('q',1)
#define IOCTL_HASDATA _IOR('q',2,int)

int main()
{
    const char *dev="/dev/pz4_dev";
    char buf[256];
    int fd = open(dev,O_RDWR);
    if(fd<0){ printf("не удалось открыть %s\n",dev); return 1; }

    // Проверка блокирующего read
    printf("пытаемся прочитать (будет ждать данные)...\n");
    int r = read(fd,buf,sizeof(buf)-1);
    buf[r]=0;
    printf("прочитано: %s\n",buf);

    // Проверка write
    write(fd,"hello",5);
    printf("записали 'hello'\n");

    // ioctl HASDATA
    int has=0;
    ioctl(fd,IOCTL_HASDATA,&has);
    printf("hasdata=%d\n",has);

    // ioctl CLEAR
    ioctl(fd,IOCTL_CLEAR);
    ioctl(fd,IOCTL_HASDATA,&has);
    printf("после очистки hasdata=%d\n",has);

    close(fd);
    return 0;
}
