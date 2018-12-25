#include <hardware/hardware.h>
#include <hardware/hello.h>
#include <fcntl.h>
#include <errno.h>
#include <cutils/log.h>
#include <cutils/atomic.h>
#include <android/log.h>
#include <unistd.h> // for close read write
#include <stdlib.h> // free
#include <errno.h> // implicit declaration of function 'strerror'
#include <string.h> // memset
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,"hello_stub",__VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,"hello_stub",__VA_ARGS__)

#define LOG_TAG "HelloStub"

#define DEVICE_NAME "/dev/hello"
#define MODULE_NAME "Hello"
#define MODULE_AUTHOR "y041039@gmail.com"

/*设备打开和关闭接口*/
static int hello_device_open(const struct hw_module_t* module, const char* name, struct hw_device_t** device);
static int hello_device_close(struct hw_device_t* device);

/*设备访问接口*/
static int hello_set_val(struct hello_device_t* dev, int val);
static int hello_get_val(struct hello_device_t* dev, int* val);

/*模块方法表*/
static struct hw_module_methods_t hello_module_methods = {
 open: hello_device_open
};

/*模块实例变量*/
struct hello_module_t HAL_MODULE_INFO_SYM = {
 common: {
 tag: HARDWARE_MODULE_TAG,
 version_major: 1,
 version_minor: 0,
 id: HELLO_HARDWARE_MODULE_ID,
 name: MODULE_NAME,
 author: MODULE_AUTHOR,
 methods: &hello_module_methods,
}
};
/* 这里，实例变量名必须为HAL_MODULE_INFO_SYM，tag 也必须为HARDWARE_MODULE_TAG，
这是Android 硬件抽象层规范规定的 */


static int hello_device_open(const struct hw_module_t* module, const char* name, struct hw_device_t** device)
{
        struct hello_device_t* dev;dev = (struct hello_device_t*)malloc(sizeof(struct hello_device_t));

      if(!dev)
      {
        LOGE("Hello Stub: failed to alloc space");
        return -EFAULT;
      }

      memset(dev, 0, sizeof(struct hello_device_t));
      dev->common.tag = HARDWARE_DEVICE_TAG;
      dev->common.version = 0;
      dev->common.module = (hw_module_t*)module;
      dev->common.close = hello_device_close;
      dev->set_val = hello_set_val;dev->get_val = hello_get_val;

      if((dev->fd = open(DEVICE_NAME, O_RDWR)) == -1) {
        LOGE("Hello Stub: failed to open /dev/hello -- %s.", strerror(errno));free(dev);
        return -EFAULT;
      }

      *device = &(dev->common);
      LOGI("Hello Stub: open /dev/hello successfully.");

      return 0;
}

/*

DEVICE_NAME 定义为"/dev/hello"。由于设备文件是在内核驱动里面通过device_create 创建的，而device_create 创建的设备文
件默认只有root 用户可读写，而hello_device_open 一般是由上层APP 来调用的，这些APP 一般不具有root 权限，这时候就
导致打开设备文件失败：
Hello Stub: failed to open /dev/hello -- Permission denied.
解决办法是类似于Linux 的udev 规则，打开Android 源代码工程目录下，进入到system/core/rootdir 目录，里面有一个名为
uevent.rc 文件，往里面添加一行：
/dev/hello 0666 root root

*/


static int hello_device_close(struct hw_device_t* device)
{
    struct hello_device_t* hello_device = (struct hello_device_t*)device;

    if(hello_device)
    {
        close(hello_device->fd);
        free(hello_device);
    }

    return 0;
}

static int hello_set_val(struct hello_device_t* dev, int val)
{
    LOGI("Hello Stub: set value %d to device.", val);

    write(dev->fd, &val, sizeof(val));

    return 0;
}

static int hello_get_val(struct hello_device_t* dev, int* val)
{
    if(!val)
    {
        LOGE("Hello Stub: error val pointer");
        return -EFAULT;
    }

    read(dev->fd, val, sizeof(*val));

    LOGI("Hello Stub: get value %d from device", *val);

    return 0;
}
