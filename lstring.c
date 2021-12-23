
//没办法还是得另定义一个字符串 

#ifndef _L_STRING_C_
#define	_L_STRING_C_

#include "lstring.h"

//#define _windows_
#ifdef _windows_

#include <stdio.h>
#include <malloc.h> //有些编译,如果 malloc.h 在后面的话会报 malloc 函数冲突,解决办法很简单,把含有 malloc 的头文件放前面,好让我们的 malloc 定义能覆盖它就可以了
#include <string.h>
//#include <time.h>
//#include <winsock.h>
#include <windows.h>
#include <time.h>
//#include <crt/eh.h> //据说 MinGW终于支持使用seh实现c++ eh了
//https://sourceforge.net/p/mingw-w64/mingw-w64/ci/18a7e88bcbe8bc0de4e07dac934ebf0653c4da7c/tree/mingw-w64-headers/crt/eh.h


#endif

//#define _android_
#ifdef _android_

#include <stdio.h>
#include <malloc.h> //有些编译,如果 malloc.h 在后面的话会报 malloc 函数冲突,解决办法很简单,把含有 malloc 的头文件放前面,好让我们的 malloc 定义能覆盖它就可以了
#include <string.h>
//#include <time.h>
//#include <winsock.h>
//#include <windows.h>
#include <time.h>
#include <stdlib.h>

#define byte unsigned  char

#endif

int gMallocCount = 0;

//简单的内存泄漏检测
void * malloc_v2(size_t size)
{
	gMallocCount++;
	return malloc(size);
}

void free_v2(void * p)
{
	gMallocCount--;
	free(p);
}

//内存测试用
//#define malloc malloc_v2
//#define free free_v2


//函数体临时使用字符串时 
//#define USEPOOL struct MemPool _function_mem_ = makemem();

//#define ENDUSEPOOL Pool_Free(&_function_mem_);

//输出打印的级别 

//最普通的错误信息 
#define printf_err printf
//最低级别的打印 
#define printf_err1
//#define printf_err1 printf

//第二低级别的打印,常规下是错误的情况,但不一定 
//#define printf_err2
#define printf_err2 printf

mempool makemem()
{
	mempool mem; 
	memset(&mem, 0, sizeof(mem));
	
	srand((unsigned) time(NULL)); //用时间做种，每次产生随机数不一样
	mem.id = rand(); //number = rand() % 101; //产生0-100的随机数
	
	return mem;
}

mempool * newmempool()
{
	mempool * pmem; 
	pmem = (mempool *)malloc(sizeof(struct MemPool));
	memset(pmem, 0, sizeof(struct MemPool));
	
	srand((unsigned) time(NULL)); //用时间做种，每次产生随机数不一样
	pmem->id = rand(); //number = rand() % 101; //产生0-100的随机数
	
	return pmem;
}

//太复杂,就当做一个字符串池,用在一个函数体结束时自动释放这个过程中产生的所有临时字符串,类似于 php 的自动释放原理  
//加入一个节点, pool 并不分配内存,只是把大家加到一个链表中统一释放而已 //函数参数的定义的函数指针是一样的 
void Pool_AddItem(struct MemPool * pool, void * p)
{
	struct MemPool_Item * item = NULL;
	_LString * s = (_LString *)NULL;

	if (pool == NULL) return;
	
	//-------------------------------------------------- 
	//简单的验证 
		
	s = (_LString *)p;//item->data;
	
	if (s->_init != NULL) //简单的初始化检测
	{
		printf("Pool_AddItem: error 未初始化的字符串指针!!!\r\n");
	} 	
	
	//-------------------------------------------------- 
	

	item = (struct MemPool_Item * )malloc(sizeof(struct MemPool_Item));
	memset(item, 0, sizeof(struct MemPool_Item));
	
	//item->FreeFunc = FreeFunc;
	item->data = p;
	
	
	//下面两步是替换掉头节点 
	item->next = pool->Items;
	pool->Items = item;
	
	pool->Count++;

}//

//释放一大片,可以做些简单的检测
void Pool_Free(struct MemPool * pool)
{
	struct MemPool_Item * item = NULL;
	int i = 0;
	
	_LString * s = NULL;
	
	for(i=0; i<pool->Count; i++)
	{
		printf_err1("Pool_Free:%d, %d\r\n", pool->Count, i);
		item = pool->Items;
		//item->FreeFunc(item->data);
		//item->FreeFunc(item); //这样更清晰一点
		
		s = item->data; 
		
		if (s->_init != NULL) //简单的初始化检测
		{
			printf("Pool_Free: error 未初始化的字符串指针!!!\r\n");
		} 
		
		
		//free(item->data);
		free(s->str);

		if (s->_malloc == 1) //结构体本身也要释放的话 
		free(s);
		
		
		pool->Items = pool->Items->next; //向下移动一个指针位置 
		free(item); //节点自己的内存也要释放
	}
	
	pool->Count = 0;

}

