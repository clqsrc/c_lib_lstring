
//û�취���ǵ�����һ���ַ��� 

#ifndef _L_STRING_C_
#define	_L_STRING_C_

#include <stdio.h>
#include <malloc.h> //��Щ����,��� malloc.h �ں���Ļ��ᱨ malloc ������ͻ,����취�ܼ�,�Ѻ��� malloc ��ͷ�ļ���ǰ��,�������ǵ� malloc �����ܸ������Ϳ�����
#include <string.h>
//#include <time.h>
//#include <winsock.h>
#include <windows.h>
#include <time.h>
//#include <crt/eh.h> //��˵ MinGW����֧��ʹ��sehʵ��c++ eh�� 
//https://sourceforge.net/p/mingw-w64/mingw-w64/ci/18a7e88bcbe8bc0de4e07dac934ebf0653c4da7c/tree/mingw-w64-headers/crt/eh.h

int gMallocCount = 0;

//�򵥵��ڴ�й©���
void * malloc_v2(size_t size)
{
	gMallocCount++;
	return malloc(size);
}

void * free_v2(void * p)
{
	gMallocCount--;
	free(p);
}

#define malloc malloc_v2
#define free free_v2

typedef struct LString _LString; //�໥���õ���ǰ������������� typedef

//�ڴ���е�һ��//����, string ���Զ��ͷžͺܸ�����,����רע�� string �İ�
struct MemPool_Item{
	//void * data; //Ҫ�ͷŵ�����,�����ǲ�ͬ���͵�
	_LString * data; //Ҫ�ͷŵ�����,�����ǲ�ͬ���͵�

	//void (*FreeFunc)(struct MemPool_Item * item); //��Ӧ�ڵ���ͷź���//̫����,�͵���ԭʼ�� free �����ͷž����� 
	
	struct MemPool_Item * next;
};

//�ڴ��,�����ͷ�һ�κ��������з�����ڴ� 
struct MemPool{

	struct MemPool_Item * Items;
    int Count;
    char * str;
    byte _const; //�Ƿ���ֻ����,�����ֻ���ľ��Ǵ������й����,��Ҫ�ͷ��� 
    int id; //ֻ��Ϊ�����ͷŶ��� 

};

struct LString{

	char * _init; //ֻ�������ж��Ƿ�����˳�ʼ��,���ɿ��ļ��ж�,Ϊ 0 ���ǳ�ʼ����

	int len;
    char * str;
    byte _const;  //�Ƿ���ֻ����,�����ֻ���ľ��Ǵ������й����,��Ҫ�ͷ���
    byte _malloc; //����ṹ�ǲ��� malloc ���ɵ�,����ǻ�Ҫ free ���ṹ�屾��
    struct MemPool * pool; //�ַ������ڵĺ������Զ��ͷ��ڴ��,����� NULL ����Ҫ�ֹ��ͷŵ�
	//�ڲ����ַ�������ʱ�����п���ʹ����


	void (*Append)(struct LString * s, struct LString * add);
	void (*AppendConst)(struct LString * s, char * add);
	//���봫ͳ C �ַ���
	void (*AppendCString)(struct LString * s, char * str, int len); //LString_AppendCString


};

#define mempool struct MemPool 

//��������ʱʹ���ַ���ʱ 
//#define USEPOOL struct MemPool _function_mem_ = makemem();

//#define ENDUSEPOOL Pool_Free(&_function_mem_);

//�����ӡ�ļ��� 

//����ͨ�Ĵ�����Ϣ 
#define printf_err printf
//��ͼ���Ĵ�ӡ 
#define printf_err1
//#define printf_err1 printf

//�ڶ��ͼ���Ĵ�ӡ,�������Ǵ�������,����һ�� 
//#define printf_err2
#define printf_err2 printf

mempool makemem()
{
	mempool mem; 
	memset(&mem, 0, sizeof(mem));
	
	srand((unsigned) time(NULL)); //��ʱ�����֣�ÿ�β����������һ��
	mem.id = rand(); //number = rand() % 101; //����0-100�������
	
	return mem;
}

mempool * newmempool()
{
	mempool * pmem; 
	pmem = malloc(sizeof(struct MemPool));
	memset(pmem, 0, sizeof(struct MemPool));
	
	srand((unsigned) time(NULL)); //��ʱ�����֣�ÿ�β����������һ��
	pmem->id = rand(); //number = rand() % 101; //����0-100�������
	
	return pmem;
}

