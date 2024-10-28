#include <jni.h>
#include "react-native-draco.h"

extern "C"
JNIEXPORT jdouble JNICALL
Java_com_draco_DracoModule_nativeMultiply(JNIEnv *env, jclass type, jdouble a, jdouble b) {
    return draco::multiply(a, b);
}
