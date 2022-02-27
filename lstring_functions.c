
//���� lstring * �ĸ��ֵײ㺯�� 
//��Ϊ lstring �����˴�ͳ C �� 0 ��β,���Դ󲿷ֿ���ֱ�Ӵ��� C �ĺ���,��Ȼ��ò��� golang ��д

#ifndef _L_STRING_FUNCTIONS_C_
#define	_L_STRING_FUNCTIONS_C_

#include <stdio.h>
#include <string.h>

//#include "lstring.c"
#include "lstring.h"  //���Ƿֳ� .h �ļ����������

//��˵�� gcc �� strstr ʵ��,���������õ�������һ������,Ч�ʲ����ܸ� 
char * _strstr1(const char*s1,const char*s2)
{
    const char*p=s1;
    const size_t len=strlen(s2);
    for(;(p=strchr(p,*s2))!=0;p++)
    {
        if(strncmp(p,s2,len)==0)
            return (char*)p;
    }
    return(0);
}

//char * _strstr2 (
//        const char * str1,
//        const char * str2
//        )
//{
//        char *cp = (char *) str1;
//        char *s1, *s2;
//
//        if ( !*str2 )
//            return((char *)str1);
//
//        while (*cp)
//        {
//                s1 = cp;
//                s2 = (char *) str2;
//
//                while ( *s1 && *s2 && !(*s1-*s2) )
//                        s1++, s2++;
//
//                if (!*s2)
//                        return(cp);
//
//                cp++;
//        }
//
//        return(NULL);
//
//}

//��˵���� kmp �㷨���� 

//����ѡ��һ���Ƚ����Եļ��㷨����//���ص���λ��,�� 0 ��ʼ 
//��Ȼ�ٶȿ�����һ��,�������Ա�֤��ȷ 
//���� golang ��Ҳ�ǵ�����һ����ƪ�ĺ�����ʵ�ֵ�,�Ժ��п������Ƚ����ܰ� 
int _strnstr (const char * s, const char * sub, int slen, int sublen)
{
        const char * cp = s;
        const char * s1, *s2;
        int i,j;
        int find; //�Ƿ��ҵ���,��һ���ַ���ͬ����û�ҵ� 

        if (sublen == 0) //�Ӵ�Ϊ�վ���û�� 
            return -1;

        for (i=0; i<slen; i++) //����ÿ�����ַ����ַ� 
        {
                s1 = cp;
                s2 = sub;
                find = 1;

				for (j=0; j<sublen; j++) //����ÿ�����ַ����ַ�
				{
					//if (*s1 && *s2)
					if (*s1 != *s2) { find = 0; break;} //��һ���ַ���ƥ�����û�ҵ� 
						
					s1++, s2++; //�Ƚ���һ���ַ� 
				}


                //if (!*s2) return(cp);//��仰��ԭ����,�����ʱ s2 �����ַ����ͱ�ʾ�ҵ���(û���������������,�������ҵ�) 
                
				if (1 == find) return i; //�ҵ��˾�ֱ������ 

                cp++;
        }

        return -1;
}


//delphi ת�����㺯��//�� C ���������Ǵ� 0 ��ʼ,���� d7 �Ĵ� 1 ��ʼ,һ��Ҫע��
//�����Ӵ�λ�� 
//php �� strpos ������Ҳ������ 
int _pos_v1(lstring * substr, lstring * s) 
{
	char * p = strstr(s->str, substr->str);
	int r = -1;

	if (p == NULL) return -1;
	
	r = p - (s->str);
	if (r >= s->len) r = -1; //��� s �� const �Ļ�,���п��ܷ���������� 

	//return p - (s->str);
	return r;
}

int pos(lstring * substr, lstring * s) 
{
//	char * p = strstr(s->str, substr->str);
//	int r = -1;
//
//	if (p == NULL) return -1;
//	
//	r = p - (s->str);
//	if (r >= s->len) r = -1; //��� s �� const �Ļ�,���п��ܷ���������� 

	return _strnstr(s->str, substr->str, s->len, substr->len);
}


