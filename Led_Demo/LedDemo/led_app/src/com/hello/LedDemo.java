package com.hello;

import com.hello.LedService;// ����Framework���LedService

import com.hello.R;
import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;

public class LedDemo extends Activity {
	static LedService led_srv;//ʵ����LedService  
    static Button btn;
    static boolean iflag = false;
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
    	 super.onCreate(savedInstanceState);
         setContentView(R.layout.main);
         Log.i("Java App", "OnCreate");
         led_srv = new LedService();
         btn = (Button)this.findViewById(R.id.Button01);
         
         btn.setOnClickListener(new OnClickListener(){
         	public void onClick(View v){
         		 Log.i("Java App", "btnOnClicked");
         	     String title = new String();
         	     if (iflag) {
         	          title = led_srv.set_off();  //ͨ��LedService�ṩ�ķ��������Ƶײ�Ӳ��  
         	          btn.setText("Turn On");
         	          setTitle(title);
         	          iflag = false;
         	        } else {
         	           title = led_srv.set_on();  //ͨ��LedService�ṩ�ķ��������Ƶײ�Ӳ��  
         	           btn.setText("Turn Off");
         	           setTitle(title);
         	           iflag = true;
         	        }        		
 			}        	
         });
     }    
}