//̫����,�͵���һ���ַ�����,����һ�����������ʱ�Զ��ͷ���������в�����������ʱ�ַ���,������ php ���Զ��ͷ�ԭ��  
//����һ���ڵ�, pool ���������ڴ�,ֻ�ǰѴ�Ҽӵ�һ��������ͳһ�ͷŶ��� //���������Ķ���ĺ���ָ����һ���� 
void Pool_AddItem(struct MemPool * pool, void * p)
{
	struct MemPool_Item * item = NULL;
	_LString * s = NULL;

	if (pool == NULL) return;
	
	//-------------------------------------------------- 
	//�򵥵���֤ 
		
	s = p;//item->data; 
	
	if (s->_init != NULL) //�򵥵ĳ�ʼ�����
	{
		printf("Pool_AddItem: error δ��ʼ�����ַ���ָ��!!!\r\n");
	} 	
	
	//-------------------------------------------------- 
	

	item = malloc(sizeof(struct MemPool_Item));
	memset(item, 0, sizeof(struct MemPool_Item));
	
	//item->FreeFunc = FreeFunc;
	item->data = p;
	
	
	//�����������滻��ͷ�ڵ� 
	item->next = pool->Items;
	pool->Items = item;
	
	pool->Count++;

}//

//�ͷ�һ��Ƭ,������Щ�򵥵ļ��
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
		//item->FreeFunc(item); //����������һ��
		
		s = item->data; 
		
		if (s->_init != NULL) //�򵥵ĳ�ʼ�����
		{
			printf("Pool_Free: error δ��ʼ�����ַ���ָ��!!!\r\n");
		} 
		
		
		//free(item->data);
		free(s->str);

		if (s->_malloc == 1) //�ṹ�屾��ҲҪ�ͷŵĻ� 
		free(s);
		
		
		pool->Items = pool->Items->next; //�����ƶ�һ��ָ��λ�� 
		free(item); //�ڵ��Լ����ڴ�ҲҪ�ͷ�
	}
	
	pool->Count = 0;

}

//�Զ��ͷ� 
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

    //const struct LBuf * buf; //����Ǳ����ڴ����ݵĵط�,��Ӧ�ñ� //�����������ָ��ֵ�̶�,���������������ֵҲ�䲻�� 
    struct LString * buf; //����Ǳ����ڴ����ݵĵط�,��Ӧ�ñ� //C ���Ե��ص�,Ϊ���ڴ��ݲ���ʱ��ʹ��ָ��,ֻ���ǰ�ָ��ŵ���Ա�� 
    
	void (*Append)(struct LString * s, struct LString * add);
	//int (*AppendConst)(struct LString * s, const char * add);

};



#define lstring struct LString 

#define PLString struct LString *

#define stringref struct LStringRef 

//#define GetStr  s.str 

//�����⼸������ǿ�����,����̫���׳�ͻ��,������߼�������Ҫ��ܸߵĵط����� 
#define GetStr  buf->str 
#define GetLen  buf->len 

//#define str()  buf->str 
//#define len()  buf->len 

#define str__  buf->str 
#define len__  buf->len 



//Ϊ�����Զ��ͷ�,ֻ����ָ��,�ṹ���ڲ�������ʱ�ᶪʧ�ֶ�ֵ,������ȫ��ģ�� C++ �ǲ����ܵ� 

//�󶨸�����Ա����
void BindStringFunctions(lstring * s);
//lstring String(char * str);
//���봫ͳ C �ַ���
void LString_AppendCString(lstring * s, char * str, int len);


//�Զ��ͷ�//ֻ�ͷ��ַ��� 
void autofree_s(struct MemPool * pool, struct LString * s)
{
	if (s == NULL) return;
	
	s->pool = pool; //���������־,����������� s ���������� string ������ͨ�����Զ��ͷ��� 

	//Pool_AddItem(pool, p);
	//Pool_AddItem(pool, s->str);
	Pool_AddItem(pool, s);
}

//�Զ��ͷ�//��ֻ�ͷ��ַ���,��ָ��һ���ͷ� 
void autofree_pstring(struct MemPool * pool, struct LString * s)
{
	if (s == NULL) return;
	
	s->pool = pool; //���������־,����������� s ���������� string ������ͨ�����Զ��ͷ��� 

	Pool_AddItem(pool, s);
	//Pool_AddItem(pool, s->str);
}

//
////���䲢����һ���ַ��� 
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
//		s.buf->str = malloc(s.buf->len + 1); //��Ҫ����� \0 ��λ�� 
//		memset(s.buf->str, 0, s.buf->len + 1);
//		
//		strcpy(s.buf->str, str);
//	}
//	
//	//�󶨸�����Ա����
//	//BindStringFunctions(&s);
//	
//	return s;
//
//}//
//

