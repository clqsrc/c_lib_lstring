

#include "lstring.h"
#include "lstring_list.h"
#include "string.h"

//绑定各个成员函数
void BindStringListFunctions(lstring_list * list);

void make_stringlist(lstring_list * list)
{
	memset(list, 0, sizeof(struct LStringList));

	list->mem = newmempool();

	//绑定各个成员函数
	BindStringListFunctions(list);

}//

lstring_list * NewStringList()
{
	lstring_list * list = (lstring_list *)malloc(sizeof(struct LStringList));

	make_stringlist(list);

	return list;

}//

void FreeStringList(lstring_list * list)
{
	freemempool(list->mem);

	memset(&list, 0, sizeof(struct LStringList));
	free(list);
}//

//----------------------------------------------------------------
//加入字符串
void LStringList_Add(lstring_list * list, lstring * s)
{
    int _buf_count = list->_buf_count; //缓存中可以存放的 item 指针个数

	list->count = list->count + 1;

	if (list->_buf_count < list->count)  //空间不足了就要扩大
	{
		list->_buf_count = list->_buf_count * 2 + 16;

		list->_list_addr_buf = (char *)malloc(list->_buf_count * sizeof(void *));
		list->items = (lstring**)list->_list_addr_buf;
	}

	lstring ** pos = (lstring **)list->_list_addr_buf;

	//pos[list->count -1] = s; //这样赋值应该也是可以的

	pos = pos + (list->count -1); //跳到要保存的位置

	(*pos) = s;


}//


//加入传统 C 字符串
void LStringList_AddCString(lstring_list * list, const char * str, int len)
{
	lstring * s = NewString("", list->mem);
	LString_AppendCString(s, str, len);

	LStringList_Add(list, s);

}//

void LStringList_AddConst(lstring_list * list, const char * add)
{
	lstring * s = NewString("", list->mem);
	LString_AppendConst(s, add);

	LStringList_Add(list, s);

}//

void LStringList_Clear(lstring_list * list)
{

}

//绑定各个成员函数
void BindStringListFunctions(lstring_list * list) 
{
	//printf_err1("BindStringFunctions \r\n");
	list->Add = LStringList_Add;
	list->AddConst = LStringList_AddConst;
    list->AddCString = LStringList_AddCString;
    list->Clear = LStringList_Clear;
    //list->equals_lstring = LString_equals_lstring;

}//