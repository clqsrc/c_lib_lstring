
//操作 lstring * 的各种底层函数 
//因为 lstring 包含了传统 C 的 0 结尾,所以大部分可以直接代用 C 的函数,当然最好参照 golang 重写

#ifndef _L_STRING_FUNCTIONS_C_
#define	_L_STRING_FUNCTIONS_C_

#include <stdio.h>
#include <string.h>

//#include "lstring.c"
#include "lstring.h"  //还是分出 .h 文件更方便编译

//据说是 gcc 的 strstr 实现,不过其中用到了另外一个函数,效率不可能高 
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

//据说有种 kmp 算法更快 

//所以选择一个比较折衷的简单算法如下//返回的是位置,以 0 开始 
//虽然速度可能慢一点,不过可以保证正确 
//不过 golang 里也是调用了一个汇篇的函数来实现的,以后有空再来比较性能吧 
int _strnstr (const char * s, const char * sub, int slen, int sublen)
{
        const char * cp = s;
        const char * s1, *s2;
        int i,j;
        int find; //是否找到了,有一个字符不同就是没找到 

        if (sublen == 0) //子串为空就是没有 
            return -1;

        for (i=0; i<slen; i++) //遍历每个长字符串字符 
        {
                s1 = cp;
                s2 = sub;
                find = 1;

				for (j=0; j<sublen; j++) //遍历每个子字符串字符
				{
					//if (*s1 && *s2)
					if (*s1 != *s2) { find = 0; break;} //有一个字符不匹配就是没找到 
						
					s1++, s2++; //比较下一个字符 
				}


                //if (!*s2) return(cp);//这句话的原意是,如果这时 s2 还有字符串就表示找到了(没遍历找完就跳出了,所以是找到) 
                
				if (1 == find) return i; //找到了就直接跳出 

                cp++;
        }

        return -1;
}


//delphi 转换方便函数//但 C 语言索引是从 0 开始,不是 d7 的从 1 开始,一定要注意
//查找子串位置 
//php 的 strpos 基本上也是这样 
int _pos_v1(lstring * substr, lstring * s) 
{
	char * p = strstr(s->str, substr->str);
	int r = -1;

	if (p == NULL) return -1;
	
	r = p - (s->str);
	if (r >= s->len) r = -1; //如果 s 是 const 的话,是有可能发生这种情况 

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
//	if (r >= s->len) r = -1; //如果 s 是 const 的话,是有可能发生这种情况 

	return _strnstr(s->str, substr->str, s->len, substr->len);
}


//查找子串位置 
int pos_c(const char * substr, lstring * s) 
{
//	char * p = strstr(s->str, substr);
//	int r = -1;
//
//	if (p == NULL) return -1;
//
//	r = p - (s->str);
//	if (r >= s->len) r = -1; //如果 s 是 const 的话,是有可能发生这种情况 

	return _strnstr(s->str, substr, s->len, strlen(substr));

}

//是否有子字符串，可以安全的用在 lstring->str 上 //2021.11 很多时候没有的话很不方便
int FindStr_c(const char * s, const char * sub_s)
{
    const char * r = strstr(s, sub_s);
    
    if (r) return 1; //找到了
    
    return 0; //没找到
}//


//substr(字符串,截取开始位置,截取长度)//从 0 开始 
//lstring * substr(lstring * s, int start, int len) 
lstring * substring(lstring * s, int start, int len) 
{
	lstring r;
	lstring * sub;
	char * p = NULL;
	
	//if (start + len > s->len) return NULL;//如果太多
	if (start + len > s->len) len = (s->len) - start;//如果太多返回后面的 
	 
	p = s->str + start;
	
	r = StringConst(p, len, s->pool);
	//r = StringCopy(r); //StringConst 不分配内存的,所以要复制一个新的出来 
	
	//sub = PString(r);
	//autofree_pstring(s->pool, sub);//返回字符串跟着 s 一起释放 
	
	sub = PStringCopy(&r);
	
	return sub;

}

//字符串是否相等 //类似 java 的 equals
int str_equals(lstring * s1, lstring * s2)
{
	if (s1 == NULL && s2 == NULL) return 1;
	
	if (s1 == NULL || s2 == NULL) return 0; //两者不可能都为空的情况下有一个为空,那就是不相等了 
	
	if (s1->len != s2->len) return 0; //长度不等肯定也不是 
	
	if ( 0 == strncmp(s1->str, s2->str, s1->len) ) return 1;

	return 0;
}

int streq(lstring * s1, lstring * s2)
{
	return str_equals(s1, s2); 
}

int str_equals_c(lstring * s1, char * s2)
{
	if (s1->len != strlen(s2)) return 0; //长度不等肯定也不是 //加个 strlen 判断会慢一点,不过确保肯定正确 
	
	//if ( 0 == strcmp(s1->str, s2) ) return 1;
	if ( 0 == strncmp(s1->str, s2, s1->len) ) return 1;

	return 0;
}

//3) 前加#，将标记转换为字符串．
//#define C(x) #x
//则C(1+1) 即 ”1+1”．

//#define C(x) #x
#define C(x) 
#define C1(a1, x, a2) strcmp(a1, a2)

//int ee_-()
void t()
{
	C(==);
	C1("",==,""); //可以这样模拟一个 == 号出来

}

//常用简写而已 
int seq(lstring * s1, char * s2)
{

	return str_equals_c(s1, s2);
}

//替换单个字符,在需要高效时使用,因为替换一长串比较慢 
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

//替换长字符//只替换一次，用户可以自己实现全替换，因为全替换效率验证保障，笔者也不太想在优化上纠缠
//str_replace 
//其实就是将字符串按分隔符分成两个部分，然后加上//以后再出优化版
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

