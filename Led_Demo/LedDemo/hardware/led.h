#include <hardware/hardware.h>
#include <fcntl.h>
#include <errno.h>
#include <cutils/log.h>
#include <cutils/atomic.h>

#define LED_HARDWARE_MODULE_ID "led"


//HAL �涨����ֱ��ʹ��hw_module_t�ṹ�������Ҫ����ôһ���̳С�
struct led_module_t {
	struct hw_module_t common; 
	/* support API for LEDServices constructor */
};


//�Զ����һ�����Led���ƵĽṹ������hw_device_t��֧�ֵ�API���� 
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