////���䲢����һ���ַ���ָ��
//lstring * PString(lstring s)
//{
//	//lstring s = String(str);
//	
//	lstring * p = malloc(sizeof(struct LString));
//	
//	*p = s;
//	p->_malloc = 1; //Ҫ�ͷŽṹ�屾�� 
//	
//	autofree_pstring(s->pool, p);
//	
//	return p;
//
//}//
//


////�ͷ� //pfree �Ƿ��ͷ�ָ�뱾�� 
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

	
	if (pfree)  //�Ƿ��ͷ�ָ�뱾��
		free(s);

}//

void FreeString(lstring s)
{
	_FreeString(&s, 0); //ֻ�ͷ����� 
}

void FreePString(lstring * s)
{
	_FreeString(s, 1); //�ͷ����ݺ�ָ��
}

////�ͷ�//���ڴ�ص��õ� 
//void FreeString_ForPool(struct MemPool_Item * poolitem)
//{
//	//void * p = poolitem->data;
//	lstring * p = poolitem->data;
//	
//	if (p == NULL) return;
//
//	printf("׼���ͷ�:%s\r\n", p->str);
//	
//	
//	FreePString(p);	
//
//
//}//
//

//����һ���ڴ���ͷŵ� 
//lstring * StringPool(struct MemPool * pool, char * str)
//{
//	
//	lstring * s = PString(str);
//	Pool_AddItem(pool, s, FreeString_ForPool);
//
//	return s;
//}

//����һ���ڴ���ͷŵ� 
//stringref StringRef(struct MemPool * pool, char * str)
//{
//	
//	lstring * s = PString(str);
//	Pool_AddItem(pool, s, FreeString_ForPool);
//
//	return *s;
//}


//�ýṹ����������Զ��ͷ�,���Ի�����ָ���,�Ͼ����� C++ 
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
//	//�󶨸�����Ա����
//	BindStringFunctions(&s);
//	
//	autofree_s(pool, s);	
//
//	return s;
//}


//�������ú����о���ʹ����������������ַ���,��Ϊ�����ɵĿ��Զ��ͷ�,���� autofree �����
//���Ӧ���ǻ�������,��Ҫʹ����������ʵ�� 
lstring * NewString(char * str, struct MemPool * pool)
{
	
	//lstring s = String(str);
	lstring * p = malloc(sizeof(struct LString));
	
	memset(p, 0, sizeof(struct LString)); 
	
	p->_malloc = 1;
	
	p->len = strlen(str);
	p->str = malloc(p->len+1);
	memset(p->str, 0, p->len+1);
	
	memcpy(p->str, str, p->len);
	
	//�󶨸�����Ա����
	BindStringFunctions(p);	

	autofree_pstring(pool, p);
	

	return p;
}

//һ�����ڲ�������
lstring StringCopy(lstring str)
{
	
	lstring s;
	
	memset(&s, 0, sizeof(struct LString));
	
	s.len = str.len;

	s.str = malloc(s.len+1);
	memset(s.str, 0, s.len+1);
	
	memcpy(s.str, str.str, str.len);
	
	//�󶨸�����Ա����
	BindStringFunctions(&s);	
	

	return s;
}



//����һ���ַ���������һ���ڴ��,����Է�Ϊ NULL ��ô�ͱ�����ɵ��ַ�����,��������ǲ�Ҫ������,Ӧ���������Ҫ��ÿ���ַ�������ʱ����medh�� 
lstring * PStringCopyToPool(lstring * s, struct MemPool * pool)
{
	
	lstring * p = malloc(sizeof(struct LString));
	
	memset(p, 0, sizeof(struct LString));
	
	p->_malloc = 1; //�ṹ�屾���Ƿ�����ڴ�,ҲҪ�ͷ� 
	
	p->len = s->len;

	p->str = malloc(s->len+1);
	memset(p->str, 0, s->len+1);
	
	memcpy(p->str, s->str, s->len);
	
	//�󶨸�����Ա����
	BindStringFunctions(p);	
	
	//if (autofree == 1) autofree_pstring(s->pool, p);
	autofree_pstring(pool, p);

	return p;
}//

