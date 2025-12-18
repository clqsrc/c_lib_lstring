
//操作 lstring * 的各种底层函数 
//因为 lstring 包含了传统 C 的 0 结尾,所以大部分可以直接代用 C 的函数,当然最好参照 golang 重写

#ifndef _BASE64_FUNCTIONS_C_
#define	_BASE64_FUNCTIONS_C_

#include <stdio.h>
#include <string.h>

#include "lstring.c"
#include "base64.c"


//base64 解码
lstring * base64_decode(lstring * s) 
{
	lstring * r = NewString("", s->pool);
	int len = 0;

	//setlength(r, s->len * 2); //按道理应该相等就可以了,这里只是为了安全而已
	setlength(r, s->len * 2 + 4); //按道理应该相等就可以了,这里只是为了安全而已

	//其实还要判断长度是否为4的倍数
	len = _base64_decode(s->str, s->len, r->str);
	r->len = len;

	return r;
}

//base64 编码
lstring * base64_encode(lstring * s) 
{
	lstring * r = NewString("", s->pool);
	int len = 0;

	//setlength(r, s->len * 2); //按道理应该相等就可以了,这里只是为了安全而已
	setlength(r, s->len * 2 + 4); //按道理应该相等就可以了,这里只是为了安全而已

	//其实还要判断长度是否为4的倍数
	len = _base64_encode(s->str, s->len, r->str);
	r->len = len;

	return r;
}




#endif

