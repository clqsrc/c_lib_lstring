
//一个方便测试 socket 程序的小文件,省得老是找 lib a 文件 
//目前是 gcc 专用,如果 vc 要用另外弄一个好了,不要在这上面弄条件编译 


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


// 系统错误信息提示
void PrintError(DWORD last_err);

//--------------------------------------------------
//直接引入的 dll 函数 

//SOCKET PASCAL socket(int,int,int);
//char * (*fun1)(char * p1,char * p2);
#ifndef _MSC_VER
//很多同学不会写函数指针声明//函数指针的写法是,先写正常的函数声明,然后将函数名加上括号,然后在函数名前再加上*号即可!!! 
SOCKET PASCAL (*_socket)(int,int,int);
//SOCKET (PASCAL *_socket)(int,int,int); //vc 要这样写,vc6,vc2010 都是如此 //就是将 PASCAL 或者 stdcall 放到函数名的括号中 
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
int PASCAL (*_setsockopt)(SOCKET,int,int,const char*,int);
#endif

#ifdef _MSC_VER
SOCKET (PASCAL *_socket)(int,int,int); //vc 要这样写,vc6,vc2010 都是如此//就是将 PASCAL 或者 stdcall 放到函数名的括号中 
int  (PASCAL *_WSAStartup)(WORD,LPWSADATA);
unsigned long  (PASCAL*_inet_addr)(const char*);
u_short  (PASCAL*_htons)(u_short);
int  (PASCAL *_connect)(SOCKET,const struct sockaddr*,int);
int  (PASCAL *_WSAGetLastError)(void);
int  (PASCAL *_send)(SOCKET,const char*,int,int);
int  (PASCAL *_recv)(SOCKET,char*,int,int);
int  (PASCAL *_select)(int nfds,fd_set*,fd_set*,fd_set*,const struct timeval*);
struct hostent *  (PASCAL *_gethostbyname)(const char*);
int (PASCAL * _ioctlsocket)(SOCKET, long, u_long*);
int (PASCAL * _setsockopt)(SOCKET, int, int, const char*, int);
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
  if (_connect(so, &SockAddr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR) //其实是不用转换的 
  {
  	PrintError(0);
	err = _WSAGetLastError(); //其实这个和 GetLastError 是一样的
    //ShowMessageFmt('connect socket error，[%d]', [WSAGetLastError]);
    MessageBox(0, "connect socket error", "", 0);
    Result = 0;
    
    //int err = _WSAGetLastError();//如果前面调用了别的 api 这里是取到 0 的,而不是错误信息码 
    
    PrintError(err);
    
    if (INVALID_SOCKET == so) printf("connect error:INVALID_SOCKET\r\n");
    
    return 0;
  }
  
  return 1;

}//

//clq 这是我新加的函数，目的是可以根据域名来访问，并且原来的代码只能访问局域网

int ConnectHost(SOCKET so, char * host, int port)
{
	const char * address = host;
	int is_connect = 0;
	int err = 0; 
	
	// Create an address structure and clear it
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	
	// Fill in the address if possible//先尝试当做IP来解析
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = _inet_addr(address);
	
	// Was the string a valid IP address?//如果不是IP就当做域名来解析
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
	
	//返回：-1 连接失败；0 连接成功
	if (_connect(so, (struct sockaddr *)&addr, sizeof(addr)) == 0)
	{
		is_connect = 1;//true;
	}
	else
	{
		is_connect = 0;//false;
		//连接失败
		//printfd3("WSAGetLastError:%d, WSAEWOULDBLOCK:%d\r\n", WSAGetLastError()-WSABASEERR, WSAEWOULDBLOCK-WSABASEERR);
		
	  	PrintError(0);
		err = _WSAGetLastError(); //其实这个和 GetLastError 是一样的
	    //ShowMessageFmt('connect socket error，[%d]', [WSAGetLastError]);
	    MessageBox(0, "connect socket error", "", 0);
	    is_connect = 0;
	    
	    //int err = _WSAGetLastError();//如果前面调用了别的 api 这里是取到 0 的,而不是错误信息码 
	    
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
	//HINSTANCE hs = LoadLibrary("wsock32.dll"); //根据不同的编译环境,有可能要从 LoadLibrary 改成 LoadLibraryA
	HINSTANCE hs = LoadLibraryA("wsock32.dll"); //根据不同的编译环境,有可能要从 LoadLibrary 改成 LoadLibraryA
	
	if (hs == 0) printf("load wsock32.dll error\r\n");
	else printf("load wsock32.dll ok\r\n");
	
	_socket = GetProcAddress(hs, "socket");
	
	printf("_socket:%d\r\n", _socket);
	if (_socket == 0) printf("load _socket error\r\n");
	
	//-------------------------------------------------- 
	//直接装载各个 dll 函数 
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
	_setsockopt = LoadFunction(hs, "setsockopt");
	
    

}

// 系统错误信息提示
void PrintError(DWORD last_err)
{
    //进行出错。
    //if (!CreateDirectory(_T("c:\\"),0))
    {
        char buf[512];//char buf[128];
		LPVOID lpMsgBuf;
		DWORD dw;
        
        memset(&buf, 0, sizeof(buf));
        
        dw = last_err;//GetLastError();
        
        if (dw == 0) dw = GetLastError(); //实际上是可以代替 WSAGetLastError 的 
        
        //dw = 5;//10035;//6000054;
        
        if (dw == 0) return;
        
        FormatMessage (
            FORMAT_MESSAGE_FROM_SYSTEM,//FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, //FORMAT_MESSAGE_ALLOCATE_BUFFER 是指要分配内存 
            NULL,
            dw,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR) &buf,//(LPTSTR) &lpMsgBuf,
            500,//0, //因为是自己分配的内存,所以要指出分配了多大 
			NULL );
            
        printf("PrintError(出错码=%d):%s\r\n", dw, buf); //奇怪,这里就是不对//是倒数第 2 个 参数的问题 
            
        FormatMessage (
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, //FORMAT_MESSAGE_ALLOCATE_BUFFER 是指要分配内存 
            NULL,
            dw,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR) &lpMsgBuf,
            0, NULL );
            
 		wsprintf(buf,
                    "出错信息 (出错码＝%d): %s",
					dw, lpMsgBuf);             
            
        printf("PrintError(出错码=%d):%s\r\n", dw, lpMsgBuf);
        //printf("PrintError(出错码=%d):%s\r\n", dw, &buf); //奇怪,这个是不对的 
        printf("PrintError(出错码=%d):%s\r\n", dw, buf);
            
        LocalFree(lpMsgBuf);
        
        //输出提示
        //OutputDebugString(szBuf);
    }
}//


