#include "common.h"
#include "kmp.h"
#include "usart.h"
#include "stdlib.h"
#include "string.h"

#define KEYWORD_MAX_LENGTH 100      //设定搜索串的最大长度
int kmp_table[KEYWORD_MAX_LENGTH];  //为搜索串建立kmp表
char prefix_stack[KEYWORD_MAX_LENGTH]; //前缀表达式栈
char suffix_stack[KEYWORD_MAX_LENGTH]; //后缀表达式栈
char keyword_length = 0;  //搜索串的长度

const u8 auchCRCHi[] = { 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,0x80, 0x41, 0x00, 0xC1, 0x81, 0x40} ; 


const u8 auchCRCLo[] = { 
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 
0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 
0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3, 
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 
0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 
0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26, 
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 
0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 
0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5, 
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 
0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 
0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C, 
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,0x43, 0x83, 0x41, 0x81, 0x80, 0x40} ;

//int record_position[KEYWORD_MAX_LENGTH]; //记录与关键字串匹配源串中的位置

/*
*GetMatchValue:获得字符串src的部分匹配值
*/
int GetMatchValue(char *src)
{
	  int i = 0;
    int value = 0;
    int src_len;
	  int flag = 0;   //用一个标志位来确定后缀栈中到最后一个元素都与前缀栈中的符号匹配
    char *begin;    //初始化指向字符串第一个字符
    char *end;  //初始化指向字符串最后一个字符 
	  char *p;	
    char *q;
    
	  src_len=strlen(src);
	  begin=src;
	  end=src + (src_len - 1);
    for(i=0;i<(src_len-1);i++)
    {
        prefix_stack[i] = *begin;
        suffix_stack[i] = *end;
        begin++;
        end--;
    }

		p=prefix_stack;
		q=suffix_stack + (src_len - 2);  //指向栈中最后一个元素
    while(q >= suffix_stack)
    {
        if(*p == *q)
        {
            value++;
            p++;
            flag=1;
        }
        else {
            flag = 0;
        }
        q--;
    }
    if(flag == 0) value = 0;
    return value;
}
/*
*创建搜索字符串的KMP表
*/
int Create_KMP_Table(char *str,int *table)
{
    int i;
    char *dst;
    keyword_length = strlen(str);
    for(i=0;i<keyword_length;i++)
    {
        if(i == 0) {
            table[i] = 0;   //第一个字符无前缀和后缀，所以为0
        }
        else {
            dst = (char*)malloc((i+2));
            if(dst == NULL)
            {
               return EXIT_FAILURE;
            }
            strncpy(dst,str,(i+1));   //匹配str的前(i+1)个字符
            dst[i+1] = '\0';    //注意字符串要以'/0'结尾
            //table[i] = GetMatchValue(dst); 
            free((void*)dst);    
        }
    }
    return EXIT_SUCCESS;
}


//在目标串dst_str中搜索关键子串search_str,打印出关键字串的位置信息,返回与关键字串匹配的数目
int Search_Keyword(char *dst_str,char *search_str)
{    
	  char count = 0;  //记录现在已经匹配的数目
    //int k = 0;     //记录与关键字串匹配的字串的数目
    int move = 0;  //当字符串不匹配时，搜索指针移动的位数  
    char *p;
    char *q;
    char *temp;
	
	  p=dst_str;
	  q=search_str;
    //创建关键字串的KMP表    
    Create_KMP_Table(search_str,kmp_table);
  
  	while(*p != '\0')   //直到搜索到目标串的最后一个字符为止
    {
        temp = p;
        while(*q != '\0')
        {
            if(*q == *temp)
            {
                count++;
                temp++;
                q++;
            }
            else break;
        }
        
        if(count == 0)
            p++;
        else {
            if(count == keyword_length)
            {
                return 1;
            }
            move = count - kmp_table[count-1];
            p += move;
        }
        count = 0;
        q = search_str;
    }
    return 0;
}

u16 crc16(u8 *puchMsg, u16 usDataLen) 
{ 
	u8 uchCRCHi = 0xFF ; 
	u8 uchCRCLo = 0xFF ; 
	u32 uIndex ; 
	while (usDataLen--) 
	{ 
		uIndex = uchCRCHi ^ *puchMsg++ ; 
		uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex] ; 
		uchCRCLo = auchCRCLo[uIndex] ; 
	} 
	return (uchCRCHi << 8 | uchCRCLo) ; 
}


