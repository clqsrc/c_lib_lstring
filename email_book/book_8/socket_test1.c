
//�޸���������Ŀ�� socketplus_t1.c �ļ� 

#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <winsock.h>

#include "lstring.c"
#include "socketplus.c"
#include "lstring_functions.c"

//vc ��Ҫ�п���Ҫ�� lib 
//#pragma comment (lib,"*.lib")
//#pragma comment (lib,"libwsock32.a")
//#pragma comment (lib,"libwsock32.a")



//SOCKET gSo = 0;
SOCKET gSo = -1;


//��ȡһ��,�����Ż� 
lstring * RecvLine(SOCKET so, struct MemPool * pool, lstring ** _buf)
{
	int i = 0;
	int index = -1;
	lstring * r = NULL;
	lstring * s = NULL;
	lstring * buf = *_buf;

	for (i=0;i<10;i++) //��ȫ���,���� while ,�� for һ�������Ϳ����� 
	{
		//index = pos("\n", buf);
		index = pos(NewString("\r\n", pool), buf);

		if (index>-1) break;
		s = RecvBuf(so, pool);
		buf->Append(buf, s);
	}
	
	if (index <0 ) return NewString("", pool);
	
	r = substring(buf, 0, index);
	buf = substring(buf, index + 2, Length(buf));
	*_buf = buf;
	return r;
}//

//����һ������,����Ƚϼ򵥾��ǰ��ո���зָ�������
//�����ÿ��µ�ָ������İ汾 
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
			line->str[i] = '\0'; //ֱ���޸�Ϊ�ַ�����������,�����ֻ�����ַ�����������ʵ�ǲ��Ե�,����Ч�ʺܸ� 
			
			cmds[index] = line->str + i; //ָ������ƶ� 
			
			if (i >= line->len - 1) break;//��������һ���ַ��˾�Ҫ�˳�,�������ָ�뻹Ҫ���ƶ�һλ
			cmds[index] = line->str + i + 1;
			
			count++;
			if (count >= cmds_count) break; //��Ҫ���ڻ����� 
		}
		
	}//


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

	mem = makemem(); m = &mem; //�ڴ��,��Ҫ 
	
	buf = NewString("", m);

	//--------------------------------------------------
	//ֱ��װ�ظ��� dll ����
	LoadFunctions_Socket();

	InitWinSocket(); //��ʼ�� socket, windows ��һ��Ҫ�� 


	gSo = CreateTcpClient();
	//r = ConnectHost(gSo, "newbt.net", 25);
	r = ConnectHost(gSo, "smtp.163.com", 25);

	if (r == 1) printf("���ӳɹ�!\r\n");

//	s = NewString("EHLO\r\n", m);
//
//	SendBuf(gSo, s->str, s->len);
//
//
//	printf(s->str);
//	s->Append(s, s);
//	printf(s->str);
//
//	s->AppendConst(s, "����\r\n");
//
//	printf(s->str);
	
	//--------------------------------------------------
	rs = RecvLine(gSo, m, &buf); //ֻ��ȡһ�� 

	printf("\r\nRecvLine:");
	printf(rs->str); printf("\r\n");
	
	DecodeCmd(rs, ' ', cmds, cmds_count); 
	printf("\r\ndomain:%s\r\n", cmds[1]); 
	
	domain = NewString(cmds[1], m);
	
	s = NewString("EHLO", m);
	LString_AppendConst(s," ");
	s->Append(s, domain); //ȥ����һ������,163 ����ͻ᷵�ش����� 
	LString_AppendConst(s,"\r\n");

	SendBuf(gSo, s->str, s->len);	
	
	rs = RecvLine(gSo, m, &buf); //ֻ��ȡһ�� 

	printf("\r\nRecvLine:");
	printf(rs->str); printf("\r\n");
	
	rs = RecvLine(gSo, m, &buf); //ֻ��ȡһ�� 

	printf("\r\nRecvLine:");
	printf(rs->str); printf("\r\n");
	
	//-------------------------------------------------- 

//	rs = RecvBuf(gSo, m); //ע���������ֻ����ȡһ�� 
//
//	printf("\r\nRecvBuf:\r\n");
//	printf(rs->str);
//	
//	rs = RecvBuf(gSo, m); //ע���������ֻ����ȡһ�� 
//	printf("\r\nRecvBuf:\r\n");
//	printf(rs->str);

	//--------------------------------------------------
    
    Pool_Free(&mem); //�ͷ��ڴ�� 
    
    printf("gMallocCount:%d \r\n", gMallocCount); //������û���ڴ�й©//�򵥵ļ�����  
    
    //-------------------------------------------------- 

    getch(); //getch().������VC�к���Ҫ��getch()��������ͷ�ļ��м���<conio.h> 

}




