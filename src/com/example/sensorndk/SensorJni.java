
package com.example.sensorndk;

import android.app.Activity;

import android.widget.TextView;
import android.os.Bundle;


public class SensorJni extends Activity
{
	
	static TextView  tv;
    
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        tv = new TextView(this);
        setContentView(tv);
        
        /*Called in sensor-jni.c as Java_com_example_sensorndk_SensorJni_stringFromJNI(JNIEnv* env,jobject thiz)*/
        stringFromJNI();
       
        
    }

    /*Method written in sensor-jni.c needs to be defined as native */
    public native String  stringFromJNI();
    
    /*Called in sensor-jni.c when it receives a new sensor value */
    public static void sensor_callback(String value) {
       
        tv.setText(value);
        
    }
    
    
    /* For loading the libsensor-jni.so library which we created using ndk-build*/
    static {
        System.loadLibrary("sensor-jni");
    }
}