//KMP匹配算法实现WIFI的数据解析
//作者：林永欣
//时间：2018.4.18
void DataTransfer(char *dst_buf,u16 rlen)
{
	u16 crcData;
	u8 crc1,crc2;
	char p[100];    //{"action":"1"}0230-0274
	u8 ff=0;
	int i,j,k,t,result,flag=0;
	char stop_send_buf[4]={0x55,0x55,0x02,0x07};
	char send_buf[7]={0x55,0x55,0x05,0x06,0x00,0x01,0x00};
	char ack_buf[6]={0xDB,0x00,0x00,0x00,0xFF,0xFF};
  char search_buf[12+45][25]={"{\"action\":\"forward\"}",
		                      "{\"action\":\"backward\"}",
													"{\"action\":\"left\"}",
	                        "{\"action\":\"right\"}",
													"{\"action\":\"rotate\"}",
													"{\"action\":\"stop\"}",
	                        "{\"action\":\"introduce\"}",
													"{\"action\":\"play\"}",
													"{\"action\":\"dance\"}",
													"{\"action\":\"actiongroup1\"}",
													"{\"action\":\"actiongroup2\"}",
													"{\"action\":\"actiongroup3\"}",
	                        "{\"action\":\"a1\"}",
	                        "{\"action\":\"a2\"}",
		                      "{\"action\":\"a3\"}",
													"{\"action\":\"a4\"}",
													"{\"action\":\"a5\"}",
	                        "{\"action\":\"a6\"}",
	                        "{\"action\":\"a7\"}",
		                      "{\"action\":\"a8\"}",
													"{\"action\":\"a9\"}",
													"{\"action\":\"a10\"}",
	                        "{\"action\":\"a11\"}",
	                        "{\"action\":\"a12\"}",
		                      "{\"action\":\"a13\"}",
													"{\"action\":\"a14\"}",
													"{\"action\":\"a15\"}",
	                        "{\"action\":\"a16\"}",
	                        "{\"action\":\"a17\"}",
		                      "{\"action\":\"a18\"}",
													"{\"action\":\"a19\"}",
													"{\"action\":\"a20\"}",
	                        "{\"action\":\"a21\"}",
	                        "{\"action\":\"a22\"}",
		                      "{\"action\":\"a23\"}",
													"{\"action\":\"a24\"}",
													"{\"action\":\"a25\"}",
	                        "{\"action\":\"a26\"}",
	                        "{\"action\":\"a27\"}",
		                      "{\"action\":\"a28\"}",
													"{\"action\":\"a29\"}",
													"{\"action\":\"a30\"}",		
	                        "{\"action\":\"a31\"}",
	                        "{\"action\":\"a32\"}",
		                      "{\"action\":\"a33\"}",
													"{\"action\":\"a34\"}",
													"{\"action\":\"a35\"}",
	                        "{\"action\":\"a36\"}",
	                        "{\"action\":\"a37\"}",
		                      "{\"action\":\"a38\"}",
													"{\"action\":\"a39\"}",
													"{\"action\":\"a40\"}",
	                        "{\"action\":\"a41\"}",
	                        "{\"action\":\"a42\"}",
		                      "{\"action\":\"a43\"}",
													"{\"action\":\"a44\"}",
													"{\"action\":\"a45\"}"};
  memcpy(p,dst_buf,rlen-2);

	crcData = crc16(p,rlen-2);
	crc1=crcData >> 8;
	crc2=crcData & 0xff;	
	
	if ((crc1==dst_buf[rlen-2]) && (crc2==dst_buf[rlen-1]))
	{
    if ((dst_buf[0]==0xDA) && (dst_buf[1]==0x00))
  	{
			if (dst_buf[2]==0x01)
			{	
				for (i=0;i<57;i++)
				{
					j=0; k=8;
					while ((dst_buf[k]==search_buf[i][j]) && (k<rlen-2)) 
					{
						k++;
						j++;
					}
					if (k==rlen-2) 
					{

						result=i;
						flag=1;
						break;
					}			
				}	
				switch (result)
				{
					case 0:send_buf[4]=0x01;send_buf[5]=0x03;break;
					case 1:send_buf[4]=0x02;send_buf[5]=0x03;break;
					case 2:send_buf[4]=0x07;send_buf[5]=0x03;break;
					case 3:send_buf[4]=0x08;send_buf[5]=0x03;break;
					case 4:send_buf[4]=0x03;send_buf[5]=0x03;break;
					case 5:ff=1;break;
					case 6:send_buf[4]=0x31;break;
					case 7:send_buf[4]=0x61;break;
					case 8:send_buf[4]=0x09;break;
					case 9:send_buf[4]=0x6A;send_buf[5]=0x01;break;
					case 10:send_buf[4]=0x70;send_buf[5]=0x01;break;
					case 11:send_buf[4]=0x71;send_buf[5]=0x01;break;
					
					case 12:send_buf[4]=0xE6;send_buf[5]=0x01;break;
					case 13:send_buf[4]=0xE7;send_buf[5]=0x01;break;
					case 14:send_buf[4]=0xE8;send_buf[5]=0x01;break;
					case 15:send_buf[4]=0xE9;send_buf[5]=0x01;break;
					case 16:send_buf[4]=0xEA;send_buf[5]=0x01;break;
					case 17:send_buf[4]=0xEB;send_buf[5]=0x01;break;
					case 18:send_buf[4]=0xEC;send_buf[5]=0x01;break;
					case 19:send_buf[4]=0xED;send_buf[5]=0x01;break;
					case 20:send_buf[4]=0xEE;send_buf[5]=0x01;break;
					case 21:send_buf[4]=0xEF;send_buf[5]=0x01;break;
					case 22:send_buf[4]=0xF0;send_buf[5]=0x01;break;
					case 23:send_buf[4]=0xF1;send_buf[5]=0x01;break;
					case 24:send_buf[4]=0xF2;send_buf[5]=0x01;break;
					case 25:send_buf[4]=0xF3;break;
					case 26:send_buf[4]=0xF4;break;
					case 27:send_buf[4]=0xF5;break;
					case 28:send_buf[4]=0xF6;break;
					case 29:send_buf[4]=0xF7;send_buf[5]=0x01;break;
					case 30:send_buf[4]=0xF8;send_buf[5]=0x01;break;
					case 31:send_buf[4]=0xF9;send_buf[5]=0x01;break;
          case 32:send_buf[4]=0xFA;send_buf[5]=0x01;break;
					case 33:send_buf[4]=0xFB;send_buf[5]=0x01;break;
					case 34:send_buf[4]=0xFC;send_buf[5]=0x01;break;
					case 35:send_buf[4]=0xFD;break;
					case 36:send_buf[4]=0xFE;break;
					case 37:send_buf[4]=0xFF;break;
					
//					case 38:send_buf[4]=0x09;break;
//					case 39:send_buf[4]=0x6A;send_buf[5]=0x01;break;
//					case 40:send_buf[4]=0x70;send_buf[5]=0x01;break;
//					case 41:send_buf[4]=0x71;send_buf[5]=0x01;break;
//					case 42:send_buf[4]=0x07;send_buf[5]=0x03;break;
//					case 43:send_buf[4]=0x08;send_buf[5]=0x03;break;
//					case 44:send_buf[4]=0x03;send_buf[5]=0x03;break;
//					case 45:ff=1;break;
//					case 46:send_buf[4]=0x31;break;
//					case 47:send_buf[4]=0x61;break;
//					case 48:send_buf[4]=0x09;break;
//					case 49:send_buf[4]=0x6A;send_buf[5]=0x01;break;
//					case 50:send_buf[4]=0x70;send_buf[5]=0x01;break;
//					case 51:send_buf[4]=0x71;send_buf[5]=0x01;break;
//					case 52:send_buf[4]=0x07;send_buf[5]=0x03;break;
//					case 53:send_buf[4]=0x08;send_buf[5]=0x03;break;
//					case 54:send_buf[4]=0x03;send_buf[5]=0x03;break;
//					case 55:ff=1;break;
//					case 56:send_buf[4]=0x31;break;
										
				}
				if (ff==1)
				{
					for (j=0;j<4;j++) 
					{
						USART_SendData(USART1,stop_send_buf[j]);
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
					}
				}
				else 
				{
					if (flag) 
					{
						for (j=0;j<7;j++) 
						{
							USART_SendData(USART1,send_buf[j]);
							while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
						}
						atk_8266_quit_trans();
						atk_8266_send_cmd("AT+CIPSEND","OK",20);         //开始透传  
						ack_buf[3]=p[3];
						for (j=0;j<6;j++) 
						{  
							USART_SendData(USART3,ack_buf[j]); 
							while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET); //循环发送,直到发送完毕 
						}
					}
				}
			}
   	}
	}
}