//����һ���ַ��� 
//lstring * PStringCopy(lstring * s, int autofree)
lstring * PStringCopy(lstring * s)
{
	return PStringCopyToPool(s, s->pool);
	
//	lstring * p = malloc(sizeof(struct LString));
//	
//	memset(p, 0, sizeof(struct LString));
//	
//	p->_malloc = 1; //�ṹ�屾���Ƿ�����ڴ�,ҲҪ�ͷ� 
//	
//	p->len = s->len;
//
//	p->str = malloc(s->len+1);
//	memset(p->str, 0, s->len+1);
//	
//	memcpy(p->str, s->str, s->len);
//	
//	//�󶨸�����Ա����
//	BindStringFunctions(p);	
//	
//	//if (autofree == 1) 
//	autofree_pstring(s->pool, p);
//
//	return p;
}//


//�������ڴ�,ֻ�ǽ�һ���������� string ��ʽ��������,���� golang �� bytes ,���Բ�Ҫ�ͷ��� 
//���Ҳ�ǻ������� ,��Ȼ�����ڴ治���ͷ�,��Ҳ����Ҫ�� pool ,�Ա�����ɵ����ַ����Զ��ͷŵĻ��� 
lstring StringConst(char * str, int len, struct MemPool * pool)
{
	lstring s;
	

	//s._const = 0;
	
	s._const = 1;
	s.len = len; //strlen(str);
	s.str = str; //malloc(s.len);
	
	s.pool = pool;
	

	//�󶨸�����Ա����
	BindStringFunctions(&s);
	
	return s;

}//

//ָ�����ȵ�,�ʺϺ��� \0 ���ַ��� 
lstring * NewStringLen(char * str, int len, struct MemPool * pool)
{
	lstring * s = NewString("", pool);
	LString_AppendCString(s, str, len);
	
	return s;

}//


void LString_Append(lstring * _s, lstring * _add)
{
	char * tmp = NULL;
	lstring s = (*_s); //����һ������������﷨//golang ��û�� -> 
	lstring add = (*_add);//����һ������������﷨//golang ��û�� -> 
	
	if (_add->len > 1024 * 1024)
		printf_err2("\r\n LString_Append() _add lstring error len: %d \r\n", _add->len); //̫�����ַ����ܿ����Ǵ��� 

	if (_s->len > 1024 * 1024)
		printf_err2("\r\n LString_Append() _s   lstring error len: %d \r\n", _s->len); //̫�����ַ����ܿ����Ǵ��� 
	
	
	
	tmp = malloc(s.len + add.len + 1); //��Ҫ����� \0 ��λ�� 
	memset(tmp, 0, s.len + add.len + 1);
	
	memcpy(tmp, s.str, s.len);
	memcpy(tmp + s.len, add.str, add.len); //����Ҫע��//���Ϻ��� 
	
	free(s.str);

	s.str = tmp;
	s.len = s.len + add.len;
	
//	char * tmp = malloc(s->len + add->len + 1); //��Ҫ����� \0 ��λ�� 
//	memset(tmp, 0, s->len + add->len + 1);
//	
//	memcpy(tmp, s->str, s->len);
//	memcpy(tmp + s->len, add->str, add->len); //����Ҫע��//���Ϻ��� 
//	
//	free(s->str);
//
//	s->str = tmp;
//	s->len = s->len + add->len;	


	*_s = s;
}//



//���봫ͳ C �ַ���
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

//����ַ���ָ���Ƿ�Ϸ�,ֻ�Ǽ򵥵ķ���,���ɿ�,����һ������ 
int CheckPString(lstring * s)
{
	if (s == NULL)
	{
		printf("CheckPString: error, string is NULL!!!\r\n"); //��Ŀǰ��Ҫ�Զ��ͷŵ�������˵,�ǲ���Ϊ NULL ��,��Ϊ���� pool ��û�д����� 
		return 0;
	}
	printf_err1("CheckPString: s->_init != NULL\r\n");
	if (s->_init != NULL) //��ʵ�����ִ��ı������Ͳ���ϵͳ��˵,��� s û�г�ʼ��,������ͺܿ��ܱ���,����ʵ�����Ǽ�ⲻ������,������β����ӡһ�¿����������û�����������������,������Ҫ��ߵĵط�,ȥ�� printf ��Ϳ��Բ���ӡ��
	{
		printf("CheckPString: error, string is not init!!!\r\n"); //��Ŀǰ��Ҫ�Զ��ͷŵ�������˵,�ǲ���Ϊ NULL ��,��Ϊ���� pool ��û�д����� 
		return 0;
	}

	printf_err1("CheckPString: s->_init != NULL ok.\r\n");
	return 1;
}//



//�󶨸�����Ա����
void BindStringFunctions(lstring * s) 
{
	printf_err1("BindStringFunctions \r\n");
	s->Append = LString_Append;
	s->AppendConst = LString_AppendConst;
	s->AppendCString = LString_AppendCString;

}//




#endif

