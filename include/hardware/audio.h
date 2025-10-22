// Minimal hardware/audio.h stub
#ifndef HARDWARE_AUDIO_H
#define HARDWARE_AUDIO_H

#include "hardware/hardware.h"

typedef struct audio_hw_device {
    struct hw_device_t common;
    int (*init_check)(const struct audio_hw_device *dev);
    int (*set_mode)(struct audio_hw_device *dev, int mode);
    int (*set_parameters)(struct audio_hw_device *dev, const char *kvpairs);
    char* (*get_parameters)(const struct audio_hw_device *dev, const char *keys);
} audio_hw_device_t;

typedef struct audio_module {
    struct hw_module_t common;
} audio_module_t;

#define AUDIO_MODULE_API_VERSION_0_1 0x00010000
#define HARDWARE_HAL_API_VERSION 0x00010000

#endif // HARDWARE_AUDIO_H
