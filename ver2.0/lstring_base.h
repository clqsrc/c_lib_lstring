//
// Created by ccc on 8/4/21.
//
/*

  更新记录
  ver 2.0
  因为在 qt 版本的 ui_align 中发现有重复释放内存的情况，考虑如何检测出来。
  * 假如 mem 已经释放，那么在操作字符串各个函数时应该能检测出来。
  * 出于极致性能的情况，允许关闭这种检测。
  * 传统的检测输出是中断程序或者是输出到命令行，但象 qt,ndk 这样有界面的环境下是可以考虑弹窗的。
  所以错误输出可以由用于自定义，同时为了不影响整体代码的稳定性，用户的代码应该在另外的文件中。
  所以 lstring_app.h 中就是用户可以自定义的宏。
  * 另外，因为 lua 中也有名为 lstring.h 的文件，所以在考虑是否换用 lstring_base.h 的文件名。
  * 另外，确实应该象老外的库一样写单元测试函数。所以有 lstring_test.h/c 文件。考虑到常见开源库的 test
  中也有 main 函数会影响到集成的方便性的问题，所以并不包含 main 函数。用户自己决定是否进行 test 。

*/

#ifndef _L_STRING_BASE_H_
#define _L_STRING_BASE_H_

#ifdef __cplusplus
extern "C" {
#endif

//---------------------------------------------------------------
//#define _android_
//#define _ios_

#include "base_c.h"
#include "lstring_app.h"

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

typedef struct LString _LString; //相互引用的提前声明好象必须用 typedef

//内存池中的一项//算了, string 的自动释放就很复杂了,还是专注于 string 的吧
struct MemPool_Item{
    //void * data; //要释放的数据,可以是不同类型的
    _LString * data; //要释放的数据,可以是不同类型的

    //void (*FreeFunc)(struct MemPool_Item * item); //对应节点的释放函数//太复杂,就当做原始的 free 函数释放就行了

    struct MemPool_Item * next;
};

//内存池,用于释放一次函数过程中分配的内存
struct MemPool{

    struct MemPool_Item * Items;
    int Count;
    char * str;
    byte _const; //是否是只读的,如果是只读的就是从数组中构造的,不要释放它
    int id; //只是为调试释放而已

    //检测是否已经释放的标志 //默认是 4 字节
    //分配了内存的就是字符串 "cccc"，释放了的就是 "ffff" //为了方便内存查看器而已，在程序里面用整数
    union{

        int free_check_tag_int;      //64 位下是 1717986918，这是 pc 字节序，其他机器还不一定，所以不能用它来和 'ffff' 硬比较
        char free_check_tag_char[4];
    };

};

struct LString{

    char * _init; //只是用来判断是否进行了初始化,不可靠的简单判断,为 0 就是初始化了

    int len;
    char * str;
    byte _const;  //是否是只读的,如果是只读的就是从数组中构造的,不要释放它
    byte _malloc; //这个结构是不是 malloc 生成的,如果是还要 free 掉结构体本身
    struct MemPool * pool; //字符串所在的函数体自动释放内存池,如果是 NULL 就是要手工释放的
    //在操作字符串的临时函数中可以使用它


    void (*Append)(struct LString * s, struct LString * add);
    void (*AppendConst)(struct LString * s, const char * add);
    //加入传统 C 字符串
    void (*AppendCString)(struct LString * s, const char * str, int len); //LString_AppendCString
    
    //是否与某个 c 语言字符串相等 //2021.11.01 太常用了而且容易出错，还是给出两个比较函数吧
    int (*equals_cstr)(struct LString * s1, const char * s2);
    //是否与另外一个 lstring 字符串相等
    int (*equals_lstring)(struct LString * s1, struct LString * s2);


};

//宏还是要少用，太容易冲突了
//#define mempool struct MemPool
typedef struct MemPool mempool;

//#define lstring struct LString
typedef struct LString lstring;

//----------------
mempool makemem();

mempool * newmempool();

void freemempool(mempool * pool);

//各个常用函数中尽量使用这个函数分配新字符串,因为它生成的可自动释放,避免 autofree 满天飞
//这个应该是基础函数,不要使用其他函数实现
lstring * NewString(const char * str, struct MemPool * pool);
//指定长度的,适合含有 \0 的字符串
lstring * NewStringLen(char * str, int len, struct MemPool * pool);

//加入传统 C 字符串
void LString_AppendCString(lstring * s, const char * str, int len);

void LString_AppendConst(lstring * s, const char * add);

//--------------
//最好不用的函数
//不分配内存,只是将一个缓冲区按 string 方式操作而已,类似 golang 的 bytes ,所以不要释放它
//这个也是基础函数 ,虽然它的内存不用释放,但也还是要传 pool ,以便给生成的子字符串自动释放的机会
lstring StringConst(const char * str, int len, struct MemPool * pool);

//复制一个字符串
//lstring * PStringCopy(lstring * s, int autofree)
lstring * PStringCopy(lstring * s);

//检查字符串指针是否合法,只是简单的方法,不可靠,但有一定作用
int CheckPString(lstring * s);

//from ver 2.0
//检测内存池是否已经释放 //使用已经释放的内存（或者不合法的内存）是最常见的 c 语言错误，所以是很有必要的
void CheckFree_LString(lstring * s);
void CheckFree_LStringMemPool(mempool * mem);


#ifdef __cplusplus
}
#endif



#endif
