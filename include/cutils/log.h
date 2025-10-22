// Minimal cutils/log.h stub for building HAL without full AOSP
#ifndef _CUTILS_LOG_H
#define _CUTILS_LOG_H

#include <stdio.h>
#include <stdarg.h>

#define ALOGI(fmt, ...) fprintf(stderr, "I/%s: " fmt "\n", "audio_hal", ##__VA_ARGS__)
#define ALOGE(fmt, ...) fprintf(stderr, "E/%s: " fmt "\n", "audio_hal", ##__VA_ARGS__)

#endif // _CUTILS_LOG_H
