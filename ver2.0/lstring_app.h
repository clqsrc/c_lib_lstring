//
// Created by ccc on 8/4/21.
//


#ifndef _L_STRING_APP_H_
#define _L_STRING_APP_H_

#ifdef __cplusplus
extern "C" {
#endif

//---------------------------------------------------------------

//#include "base_c.h"

#ifdef _android_

    #include <stdio.h>
    #include <malloc.h> //有些编译,如果 malloc.h 在后面的话会报 malloc 函数冲突,解决办法很简单,把含有 malloc 的头文件放前面,好让我们的 malloc 定义能覆盖它就可以了
    #include <string.h>
    //#include <time.h>
    //#include <winsock.h>
    //#include <windows.h>
    #include <time.h>

    #define byte unsigned  char

#endif

#ifdef _ios_

    #include <stdio.h>

    //ios 没有这个？
    //#include <malloc.h> //有些编译,如果 malloc.h 在后面的话会报 malloc 函数冲突,解决办法很简单,把含有 malloc 的头文件放前面,好让我们的 malloc 定义能覆盖它就可以了

    #include <stdlib.h>  //ios for malloc
    #include <ctype.h>  //ios for tolower

    #include <string.h>
    //#include <time.h>
    //#include <winsock.h>
    //#include <windows.h>
    #include <time.h>

    #define byte unsigned  char

#endif

#ifdef _windows_

    #include <stdio.h>

    //ios 没有这个？
    //#include <malloc.h> //有些编译,如果 malloc.h 在后面的话会报 malloc 函数冲突,解决办法很简单,把含有 malloc 的头文件放前面,好让我们的 malloc 定义能覆盖它就可以了

    #include <stdlib.h>  //ios for malloc
    #include <ctype.h>  //ios for tolower

    #include <string.h>
    //#include <time.h>
    //#include <winsock.h>
    //#include <windows.h>
    #include <time.h>

    #define byte unsigned  char

#endif

#ifdef _qt_  //直接用 ios 的就行

    #include <stdio.h>

    //ios 没有这个？
    //#include <malloc.h> //有些编译,如果 malloc.h 在后面的话会报 malloc 函数冲突,解决办法很简单,把含有 malloc 的头文件放前面,好让我们的 malloc 定义能覆盖它就可以了

    #include <stdlib.h>  //ios for malloc
    #include <ctype.h>  //ios for tolower

    #include <string.h>
    //#include <time.h>
    //#include <winsock.h>
    //#include <windows.h>
    #include <time.h>

    //#define byte unsigned  char
    typedef unsigned char byte;

#endif


//---------------------------------------------------------------

#define _lstring_check_mem_ CheckPString

#define _lstring_check_mem_ShowError_ _lstring_check_mem_ShowError_APP

//bing 搜索 Macros in C
//不想检测，加快速度的话，可以取消这个定义，不过实际上速度影响不大
#define _MM_CheckFree_LStringMemPool_ CheckFree_LStringMemPool

//检查字符串指针是否合法,只是简单的方法,不可靠,但有一定作用
//int CheckPString(lstring * s);

void _lstring_check_mem_ShowError_APP(const char * s);


#ifdef __cplusplus
}
#endif



#endif
