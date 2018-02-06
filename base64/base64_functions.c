
//���� lstring * �ĸ��ֵײ㺯�� 
//��Ϊ lstring �����˴�ͳ C �� 0 ��β,���Դ󲿷ֿ���ֱ�Ӵ��� C �ĺ���,��Ȼ��ò��� golang ��д

#ifndef _BASE64_FUNCTIONS_C_
#define	_BASE64_FUNCTIONS_C_

#include <stdio.h>
#include <string.h>

#include "lstring.c"
#include "base64.c"


//base64 ����
lstring * base64_decode(lstring * s) 
{
	lstring * r = NewString("", s->pool);
	int len = 0;

	//setlength(r, s->len * 2); //������Ӧ����ȾͿ�����,����ֻ��Ϊ�˰�ȫ����
	setlength(r, s->len * 2 + 4); //������Ӧ����ȾͿ�����,����ֻ��Ϊ�˰�ȫ����

	//��ʵ��Ҫ�жϳ����Ƿ�Ϊ4�ı���
	len = _base64_decode(s->str, s->len, r->str);
	r->len = len;

	return r;
}

//base64 ����
lstring * base64_encode(lstring * s) 
{
	lstring * r = NewString("", s->pool);
	int len = 0;

	//setlength(r, s->len * 2); //������Ӧ����ȾͿ�����,����ֻ��Ϊ�˰�ȫ����
	setlength(r, s->len * 2 + 4); //������Ӧ����ȾͿ�����,����ֻ��Ϊ�˰�ȫ����

	//��ʵ��Ҫ�жϳ����Ƿ�Ϊ4�ı���
	len = _base64_encode(s->str, s->len, r->str);
	r->len = len;

	return r;
}




#endif

