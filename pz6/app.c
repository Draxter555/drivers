#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>        // <-- добавили эту строку
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>

int main(int argc, char *argv[])
{
    int sock;
    struct ifreq ifr;

    // Создаём сокет для работы с сетевыми интерфейсами
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");
        exit(1);
    }

    // Указываем имя интерфейса
    strcpy(ifr.ifr_name, "my_pci_net");

    // Получаем MAC-адрес
    if (ioctl(sock, SIOCGIFHWADDR, &ifr) < 0) {
        perror("ioctl SIOCGIFHWADDR");
        close(sock);
        exit(1);
    }

    printf("MAC-адрес интерфейса %s: ", ifr.ifr_name);
    for (int i = 0; i < 6; i++) {
        printf("%02X%s", (unsigned char)ifr.ifr_hwaddr.sa_data[i],
               (i < 5) ? ":" : "\n");
    }

    close(sock);
    return 0;
}