//�����Ӵ�λ�� 
int pos_c(const char * substr, lstring * s) 
{
//	char * p = strstr(s->str, substr);
//	int r = -1;
//
//	if (p == NULL) return -1;
//
//	r = p - (s->str);
//	if (r >= s->len) r = -1; //��� s �� const �Ļ�,���п��ܷ���������� 

	return _strnstr(s->str, substr, s->len, strlen(substr));

}

//�Ƿ������ַ��������԰�ȫ������ lstring->str �� //2021.11 �ܶ�ʱ��û�еĻ��ܲ�����
int FindStr_c(const char * s, const char * sub_s)
{
    const char * r = strstr(s, sub_s);
    
    if (r) return 1; //�ҵ���
    
    return 0; //û�ҵ�
}//


//substr(�ַ���,��ȡ��ʼλ��,��ȡ����)//�� 0 ��ʼ 
//lstring * substr(lstring * s, int start, int len) 
lstring * substring(lstring * s, int start, int len) 
{
	lstring r;
	lstring * sub;
	char * p = NULL;
	
	//if (start + len > s->len) return NULL;//���̫��
	if (start + len > s->len) len = (s->len) - start;//���̫�෵�غ���� 
	 
	p = s->str + start;
	
	r = StringConst(p, len, s->pool);
	//r = StringCopy(r); //StringConst �������ڴ��,����Ҫ����һ���µĳ��� 
	
	//sub = PString(r);
	//autofree_pstring(s->pool, sub);//�����ַ������� s һ���ͷ� 
	
	sub = PStringCopy(&r);
	
	return sub;

}

//�ַ����Ƿ���� //���� java �� equals
int str_equals(lstring * s1, lstring * s2)
{
	if (s1 == NULL && s2 == NULL) return 1;
	
	if (s1 == NULL || s2 == NULL) return 0; //���߲����ܶ�Ϊ�յ��������һ��Ϊ��,�Ǿ��ǲ������ 
	
	if (s1->len != s2->len) return 0; //���Ȳ��ȿ϶�Ҳ���� 
	
	if ( 0 == strncmp(s1->str, s2->str, s1->len) ) return 1;

	return 0;
}

int streq(lstring * s1, lstring * s2)
{
	return str_equals(s1, s2); 
}

int str_equals_c(lstring * s1, char * s2)
{
	if (s1->len != strlen(s2)) return 0; //���Ȳ��ȿ϶�Ҳ���� //�Ӹ� strlen �жϻ���һ��,����ȷ���϶���ȷ 
	
	//if ( 0 == strcmp(s1->str, s2) ) return 1;
	if ( 0 == strncmp(s1->str, s2, s1->len) ) return 1;

	return 0;
}

//3) ǰ��#�������ת��Ϊ�ַ�����
//#define C(x) #x
//��C(1+1) �� ��1+1����

//#define C(x) #x
#define C(x) 
#define C1(a1, x, a2) strcmp(a1, a2)

//int ee_-()
void t()
{
	C(==);
	C1("",==,""); //��������ģ��һ�� == �ų���

}

//���ü�д���� 
int seq(lstring * s1, char * s2)
{

	return str_equals_c(s1, s2);
}

//�滻�����ַ�,����Ҫ��Чʱʹ��,��Ϊ�滻һ�����Ƚ��� 
//str_replace 
lstring * str_replace_ch(lstring * s, char ch, char newch)
{
	lstring * r = PStringCopy(s);
	
	int i;
	
	for (i=0; i<r->len; i++)
	{
		if (r->str[i] == ch)
			r->str[i] = newch;
	}
	
	return r;

}//

