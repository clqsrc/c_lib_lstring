
//修改自其他项目的 socketplus_t1.c 文件 

#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <winsock.h>

#include "lstring.c"
#include "socketplus.c"
#include "lstring_functions.c"

//vc 下要有可能要加 lib 
//#pragma comment (lib,"*.lib")
//#pragma comment (lib,"libwsock32.a")
//#pragma comment (lib,"libwsock32.a")



//SOCKET gSo = 0;
SOCKET gSo = -1;


//收取一行,可再优化 
lstring * RecvLine(SOCKET so, struct MemPool * pool, lstring ** _buf)
{
	int i = 0;
	int index = -1;
	int canread = 0;
	lstring * r = NULL;
	lstring * s = NULL;
	lstring * buf = *_buf;

	for (i=0;i<10;i++) //安全起见,不用 while ,用 for 一定次数就可以了 
	{
		//index = pos("\n", buf);
		index = pos(NewString("\r\n", pool), buf);

		if (index>-1) break;
		
		canread = SelectRead_Timeout(so, 3);//是否可读取,时间//超时返回,单位为秒 
		if (0 == canread) break;
		
		s = RecvBuf(so, pool);
		buf->Append(buf, s);
	}
	
	if (index <0 ) return NewString("", pool);
	
	r = substring(buf, 0, index);
	buf = substring(buf, index + 2, Length(buf));
	*_buf = buf;
	return r;
}//

//解码一行命令,这里比较简单就是按空格进行分隔就行了
//这是用可怕的指针运算的版本 
void DecodeCmd(lstring * line, char sp, char ** cmds, int cmds_count)
{
	int i = 0;
	int index = 0;
	int count = 0;
	
	cmds[index] = line->str;
	
	for (i=0; i<line->len; i++)
	{
		if (sp == line->str[i]) 
		{
			index++;
			line->str[i] = '\0'; //直接修改为字符串结束符号,如果是只读的字符串这样做其实是不对的,不过效率很高 
			
			cmds[index] = line->str + i; //指针向后移动 
			
			if (i >= line->len - 1) break;//如果是最后一定字符了就要退出,如果不是指针还要再移动一位
			cmds[index] = line->str + i + 1;
			
			count++;
			if (count >= cmds_count) break; //不要大于缓冲区 
		}
		
	}//


}//

//读取多行结果 
lstring * RecvMCmd(SOCKET so, struct MemPool * pool, lstring ** _buf)
{
	int i = 0;
	int index = 0;
	int count = 0;
	lstring * rs;
	char c4 = '\0'; //判断第4个字符 
	
	lstring * mline = NewString("", pool);

	
	for (i=0; i<50; i++)
	{
		rs = RecvLine(so, pool, _buf); //只收取一行 
		
		mline->Append(mline, rs);
		LString_AppendConst(mline, "\r\n");
		
		//printf("\r\nRecvMCmd:%s\r\n", rs->str); 
			
		if (rs->len<4) break; //长度要足够 
		c4 = rs->str[4-1]; //第4个字符
		//if ('\x20' == c4) break; //"\xhh" 任意字符 二位十六进制//其实现在的转义符已经扩展得相当复杂，不建议用这个表示空格 
		if (' ' == c4) break; //第4个字符是空格就表示读取完了//也可以判断 "250[空格]" 
	
		
	}//

	return mline;
}//


void main()
{
	int r;
	mempool mem, * m;
	lstring * s;
	lstring * rs;
	lstring * buf;
	lstring * domain;
	
	char * cmds[5] = {NULL}; 
	int cmds_count = 5;
	
	//--------------------------------------------------

	mem = makemem(); m = &mem; //内存池,重要 
	
	buf = NewString("", m);

	//--------------------------------------------------
	//直接装载各个 dll 函数
	LoadFunctions_Socket();

	InitWinSocket(); //初始化 socket, windows 下一定要有 


	gSo = CreateTcpClient();
	//r = ConnectHost(gSo, "newbt.net", 25);
	r = ConnectHost(gSo, "smtp.163.com", 25);

	if (r == 1) printf("连接成功!\r\n");

	
	//--------------------------------------------------
	rs = RecvLine(gSo, m, &buf); //只收取一行 

	printf("\r\nRecvLine:");
	printf(rs->str); printf("\r\n");
	
	DecodeCmd(rs, ' ', cmds, cmds_count); 
	printf("\r\ndomain:%s\r\n", cmds[1]); 
	
	domain = NewString(cmds[1], m);
	
	s = NewString("EHLO", m);
	LString_AppendConst(s," ");
	s->Append(s, domain); //去掉这一行试试,163 邮箱就会返回错误了 
	LString_AppendConst(s,"\r\n");

	SendBuf(gSo, s->str, s->len);	

	
	////rs = RecvLine(gSo, m, &buf); //只收取一行 
	rs = RecvMCmd(gSo, m, &buf); //只收取一行 

	printf("\r\nRecvLine:");
	printf(rs->str); printf("\r\n");
	

	//--------------------------------------------------
    
    Pool_Free(&mem); //释放内存池 
    
    printf("gMallocCount:%d \r\n", gMallocCount); //看看有没有内存泄漏//简单的检测而已  
    
    //-------------------------------------------------- 

    getch(); //getch().不过在VC中好象要用getch()，必须在头文件中加上<conio.h> 

}




