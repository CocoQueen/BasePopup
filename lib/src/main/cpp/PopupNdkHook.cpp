//
// Created by 大灯泡 on 2018/9/3.
// 用于突破Android P 的blacklist
// 感谢360团队的方法：https://mp.weixin.qq.com/s/b1suvQyGOw2hVvfufOAZ0g
// WARN:本cpp采取方法2，只能调用系统类，不能调用自己的类，否则会出问题！
//

#include <jni.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <dlfcn.h>
#include <android/log.h>

#define LIB_ART_PATH "/system/lib/libart.so"
#define JAVA_PATH "razerdp/hook/PopupReflectionHelper"
#define ARRAYLEN(x) ((int)(sizeof(x)/sizeof((x)[0])))
#define LOG_TAG "PopupHook"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,  LOG_TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

//ObjPtr修改自Android源码
class ObjPtr {
public:
    uintptr_t reference_;
};

void (*setClassLoader)(void *pClass, void *new_cl);

ObjPtr (*toClass)(jclass global_jclass);

void *my_dlsym(char *handle, const char *symbol) {
    return dlsym(dlopen(handle, RTLD_NOW), symbol);
}

void makeHiddenApiAccessable(JNIEnv *env) {
    *(void **) (&toClass) = my_dlsym(LIB_ART_PATH, "_ZN3art16WellKnownClasses7ToClassEP7_jclass");
    *(void **) (&setClassLoader) = my_dlsym(LIB_ART_PATH,
                                            "_ZN3art6mirror5Class14SetClassLoaderENS_6ObjPtrINSO_11ClassLoaderEEE");
    jclass cls = env->FindClass(JAVA_PATH);
    ObjPtr op = toClass(cls);
    setClassLoader((void *) op.reference_, NULL);
    LOGD("PopupHook:%s", "Hook成功");
}


static JNINativeMethod nativeMethods[] = {
        {"makeHiddenApiAccessable", "()V", (void *) makeHiddenApiAccessable}
};


static int registerNativeMethods(JNIEnv *env, const char *className, JNINativeMethod *gMethods,
                                 int numMethods) {
    jclass clazz;
    clazz = env->FindClass(className);
    if (clazz == NULL) {
        return JNI_FALSE;
    }

    if (env->RegisterNatives(clazz, gMethods, numMethods) < 0) {
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

static int registerNatives(JNIEnv *env) {
    return registerNativeMethods(env, JAVA_PATH, nativeMethods, ARRAYLEN(nativeMethods));
}


extern "C" JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = NULL;
    jint result = -1;

    if (vm->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {
        LOGE("JNI_ONLOAD:%s", "failed");
        return -1;
    }
    assert(env != NULL);

    if (!registerNatives(env)) {//注册
        return -1;
    }

/* success -- return valid version number */

    result = JNI_VERSION_1_4;

    return result;
}

