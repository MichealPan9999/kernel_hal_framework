#include <hardware/hardware.h>
#include <fcntl.h>
#include <errno.h>
#include <cutils/log.h>
#include <cutils/atomic.h>

#define LED_HARDWARE_MODULE_ID "led"


//HAL 规定不能直接使用hw_module_t结构，因此需要做这么一个继承。
struct led_module_t {
	struct hw_module_t common; 
	/* support API for LEDServices constructor */
};


//自定义的一个针对Led控制的结构，包含hw_device_t和支持的API操作 
struct led_control_device_t {
	struct hw_device_t common;
	/* supporting control APIs go here */
	int (*getcount_led)(struct led_control_device_t *dev);
	int (*set_on)(struct led_control_device_t *dev);
	int (*set_off)(struct led_control_device_t *dev);
};

struct led_control_context_t {
	struct led_control_device_t device;
}; 