//以后再优化
lstring * str_replace(lstring * s, lstring * olds, lstring * news)
//struct LString * str_replace(struct LString * s, struct LString * olds, struct LString * news)
{
	//lstring * r = PStringCopy(s);
	lstring * r = str_replace_first(s, olds, news);
	
	int i;
	
	for (i = 0; i < 1024; i++) //死循环保护，只替换 1024 次
	{
		int bfind = FindStr_c(r->str, olds->str);

		if (0 == bfind) break;

		r = str_replace_first(r, olds, news);
	}//
	
	
	return r;

}//


//主要用于判断空字符串//delphi 风格 
int length(lstring * s)
{
	if (s == NULL) return 0;
	
	return s->len;
}

//delphi 风格 
int Length(lstring * s)
{
	return length(s);
}

//主要用于判断空字符串//golang 风格 
int len(lstring * s)
{
	if (s == NULL) return 0;
	
	return s->len;
}

//转换为小写,注意这个在中文下会有问题//为了兼容字符串自动释放,当参数为空时只好返回空,要不找不到父节点 
lstring * lowercase(lstring * s)
{
	lstring * r = NULL;
	int i;
	if (s == NULL)
	{
		printf("lowercase: error, string is NULL!!!"); //就目前的要自动释放的需求来说,是不能为 NULL 的,因为那样 pool 就没有传入了 
		return NULL;
	}
	
	//检查字符串指针是否合法,只是简单的方法,不可靠,但有一定作用 
	CheckPString(s);
	
	r = PStringCopy(s);
	//r->pool = s->pool; //让原字符串释放它
	//autofree_pstring(s->pool, r); //让原字符串释放它
	
	for (i = 0; i < r->len; i++) 
	{
		r->str[i] = tolower(r->str[i]);
	}
	
	//没有返回值,确实有问题 
	
	return r;
}

//转换为大写,注意这个在中文下会有问题//为了兼容字符串自动释放,当参数为空时只好返回空,要不找不到父节点 
lstring * uppercase(lstring * s)
{
	lstring * r = NULL;
	int i;
	if (s == NULL)
	{
		printf("lowercase: error, string is NULL!!!"); //就目前的要自动释放的需求来说,是不能为 NULL 的,因为那样 pool 就没有传入了 
		return NULL;
	}
	
	//检查字符串指针是否合法,只是简单的方法,不可靠,但有一定作用 
	CheckPString(s);
	
	r = PStringCopy(s);
	//r->pool = s->pool; //让原字符串释放它
	//autofree_pstring(s->pool, r); //让原字符串释放它
	
	for (i = 0; i < r->len; i++) 
	{
		r->str[i] = toupper(r->str[i]);
	}
	
	//没有返回值,确实有问题 
	
	return r;
}


//与传统 get_value 不同,这个的匹配结尾字符串为查找互的第一个而不是最后一个
//应该不区分大小写
//要注意 C 语言的字符串是从 0 起始,而原来 delphi 的是从 1 起始的,所以 d7 转换过来的算法不能全部照搬的
lstring * get_value_first(lstring *s, lstring * b_sp, lstring * e_sp)
{
	
	//开始复制的位置
	int b_pos = 0;
	//复制结束的位置
	int e_pos = 0;
	lstring * ls;
	lstring * r = NULL;
	
	if (len(e_sp) == 0) e_pos = length(s);

	ls = lowercase(s);
	b_sp = lowercase(b_sp);
	e_sp = lowercase(e_sp);

	//检查字符串指针是否合法,只是简单的方法,不可靠,但有一定作用
	CheckPString(ls);
	CheckPString(b_sp);
	CheckPString(e_sp);
	//--------------------------------------------------

	b_pos = pos(b_sp, ls);

	//if (length(b_sp) == 0) b_pos = 1;
	if (length(b_sp) == 0) b_pos = 0;

	//if (b_pos == 0)
	if (b_pos == -1) //没找到
	{
		r = NewString("", s->pool);
		//autofree_pstring(s->pool, r); //返回的字符串跟着 s 一起释放
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

//c 语言的参数 
lstring * getValueFirst_c(lstring *s, char * b_sp, char * e_sp)
{
	return get_value_first(s, NewString(b_sp, s->pool), NewString(e_sp, s->pool));

}

//c 语言的参数 
lstring * get_value_first_c(lstring *s, char * b_sp, char * e_sp)
{
	return get_value_first(s, NewString(b_sp, s->pool), NewString(e_sp, s->pool));

}






//按分隔符号分成两个字符串 
//2021.11.28 没找到那么左右字符串如何分配？以后再细究。先加一个返回值表示有没有找到
int string_sp_to2(lstring * s, lstring * sp, lstring ** _ls, lstring ** _rs)
{

	
	//开始复制的位置
	int pos1 = 0;
	//复制结束的位置
	int pos2 = 0;
	int len1, len2;
	lstring * ls;
	lstring * rs;
	//lstring * t;
	

	//检查字符串指针是否合法,只是简单的方法,不可靠,但有一定作用
	CheckPString(s);
	CheckPString(sp);
	//CheckPString(e_sp);
	//--------------------------------------------------

	pos1 = pos(sp, s);

	if (pos1 == -1) //没找到
	{
		ls = NewString("", s->pool); //没找到的情况下左字符串为空,右字符串为原字符串 
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

//设置字符串长度,用 0 填充 
void setlength(lstring * s, int len)
{
	
	char * tmp;
	
	if (len <= s->len) return;
	

	tmp = (char *)malloc(len + 1); //还要留最后 \0 的位置 
	memset(tmp, 0, len + 1);
	
	memcpy(tmp, s->str, s->len);
	
	free(s->str);

	s->str = tmp;
	s->len = len;


}//
#endif