//�滻���ַ�//ֻ�滻һ�Σ��û������Լ�ʵ��ȫ�滻����Ϊȫ�滻Ч����֤���ϣ�����Ҳ��̫�����Ż��Ͼ���
//str_replace 
//��ʵ���ǽ��ַ������ָ����ֳ��������֣�Ȼ�����//�Ժ��ٳ��Ż���
lstring * str_replace_first(lstring * s, lstring * olds, lstring * news)
{
	//lstring * r = PStringCopy(s);
	lstring * r  = NewString("", s->pool);
	lstring * ls = NewString("", s->pool);
	lstring * rs = NewString("", s->pool);
	
	//int i;
	
	int find = string_sp_to2(s, olds, &ls, &rs);

	if (1 == find) {
		r->Append(r, ls);
		r->Append(r, news);
		r->Append(r, rs);
	}else{
		return s;
	}
	
	return r;

}//

//�Ժ����Ż�
lstring * str_replace(lstring * s, lstring * olds, lstring * news)
//struct LString * str_replace(struct LString * s, struct LString * olds, struct LString * news)
{
	//lstring * r = PStringCopy(s);
	lstring * r = str_replace_first(s, olds, news);
	
	int i;
	
	for (i = 0; i < 1024; i++) //��ѭ��������ֻ�滻 1024 ��
	{
		int bfind = FindStr_c(r->str, olds->str);

		if (0 == bfind) break;

		r = str_replace_first(r, olds, news);
	}//
	
	
	return r;

}//


//��Ҫ�����жϿ��ַ���//delphi ��� 
int length(lstring * s)
{
	if (s == NULL) return 0;
	
	return s->len;
}

//delphi ��� 
int Length(lstring * s)
{
	return length(s);
}

//��Ҫ�����жϿ��ַ���//golang ��� 
int len(lstring * s)
{
	if (s == NULL) return 0;
	
	return s->len;
}

//ת��ΪСд,ע������������»�������//Ϊ�˼����ַ����Զ��ͷ�,������Ϊ��ʱֻ�÷��ؿ�,Ҫ���Ҳ������ڵ� 
lstring * lowercase(lstring * s)
{
	lstring * r = NULL;
	int i;
	if (s == NULL)
	{
		printf("lowercase: error, string is NULL!!!"); //��Ŀǰ��Ҫ�Զ��ͷŵ�������˵,�ǲ���Ϊ NULL ��,��Ϊ���� pool ��û�д����� 
		return NULL;
	}
	
	//����ַ���ָ���Ƿ�Ϸ�,ֻ�Ǽ򵥵ķ���,���ɿ�,����һ������ 
	CheckPString(s);
	
	r = PStringCopy(s);
	//r->pool = s->pool; //��ԭ�ַ����ͷ���
	//autofree_pstring(s->pool, r); //��ԭ�ַ����ͷ���
	
	for (i = 0; i < r->len; i++) 
	{
		r->str[i] = tolower(r->str[i]);
	}
	
	//û�з���ֵ,ȷʵ������ 
	
	return r;
}

//ת��Ϊ��д,ע������������»�������//Ϊ�˼����ַ����Զ��ͷ�,������Ϊ��ʱֻ�÷��ؿ�,Ҫ���Ҳ������ڵ� 
lstring * uppercase(lstring * s)
{
	lstring * r = NULL;
	int i;
	if (s == NULL)
	{
		printf("lowercase: error, string is NULL!!!"); //��Ŀǰ��Ҫ�Զ��ͷŵ�������˵,�ǲ���Ϊ NULL ��,��Ϊ���� pool ��û�д����� 
		return NULL;
	}
	
	//����ַ���ָ���Ƿ�Ϸ�,ֻ�Ǽ򵥵ķ���,���ɿ�,����һ������ 
	CheckPString(s);
	
	r = PStringCopy(s);
	//r->pool = s->pool; //��ԭ�ַ����ͷ���
	//autofree_pstring(s->pool, r); //��ԭ�ַ����ͷ���
	
	for (i = 0; i < r->len; i++) 
	{
		r->str[i] = toupper(r->str[i]);
	}
	
	//û�з���ֵ,ȷʵ������ 
	
	return r;
}


