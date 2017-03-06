
#include <jni.h>
#include <android/log.h>

#include "TouchControlsContainer.h"
extern "C"
{

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO,"JNITouchControlsUtils", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "JNITouchControlsUtils", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR,"JNITouchControlsUtils", __VA_ARGS__))

//static JNIEnv* jni_env_UI = 0;

static JavaVM* jvm;


void setTCJNIEnv( JavaVM* jvm_)
{
	jvm = jvm_;
}

static JNIEnv *my_getJNIEnv ( )
{
	JNIEnv *pJNIEnv ;

	if ( jvm && ( jvm->GetEnv ( (void**) &pJNIEnv, JNI_VERSION_1_4 ) >= 0 ) )
	{
		return pJNIEnv ;
	}
	return 0 ;
}

JNIEnv * getEnv()
{
	if (!jvm)
		LOGE("ERROR, jvm for getEnv is NULL");


	JNIEnv* pJNIEnv = 0;

	bool isAttached = false;
	int status = jvm->GetEnv((void **) &pJNIEnv, JNI_VERSION_1_4);
	if(status < 0) {
		//LOGI("Attaching...");

		status = jvm->AttachCurrentThread(&pJNIEnv, 0);

		if(status < 0) {
			LOGE("getEnv: callback_handler: failed to attach current thread");
		}
		isAttached = true;
	}
	if (pJNIEnv)
		LOGI("getEnv env looks OK");
	else
		LOGE("ERROR, getEnv env is NULL");

	return pJNIEnv;
}


void showTouchSettings()
{
	JNIEnv * env = getEnv();

	jclass myClass =  env->FindClass("com/beloko/touchcontrols/TouchControlsSettings");
	jmethodID myMethod = env->GetStaticMethodID(myClass, "showSettings", "()V");
	env->CallStaticVoidMethod(myClass, myMethod);
}

void showEditButtons()
{
	JNIEnv * env = getEnv();

	jclass myClass =  env->FindClass("com/beloko/touchcontrols/TouchControlsEditing");
	jmethodID myMethod = env->GetStaticMethodID(myClass, "show", "()V");
	env->CallStaticVoidMethod(myClass, myMethod);
}


void showCustomCommands()
{
	JNIEnv * env = getEnv();

	jclass myClass =  env->FindClass("com/beloko/touchcontrols/CustomCommands");
	jmethodID myMethod = env->GetStaticMethodID(myClass,  "showCommands", "()V");
	env->CallStaticVoidMethod(myClass, myMethod);

}

void toggleKeyboard()
{
	JNIEnv * env = getEnv();

	jclass myClass =  env->FindClass("com/beloko/touchcontrols/ShowKeyboard");
	jmethodID myMethod = env->GetStaticMethodID(myClass,  "toggleKeyboard", "()V");
	env->CallStaticVoidMethod(myClass, myMethod);

}

void showKeyboard(int val)
{
	JNIEnv * env = getEnv();

	jclass myClass =  env->FindClass("com/beloko/touchcontrols/ShowKeyboard");
	jmethodID myMethod = env->GetStaticMethodID(myClass,  "showKeyboard", "(I)V");
	env->CallStaticVoidMethod(myClass, myMethod, val);

}

//This is to try and stop the occasional crash on shutdown
int android_app_is_shutting_down = 0;
void appShutdown()
{
	android_app_is_shutting_down = 1;
}

//This should not be here
void ChangeDroidMusic(int action,int param1,int param2)
{
	JNIEnv * env = getEnv();

	jclass myClass =  env->FindClass("com/beloko/opengames/CDAudioPlayer");
	jmethodID myMethod = env->GetStaticMethodID(myClass,  "callback", "(III)V");
	env->CallStaticVoidMethod(myClass, myMethod, action,param1,param2);
}


//Button editing

static touchcontrols::TouchControlsContainer *controlsContainer;
static std::vector<touchcontrols::ControlSuper *> editiableControls;

#define EXPORT_ME __attribute__ ((visibility("default")))

void setControlsContainer(touchcontrols::TouchControlsContainer * cc)
{
    controlsContainer = cc;
}

jint EXPORT_ME Java_com_beloko_touchcontrols_TouchControlsEditing_JNIGetNbrControls(JNIEnv *env, jobject obj)
{
    if (!controlsContainer)
        return 0;

    touchcontrols::TouchControls *editing = controlsContainer->getEditingControls();

    if (!editing)
        return 0;

    editiableControls.clear();

    for (int n=0;n<editing->getControls()->size();n++)
    {
        if ((editing->getControls()->at(n)->type == TC_TYPE_BUTTON) && (editing->getControls()->at(n)->tag != ""))
        {
            editiableControls.push_back(editing->getControls()->at(n));
        }
    }

    return editiableControls.size();
}

void EXPORT_ME Java_com_beloko_touchcontrols_TouchControlsEditing_JNIGetControlInfo(JNIEnv *env, jobject obj,
        jint pos,jobject info)
{
    jclass clazz;
    jfieldID fid;
    clazz = env->GetObjectClass( info);
    if (0 == clazz)
    {
        LOGE("Java_com_beloko_duke_TouchControlsSettings_JNIGetControlInfo, GetObjectClass returned 0");
        return;
    }


    jstring tag = env->NewStringUTF(editiableControls.at(pos)->tag.c_str());
    fid = env->GetFieldID(clazz,"tag","Ljava/lang/String;");
    env->SetObjectField(info,fid,tag);

    jstring image = env->NewStringUTF(static_cast<touchcontrols::Button*>(editiableControls.at(pos))->image.c_str());
    fid = env->GetFieldID(clazz,"image","Ljava/lang/String;");
    env->SetObjectField(info,fid,image);


    fid = env->GetFieldID(clazz,"enabled","Z");
    env->SetBooleanField(info,fid,editiableControls.at(pos)->isEnabled());

    fid = env->GetFieldID(clazz,"hidden","Z");
       env->SetBooleanField(info,fid,editiableControls.at(pos)->isHidden());
   // env->SetIntField()
}

void EXPORT_ME Java_com_beloko_touchcontrols_TouchControlsEditing_JNISetHidden(JNIEnv *env, jobject obj,
        jint pos,jboolean hidden)
{
    editiableControls.at(pos)->setHidden(hidden);
}
}
