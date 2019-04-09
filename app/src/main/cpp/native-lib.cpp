#include <jni.h>
#include "visca.h"

extern "C" {
JNIEXPORT jint  JNICALL
Java_androidjni_example_com_viscaandroid_ViscaJni_setTraceCameraAddress(JNIEnv *env, jobject,
                                                                        jstring ip, jint port) {
    const char *address = (char *) env->GetStringUTFChars(ip, JNI_FALSE);
    ViscaManager viscaHelper;
    jint result = viscaHelper.connectHostCam(address, port);
    env->ReleaseStringUTFChars(ip, address);
    return result;
}
JNIEXPORT jint JNICALL Java_androidjni_example_com_viscaandroid_ViscaJni_setPantiltPos
        (JNIEnv *env, jobject jobject, jint camId, jint dir) {
    ViscaManager viscaHelper;
    return viscaHelper.setPantiltPos(camId, dir);
}

JNIEXPORT jint JNICALL Java_androidjni_example_com_viscaandroid_ViscaJni_setPantiltHome
        (JNIEnv *env, jobject jobject, jint camId) {
    ViscaManager viscaHelper;
    return viscaHelper.setPantiltHome(camId);
}

JNIEXPORT jint JNICALL Java_androidjni_example_com_viscaandroid_ViscaJni_setPantileSpeed
        (JNIEnv *env, jobject jobject, jint camId, jint speed) {
    ViscaManager viscaHelper;
    return viscaHelper.setPantiltSpeed(camId, speed);
}

JNIEXPORT jint JNICALL Java_androidjni_example_com_viscaandroid_ViscaJni_setCamMemory
        (JNIEnv *env, jobject jobject, jint camId, jint channel) {
    ViscaManager viscaHelper;
    return viscaHelper.setCamMemory(camId, channel);
}

JNIEXPORT jint JNICALL Java_androidjni_example_com_viscaandroid_ViscaJni_reCallCamMemory
        (JNIEnv *env, jobject jobject, jint camId, jint channel) {
    ViscaManager viscaHelper;
    return viscaHelper.recallCamMemory(camId, channel);
}
JNIEXPORT jint JNICALL Java_androidjni_example_com_viscaandroid_ViscaJni_setZoomPosition
        (JNIEnv *env, jobject jobject, jint camId, jboolean far) {
    ViscaManager viscaHelper;
    return viscaHelper.setZoomPosition(camId, far);
}

}



