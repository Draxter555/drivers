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
# 2. Вывод утилиты strace
execve("./main", ["./main"], 0x7ffc8fe2db40 /* 28 vars */) = 0
brk(NULL)                               = 0x555b93bf0000
arch_prctl(0x3001 /* ARCH_??? */, 0x7ffd1973d3e0) = -1 EINVAL (Invalid argument)
mmap(NULL, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7f89e74b0000
access("/etc/ld.so.preload", R_OK)      = -1 ENOENT (No such file or directory)
openat(AT_FDCWD, "/etc/ld.so.cache", O_RDONLY|O_CLOEXEC) = 3
newfstatat(3, "", {st_mode=S_IFREG|0644, st_size=22303, ...}, AT_EMPTY_PATH) = 0
mmap(NULL, 22303, PROT_READ, MAP_PRIVATE, 3, 0) = 0x7f89e74aa000
close(3)                                = 0
openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libc.so.6", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\3\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0P\237\2\0\0\0\0\0"..., 832) = 832
pread64(3, "\6\0\0\0\4\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0"..., 784, 64) = 784
pread64(3, "\4\0\0\0 \0\0\0\5\0\0\0GNU\0\2\0\0\300\4\0\0\0\3\0\0\0\0\0\0\0"..., 48, 848) = 48
pread64(3, "\4\0\0\0\24\0\0\0\3\0\0\0GNU\0O{\f\225\\=\201\327\312\301P\32$\230\266\235"..., 68, 896) = 68
newfstatat(3, "", {st_mode=S_IFREG|0755, st_size=2220400, ...}, AT_EMPTY_PATH) = 0
pread64(3, "\6\0\0\0\4\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0"..., 784, 64) = 784
mmap(NULL, 2264656, PROT_READ, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7f89e7281000
mprotect(0x7f89e72a9000, 2023424, PROT_NONE) = 0
mmap(0x7f89e72a9000, 1658880, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x28000) = 0x7f89e72a9000
mmap(0x7f89e743e000, 360448, PROT_READ, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x1bd000) = 0x7f89e743e000
mmap(0x7f89e7497000, 24576, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x215000) = 0x7f89e7497000
mmap(0x7f89e749d000, 52816, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, -1, 0) = 0x7f89e749d000
close(3)                                = 0
mmap(NULL, 12288, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7f89e727e000
arch_prctl(ARCH_SET_FS, 0x7f89e727e740) = 0
set_tid_address(0x7f89e727ea10)         = 1211
set_robust_list(0x7f89e727ea20, 24)     = 0
rseq(0x7f89e727f0e0, 0x20, 0, 0x53053053) = 0
mprotect(0x7f89e7497000, 16384, PROT_READ) = 0
mprotect(0x555b8bc8d000, 4096, PROT_READ) = 0
mprotect(0x7f89e74ea000, 8192, PROT_READ) = 0
prlimit64(0, RLIMIT_STACK, NULL, {rlim_cur=8192*1024, rlim_max=RLIM64_INFINITY}) = 0
munmap(0x7f89e74aa000, 22303)           = 0
openat(AT_FDCWD, "/dev/zero", O_RDONLY) = 3
read(3, "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"..., 100) = 100
close(3)                                = 0
exit_group(0)                           = ?
+++ exited with 0 +++

# 2. 
