

#ifndef _base_c_h_
#define _base_c_h_

//各平台下不一定一样的接口文件，但从此文件引出的接口必须是一样的


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

//---------------------------------------------------------------
//#define _android_
//#define _ios_
//#define _windows_
#define _qt_

#ifndef chandle
typedef void * chandle; //base_handle;  //所谓句柄。其实就是指针啦
//typedef int64_t chandle; //所谓句柄。其实就是指针啦. //ios 下有 arc, 调试时内容的时会很乱，这时可以先用 int64
#endif

#ifdef _windows_
    #ifndef int64_t
    typedef long long int64_t;  
    #endif
#endif

#ifdef _qt_
    #ifndef int64_t
        #ifndef INT64_C
            //64 位 windows 下目前 qt5 中的 _WIN32 也是有效的
            #ifdef _WIN32
            typedef long long int64_t;
            #endif
        #endif
    #endif
#endif

#ifndef cint64
typedef int64_t cint64;  //_int64;  //要能存放指针，所以不能直接用 int , int 应该固定为 32 位了
#endif




#ifdef __cplusplus
}
#endif

#endif //_base_ndk__h_