//�봫ͳ get_value ��ͬ,�����ƥ���β�ַ���Ϊ���һ��ĵ�һ�����������һ��
//Ӧ�ò����ִ�Сд
//Ҫע�� C ���Ե��ַ����Ǵ� 0 ��ʼ,��ԭ�� delphi ���Ǵ� 1 ��ʼ��,���� d7 ת���������㷨����ȫ���հ��
lstring * get_value_first(lstring *s, lstring * b_sp, lstring * e_sp)
{
	
	//��ʼ���Ƶ�λ��
	int b_pos = 0;
	//���ƽ�����λ��
	int e_pos = 0;
	lstring * ls;
	lstring * r = NULL;
	
	if (len(e_sp) == 0) e_pos = length(s);

	ls = lowercase(s);
	b_sp = lowercase(b_sp);
	e_sp = lowercase(e_sp);

	//����ַ���ָ���Ƿ�Ϸ�,ֻ�Ǽ򵥵ķ���,���ɿ�,����һ������
	CheckPString(ls);
	CheckPString(b_sp);
	CheckPString(e_sp);
	//--------------------------------------------------

	b_pos = pos(b_sp, ls);

	//if (length(b_sp) == 0) b_pos = 1;
	if (length(b_sp) == 0) b_pos = 0;

	//if (b_pos == 0)
	if (b_pos == -1) //û�ҵ�
	{
		r = NewString("", s->pool);
		//autofree_pstring(s->pool, r); //���ص��ַ������� s һ���ͷ�
		return r;
	};

	b_pos = b_pos + length(b_sp);
	r = substring(s, b_pos, length(s));
	//result = copy(s, b_pos, length(s));

	//--------------------------------------------------

	//e_pos = pos(e_sp, lowercase(r)) - 1;
	e_pos = pos(e_sp, lowercase(r));

	if (e_pos == -1) e_pos = length(r);

	//r = substring(r, 1, e_pos);
	r = substring(r, 0, e_pos);

	return r;
}

//c ���ԵĲ��� 
lstring * getValueFirst_c(lstring *s, char * b_sp, char * e_sp)
{
	return get_value_first(s, NewString(b_sp, s->pool), NewString(e_sp, s->pool));

}

//c ���ԵĲ��� 
lstring * get_value_first_c(lstring *s, char * b_sp, char * e_sp)
{
	return get_value_first(s, NewString(b_sp, s->pool), NewString(e_sp, s->pool));

}






//���ָ����ŷֳ������ַ��� 
//2021.11.28 û�ҵ���ô�����ַ�����η��䣿�Ժ���ϸ�����ȼ�һ������ֵ��ʾ��û���ҵ�
int string_sp_to2(lstring * s, lstring * sp, lstring ** _ls, lstring ** _rs)
{

	
	//��ʼ���Ƶ�λ��
	int pos1 = 0;
	//���ƽ�����λ��
	int pos2 = 0;
	int len1, len2;
	lstring * ls;
	lstring * rs;
	//lstring * t;
	

	//����ַ���ָ���Ƿ�Ϸ�,ֻ�Ǽ򵥵ķ���,���ɿ�,����һ������
	CheckPString(s);
	CheckPString(sp);
	//CheckPString(e_sp);
	//--------------------------------------------------

	pos1 = pos(sp, s);

	if (pos1 == -1) //û�ҵ�
	{
		ls = NewString("", s->pool); //û�ҵ�����������ַ���Ϊ��,���ַ���Ϊԭ�ַ��� 
		rs = PStringCopy(s);

		*_ls = ls;
		*_rs = rs;

		return 0;
	};
	
	ls = substring(s, 0, pos1);
	
	pos2 = pos1 + length(sp);
	len2 = length(s) - pos2;
	
	rs = substring(s, pos2, len2);

	//-------------------------------------------------- 
	*_ls = ls;
	*_rs = rs;


	return 1;
} //

//�����ַ�������,�� 0 ��� 
void setlength(lstring * s, int len)
{
	
	char * tmp;
	
	if (len <= s->len) return;
	

	tmp = (char *)malloc(len + 1); //��Ҫ����� \0 ��λ�� 
	memset(tmp, 0, len + 1);
	
	memcpy(tmp, s->str, s->len);
	
	free(s->str);

	s->str = tmp;
	s->len = len;


}//
#endif

