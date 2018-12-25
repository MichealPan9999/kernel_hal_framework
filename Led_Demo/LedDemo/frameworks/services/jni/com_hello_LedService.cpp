#define LOG_TAG "LedService"
#include "utils/Log.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <jni.h>
#include "../../../hardware/led.h"

static led_control_device_t *sLedDevice = 0;
static led_module_t* sLedModule=0;

static jint get_count(void)
{
	LOGI("%s E", __func__);
	if(sLedDevice)
		sLedDevice->getcount_led(sLedDevice);
	else
		LOGI("sLedDevice is null");
	return 0;
}

static jint led_setOn(JNIEnv* env, jobject thiz) {
	LOGI("%s E", __func__);
	if (sLedDevice) {
		sLedDevice->set_on(sLedDevice);//调用hal层的注册的方法  
	}else{
		LOGI("sLedDevice is null");
	}
	return 0;
}



static jint led_setOff(JNIEnv* env, jobject thiz) {
	LOGI("%s E", __func__);
	if (sLedDevice) {
		sLedDevice->set_off(sLedDevice);//调用hal层的注册的方法 
	}else{
		LOGI("sLedDevice is null");
	}

	return 0;
}

/** helper APIs */
static inline int led_control_open(const struct hw_module_t* module,
	struct led_control_device_t** device) {
	LOGI("%s E ", __func__);
	return module->methods->open(module,
		LED_HARDWARE_MODULE_ID, (struct hw_device_t**)device);
		//这个过程非常重要，jni通过LED_HARDWARE_MODULE_ID找到对应的stub  
}

static jint led_init(JNIEnv *env, jclass clazz)
{
	led_module_t const * module;
	LOGI("%s E ", __func__);

	if (hw_get_module(LED_HARDWARE_MODULE_ID, (const hw_module_t**)&module) == 0) {
	//根据LED_HARDWARE_MODULE_ID找到hw_module_t，参考hal层的实现
		LOGI("get Module OK");     
		sLedModule = (led_module_t *) module;
		if (led_control_open(&module->common, &sLedDevice) != 0) {
		//通过hw_module_t找到led_control_device_t  
			LOGI("led_init error");
			return -1;
		}
	}

	LOGI("led_init success");
	return 0;
}

/*
 *
 ** Array of methods.
 ** Each entry has three fields: the name of the method, the method
 ** signature, and a pointer to the native implementation.
 */ 
 
 /* 
*JNINativeMethod是jni层注册的方法，Framework层可以使用这些方法 
*_init 、_set_on、_set_off是在Framework中调用的方法名称，函数的类型及返回值如下： 
*（）Z   无参数    返回值为bool型 
* (I)Z   整型参数  返回值为bool型 
*/
static const JNINativeMethod gMethods[] = {
//Framework层调用_init时映射
	{"_init",       "()Z",(void*)led_init},
	{ "_set_on",    "()I",(void*)led_setOn },
	{ "_set_off",   "()I",(void*)led_setOff },
	{ "_get_count", "()I",(void*)get_count },
};

static int registerMethods(JNIEnv* env) {
	static const char* const kClassName = "com/hello/LedService";
	//注意：必须和你Framework层的service类名相同 
	jclass clazz; 
	/* look up the class */
	clazz = env->FindClass(kClassName);
	if (clazz == NULL) {
		LOGE("Can't find class %s\n", kClassName);
		return -1;
	} 

	/* register all the methods */
	if (env->RegisterNatives(clazz, gMethods,
			sizeof(gMethods) / sizeof(gMethods[0])) != JNI_OK)
	{
		LOGE("Failed registering methods for %s\n", kClassName);
		return -1;
	}
	/* fill out the rest of the ID cache */
	return 0;
} 

/*
 *
 *   * This is called by the VM when the shared library is first loaded.
 */ 
jint JNI_OnLoad(JavaVM* vm, void* reserved) {
//Framework层加载jni库时调用 
	JNIEnv* env = NULL;
	jint result = -1;
	LOGI("JNI_OnLoad");

	if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
		LOGE("ERROR: GetEnv failed\n");
		goto fail;
	}

	assert(env != NULL);
	if (registerMethods(env) != 0) { //注册你的JNINativeMethod  
		LOGE("ERROR: PlatformLibrary native registration failed\n");
		goto fail;
	}
	/* success -- return valid version number */	
	result = JNI_VERSION_1_4;

fail:
	return result;
} 


