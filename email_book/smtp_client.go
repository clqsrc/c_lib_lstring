package main //clq

//用于不加密环境的 smtp 发送电子邮件过程,因为不是所有的 smtp 环境都有加密支持的,不加密的适用范围更广一点,而且 smtp 服务器之间没有密码加密的过程
//(因为对方 smtp 服务器不可能知道你的密码)所以原有的 golang 1.7.3 net/smtp 过程就不适合 smtp 之间发送邮件

import (
	"fmt"
	"bufio"
//	"crypto/tls"
	"encoding/base64"
//	"errors"
//	"io"
	"net"
//	"net/smtp" //clq add
//	"net/textproto"
	"strings"
	"strconv"
)

var gConn net.Conn;
var gRead * bufio.Reader;
var gWrite * bufio.Writer;

//可以放到这样的类里
type TcpClient struct {
	Conn net.Conn;
	Read * bufio.Reader;
	Write * bufio.Writer;
}//


func Connect(host string, port int) (net.Conn, * bufio.Reader, * bufio.Writer) {
	
	addr := host + ":" + strconv.Itoa(port);
	conn, err := net.Dial("tcp", addr);
	if err != nil {
		return nil, nil, nil
	}
	
	reader := bufio.NewReader(conn);
	writer := bufio.NewWriter(conn);
	
	//writer.WriteString("EHLO\r\n");
	//writer.Flush();
	
	//host, _, _ := net.SplitHostPort(addr)
	//return NewClient(conn, host)	
	return conn, reader, writer;
}//

//收取一行,可再优化 
//func RecvLine(conn *net.Conn) (string) {
//func RecvLine(conn net.Conn, reader * bufio.Reader) (string) {
func _RecvLine() (string) {
	
    //defer conn.Close();
    ////reader := bufio.NewReader(conn);
	//reader := bufio.NewReaderSize(conn,409600)
	
	//line, err := reader.ReadString('\n'); //如何设定超时?
	line, err := gRead.ReadString('\n'); //如何设定超时?
	
	if err != nil { return ""; }
	
	line = strings.Split(line, "\r")[0]; //还要再去掉 "\r"，其实不去掉也可以
	
	return line;
}//

func SendLine(line string){
	gWrite.WriteString(line + "\r\n");
	gWrite.Flush();
}//

//解码一行命令,这里比较简单就是按空格进行分隔就行了
func DecodeCmd(line string, sp string) ([]string){

    //String[] tmp = line.split(sp); //用空格分开//“.”和“|”都是转义字符，必须得加"\\";//不一定是空格也有可能是其他的
    //String[] cmds = {"", "", "", "", ""}; //先定义多几个，以面后面使用时产生异常
	
	tmp := strings.Split(line, sp);
	//var cmds = [5]string{"", "", "", "", ""}; //先定义多几个，以面后面使用时产生异常
	var cmds = []string{"", "", "", "", ""}; //先定义多几个，以面后面使用时产生异常
    //i:=0;
    for i:=0;i<len(tmp);i++ {
        if i >= len(cmds) { break;}
        cmds[i] = tmp[i];
    }
    return []string(cmds);
}//

//读取多行结果
func RecvMCmd() (string) {
	i := 0;
    //index := 0;
    //count := 0;
    rs := "";
	//var c rune='\r';
    //var c4 rune = '\0'; //判断第4个字符//golang 似乎不支持这种表示
    
    mline := "";

    for i=0; i<50; i++ {
        rs = _RecvLine(); //只收取一行
        
        mline = mline + rs + "\r\n";
        
        //printf("\r\nRecvMCmd:%s\r\n", rs->str);
            
        if len(rs)<4 {break;} //长度要足够
        c4 := rs[4-1]; //第4个字符
        //if ('\x20' == c4) break; //"\xhh" 任意字符 二位十六进制//其实现在的转义符已经扩展得相当复杂，不建议用这个表示空格
        if ' ' == c4 { break;} //第4个字符是空格就表示读取完了//也可以判断 "250[空格]"
    
        
    }//

    return mline;
	
}//

