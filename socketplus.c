
//һ��������� socket �����С�ļ�,ʡ�������� lib a �ļ� 
//Ŀǰ�� gcc ר��,��� vc Ҫ������Ūһ������,��Ҫ��������Ū�������� 


#ifndef _SOCKET_PLUS_C_
#define	_SOCKET_PLUS_C_

#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <winsock.h>
//#include <>

#include "lstring.c"

//#pragma comment (lib,"*.lib")
//#pragma comment (lib,"libwsock32.a")
//#pragma comment (lib,"libwsock32.a")


// ϵͳ������Ϣ��ʾ
void PrintError(DWORD last_err);

//--------------------------------------------------
//ֱ������� dll ���� 

//SOCKET PASCAL socket(int,int,int);
//char * (*fun1)(char * p1,char * p2);
#ifndef _MSC_VER
//�ܶ�ͬѧ����д����ָ������//����ָ���д����,��д�����ĺ�������,Ȼ�󽫺�������������,Ȼ���ں�����ǰ�ټ���*�ż���!!! 
SOCKET PASCAL (*_socket)(int,int,int);
//SOCKET (PASCAL *_socket)(int,int,int); //vc Ҫ����д,vc6,vc2010 ������� //���ǽ� PASCAL ���� stdcall �ŵ��������������� 
int PASCAL (*_WSAStartup)(WORD,LPWSADATA);
unsigned long PASCAL (*_inet_addr)(const char*);
u_short PASCAL (*_htons)(u_short);
int PASCAL (*_connect)(SOCKET,const struct sockaddr*,int);
int PASCAL (*_WSAGetLastError)(void);
int PASCAL (*_send)(SOCKET,const char*,int,int);
int PASCAL (*_recv)(SOCKET,char*,int,int);
int PASCAL (*_select)(int nfds,fd_set*,fd_set*,fd_set*,const struct timeval*);
struct hostent * PASCAL (*_gethostbyname)(const char*);
int PASCAL (*_ioctlsocket)(SOCKET,long,u_long *);
#endif

#ifdef _MSC_VER
SOCKET (PASCAL *_socket)(int,int,int); //vc Ҫ����д,vc6,vc2010 �������//���ǽ� PASCAL ���� stdcall �ŵ��������������� 
int  (PASCAL *_WSAStartup)(WORD,LPWSADATA);
unsigned long  (PASCAL*_inet_addr)(const char*);
u_short  (PASCAL*_htons)(u_short);
int  (PASCAL *_connect)(SOCKET,const struct sockaddr*,int);
int  (PASCAL *_WSAGetLastError)(void);
int  (PASCAL *_send)(SOCKET,const char*,int,int);
int  (PASCAL *_recv)(SOCKET,char*,int,int);
int  (PASCAL *_select)(int nfds,fd_set*,fd_set*,fd_set*,const struct timeval*);
struct hostent *  (PASCAL *_gethostbyname)(const char*);
#endif

//-------------------------------------------------- 

