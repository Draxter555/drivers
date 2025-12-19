#ifndef __IOCTL_H
#define __IOCTL_H

#include <linux/ioctl.h>

#define HISTO_MAX 500

#define IOCTL_HISTO_LEN _IOR('k', 0, size_t)
#define IOCTL_HISTO_BUF _IOR('k', 1, size_t[HISTO_MAX])

#endif
