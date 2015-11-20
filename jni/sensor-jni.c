#include <string.h>
#include <jni.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <android/sensor.h>
#include <android/looper.h>
#include <android/log.h>
#include <inttypes.h>


#define LOOPER_ID_USER 1
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "sensor-jni", __VA_ARGS__))

struct engine {
	/* we are using android's sensor library for fetching the sensor data */
    ASensorManager* sensorManager;
    const ASensor* accelerometerSensor;
    ASensorEventQueue* sensorEventQueue;
    ALooper* looper;

};

struct engine engine;

JNIEnv* menv;
jclass cls;
jclass gCls;

jmethodID methodid;

static int get_sensor_events(int fd, int events, void* data) {
	ASensorEvent event;

	if(*menv == NULL){
		LOGI("menv is NULL");
		return 0;
	}

	if(!gCls){
		LOGI("cls is NULL");
		return 0;
	}

	if(!methodid) {
		LOGI("methodid is NULL");
				      return 0;
	}

	char *pstr = malloc(60 * sizeof(char));


	while (ASensorEventQueue_getEvents(engine.sensorEventQueue, &event, 1) > 0)
	{
		if(event.type==ASENSOR_TYPE_ACCELEROMETER)
			{
				//LOGI("acc, %16.16f, %16.16f, %16.16f, %"PRId64,
					//	event.acceleration.x, event.acceleration.y,
						//event.acceleration.z, event.timestamp);

						sprintf(pstr, "x=%f,y=%f,z=%f", event.acceleration.x,event.acceleration.y,event.acceleration.z);
						jstring jstr = (*menv)->NewStringUTF(menv, pstr);
						(*menv)->CallStaticVoidMethod(menv, gCls, methodid, jstr);

			}

		return 1;
}



}



void
Java_com_example_sensorndk_SensorJni_stringFromJNI( JNIEnv* env,
                                                  jobject thiz )
{


	 menv = env;

	 cls = (*menv)->FindClass(menv, "com/example/sensorndk/SensorJni");

	 /*NewGlobalRef is used to keep the gCls variable live even after the scope of this method. A destroy method needs to be called to kill the scope of the object */
	 gCls = (*menv)->NewGlobalRef(menv, cls);


	 /* sensor_callback is the method defined in SensorJni.java */
	 methodid = (*menv)->GetStaticMethodID(menv, gCls, "sensor_callback", "(Ljava/lang/String;)V");
		if(!methodid) {
			      return;
		}


	 engine.looper = ALooper_forThread();


	 if(engine.looper == NULL){
		 LOGI("looper is null");
		 engine.looper = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
	 }
	engine.sensorManager = ASensorManager_getInstance();
	engine.accelerometerSensor = ASensorManager_getDefaultSensor(engine.sensorManager,
	            ASENSOR_TYPE_ACCELEROMETER);
	if(engine.accelerometerSensor!=NULL){
		LOGI("accelerometerSensor is not null");
		  LOGI("accelerometerSensor: %s, vendor: %s",
		ASensor_getName(engine.accelerometerSensor),
		ASensor_getVendor(engine.accelerometerSensor));

	}
	engine.sensorEventQueue = ASensorManager_createEventQueue(engine.sensorManager,
	            engine.looper, LOOPER_ID_USER, get_sensor_events, NULL);

	if(engine.sensorEventQueue!=NULL){
		LOGI("sensorEventQueue is not null");
		}
	ASensorEventQueue_enableSensor(engine.sensorEventQueue, engine.accelerometerSensor);

	/*Note that we can set the event rate to user specified value */
	ASensorEventQueue_setEventRate(engine.sensorEventQueue, engine.accelerometerSensor, (1000L/60)*1000);

	 struct android_poll_source* source;

}