int CreateTcpClient()
{
	//LoadLibrary("wsock32.dll");
	return _socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

int InitWinSocket()
{
  WSADATA wData;

  //Result := WSAStartup(MakeWord(2, 2), wData) = 0;
  return _WSAStartup(MAKEWORD(2, 2), &wData);
  
}

int ConnectIP(SOCKET so, char * ip, int port)
{
  //sock: TSocket;
  //SockAddr: TSockAddr;
  //struct sockaddr SockAddr;
  struct sockaddr_in SockAddr;


  int err;
  int Result = 1;


  memset(&SockAddr, 0, sizeof(SockAddr));

  SockAddr.sin_family = AF_INET;
  SockAddr.sin_port = _htons(port);
  SockAddr.sin_addr.s_addr = _inet_addr(ip);

  //if (_connect(so, (struct sockaddr *)(&SockAddr), sizeof(SOCKADDR_IN)) == SOCKET_ERROR) 
  if (_connect(so, &SockAddr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR) //��ʵ�ǲ���ת���� 
  {
  	PrintError(0);
	err = _WSAGetLastError(); //��ʵ����� GetLastError ��һ����
    //ShowMessageFmt('connect socket error��[%d]', [WSAGetLastError]);
    MessageBox(0, "connect socket error", "", 0);
    Result = 0;
    
    //int err = _WSAGetLastError();//���ǰ������˱�� api ������ȡ�� 0 ��,�����Ǵ�����Ϣ�� 
    
    PrintError(err);
    
    if (INVALID_SOCKET == so) printf("connect error:INVALID_SOCKET\r\n");
    
    return 0;
  }
  
  return 1;

}//

//clq �������¼ӵĺ�����Ŀ���ǿ��Ը������������ʣ�����ԭ���Ĵ���ֻ�ܷ��ʾ�����

int ConnectHost(SOCKET so, char * host, int port)
{
	const char * address = host;
	int is_connect = 0;
	int err = 0; 
	
	// Create an address structure and clear it
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	
	// Fill in the address if possible//�ȳ��Ե���IP������
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = _inet_addr(address);
	
	// Was the string a valid IP address?//�������IP�͵�������������
	if (addr.sin_addr.s_addr == -1)
	{
		// No, so get the actual IP address of the host name specified
		struct hostent *pHost;
		pHost = _gethostbyname(address);
	
		if (pHost != NULL)
		{
			if (pHost->h_addr == NULL)
				return 0;//false;
				
			addr.sin_addr.s_addr = ((struct in_addr *)pHost->h_addr)->s_addr;
		}
		else
			return 0;//false;
	}	

	addr.sin_port = _htons(port);
	
	//���أ�-1 ����ʧ�ܣ�0 ���ӳɹ�
	if (_connect(so, (struct sockaddr *)&addr, sizeof(addr)) == 0)
	{
		is_connect = 1;//true;
	}
	else
	{
		is_connect = 0;//false;
		//����ʧ��
		//printfd3("WSAGetLastError:%d, WSAEWOULDBLOCK:%d\r\n", WSAGetLastError()-WSABASEERR, WSAEWOULDBLOCK-WSABASEERR);
		
	  	PrintError(0);
		err = _WSAGetLastError(); //��ʵ����� GetLastError ��һ����
	    //ShowMessageFmt('connect socket error��[%d]', [WSAGetLastError]);
	    MessageBox(0, "connect socket error", "", 0);
	    is_connect = 0;
	    
	    //int err = _WSAGetLastError();//���ǰ������˱�� api ������ȡ�� 0 ��,�����Ǵ�����Ϣ�� 
	    
	    PrintError(err);
	    
	    if (INVALID_SOCKET == so) printf("connect error:INVALID_SOCKET\r\n");		
	}
	
	return is_connect;
}//


#ifndef faveLoadFunction
#define faveLoadFunction
FARPROC WINAPI LoadFunction(HINSTANCE h, LPCSTR fun_name)
{
	FARPROC WINAPI r = 0;
	
	r = GetProcAddress(h, fun_name);
	
	if (r == 0) printf("load function %s error\r\n", fun_name);
	else printf("load function %s ok\r\n", fun_name);	

	return r;
}//
#endif


void LoadFunctions_Socket()
{
	//HINSTANCE hs = LoadLibrary("wsock32.dll"); //���ݲ�ͬ�ı��뻷��,�п���Ҫ�� LoadLibrary �ĳ� LoadLibraryA
	HINSTANCE hs = LoadLibraryA("wsock32.dll"); //���ݲ�ͬ�ı��뻷��,�п���Ҫ�� LoadLibrary �ĳ� LoadLibraryA
	
	if (hs == 0) printf("load wsock32.dll error\r\n", hs);
	else printf("load wsock32.dll ok\r\n", hs);
	
	_socket = GetProcAddress(hs, "socket");
	
	printf("_socket:%d\r\n", _socket);
	if (_socket == 0) printf("load _socket error\r\n", hs);
	
	//-------------------------------------------------- 
	//ֱ��װ�ظ��� dll ���� 
	_socket = LoadFunction(hs, "socket");
	_WSAStartup = LoadFunction(hs, "WSAStartup");
	_inet_addr = LoadFunction(hs, "inet_addr");
	_htons = LoadFunction(hs, "htons");
	_connect = LoadFunction(hs, "connect");
	_WSAGetLastError = LoadFunction(hs, "WSAGetLastError");
	_send = LoadFunction(hs, "send");
	_recv = LoadFunction(hs, "recv");
	_select = LoadFunction(hs, "select");
	_gethostbyname = LoadFunction(hs, "gethostbyname");
	_ioctlsocket = LoadFunction(hs, "ioctlsocket");
	
    

}

// ϵͳ������Ϣ��ʾ
void PrintError(DWORD last_err)
{
    //���г���
    //if (!CreateDirectory(_T("c:\\"),0))
    {
        char buf[512];//char buf[128];
		LPVOID lpMsgBuf;
		DWORD dw;
        
        memset(&buf, 0, sizeof(buf));
        
        dw = last_err;//GetLastError();
        
        if (dw == 0) dw = GetLastError(); //ʵ�����ǿ��Դ��� WSAGetLastError �� 
        
        //dw = 5;//10035;//6000054;
        
        if (dw == 0) return;
        
        FormatMessage (
            FORMAT_MESSAGE_FROM_SYSTEM,//FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, //FORMAT_MESSAGE_ALLOCATE_BUFFER ��ָҪ�����ڴ� 
            NULL,
            dw,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR) &buf,//(LPTSTR) &lpMsgBuf,
            500,//0, //��Ϊ���Լ�������ڴ�,����Ҫָ�������˶�� 
			NULL );
            
        printf("PrintError(������=%d):%s\r\n", dw, buf); //���,������ǲ���//�ǵ����� 2 �� ���������� 
            
        FormatMessage (
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, //FORMAT_MESSAGE_ALLOCATE_BUFFER ��ָҪ�����ڴ� 
            NULL,
            dw,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR) &lpMsgBuf,
            0, NULL );
            
 		wsprintf(buf,
                    "������Ϣ (�����룽%d): %s",
					dw, lpMsgBuf);             
            
        printf("PrintError(������=%d):%s\r\n", dw, lpMsgBuf);
        //printf("PrintError(������=%d):%s\r\n", dw, &buf); //���,����ǲ��Ե� 
        printf("PrintError(������=%d):%s\r\n", dw, buf);
            
        LocalFree(lpMsgBuf);
        
        //�����ʾ
        //OutputDebugString(szBuf);
    }
}//


