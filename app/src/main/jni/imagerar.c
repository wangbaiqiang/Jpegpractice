//
// Created by Administrator on 2016/12/29.
//
#include "com_wbq_jpegpractice_NativeUtil.h"
#include <string.h>
#include <android/bitmap.h>
#include <android/log.h>
#include <jni.h>
#include <stdio.h>
#include <setjmp.h>
#include <math.h>
#include <stdint.h>
#include <time.h>
#include "jpeg/jpeglib.h"
#include "jpeg/cdjpeg.h"		/* Common decls for cjpeg/djpeg applications */
#include "jpeg/jversion.h"		/* for version message */
#include "jpeg/android/config.h"
typedef uint8_t BYTE;
#define TAG "WBQ"
#define true 1
#define false 0
#define LOGE(...) __android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__)
char *error;
struct my_error_mgr {
  struct jpeg_error_mgr pub;
  jmp_buf setjmp_buffer;
};
typedef struct my_error_mgr * my_error_ptr;

METHODDEF(void)
my_error_exit (j_common_ptr cinfo)
{
  my_error_ptr myerr = (my_error_ptr) cinfo->err;
  (*cinfo->err->output_message) (cinfo);
  error=myerr->pub.jpeg_message_table[myerr->pub.msg_code];
  LOGE("jpeg_message_table[%d]:%s", myerr->pub.msg_code,myerr->pub.jpeg_message_table[myerr->pub.msg_code]);
 // LOGE("addon_message_table:%s", myerr->pub.addon_message_table);
//  LOGE("SIZEOF:%d",myerr->pub.msg_parm.i[0]);
//  LOGE("sizeof:%d",myerr->pub.msg_parm.i[1]);
  longjmp(myerr->setjmp_buffer, 1);
}
int generateJPEG(BYTE* data, int w, int h, int quality,
		const char* outfilename, jboolean optimize) {
	int nComponent = 3;

	struct jpeg_compress_struct jcs;

	struct my_error_mgr jem;

	jcs.err = jpeg_std_error(&jem.pub);
	jem.pub.error_exit = my_error_exit;
	    if (setjmp(jem.setjmp_buffer)) {
	        return 0;
	     }
	jpeg_create_compress(&jcs);
	FILE* f = fopen(outfilename, "wb");
	if (f == NULL) {
		return 0;
	}
	jpeg_stdio_dest(&jcs, f);
	jcs.image_width = w;
	jcs.image_height = h;
	if (optimize) {
		LOGE("optimize==ture");
	} else {
		LOGE("optimize==false");
	}

	jcs.arith_code = false;
	jcs.input_components = nComponent;
	if (nComponent == 1)
		jcs.in_color_space = JCS_GRAYSCALE;
	else
		jcs.in_color_space = JCS_RGB;

	jpeg_set_defaults(&jcs);
	jcs.optimize_coding = optimize;
	jpeg_set_quality(&jcs, quality, true);

	jpeg_start_compress(&jcs, TRUE);

	JSAMPROW row_pointer[1];
	int row_stride;
	row_stride = jcs.image_width * nComponent;
	while (jcs.next_scanline < jcs.image_height) {
		row_pointer[0] = &data[jcs.next_scanline * row_stride];

		jpeg_write_scanlines(&jcs, row_pointer, 1);
	}

	if (jcs.optimize_coding) {
			LOGE("optimize==ture");
		} else {
			LOGE("optimize==false");
		}
	jpeg_finish_compress(&jcs);
	jpeg_destroy_compress(&jcs);
	fclose(f);

	return 1;
}
char* jstrinTostring(JNIEnv* env, jbyteArray barr) {
	char* rtn = NULL;
	jsize alen = (*env)->GetArrayLength(env, barr);
	jbyte* ba = (*env)->GetByteArrayElements(env, barr, 0);
	if (alen > 0) {
		rtn = (char*) malloc(alen + 1);
		memcpy(rtn, ba, alen);
		rtn[alen] = 0;
	}
	(*env)->ReleaseByteArrayElements(env, barr, ba, 0);
	return rtn;
}
JNIEXPORT jstring JNICALL Java_com_wbq_jpegpractice_NativeUtil_compressBitmap
  (JNIEnv * env, jclass jclazz, jobject bitmapColor, jint w, jint h, jint quality, jbyteArray fileNameStr, jboolean optimize){
    AndroidBitmapInfo infocolor;
    int ret;
    BYTE * data;
    BYTE * pixelColor;
    BYTE * tmpdata;
    char *filename=jstrinTojstring(env,fileNameStr);
     if((ret=AndroidBitmap_getInfo(env,bitmapColor,&infocolor))<0){
            LOGE("解析错误");
            return (*env)->NewStringUTF(env,"0");
     }
     //这里是把bitmap对象的像素点二维数组给了pixelcolor指针
     if((ret=AndroidBitmap_lockPixels(env,bitmapColor,&pixelColor))<0){
        LOGE("加载失败");
     }
     BYTE r,g,b;
     int color;
     data=NULL;
     data=malloc(w*h*3);
     tmpdata=data;
     int i=0;
     int j=0;
     for(int i=0;i<h;i++){
        for(int j=0;j<w;j++){
            color=*((int*)pixelColor);
            //取出int的高8位就是当前像素的r的值 0x00FF0000是红色
            r=((color&0x00FF0000)>>16);
            g=((color&0x0000FF00)>>8);
            b=color&0x000000FF;
            *data = b;
            *(data + 1) = g;
            *(data + 2) = r;
            data+=3;
            pixelColor+=4;
        }
     }

    AndroidBitmap_unlockPixels(env,bitmapColor);
    //调用jpeg的方法
    int resultCode=generateJPEG(tmpdata,w,h,quality,filename,optimize);
    free(tmpdata);
    if(resultCode==0){
           //当执行错误的时候
         jstring result=(*env)-> NewStringUTF(env,"0");
    }
    return (*env)->NewStringUTF(env,"1");
  }