//自动释放 
//void autofree(struct MemPool * pool, void * p)
//{
//	Pool_AddItem(pool, p);
//}

void freemempool(mempool * pool)
{

	Pool_Free(pool);

	free(pool);
}

#define freemem Pool_Free




struct LStringRef{

    //const struct LBuf * buf; //这个是保存内存内容的地方,不应该变 //本意是让这个指针值固定,但这样导致里面的值也变不了 
    struct LString * buf; //这个潜４婺诖婺谌莸牡胤�,不应该变 //C 语言的特点,为了在传递参数时不使用指针,只能是把指针放到成员中 
    
	void (*Append)(struct LString * s, struct LString * add);
	//int (*AppendConst)(struct LString * s, const char * add);

};





#define PLString struct LString *

#define stringref struct LStringRef 

//#define GetStr  s.str 

//下面这几个宏可是可以用,不过太容易冲突了,最好是逻辑清晰度要求很高的地方才用 
#define GetStr  buf->str 
#define GetLen  buf->len 

//#define str()  buf->str 
//#define len()  buf->len 

#define str__  buf->str 
#define len__  buf->len 



//为了能自动释放,只能是指针,结构体在参数传递时会丢失字段值,所以完全的模仿 C++ 是不可能的 

//绑定各个成员函数
void BindStringFunctions(lstring * s);
//lstring String(char * str);
//加入传统 C 字符串
void LString_AppendCString(lstring * s, char * str, int len);


//自动释放//只释放字符串 
void autofree_s(struct MemPool * pool, struct LString * s)
{
	if (s == NULL) return;
	
	s->pool = pool; //加上这个标志,这样根据这个 s 操作出来的 string 都可以通过它自动释放了 

	//Pool_AddItem(pool, p);
	//Pool_AddItem(pool, s->str);
	Pool_AddItem(pool, s);
}

//自动释放//不只释放字符串,连指针一起释放 
void autofree_pstring(struct MemPool * pool, struct LString * s)
{
	if (s == NULL) return;
	
	s->pool = pool; //加上这个标志,这样根据这个 s 操作出来的 string 都可以通过它自动释放了 

	Pool_AddItem(pool, s);
	//Pool_AddItem(pool, s->str);
}

//
////分配并返回一个字符串 
//stringref MakeString(char * str)
//{
//	stringref s;
//	
//	s.buf = malloc(sizeof(struct LString));
//	memset(s.buf, 0, sizeof(struct LString));
//	
//	s.buf->_const = 0;
//	s.buf->len = strlen(str);
//	s.buf->str = NULL;
//	
//	if (str != NULL)
//	{
//		s.buf->str = malloc(s.buf->len + 1); //还要留最后 \0 的位置 
//		memset(s.buf->str, 0, s.buf->len + 1);
//		
//		strcpy(s.buf->str, str);
//	}
//	
//	//绑定各个成员函数
//	//BindStringFunctions(&s);
//	
//	return s;
//
//}//
//

////分配并返回一个字符串指针
//lstring * PString(lstring s)
//{
//	//lstring s = String(str);
//	
//	lstring * p = malloc(sizeof(struct LString));
//	
//	*p = s;
//	p->_malloc = 1; //要释放结构体本身 
//	
//	autofree_pstring(s->pool, p);
//	
//	return p;
//
//}//
//


////释放 //pfree 是否释放指针本身 
//void FreeStringRef(stringref * s, int pfree)
//{
//	if (s->buf == NULL || s->buf->str == NULL)
//	{
//		printf("FreeString() error: string is NULL");
//	
//		return;
//	}
//	
//	if (s->buf->_const != 0)
//	{
//		printf("FreeString() error: string is readonly");
//	
//		//return;
//	}
//	else
//	{
//		free(s->buf->str);
//		s->buf->str = NULL;
//	}
//	
//	free(s->buf);
//	
//	if (pfree)
//		free(s);
//
//}//

void _FreeString(lstring * s, int pfree)
{
	if (s == NULL)
	{
		printf("FreeString() error: lstring * s is NULL");
	
		return;
	}
	
	if (s->str == NULL)
	{
		printf("FreeString() error: s->str is NULL");
	
		return;
	}
	
	if (s->_const != 0)
	{
		printf("FreeString() error: string is readonly");
	
		//return;
	}
	else
	{
		free(s->str);
		s->str = NULL;
	}

	
	if (pfree)  //是否释放指针本身
		free(s);

}//

void FreeString(lstring s)
{
	_FreeString(&s, 0); //只释放数据 
}

void FreePString(lstring * s)
{
	_FreeString(s, 1); //释放数据和指针
}

