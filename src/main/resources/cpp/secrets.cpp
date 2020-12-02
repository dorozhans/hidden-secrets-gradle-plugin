#include "secrets.hpp"

#include <jni.h>

#include "SHA256.hpp"
#include "SHA256.cpp"

/* Copyright (c) 2020-present Klaxit SAS
*
* Permission is hereby granted, free of charge, to any person
* obtaining a copy of this software and associated documentation
* files (the "Software"), to deal in the Software without
* restriction, including without limitation the rights to use,
* copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following
* conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
* HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*/

void customDecode(char *str) {
    /* Add your own logic here
    * To improve your key security you can encode it before to integrate it in the app.
    * And then decode it with your own logic in this function.
    */
}

jstring getOriginalKey(
        char *obfuscatedSecret,
        int obfuscatedSecretSize,
        jstring obfuscatingJStr,
        JNIEnv *pEnv) {

    // Get the obfuscating string SHA256 as the obfuscator
    const char *obfuscatingStr = pEnv->GetStringUTFChars(obfuscatingJStr, NULL);
    const char *obfuscator = sha256(obfuscatingStr);

   int len =  strlen(obfuscator);

    // Apply a XOR between the obfuscated key and the obfuscating string to get original sting
//    char out[obfuscatedSecretSize + 1];
    char out[obfuscatedSecretSize];
    for (int i = 0; i < obfuscatedSecretSize; i++) {
        out[i] = obfuscatedSecret[i] ^ obfuscator[i % len];
    }

    // Add string terminal delimiter
//    out[obfuscatedSecretSize] = 0x0;
//
    //(Optional) To improve key security
    customDecode(out);

    jobject bb = pEnv->NewDirectByteBuffer(out, obfuscatedSecretSize);
//    jobject bb = pEnv->NewDirectByteBuffer(out, obfuscatedSecretSize + 1);
    jclass cls_Charset = pEnv->FindClass("java/nio/charset/Charset");
    jmethodID mid_Charset_forName = pEnv->GetStaticMethodID(cls_Charset, "forName", "(Ljava/lang/String;)Ljava/nio/charset/Charset;");
    jobject charset = pEnv->CallStaticObjectMethod(cls_Charset, mid_Charset_forName, pEnv->NewStringUTF("UTF-8"));

    jmethodID mid_Charset_decode = pEnv->GetMethodID(cls_Charset, "decode", "(Ljava/nio/ByteBuffer;)Ljava/nio/CharBuffer;");
    jobject cb = pEnv->CallObjectMethod(charset, mid_Charset_decode, bb);
    pEnv->DeleteLocalRef(bb);

    jclass cls_CharBuffer = pEnv->FindClass("java/nio/CharBuffer");
    jmethodID mid_CharBuffer_toString = pEnv->GetMethodID(cls_CharBuffer, "toString", "()Ljava/lang/String;");
    jstring str = static_cast<jstring>(pEnv->CallObjectMethod(cb, mid_CharBuffer_toString));

//    jstring g = pEnv->NewStringUTF(out);

    return str;
}

extern "C"
JNIEXPORT jstring JNICALL
Java_YOUR_PACKAGE_GOES_HERE_Secrets_getYOUR_KEY_NAME_GOES_HERE(
        JNIEnv *pEnv,
        jobject pThis,
        jstring packageName) {
        char obfuscatedSecret[] = {YOUR_KEY_GOES_HERE};
    return getOriginalKey(obfuscatedSecret, sizeof(obfuscatedSecret), packageName, pEnv);
}