//简单的测试一下 smtp
func test_smtp() {
	
	//连接
	//gConn, gRead, gWrite = Connect("newbt.net", 25);
	//gConn, gRead, gWrite = Connect("newbt.net", 25);
	gConn, gRead, gWrite = Connect("smtp.163.com", 25);
	
	//收取一行
	line := _RecvLine();
	fmt.Println("recv:" + line);
	
	//解码一下,这样后面的 EHLO 才能有正确的第二个参数
    cmds := DecodeCmd(line, " ");
	domain := cmds[1]; //要从对方的应答中取出域名//空格分开的各个命令参数中的第二个
	
	//发送一个命令
    //SendLine("EHLO"); //163 这样是不行的,一定要有 domain
    SendLine("EHLO" + " " + domain); //domain 要求其实来自 HELO 命令//HELO <SP> <domain> <CRLF>	
	
	//收取多行
	//line = _RecvLine();
	line = RecvMCmd();
	fmt.Println("recv:" + line);
	
	//--------------------------------------------------
	//用 base64 登录 
	SendLine("AUTH LOGIN");	
	
	//收取一行
	line = _RecvLine();
	fmt.Println("recv:" + line);
	
	//s :="test1@newbt.net"; //要换成你的用户名,注意 163 邮箱的话不要带后面的 @域名 部分 
	s :="clq_test"; //要换成你的用户名,注意 163 邮箱的话不要带后面的 @域名 部分 
	s = base64.StdEncoding.EncodeToString([]byte(s));
	//s = base64_encode(s);
	SendLine(s);	
	
	//收取一行
	line = _RecvLine();
	fmt.Println("recv:" + line);
	

	s = "123456"; //要换成您的密码 
	//s = base64_encode(s);
	s = base64.StdEncoding.EncodeToString([]byte(s));
	SendLine(s);	
	
	//收取一行
	line = _RecvLine();
	fmt.Println("recv:" + line);
	
	//--------------------------------------------------	
	//邮件内容 
	//from := "test1@newbt.net";
	from := "clq_test@163.com";
	to := "clq@newbt.net";
	
	SendLine("MAIL FROM: <" + from +">"); //注意"<" 符号和前面的空格。空格在协议中有和没有都可能，最好还是有 
	//收取一行
	line = _RecvLine();
	fmt.Println("recv:" + line);		
	
	SendLine("RCPT TO: <" + to+ ">");
	//收取一行
	line = _RecvLine();
	fmt.Println("recv:" + line);		
	
	SendLine("DATA");
	//收取一行
	line = _RecvLine();
	fmt.Println("recv:" + line)		
	
	// = "From: \"test1@newbt.net\" <test1@newbt.net>\r\nTo: \"clq@newbt.net\" <clq@newbt.net>\r\nSubject: test golang\r\nDate: Sun, 21 Jan 2018 11:48:15 GMT\r\n\r\nHello World.\r\n";//邮件内容，正式的应该用一个函数生成 
	s = MakeMail(from,to,"test golang","Hello World.");
	SendLine(s);	
	
	
	s = "\r\n.\r\n"; //邮件结束符 
	SendLine(s);
	
	//收取一行
	line = _RecvLine();
	fmt.Println("recv:" + line)		
	
}//

//这只是个简单的内容，真实的邮件内容复杂得多
func MakeMail(from,to,subject,text string)(string) {
	//s := "From: \"test1@newbt.net\" <test1@newbt.net>\r\nTo: \"clq@newbt.net\" <clq@newbt.net>\r\nSubject: test golang\r\nDate: Sun, 21 Jan 2018 11:48:15 GMT\r\n\r\nHello World.\r\n";//邮件内容，正式的应该用一个函数生成 
	s := "From: \"" + from + "\"\r\nTo: \"" + to + "\" " + to + "\r\nSubject: " + subject + 
		"\r\nDate: Sun, 21 Jan 2018 11:48:15 GMT\r\n\r\n" + //内容前是两个回车换行
		text + "\r\n";
	
	return s;	

}//

//func Dial(addr string) (*Client, error) {
//	conn, err := net.Dial("tcp", addr)
//	if err != nil {
//		return nil, err
//	}
//	host, _, _ := net.SplitHostPort(addr)
//	return NewClient(conn, host)
//}

