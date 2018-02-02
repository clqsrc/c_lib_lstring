package st;

import java.io.*;
import java.net.*;

public class SocketTest1 {
    
    public static BufferedReader br = null;
    public static PrintWriter pw = null;
    public static Socket socket = null;
    public static OutputStream os = null;
    public static InputStream is = null;

    public static void main(String[] args) {

        try{

            System.out.println("start");

            //简单的测试一下 smtp
            test_smtp();

            br.close();
            is.close();
            pw.close();
            os.close();
            socket.close();

        }catch(Exception e) {
            System.out.println("Error."+e);

        }
        

    }//
    

    
    //简单的测试一下 smtp
    public static void test_smtp_v1()
    {
        //连接
        Connect("newbt.net", 25);
        
        //收取一行
        String line = RecvLine();
        System.out.println("recv:" + line);
        
        //发送一个命令
        SendLine("EHLO");
        
        //收取一行
        line = RecvLine();
        System.out.println("recv:" + line);
    }//
    
    //简单的测试一下 smtp
    public static void test_smtp()
    {
        //连接
        //Connect("newbt.net", 25);
        Connect("smtp.163.com", 25);
        
        //收取一行
        String line = RecvLine();
        System.out.println("recv:" + line);
        //解码一下,这样后面的 EHLO 才能有正确的第二个参数
        String cmds[] = DecodeCmd(line, " "); 
        
        String domain = cmds[1]; //要从对方的应答中取出域名//空格分开的各个命令参数中的第二个
        
        //发送一个命令
        //SendLine("EHLO"); //163 这样是不行的,一定要有 domain
        SendLine("EHLO" + " " + domain); //domain 要求其实来自 HELO 命令//HELO <SP> <domain> <CRLF>
        
        //收取一行
        line = RecvLine();
        System.out.println("recv:" + line);
    }//    
    
    //解码一行命令,这里比较简单就是按空格进行分隔就行了
    public static String[] DecodeCmd(String line, String sp)
    {
        //String[] aa = "aaa|bbb|ccc".split("|"); 
        String[] tmp = line.split(sp); //用空格分开//“.”和“|”都是转义字符，必须得加"\\";//不一定是空格也有可能是其他的
        String[] cmds = {"", "", "", "", ""}; //先定义多几个，以面后面使用时产生异常
        
        for(int i=0;i<tmp.length;i++)
        {   
            if (i >= cmds.length) break;
            cmds[i] = tmp[i]; 
        }
        return cmds;
    }//
    
    public static void Connect(String host, int port)
    {
        try{
            //socket = new Socket("newbt.net", 25);
            socket = new Socket(host, port);
            
            //--------------------------------------------------
            os = socket.getOutputStream();//字节输出流
            pw = new PrintWriter(os);//将输出流包装成打印流
            
            is = socket.getInputStream();
            br = new BufferedReader(new InputStreamReader(is));
            
        }catch(Exception e) {
            System.out.println("Error."+e);

        }    
    }

    //发送一个命令行
    public static void SendLine(String s)
    {
        //pw.write("EHLO\r\n");
        pw.write(s + "\r\n");
        pw.flush();
    }
    
    //收取一行服务器发来的信息
    public static String RecvLine()
    {
        try{
            String s = br.readLine();
            return s;
        }catch(Exception e) {
            System.out.println("Error."+e);

        }
        
        return null;
    }
    
}//
