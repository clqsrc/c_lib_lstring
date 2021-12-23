
//操作 lstring * 的各种底层函数 
//因为 lstring 包含了传统 C 的 0 结尾,所以大部分可以直接代用 C 的函数,当然最好参照 golang 重写

#ifndef _L_STRING_FUNCTIONS_H_
#define _L_STRING_FUNCTIONS_H_

#ifdef __cplusplus
extern "C" {
#endif

//---------------------------------------------------------------

//据说是 gcc 的 strstr 实现,不过其中用到了另外一个函数,效率不可能高 
char * _strstr1(const char*s1,const char*s2);

//据说有种 kmp 算法更快 

//所以选择一个比较折衷的简单算法如下//返回的是位置,以 0 开始 
//虽然速度可能慢一点,不过可以保证正确 
//不过 golang 里也是调用了一个汇篇的函数来实现的,以后有空再来比较性能吧 
int _strnstr (const char * s, const char * sub, int slen, int sublen);


//delphi 转换方便函数//但 C 语言索引是从 0 开始,不是 d7 的从 1 开始,一定要注意
//查找子串位置
int pos(lstring * substr, lstring * s);


//查找子串位置 
int pos_c(const char * substr, lstring * s);

//是否有子字符串，可以安全的用在 lstring->str 上 //2021.11 很多时候没有的话很不方便
int FindStr_c(const char * s, const char * sub_s);


//substr(字符串,截取开始位置,截取长度)//从 0 开始 
//lstring * substr(lstring * s, int start, int len) 
lstring * substring(lstring * s, int start, int len) ;

//字符串是否相等 //类似 java 的 equals
int str_equals(lstring * s1, lstring * s2);

int streq(lstring * s1, lstring * s2);

int str_equals_c(lstring * s1, char * s2);

//常用简写而已 
int seq(lstring * s1, char * s2);

//替换单个字符,在需要高效时使用,因为替换一长串比较慢 
//str_replace 
lstring * str_replace_ch(lstring * s, char ch, char newch);


//主要用于判断空字符串//delphi 风格 
int length(lstring * s);

//delphi 风格 
int Length(lstring * s);

//主要用于判断空字符串//golang 风格 
int len(lstring * s);

//转换为小写,注意这个在中文下会有问题//为了兼容字符串自动释放,当参数为空时只好返回空,要不找不到父节点 
lstring * lowercase(lstring * s);

//转换为大写,注意这个在中文下会有问题//为了兼容字符串自动释放,当参数为空时只好返回空,要不找不到父节点 
lstring * uppercase(lstring * s);



//与传统 get_value 不同,这个的匹配结尾字符串为查找互的第一个而不是最后一个
//应该不区分大小写
//要注意 C 语言的字符串是从 0 起始,而原来 delphi 的是从 1 起始的,所以 d7 转换过来的算法不能全部照搬的
lstring * get_value_first(lstring *s, lstring * b_sp, lstring * e_sp);

//c 语言的参数 
lstring * getValueFirst_c(lstring *s, char * b_sp, char * e_sp);

//c 语言的参数 
lstring * get_value_first_c(lstring *s, char * b_sp, char * e_sp);


//按分隔符号分成两个字符串 
void string_sp_to2(lstring * s, lstring * sp, lstring ** _ls, lstring ** _rs);

//设置字符串长度,用 0 填充 
void setlength(lstring * s, int len);


//------------------------------------------------
#ifdef __cplusplus
}
#endif



#endif

