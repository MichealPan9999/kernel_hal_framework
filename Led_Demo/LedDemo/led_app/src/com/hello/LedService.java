package com.hello;
import  android.util.Log; 

public class LedService {
//对于这种直接模式不需要进程通讯，所以可以不加extends ILedService.Stub
	/*
     * load native service.
     */
    static {
        Log.i ( "Java Service" , "Load Native Serivce LIB" );
        System.loadLibrary ( "led_runtime" );
        //加载jni的动态库，同时调用一个函数JNI_OnLoad
    }

    public LedService() {
        int icount ;
        Log.i ( "Java Service" , "do init Native Call" );
        _init ();
        icount = _get_count ();
        Log.d ( "Java Service" , "Init OK " );
    }

    /*
     * LED native methods.
     */
    public String set_on() {
        Log.i ( "com.hello.LedService" , "LED On" );
        _set_on ();
        return "led on" ;
    }
 

    public String set_off() {
        Log.i ( "com.hello.LedService" , "LED Off" );
        _set_off ();
        return "led off" ;
    }

     /*
     * declare all the native interface.//声明jni库可以提供的方法  
     */
    private static native boolean _init();
    private static native int _set_on();
    private static native int _set_off();
    private static native int _get_count(); 

}
