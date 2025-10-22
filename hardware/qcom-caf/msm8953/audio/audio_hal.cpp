#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define LOG_TAG "audio_primary_msm8953"
#include <cutils/log.h>

#include <hardware/hardware.h>
#include <system/audio.h>
#include <hardware/audio.h>

/*
 * Minimal legacy audio HAL skeleton for LineageOS 22.1 (Android 13) to be
 * compiled inside the platform tree with mmm. This provides the basic
 * module descriptor and an open function returning a minimal device.
 *
 * IMPORTANT: This is a stub for testing and does not implement real audio
 * functionality. It exports the legacy HAL symbol expected by platform.
 */

struct rahal_device {
    struct audio_hw_device device;
    /* add device state here if needed */
};

static int rahal_init_check(const struct audio_hw_device *dev) {
    (void)dev;
    ALOGI("rahal: init_check");
    return 0;
}

static int rahal_set_mode(struct audio_hw_device *dev, audio_mode_t mode) {
    (void)dev; (void)mode;
    ALOGI("rahal: set_mode %d", mode);
    return 0;
}

static int rahal_set_parameters(struct audio_hw_device *dev, const char *kvpairs) {
    (void)dev; (void)kvpairs;
    ALOGI("rahal: set_parameters %s", kvpairs ? kvpairs : "");
    return 0;
}

static char* rahal_get_parameters(const struct audio_hw_device *dev, const char *keys) {
    (void)dev; (void)keys;
    ALOGI("rahal: get_parameters %s", keys ? keys : "");
    return strdup("");
}

static int rahal_close(hw_device_t *device) {
    struct rahal_device *dev = (struct rahal_device *)device;
    if (!dev) return -EINVAL;
    ALOGI("rahal: close");
    free(dev);
    return 0;
}

static int rahal_open(const hw_module_t* module, const char* name, hw_device_t** device) {
    (void)module;
    if (strcmp(name, AUDIO_HARDWARE_INTERFACE) != 0) {
        ALOGE("rahal: open called with unexpected name %s", name);
        return -EINVAL;
    }

    struct rahal_device *dev = (struct rahal_device *)calloc(1, sizeof(*dev));
    if (!dev) return -ENOMEM;

    dev->device.common.tag = HARDWARE_DEVICE_TAG;
    dev->device.common.version = 0;
    dev->device.common.module = (struct hw_module_t *)module;
    dev->device.common.close = rahal_close;

    dev->device.init_check = rahal_init_check;
    dev->device.set_mode = rahal_set_mode;
    dev->device.set_parameters = rahal_set_parameters;
    dev->device.get_parameters = rahal_get_parameters;

    *device = &dev->device.common;
    ALOGI("rahal: opened audio device");
    return 0;
}

/* Module descriptor */
struct hw_module_methods_t rahal_module_methods = {
    .open = rahal_open,
};

struct audio_module HAL_MODULE_INFO_SYM = {
    .common = {
        .tag = HARDWARE_MODULE_TAG,
        .module_api_version = AUDIO_MODULE_API_VERSION_0_1,
        .hal_api_version = HARDWARE_HAL_API_VERSION,
        .id = AUDIO_HARDWARE_MODULE_ID,
        .name = "Minimal audio primary HAL for msm8953",
        .author = "lineageos-hal stub",
        .methods = &rahal_module_methods,
    }
};
typedef struct hw_device_t hw_device_t;

struct hw_module_methods_t {
    int (*open)(const hw_module_t* module, const char* id, hw_device_t** device);
};

struct hw_module_t {
    uint32_t tag;
    uint16_t module_api_version_major;
    uint16_t module_api_version_minor;
    const char* id;
    const char* name;
    const char* author;
    const hw_module_methods_t* methods;
};

struct hw_device_t {
    uint32_t tag;
    uint16_t version;
    const hw_module_t* module;
    int (*close)(hw_device_t* device);
};

// Minimal audio_hw_device - just extend hw_device_t
typedef struct audio_hw_device {
    struct hw_device_t common;
    // real HAL would have many function pointers here
} audio_hw_device;

static int audio_device_close(hw_device_t* device) {
    ALOGI("audio_device_close called");
    if (device) free(device);
    return 0;
}

static int audio_module_open(const hw_module_t* module, const char* id, hw_device_t** device) {
    (void)id;
    ALOGI("audio_module_open called for module=%s id=%s", module ? module->name : "<null>", id ? id : "<null>");
    audio_hw_device* dev = (audio_hw_device*)malloc(sizeof(audio_hw_device));
    if (!dev) return -1;
    memset(dev, 0, sizeof(*dev));
    dev->common.tag = 0xA57F; // arbitrary
    dev->common.version = 1;
    dev->common.module = module;
    dev->common.close = audio_device_close;
    *device = &dev->common;
    return 0;
}

static const hw_module_methods_t audio_module_methods = {
    .open = audio_module_open,
};

// Exported module symbol expected by HAL loader
__attribute__((visibility("default")))
hw_module_t HAL_MODULE_INFO_SYM = {
    .tag = 0xF00D,
    .module_api_version_major = 1,
    .module_api_version_minor = 0,
    .id = "audio",
    .name = "Minimal audio primary HAL for msm8953",
    .author = "lineageos-hal-stub",
    .methods = &audio_module_methods,
};

} // extern "C"