////释放//给内存池调用的 
//void FreeString_ForPool(struct MemPool_Item * poolitem)
//{
//	//void * p = poolitem->data;
//	lstring * p = poolitem->data;
//	
//	if (p == NULL) return;
//
//	printf("准备释放:%s\r\n", p->str);
//	
//	
//	FreePString(p);	
//
//
//}//
//

//分配一个内存池释放的 
//lstring * StringPool(struct MemPool * pool, char * str)
//{
//	
//	lstring * s = PString(str);
//	Pool_AddItem(pool, s, FreeString_ForPool);
//
//	return s;
//}

//分配一个内存池释放的 
//stringref StringRef(struct MemPool * pool, char * str)
//{
//	
//	lstring * s = PString(str);
//	Pool_AddItem(pool, s, FreeString_ForPool);
//
//	return *s;
//}


//用结构体就做不了自动释放,所以还是用指针吧,毕竟不是 C++ 
//lstring String(char * str, struct MemPool * pool)
//{
//	
//	lstring s;
//	
//	memset(&s, 0, sizeof(struct LString));
//	
//	s.len = strlen(str);
//	s.str = malloc(s.len+1);
//	memset(s.str, 0, s.len+1);
//	
//	memcpy(s.str, str, s.len);
//	
//	//绑定各个成员函数
//	BindStringFunctions(&s);
//	
//	autofree_s(pool, s);	
//
//	return s;
//}


//各个常用函数中尽量使用这个函数分配新字符串,因为它生成的可自动释放,避免 autofree 满天飞
//这个应该是基础函数,不要使用其他函数实现 
lstring * NewString(const char * str, struct MemPool * pool)
//lstring * NewString(const char * str, struct MemPool * pool)
{
	
	//lstring s = String(str);
	lstring * p = malloc(sizeof(struct LString));
	
	memset(p, 0, sizeof(struct LString)); 
	
	p->_malloc = 1;
	
	p->len = strlen(str);
	p->str = malloc(p->len+1);
	memset(p->str, 0, p->len+1);
	
	memcpy(p->str, str, p->len);
	
	//绑定各个成员函数
	BindStringFunctions(p);	

	autofree_pstring(pool, p);
	

	return p;
}

//一般用于参数传递
lstring StringCopy(lstring str)
{
	
	lstring s;
	
	memset(&s, 0, sizeof(struct LString));
	
	s.len = str.len;

	s.str = malloc(s.len+1);
	memset(s.str, 0, s.len+1);
	
	memcpy(s.str, str.str, str.len);
	
	//绑定各个成员函数
	BindStringFunctions(&s);	
	

	return s;
}



//复制一个字符串给另外一个内存池,如果对方为 NULL 那么就变成自由的字符串了,不过最好是不要这样做,应当从设计上要求每个字符串生成时都有medh池 
lstring * PStringCopyToPool(lstring * s, struct MemPool * pool)
{
	
	lstring * p = malloc(sizeof(struct LString));
	
	memset(p, 0, sizeof(struct LString));
	
	p->_malloc = 1; //结构体本身是分配的内存,也要释放 
	
	p->len = s->len;

	p->str = malloc(s->len+1);
	memset(p->str, 0, s->len+1);
	
	memcpy(p->str, s->str, s->len);
	
	//绑定各个成员函数
	BindStringFunctions(p);	
	
	//if (autofree == 1) autofree_pstring(s->pool, p);
	autofree_pstring(pool, p);

	return p;
}//

//复制一个字符串 
//lstring * PStringCopy(lstring * s, int autofree)
lstring * PStringCopy(lstring * s)
{
	return PStringCopyToPool(s, s->pool);
	
//	lstring * p = malloc(sizeof(struct LString));
//	
//	memset(p, 0, sizeof(struct LString));
//	
//	p->_malloc = 1; //结构体本身是分配的内存,也要释放 
//	
//	p->len = s->len;
//
//	p->str = malloc(s->len+1);
//	memset(p->str, 0, s->len+1);
//	
//	memcpy(p->str, s->str, s->len);
//	
//	//绑定各个成员函数
//	BindStringFunctions(p);	
//	
//	//if (autofree == 1) 
//	autofree_pstring(s->pool, p);
//
//	return p;
}//


//不分配内存,只是将一个缓冲区按 string 方式操作而已,类似 golang 的 bytes ,所以不要释放它 
//这个也是基础函数 ,虽然它的内存不用释放,但也还是要传 pool ,以便给生成的子字符串自动释放的机会 
lstring StringConst(char * str, int len, struct MemPool * pool)
{
	lstring s;
	

	//s._const = 0;
	
	s._const = 1;
	s.len = len; //strlen(str);
	s.str = str; //malloc(s.len);
	
	s.pool = pool;
	

	//绑定各个成员函数
	BindStringFunctions(&s);
	
	return s;

}//