//尚未精测试,可能有误
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

    if (count>10) //超过多少次就不发送了 
    {
      MessageBox(0, "send error", "", 0);

      return Result;
    }

  }
  
  return Result;

}//

//注意,返回的字符串要自己释放 
//lstring RecvBuf(SOCKET so)
//算了,还是传可自动释放的字符串进去方便点 
//void RecvBuf(SOCKET so, lstring * buf)//用这个格式也可以,不过与其他语言不通用 
lstring * RecvBuf(SOCKET so, struct MemPool * pool)
{
  char buf[1024+1];
  int r = 0;
  //lstring s = String("");
  //lstring * s = NewString("", _buf->pool);
  //CheckPString(_buf);
  lstring * s = NewString("", pool);

  memset(&buf, 0, sizeof(buf));


  r = _recv(so, buf, sizeof(buf)-1, 0); //留下一个 #0 结尾
  if (r > 0) 
  {
    //SetLength(s, r);
    //Move(buf, s[1], r);
    //s.Append(&s, StringConst(buf, r));
    LString_AppendCString(s, buf, r);
  }

  if (r == 0) //一般都是断开了
  {
    MessageBox(0, "recv error.[socket close]", "", 0);

	//return String(""); //不应该再生成一个新的
	return s;
  }

  return s;

}//


//是否可读取,时间//立即返回 
int SelectRead(SOCKET so)
{
  fd_set fd_read; //fd_read:TFDSet;
  struct timeval timeout; // : TTimeVal;

  int Result = 0;

  FD_ZERO( &fd_read );
  FD_SET(so, &fd_read ); //个数受限于 FD_SETSIZE 

  timeout.tv_sec = 0; //秒
  timeout.tv_usec = 500;  //毫秒,不对是 微秒 
  timeout.tv_usec = 0;  //毫秒,不对是 微秒
  
  	//linux 第一个参数一定要赋值
	////int r = ::select(socket_handle+1, &fd_read, NULL, NULL, &l_timeout);

  //if select( 0, &fd_read, nil, nil, &timeout ) > 0 then //至少有1个等待Accept的connection
  if (_select( so+1, &fd_read, NULL, NULL, &timeout ) > 0) //至少有1个等待Accept的connection
    Result = 1;
    

  return Result;    

}//

//是否可读取,时间//超时返回,单位为秒 
int SelectRead_Timeout(SOCKET so, int sec)
{
  fd_set fd_read; //fd_read:TFDSet;
  struct timeval timeout; // : TTimeVal;

  int Result = 0;

  FD_ZERO( &fd_read );
  FD_SET(so, &fd_read ); //个数受限于 FD_SETSIZE 

  //timeout.tv_sec = 0; //秒
  timeout.tv_sec = sec; //秒
  //timeout.tv_usec = 500;  //毫秒,不对是 微秒 
  timeout.tv_usec = 0;  //毫秒,不对是 微秒
  
  	//linux 第一个参数一定要赋值
	////int r = ::select(socket_handle+1, &fd_read, NULL, NULL, &l_timeout);

  //if select( 0, &fd_read, nil, nil, &timeout ) > 0 then //至少有1个等待Accept的connection
  if (_select( so+1, &fd_read, NULL, NULL, &timeout ) > 0) //至少有1个等待Accept的connection
    Result = 1;
    

  return Result;    

}//

//设置为非阻塞模式 
void SetNoBlock(SOCKET so)
{
	u_long arg = 0; //这里有个问题,32位下是 4 字节,64 位下可能是 8 字节,所以在 64 位环境下大家再自己测试下 
	
	//首先，设置通讯为非阻塞模式
	arg = 1;
	//ioctlsocket(so, FIONBIO, &arg);
	_ioctlsocket(so, FIONBIO, &arg);

}//

//设置发送超时
void SetTimeout_send(SOCKET so, int sec)
{
	struct timeval timeout; // : TTimeVal;
	
	timeout.tv_sec = sec; //秒
	timeout.tv_usec = 0;  //毫秒,不对是 微秒
	
	_setsockopt(so, SOL_SOCKET,SO_SNDTIMEO, (char *)&timeout, sizeof(struct timeval));
}//

//设置接收超时
void SetTimeout_recv(SOCKET so, int sec)
{
	struct timeval timeout; // : TTimeVal;
	
	timeout.tv_sec = sec; //秒
	timeout.tv_usec = 0;  //毫秒,不对是 微秒
		
	_setsockopt(so, SOL_SOCKET,SO_RCVTIMEO, (char *)&timeout, sizeof(struct timeval));
}//



#endif

