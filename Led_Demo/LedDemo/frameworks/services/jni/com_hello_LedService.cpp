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
		sLedDevice->set_on(sLedDevice);//����hal���ע��ķ���  
	}else{
		LOGI("sLedDevice is null");
	}
	return 0;
}



static jint led_setOff(JNIEnv* env, jobject thiz) {
	LOGI("%s E", __func__);
	if (sLedDevice) {
		sLedDevice->set_off(sLedDevice);//����hal���ע��ķ��� 
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
		//������̷ǳ���Ҫ��jniͨ��LED_HARDWARE_MODULE_ID�ҵ���Ӧ��stub  
}

static jint led_init(JNIEnv *env, jclass clazz)
{
	led_module_t const * module;
	LOGI("%s E ", __func__);

	if (hw_get_module(LED_HARDWARE_MODULE_ID, (const hw_module_t**)&module) == 0) {
	//����LED_HARDWARE_MODULE_ID�ҵ�hw_module_t���ο�hal���ʵ��
		LOGI("get Module OK");     
		sLedModule = (led_module_t *) module;
		if (led_control_open(&module->common, &sLedDevice) != 0) {
		//ͨ��hw_module_t�ҵ�led_control_device_t  
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
*JNINativeMethod��jni��ע��ķ�����Framework�����ʹ����Щ���� 
*_init ��_set_on��_set_off����Framework�е��õķ������ƣ����������ͼ�����ֵ���£� 
*����Z   �޲���    ����ֵΪbool�� 
* (I)Z   ���Ͳ���  ����ֵΪbool�� 
*/
static const JNINativeMethod gMethods[] = {
//Framework�����_initʱӳ��
	{"_init",       "()Z",(void*)led_init},
	{ "_set_on",    "()I",(void*)led_setOn },
	{ "_set_off",   "()I",(void*)led_setOff },
	{ "_get_count", "()I",(void*)get_count },
};

static int registerMethods(JNIEnv* env) {
	static const char* const kClassName = "com/hello/LedService";
	//ע�⣺�������Framework���service������ͬ 
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
//Framework�����jni��ʱ���� 
	JNIEnv* env = NULL;
	jint result = -1;
	LOGI("JNI_OnLoad");

	if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
		LOGE("ERROR: GetEnv failed\n");
		goto fail;
	}

	assert(env != NULL);
	if (registerMethods(env) != 0) { //ע�����JNINativeMethod  
		LOGE("ERROR: PlatformLibrary native registration failed\n");
		goto fail;
	}
	/* success -- return valid version number */	
	result = JNI_VERSION_1_4;

fail:
	return result;
} 


