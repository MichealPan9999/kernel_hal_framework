package android.os;

import android.app.ActivityThread;
import android.content.Context;

public class HelloService extends IHelloService.Stub {
private static final String TAG = "HelloService";

HelloService() 
{
     init_native();
}

public void setVal(int val) 
{
    setVal_native(val);
}

public int getVal() 
{
    return getVal_native();
}

private static native boolean init_native();
private static native void setVal_native(int val);
private static native int getVal_native();

};
