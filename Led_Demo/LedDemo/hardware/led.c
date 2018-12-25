#define LOG_TAG "LedStub"
#include <hardware/hardware.h>
#include <fcntl.h>
#include <errno.h>
#include <cutils/log.h>
#include <cutils/atomic.h>
#include <sys/ioctl.h>
#include "led.h"

#define GPG3DAT2_ON 0x4800
#define GPG3DAT2_OFF 0x4801

 //Ӳ��led���豸������ ����Ҳ������led_control_device_t�ṹ�ж����fd 
int fd;

static int led_device_close(struct hw_device_t* device)
{
	struct led_control_context_t* ctx = (struct led_control_context_t*)device;
	if (ctx) {
		free(ctx);
	}
	close(fd);
	return 0;
}

static int led_getcount(struct led_control_device_t *dev)
{
	LOGI("led_getcount");
	return 4;
}


static int led_set_on(struct led_control_device_t *dev)
{    
	//FIXME: do system call to control gpio led
	LOGI("led_set_on");
	ioctl(fd,GPG3DAT2_ON,NULL); //����Led���𣬺�Ӳ�����
	return 0;
} 

int led_set_off(struct led_control_device_t *dev)
{
	//FIXME: do system call to control gpio led
	LOGI("led_set_off");
	ioctl(fd,GPG3DAT2_OFF,NULL); //GPF0 1
	return 0;

}

static int led_device_open(const struct hw_module_t* module, const char* name,
	struct hw_device_t** device)
{
	struct led_control_context_t *context;
	LOGI("%s E ", __func__);
	context = (struct led_control_context_t *)malloc(sizeof(*context));
	memset(context, 0, sizeof(*context)); 

	//HAL must init property
	context->device.common.tag= HARDWARE_DEVICE_TAG;
	context->device.common.version = 0;
	context->device.common.module= module;
	context->device.common.close = led_device_close; 

	// ��ʼ������ API
	context->device.set_on= led_set_on;//ʵ����֧�ֵĲ���
	context->device.set_off= led_set_off;
	context->device.getcount_led = led_getcount;//��ʵ�������led_control_device_t��ַ���ظ�jni��  
                                                    //����jni��Ϳ���ֱ�ӵ���led_on��led_off��led_device_close�����ˡ�  
        //��Ӳ���豸 
	*device= (struct hw_device_t *)&(context->device);
	if((fd=open("/dev/led",O_RDWR))==-1)
	{
		LOGI("open error");
	//	exit(1);
	}else
	LOGI("open ok\n");

	return 0;
}


static struct hw_module_methods_t led_module_methods = {
    open: led_device_open  
};

//����������������ϵͳע����һ��IDΪLED_HARDWARE_MODULE_ID��stub��ע������HAL_MODULE_INFO_SYM�����Ʋ��ܸġ�  
const struct led_module_t HAL_MODULE_INFO_SYM = {
    common: {
        tag: HARDWARE_MODULE_TAG,
	    version_major: 1,
	    version_minor: 0,
	    id: LED_HARDWARE_MODULE_ID, 
	    name: "led HAL module",
	    author: "farsight",      //ʵ����һ��open�ķ�����jni����ã�  
                                    //�Ӷ�ʵ����led_control_device_t  
	    methods: &led_module_methods, 
	}, 
		/* supporting APIs go here */
};