//��δ������,��������
int SendBuf(SOCKET so, char * s, int len)
{
  int r = 0;
  int count = 0;
  char * p = s;

  //r := 0;
  int Result = 0;
  //count := 0;
  //p := @s[1];

  while (Result<len)
  {
    r = _send(so, p, len - Result, 0);
    if  (r > 0)
    {
      Result = Result + r;
      p = p + r;
      
    };


    count++;

    if (count>10) //�������ٴξͲ������� 
    {
      MessageBox(0, "send error", "", 0);

      return Result;
    }

  }
  
  return Result;

}//

//ע��,���ص��ַ���Ҫ�Լ��ͷ� 
//lstring RecvBuf(SOCKET so)
//����,���Ǵ����Զ��ͷŵ��ַ�����ȥ����� 
//void RecvBuf(SOCKET so, lstring * buf)//�������ʽҲ����,�������������Բ�ͨ�� 
lstring * RecvBuf(SOCKET so, struct MemPool * pool)
{
  char buf[1024+1];
  int r = 0;
  //lstring s = String("");
  //lstring * s = NewString("", _buf->pool);
  //CheckPString(_buf);
  lstring * s = NewString("", pool);

  memset(&buf, 0, sizeof(buf));


  r = _recv(so, buf, sizeof(buf)-1, 0); //����һ�� #0 ��β
  if (r > 0) 
  {
    //SetLength(s, r);
    //Move(buf, s[1], r);
    //s.Append(&s, StringConst(buf, r));
    LString_AppendCString(s, buf, r);
  }

  if (r == 0) //һ�㶼�ǶϿ���
  {
    MessageBox(0, "recv error.[socket close]", "", 0);

	//return String(""); //��Ӧ��������һ���µ�
	return s;
  }

  return s;

}//


//�Ƿ�ɶ�ȡ,ʱ��//�������� 
int SelectRead(SOCKET so)
{
  fd_set fd_read; //fd_read:TFDSet;
  struct timeval timeout; // : TTimeVal;

  int Result = 0;

  FD_ZERO( &fd_read );
  FD_SET(so, &fd_read ); //���������� FD_SETSIZE 

  timeout.tv_sec = 0; //��
  timeout.tv_usec = 500;  //����,������ ΢�� 
  timeout.tv_usec = 0;  //����,������ ΢��
  
  	//linux ��һ������һ��Ҫ��ֵ
	////int r = ::select(socket_handle+1, &fd_read, NULL, NULL, &l_timeout);

  //if select( 0, &fd_read, nil, nil, &timeout ) > 0 then //������1���ȴ�Accept��connection
  if (_select( so+1, &fd_read, NULL, NULL, &timeout ) > 0) //������1���ȴ�Accept��connection
    Result = 1;
    

  return Result;    

}//

//�Ƿ�ɶ�ȡ,ʱ��//��ʱ����,��λΪ�� 
int SelectRead_Timeout(SOCKET so, int sec)
{
  fd_set fd_read; //fd_read:TFDSet;
  struct timeval timeout; // : TTimeVal;

  int Result = 0;

  FD_ZERO( &fd_read );
  FD_SET(so, &fd_read ); //���������� FD_SETSIZE 

  //timeout.tv_sec = 0; //��
  timeout.tv_sec = sec; //��
  //timeout.tv_usec = 500;  //����,������ ΢�� 
  //timeout.tv_usec = 0;  //����,������ ΢��
  
  	//linux ��һ������һ��Ҫ��ֵ
	////int r = ::select(socket_handle+1, &fd_read, NULL, NULL, &l_timeout);

  //if select( 0, &fd_read, nil, nil, &timeout ) > 0 then //������1���ȴ�Accept��connection
  if (_select( so+1, &fd_read, NULL, NULL, &timeout ) > 0) //������1���ȴ�Accept��connection
    Result = 1;
    

  return Result;    

}//

//����Ϊ������ģʽ 
void SetNoBlock(SOCKET so)
{
	u_long arg = 0; //�����и�����,32λ���� 4 �ֽ�,64 λ�¿����� 8 �ֽ�,������ 64 λ�����´�����Լ������� 
	
	//���ȣ�����ͨѶΪ������ģʽ
	arg = 1;
	//ioctlsocket(so, FIONBIO, &arg);
	_ioctlsocket(so, FIONBIO, &arg);

}//



#endif

