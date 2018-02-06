

//�㷨���� http://www.cnblogs.com/IwAdream/p/6088283.html, �������Լ���ע�� 
//�����Ͼ��ǿ���λ�㷨�� 3 ���ֽڱ�� 4 ���ֽ�,���߽� 4 ���ֽڱ�� �����ֽ�
//���Կ� https://zh.wikipedia.org/wiki/Base64 �еı��ͼʾ 
//ͼƬ�����ڱ�Դ���и����� base64.png �п���[���ת�ص�����Ҳת����ͼƬ�Ļ��ͻ���] 
//��ʵһ��ͼ������������ԭ�� 
//�ѱ�����ǰ�Ա���������֧�� 

#ifndef _BASE64_H_
#define _BASE64_H_

#include <stdio.h>
#include <stdint.h> //clq �����ʵҲ���Ǳ���� 
#include <string.h>
#include <malloc.h> //clq ���Բ���,��ʱ���ͻ 
 
char base64_table[] = {
     'A','B','C','D','E','F','G','H','I','J',
     'K','L','M','N','O','P','Q','R','S','T',
     'U','V','W','X','Y','Z','a','b','c','d',
     'e','f','g','h','i','j','k','l','m','n',
     'o','p','q','r','s','t','u','v','w','x',
     'y','z','0','1','2','3','4','5','6','7',
     '8','9','+', '/', '\0'
};
 
void base64_map(uint8_t *in_block, int len) {
	int i = 0;
    for(i = 0; i < len; ++i) {
        in_block[i] = base64_table[in_block[i]];
        //printf("%d %c",in_block[i], base64_table[in_block[i]]);
    }
    if(len % 4 == 3)
        in_block[len] = '=';
    else if(len % 4 == 2)
        in_block[len] = in_block[len+1] = '=';
    return ;
}


void  base64_unmap(char *in_block) {
    int i;
	char *c;
	int decode_count = 0;
 
    for(i = 0; i < 4; ++i) {
        c = in_block + i;
 
        if(*c>='A' && *c<='Z') {
            *c -= 'A';
            continue;
        }
 
        if(*c>='a' && *c<='z') {
            *c -= 'a';
            *c += 26;
            continue;
        }
 
        if(*c == '+') {
            *c = 62;
            continue;
        }
 
        if(*c == '/') {
            *c = 63;
            continue;
        }
 
        if(*c == '=') {
            *c = 0;
            continue;
        }
 
        *c -= '0';
        *c += 52;
    }
}
 
int _base64_encode(char *in, int inlen, uint8_t *out) {
    char *in_block;
    uint8_t *out_block;
	char temp[3];
	int outlen = 0; //clq add ��һ�����������ݳ���
    int i = 0;
 
    out_block = out;
    in_block = in;
 
    for(i = 0; i < inlen; i += 3) {
        memset(temp, 0, 3);
        memcpy(temp, in_block, i + 3 < inlen ? 3 : inlen - i);
        memset(out_block, 0, 4);
 
        out_block[0] = (temp[0] >> 2) & 0x3f;
        out_block[1] = ((temp[0] << 4) & 0x30) | ((temp[1] >> 4) & 0x0f);
        out_block[2] = ((temp[1] << 2) & 0x3c) | ((temp[2] >> 6) & 0x03);
        out_block[3] = (temp[2]) & 0x3f;
        //printf("%.2x %.2x %.2x\n", temp[0], temp[1], temp[2]);
        //printf("%.2x %.2x %.2x %.2x\n", out_block[0], out_block[1], out_block[2], out_block[3]);
        out_block += 4;
        in_block += 3;
        
        outlen += 4; //clq add ��һ�����������ݳ���
    }
 
    base64_map(out, ((inlen * 4) - 1) / 3 + 1);
    
    return outlen;
}
 
int _base64_decode(char *in, int inlen, uint8_t *out) {
    char *in_block;
    uint8_t *out_block;
    char temp[4];
	int outlen = 0; //clq add ��һ�����������ݳ���
	int i = 0;
 
    out_block = out;
    in_block = in;

 
    for(i = 0; i < inlen; i += 4) {
        if(*in_block == '=')
            return 0;
        memcpy(temp, in_block, 4);
        memset(out_block, 0, 3);
        base64_unmap(temp);
 
        out_block[0] = ((temp[0]<<2) & 0xfc) | ((temp[1]>>4) & 3);
        out_block[1] = ((temp[1]<<4) & 0xf0) | ((temp[2]>>2) & 0xf);
        out_block[2] = ((temp[2]<<6) & 0xc0) | ((temp[3]   ) & 0x3f);
 
        out_block += 3;
        in_block +=4;
        
		outlen += 3; //clq add ��һ�����������ݳ���//���������ʵ����,��Ϊ��һ���� 3 �ı���
		//if (temp[3] == '=') outlen -= 1; //clq add ��һ���Ⱥžͱ�ʾ������һ���ֽ�//�Ż��ŵ����ȥ���� 
		//if (temp[3] == '=') outlen -= 1; //clq add ��һ���Ⱥžͱ�ʾ������һ���ֽ�//�Ż��ŵ����ȥ���� 
		
    }
    
	if (in[inlen-1] == '=') outlen -= 1; //clq add ��һ���Ⱥžͱ�ʾ������һ���ֽ�//�Ż��ŵ����ȥ���� 
	if (in[inlen-2] == '=') outlen -= 1; //clq add ��һ���Ⱥžͱ�ʾ������һ���ֽ�//�Ż��ŵ����ȥ���� 

    //return 0;
    return outlen; //clq add ��һ�����������ݳ��� 
}
 
 
/*
int main2() {
 
    char cipher_text[64];
    int i = 0;
    
    while(scanf("%s", cipher_text) != EOF) {
        printf("%s\n", cipher_text);
 
        uint8_t *tran_cipher = (uint8_t *)malloc(sizeof(uint8_t) * 64);
        memset(tran_cipher, 0, sizeof(uint8_t) * 64);
 
        #define ENCODE
        #define DECODE
        #ifdef ENCODE
        printf("----------------ENCODE-----------------");
        base64_encode(cipher_text, strlen(cipher_text), tran_cipher);
        int len = (strlen(cipher_text) * 4 - 1) / 3 + 1;
        len = len % 4 == 3 ? len + 1 : len + 2;
        for(i = 0; i < len ; ++i)
            printf("%c", tran_cipher[i]);
        printf("\n");
 
        #endif // ENCODE
 
        #ifdef DECODE
        printf("----------------DECODE-----------------");
        base64_decode(cipher_text, strlen(cipher_text), tran_cipher);
        len = strlen(cipher_text);
        int n = len;
        while(cipher_text[--n] == '=')
            ;
        if(n == len-2)
            len = (len >> 2) * 3 - 1;
        else if(n == len-3)
            len = (len >> 2) * 3 - 2;
        else if(n == len-1)
            len = (len >> 2) * 3;
 
        for(i = 0; i < len; ++i)
            printf("%c", tran_cipher[i]);
        printf("\n");
        #endif // DECODE
    }
    return 0;
}

*/


#endif





