package com.hello;

import com.hello.LedService;// 导入Framework层的LedService

import com.hello.R;
import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;

public class LedDemo extends Activity {
	static LedService led_srv;//实例化LedService  
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
         	          title = led_srv.set_off();  //通过LedService提供的方法，控制底层硬件  
         	          btn.setText("Turn On");
         	          setTitle(title);
         	          iflag = false;
         	        } else {
         	           title = led_srv.set_on();  //通过LedService提供的方法，控制底层硬件  
         	           btn.setText("Turn Off");
         	           setTitle(title);
         	           iflag = true;
         	        }        		
 			}        	
         });
     }    
}
