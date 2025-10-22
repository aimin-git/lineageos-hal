// Minimal hardware/hardware.h stub
#ifndef HARDWARE_H
#define HARDWARE_H

typedef struct hw_module_t {
    unsigned int tag;
    unsigned int module_api_version;
    unsigned int hal_api_version;
    const char * id;
    const char * name;
    const char * author;
    struct hw_module_methods_t *methods;
} hw_module_t;

typedef struct hw_device_t {
    struct hw_module_t *module;
    int (*close)(struct hw_device_t*);
} hw_device_t;

struct hw_module_methods_t {
    int (*open)(const struct hw_module_t*, const char*, struct hw_device_t**);
};

#define HARDWARE_MODULE_TAG 0x87654321
#define HARDWARE_DEVICE_TAG 0x12345678

#endif // HARDWARE_H
