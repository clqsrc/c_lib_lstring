//
// Created by clq on 2022
//

#ifndef _L_STRING_LIST_H_
#define _L_STRING_LIST_H_

//字符串列表的简单实现

#ifdef __cplusplus
extern "C" {
#endif

//---------------------------------------------------------------
#include "lstring.h"


//---------------------------------------------------------------


struct LStringList{

    int count;

    char * _list_addr_buf;  //列表中各个 item 的指针全部保存在一个内存块中，类似于 delphi 的 tlist
    lstring ** items;  //列表中各个 item 的指针全部保存在一个内存块中，类似于 delphi 的 tlist //为方便读取，还是改成 lstring 的指针吧，其实内存地址都一样的
    //_list_addr_buf 和 items 指向同一块内存，保留二者是方便讲解其实现，其实用 items 就好了。但 _list_addr_buf 可以留下更多扩展的可能
    int _buf_count; //缓存中可以存放的 item 指针个数
    
    mempool * mem;  //存放字符串的缓存池

    void (*Add)(struct LStringList * list, struct LString * add);
    void (*AddConst)(struct LStringList * list, const char * add);
    //加入传统 C 字符串
    void (*AddCString)(struct LStringList * list, const char * str, int len); 
    
    //清空所有 item
    void (*Clear)(struct LStringList * list); 


};

//宏还是要少用，太容易冲突了
typedef struct LStringList lstring_list;

//----------------
void make_stringlist(lstring_list * list);

lstring_list * NewStringList();

void FreeStringList(lstring_list * list);


//加入字符串
void LStringList_Add(lstring_list * list, lstring * s);


//加入传统 C 字符串
void LStringList_AddCString(lstring_list * list, const char * str, int len);

void LStringList_AddConst(lstring_list * list, const char * add);

void LStringList_Clear(lstring_list * list);




#ifdef __cplusplus
}
#endif



#endif