//指定长度的,适合含有 \0 的字符串 
lstring * NewStringLen(char * str, int len, struct MemPool * pool)
{
	lstring * s = NewString("", pool);
	LString_AppendCString(s, str, len);
	
	return s;

}//


void LString_Append(lstring * _s, lstring * _add)
{
	char * tmp = NULL;
	lstring s = (*_s); //牺牲一点点性能来换语法//golang 就没有 -> 
	lstring add = (*_add);//牺牲一点点性能来换语法//golang 就没有 -> 
	
	if (_add->len > 1024 * 1024)
		printf_err2("\r\n LString_Append() _add lstring error len: %d \r\n", _add->len); //太长的字符串很可能是错误 

	if (_s->len > 1024 * 1024)
		printf_err2("\r\n LString_Append() _s   lstring error len: %d \r\n", _s->len); //太长的字符串很可能是错误 
	
	
	
	tmp = malloc(s.len + add.len + 1); //还要留最后 \0 的位置 
	memset(tmp, 0, s.len + add.len + 1);
	
	memcpy(tmp, s.str, s.len);
	memcpy(tmp + s.len, add.str, add.len); //这里要注意//加上后半段 
	
	free(s.str);

	s.str = tmp;
	s.len = s.len + add.len;
	
//	char * tmp = malloc(s->len + add->len + 1); //还要留最后 \0 的位置 
//	memset(tmp, 0, s->len + add->len + 1);
//	
//	memcpy(tmp, s->str, s->len);
//	memcpy(tmp + s->len, add->str, add->len); //这里要注意//加上后半段 
//	
//	free(s->str);
//
//	s->str = tmp;
//	s->len = s->len + add->len;	


	*_s = s;
}//



//加入传统 C 字符串
void LString_AppendCString(lstring * s, char * str, int len)
{
	lstring add = StringConst(str, len, s->pool);

	//printf("add len:%d", add.len);
	LString_Append(s, &add);
	
}//

void LString_AppendConst(lstring * s, char * add)
{

	//printf("add len:%d", add->len);
	LString_AppendCString(s, add, strlen(add));
	
}//

//int LString_AppendConst(lstring * _s, const char * _add)
//{
//	printf("ok4 \r\n");
//	lstring add = StringConst((char *)_add, strlen(_add));
//	
//	printf("ok1");
//	LString_Append(_s, &add);
//	printf("ok");
//}//

//检查字符串指针是否合法,只是简单的方法,不可靠,但有一定作用 
int CheckPString(lstring * s)
{
	if (s == NULL)
	{
		printf("CheckPString: error, string is NULL!!!\r\n"); //就目前的要自动释放的需求来说,是不能为 NULL 的,因为那样 pool 就没有传入了 
		return 0;
	}
	printf_err1("CheckPString: s->_init != NULL\r\n");
	if (s->_init != NULL) //其实对于现代的编译器和操作系统来说,如果 s 没有初始化,在这里就很可能崩溃,所以实际上是检测不出来的,所以首尾都打印一下看看这个过程没结束就是这里出错了,对性能要求高的地方,去掉 printf 宏就可以不打印了
	{
		printf("CheckPString: error, string is not init!!!\r\n"); //就目前的要自动释放的需求来说,是不能为 NULL 的,因为那样 pool 就没有传入了 
		return 0;
	}

	printf_err1("CheckPString: s->_init != NULL ok.\r\n");
	return 1;
}//

//是否与某个 c 语言字符串相等 //2021.11.01 太常用了而且容易出错，还是给出两个比较函数吧
int LString_equals_cstr(struct LString * s1, const char * s2)
{
    if (s1->len != strlen(s2)) return 0; //长度不等肯定也不是 //加个 strlen 判断会慢一点,不过确保肯定正确
    
    //if ( 0 == strcmp(s1->str, s2) ) return 1;
    if ( 0 == strncmp(s1->str, s2, s1->len) ) return 1;

    return 0;
}//

//是否与另外一个 lstring 字符串相等
int LString_equals_lstring(struct LString * s1, lstring * s2)
{
    if (s1 == NULL && s2 == NULL) return 1;
    
    if (s1 == NULL || s2 == NULL) return 0; //两者不可能都为空的情况下有一个为空,那就是不相等了
    
    if (s1->len != s2->len) return 0; //长度不等肯定也不是
    
    if ( 0 == strncmp(s1->str, s2->str, s1->len) ) return 1;

    return 0;
}//



//绑定各个成员函数
void BindStringFunctions(lstring * s) 
{
	printf_err1("BindStringFunctions \r\n");
	s->Append = LString_Append;
	s->AppendConst = LString_AppendConst;
    s->AppendCString = LString_AppendCString;
    s->equals_cstr = LString_equals_cstr;
    s->equals_lstring = LString_equals_lstring;

}//




#endif

