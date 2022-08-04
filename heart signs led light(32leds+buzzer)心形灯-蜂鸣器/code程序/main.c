#include<reg51.h>
#define uchar unsigned char 
sfr P4 = 0xc0;
sbit   speaker = P1^0;
unsigned char timer0h, timer0l, time;
unsigned char cp,cp1,n,k=0,i=0,flag=250;
int cp2,sec;
sbit P11 = P1^1;
sbit P12 = P1^2;
sbit P13 = P1^3;
sbit P14 = P1^4;
sbit P15 = P1^5;
sbit P16 = P1^6;
sbit P17 = P1^7;
void P1_1(char i)
{
	P11 = i & 0x02;
	P12 = i & 0x04;
	P13 = i & 0x08;
	P14 = i & 0x10;
	P15 = i & 0x20;
	P16 = i & 0x40;
	P17 = i & 0x80;
}
//-------------------------------------- 
//单片机晶振采用11.0592MHz 音调为二进制半周期时间取反得到 
code unsigned char FREQH[] = {      0xF2, 0xF3, 0xF5, 0xF5, 0xF6, 0xF7, 0xF8,    //低音1234567      
									0xF9, 0xF9, 0xFA, 0xFA, 0xFB, 0xFB, 0xFC, 0xFC,//1,2,3,4,5,6,7,i     
									0xFC, 0xFD, 0xFD, 0xFD, 0xFD, 0xFE,            //高音 234567     
									0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFF	   //超高音 1234567 
								};    
// 频率-半周期数据表 低八位 
code unsigned char FREQL[] = {      0x42, 0xC1, 0x17, 0xB6, 0xD0, 0xD1, 0xB6,    //低音1234567      
									0x21, 0xE1, 0x8C, 0xD8, 0x68, 0xE9, 0x5B, 0x8F, //1,2,3,4,5,6,7,i     
									0xEE, 0x44, 0x6B, 0xB4, 0xF4, 0x2D,             //高音 234567     
									0x47, 0x77, 0xA2, 0xB6, 0xDA, 0xFA, 0x16};   //超高音 1234567 
//世上只有妈妈好数据表           要想演奏不同的乐曲, 只需要修改这个数据表 
//code unsigned char sszymmh[] = {  
//				6, 2, 3, 5, 2, 1, 3, 2, 2,5, 2, 2, 1, 3, 2, 6, 2, 1, 5, 2, 1,   
////一个音符有三个数字。前为第几个音、中为第几个八度、后为时长（以半拍为单位）。 
////6, 2, 3代表：６, 中音, ３个半拍; //5, 2, 1代表：５, 中音, １个半拍;
////3, 2, 2代表：３, 中音, ２个半拍; 
////5, 2, 2代表：５, 中音, ２个半拍; //1, 3, 2代表：１, 高音, ２个半拍; 
//    			6, 2, 4, 3, 2, 2, 5, 2, 1, 6, 2, 1, 5, 2, 2, 3, 2, 2, 1, 2, 1,
//				6, 1, 1, 5, 2, 1, 3, 2, 1, 2, 2, 4, 2, 2, 3, 3, 2, 1, 5, 2, 2, 
//				5, 2, 1, 6, 2, 1, 3, 2, 2, 2, 2, 2, 1, 2, 4, 5, 2, 3, 3, 2, 1, 
//				2, 2, 1, 1, 2, 1, 6, 1, 1, 1, 2, 1, 5, 1, 6, 0, 0, 0};    
//
//code unsigned char sszymmh[] = {3,2,2, 3,2,1, 3,2,1, 4,2,2, 5,2,2, 3,2,3, 2,2,1, 2,2,4,
//								1,2,2, 1,2,1, 1,2,1, 2,2,2, 3,2,2, 3,2,3, 7,1,1, 7,1,4,
//								6,1,2, 3,2,2, 2,2,4, 6,1,2, 3,2,2, 2,2,4, 6,1,2, 3,2,2, 2,2,3, 1,2,1, 1,2,8,
//								3,2,2, 3,2,1, 3,2,1, 4,2,2, 5,2,2, 3,2,3, 2,2,1, 2,2,4,
//								1,2,2, 1,2,1, 1,2,1, 2,2,2, 3,2,2, 3,2,3, 7,1,1, 7,1,4,
//								6,1,2, 3,2,2, 2,2,4, 6,1,2, 3,2,2, 2,2,4, 6,1,2, 3,2,2, 2,2,3, 1,2,1, 1,2,8,
//								0,0,1, 3,2,1, 2,2,1, 5,2,6, 4,2,1, 3,2,1, 3,2,1, 2,2,1, 2,2,4, 5,2,1, 4,2,1,
//								3,2,2, 4,2,1, 5,2,3, 3,2,2, 2,2,6, 1,2,1, 6,1,2, 3,2,2, 2,2,3, 1,2,1,
//								5,1,2, 2,2,1, 1,2,1, 1,2,4, 4,2,1, 3,2,1, 4,2,1, 3,2,1, 1,2,4, 
//								4,2,1, 3,2,1, 4,2,1, 3,2,1, 1,2,3, 2,2,1, 1,2,8};
code unsigned char new[] = 
{
	5,2,1, 5,2,1, 6,2,2, 5,2,2, 1,3,2, 7,2,2, 0,0,2, 5,2,1, 5,2,1,
	6,2,2, 5,2,2, 2,3,2, 1,3,4, 5,2,1, 5,2,1, 5,3,2, 3,3,2, 1,3,2, 7,2,2, 6,2,4,
	0,0,2, 4,3,1, 4,3,1, 3,3,2, 1,3,2, 2,3,2, 1,3,4, 
};
//code unsigned char happy_year[] = 
//{
//1,2,1, 1,2,1, 1,2,2, 5,1,2, 
//3,2,1, 3,2,1, 3,2,2, 1,2,2, 
//1,2,1, 3,2,1, 5,2,2, 5,2,2, 4,2,1, 3,2,1, 2,2,2,
//2,2,1, 3,2,1, 4,2,2, 4,2,2, 3,2,1, 2,2,1, 3,2,2, 1,2,2,
//1,2,1, 3,2,1, 2,2,2, 5,1,2, 7,1,2, 2,1,2, 1,2,4
//};

uchar code Tab1[]={0xFE,0xFD,0xFB,0xF7,0xEF,0xDF,0xBF,0x7F,0xFF};//暗中左移  向下
uchar code Tab2[]={0x7F,0xBF,0xDF,0xEF,0xF7,0xFB,0xFD,0xFE,0xFF};//暗中右移  向上
uchar code Tab3[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x00};//亮中左移 向下
uchar code Tab4[]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01,0x00};//亮中右移  向上

uchar code Tab11[]={0xFE,0xFC,0xF8,0xF0,0xE0,0xC0,0x80,0x00,0xff};//暗中左移  向下
uchar code Tab22[]={0x7F,0x3F,0x1F,0x0F,0x07,0x03,0x01,0x00,0xff}; // 暗中右移
uchar code Tab33[]={0x80,0xC0,0xE0,0xF0,0xF8,0xFC,0xFE,0xFF};  //	亮中右移
uchar code Tab44[]={0x01,0x03,0x07,0x0F,0x1F,0x3F,0x7F,0xFF};  //	 亮中左移 
uchar code Tab55[]={0x08,0xc0,0xe0,0xf0,0xf8,0xfc,0xfe,0xff,0xff};//
uchar code Tab5[]={0x00,0x80,0xC0,0xE0,0xF0,0xF8,0xFC,0xFE,0xff}; //亮中右移
uchar code Tab6[]={0x00,0x01,0x03,0x07,0x0F,0x1F,0x3F,0x7F,0xff};//亮中左移
uchar code Tab7[]={0x7f,0xbf,0xdf,0xef,0xf7,0xfb,0xfd,0xfe};  //暗中右移
uchar code Tab8[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};   //暗中左移

void delay(void)   
{
    unsigned char m,n;
	for(m=0;m<flag;m++)
 	for(n=0;n<250;n++);             
 }

void azxy_1(void)  //暗中向下
{
  unsigned char i,i1;
 for(flag=80;flag>0;flag=flag-10)
  {
  	for(i=0;i<8;i++) 
    {
		P1_1(0xff);P3=0xff;
	    P2=Tab2[i];P0=Tab2[i];
		delay();   
 	}
  	for(i=0;i<9;i++) 
    {
		P2=0xff;P0=0xff;
	    P1_1(Tab1[i]);P3=Tab2[i]; 
	  	delay();
 	}
  }
  flag=10;
  for(i1=0;i1<30;i1++)
  {
  	for(i=0;i<8;i++) 
    {
			P1_1(0xff);P3=0xff;
		   	P2=Tab2[i];P0=Tab2[i];
			delay();   
 	}
  	for(i=0;i<9;i++) 
    {
		P2=0xff;P0=0xff;
	    P1_1(Tab1[i]);P3=Tab2[i];
	  	delay();
 	}				 
  }
  flag=200;
}

void hy2_1(void)  //暗中逆时针转90度。（一个一个点亮）
{
	unsigned char i;
	flag=200;
	P0=0xff;P3=0xff;P2=0xff;P1_1(0xff);
	for(i=0;i<9;i++) 
	{
		P0=Tab22[i];
		delay();
	}
	flag=110;
	for(i=0;i<9;i++) 
	{
		P1_1(Tab11[i]);
		delay();
	}
	flag=80;
	for(i=0;i<9;i++) 
	{
		P3=Tab11[i];
		delay();
	}
	flag=80;
	for(i=0;i<9;i++) 
	{
		P2=Tab11[i];
		delay();
	}
}

void hy2_2(void)  //暗中逆时针转180度。（一个一个点亮）
{
	unsigned char i;
	P0=0xff;P3=0xff;P2=0xff;P1_1(0xff);
	for(i=0;i<8;i++) 
	{
		P0=Tab22[i];
		delay();
	}
	for(i=0;i<8;i++) 
	{
		P1_1(Tab11[i]);
		delay();
	}
	P0=0xff;P1_1(0xff);
	for(i=0;i<8;i++) 
	{
		P3=Tab11[i];
		delay(); 
	}
	for(i=0;i<8;i++) 
	{
		P2=Tab11[i];
		delay();
	}
	P2=0xff;P3=0xff;
}

void hy2(void)  //暗中逆时针转360。（一个一个点亮）
{
	unsigned char i;
	for(flag=250;flag>0;flag=flag-50)
	{
		P0=0xff;P3=0xff;P2=0xff;P1_1(0xff);
		for(i=0;i<8;i++) 
		{
			P0=Tab22[i];
			delay();
		}
		for(i=0;i<8;i++) 
		{
			P1_1(Tab11[i]);
			delay();
		}
		for(i=0;i<8;i++) 
		{
			P3=Tab11[i];
			delay();
		}
		for(i=0;i<8;i++) 
		{
			P2=Tab11[i];
			delay();
		}
	}
	flag=250;
}

void hy3(void)  //点亮中逆时针转360°（一个一个灭）
{
	unsigned char i;
	flag=50;
	P0=0x00;P3=0x00;P2=0x00;P1_1(0x00);
	for(i=0;i<8;i++) 
	{
		P0=Tab33[i];
		delay();
	}
	for(i=0;i<8;i++) 
	{
		P1_1(Tab44[i]);
		delay();
	}
	for(i=0;i<8;i++) 
	{
		P3=Tab44[i];
		delay();
	}
	for(i=0;i<8;i++) 
	{
		P2=Tab44[i];
		delay();
	}
	flag=150;
}

void shansuo(void)//全部灯闪烁
{
	uchar i;
	for(i=0;i<3;i++)
	{
		P0=0xff; P3=0xff; P2=0xff; P1_1(0xff);
		delay(); delay();
		P0=0x00; P3=0x00; P2=0x00; P1_1(0x00);
		delay(); delay();
	}
}

void hy1(void)  //点亮状态逆时针旋转90度（一个一个灭）
{
  	unsigned char i;
 	for(i=0;i<8;i++) 
 	{
		P0=Tab11[i];P2=Tab22[i];
		P1_1(Tab11[i]);P3=Tab22[i];
		delay();
 	}
  	for(i=0;i<8;i++) 
    {
		P0=Tab44[i];P2=Tab55[i];
	    P1_1(Tab44[i]);P3=Tab55[i];
	  	delay();
	}
}

void jc(void)//	  交叉四叶草
{
	uchar i;
	for(i=0;i<3;i++) 
	{
		P0=0x00;
		P3=0x00;
		P2=0xff;
		P1_1(0xff);
		delay();delay();delay();delay();
		P2=0x00;
		P1_1(0x00);
		P0=0xff;
		P3=0xff;
		delay();delay();delay();delay();
	}
}

void hy4(void)  //点亮状态顺时针旋转90度（一个一个灭）
{
	unsigned char i;
	for(i=0;i<8;i++) 
	{
		P2=Tab22[i];P0=Tab11[i];
		P3=Tab22[i];P1_1(Tab22[i]);
		delay();
	}
	for(i=0;i<9;i++) 
	{
		P2=Tab5[i];P0=Tab6[i];
		P3=Tab5[i];P1_1(Tab5[i]);
		delay();
	}
}

void lzxy(void) //亮中下移 
{
	unsigned char i;
	for(i=0;i<9;i++) 
	{
		P1_1(0);P3=0;
	    P2=Tab4[i];P0=Tab4[i];
		delay();   
	}
	for(i=0;i<9;i++)  
	{
	    P1_1(Tab3[i]);P3=Tab4[i];
	  	delay();
	 }
}

void jh(void)//相邻2个交换亮
{
	uchar i;
	for(i=0;i<3;i++)
	{
		P0=0x55;P2=0xAA;P1_1(0xAA);P3=0xAA;
		delay();delay();
		P0=0xAA;P2=0x55;P1_1(0x55);P3=0x55;
		delay();delay();
	}
}

 void lzsy(void)//亮中上移  
{
	unsigned char i;
	for(i=0;i<9;i++)  
    {
		P0=0x00;P2=0x00;
	    P1_1(Tab4[i]);P3=Tab3[i];
		delay();   
 	}
	for(i=0;i<9;i++)  
    {
	    P0=Tab3[i];P2=Tab3[i];
	  	delay();
 	}
}

void sxb(void)//上下半
{
	unsigned char i;
	P0=0xff;P2=0xff;
	P1_1(0x00);P3=0x00;
	delay();delay();delay();delay();
    for(i=0;i<8;i++) 
    {
 	  P0=Tab22[i];
      P2=Tab22[i];
	  delay();
    }
    for(i=0;i<8;i++) 
    {
	  P0=Tab22[i];
      P2=Tab22[i];
	  delay();
    }
    for(i=0;i<8;i++) 
    {
 	  P0=Tab22[i];
      P2=Tab22[i];
	  delay();
    }
	for(i=0;i<3;i++)
	{ 
		P0=0x00;P2=0x00;
		delay();delay();
		P0=0xff;P2=0xff;
		delay();delay();
	}
}

void hy2_3(void)  //暗中向下180。（一个一个点亮）
{
    unsigned char i;
	P0=0xff;P3=0xff;P2=0xff;P1_1(0xff);
    for(i=0;i<8;i++) 
    {
        P0=Tab22[i];
        P2=Tab22[i];
		delay();
	}
    for(i=0;i<8;i++) 
    {
	    P1_1(Tab11[i]);
	    P3=Tab22[i];
	    delay();
    }

    flag=100;
    for(i=0;i<8;i++) 
    {
		P1_1(Tab33[i]);
	    P3=Tab44[i];
		delay();
	}
    for(i=0;i<8;i++) 
    {
	    P0=Tab44[i];
		P2=Tab44[i];
		delay();
    	flag=200;
	}
}

void js(void)//结束
{
	uchar i;
	hy2_3();
	for(i=0;i<3;i++) 
	{
		P0=0xff;P1_1(0xff);
		delay();delay();
		P0=0x00;P1_1(0x00);
		delay();delay();
	}
///////////////////
	for(i=0;i<3;i++) 
	{
		P2=0xff;P3=0xff;
		delay();delay();
		P2=0x00;P3=0x00;
		delay();delay();
	}
//////////////////
	for(i=0;i<5;i++)
	{
		P2=0x00;P3=0x00;
		P0=0x00;P1_1(0x00);
		delay();delay();
		P2=0xff;P3=0xff;
		P0=0xff;P1_1(0xff);
		delay();delay();
	}
}

void int0_isr(void) interrupt 0
{
	PCON = 0x00;	
	EX0 = 0;
}
void int0_init(void)
{
	IT0 = 0;
	EX0 = 1;
}

void t0int() interrupt 1       //T0中断程序，控制发音的音调 
{ 
	 TR0 = 0;                     //先关闭T0  
	 speaker = ~speaker;          //输出方波, 发音  
	 TH0 = timer0h;               //下次的中断时间, 这个时间, 控制音调高低 
	 TL0 = timer0l;
	 n = 1;  
	 TR0 = 1;                     //启动T0 
}  
//--------------------------------------
void t1init() interrupt 3
{
	
	TH1 = (65536 - 1000) / 256;
	TL1 = (65536 - 1000) % 256;
	cp2++;
	if(cp2 >= 1000)
	{
		cp2 = 0;
		sec++;
		if(sec >=10)
		{
			sec = 0;
			P0 = P1 = P2 = P3 = 0xff;
			k = 0;
			i = 0;
			EX0 = 1;			
			PCON = PCON | 0x02;	
		}
	}
	cp++;
	TR0 = 1;
	if(cp >= 250)
	{
		cp = 0;
		cp1++;
		if(cp1 >= time)
		{
			cp1 = 0;
			k = new[i] + 7 * new[i + 1] - 1;
			timer0h = FREQH[k];      //从数据表中读出频率数值       
			timer0l = FREQL[k];      //实际上, 是定时的时间长度       
			time = new[i + 2];   //读出时间长度数值
			i += 3; 
			if(i >= 270) i=0;	
		}
	}

}


void timer_init()
{
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x11;		//设置定时器模式

	TL0 = timer0l;		//设置定时初值
	TH0 = timer0h;		//设置定时初值

	TL1 = (65536 - 1000) % 256;		//设置定时初值
	TH1 = (65536 - 1000) / 256;		//设置定时初值

	EA = 1;
	ET0 = 1;
	ET1 = 1;

	TR1 = 1;		//定时器1开始计时
}
 
//-------------------------------------- 
void main(void) 
{  
	int0_init();
	timer_init();                   
	i = 0;     
	time = 1;
	while(1)
	{
		azxy_1();
		hy2_1();//逆90 逐渐加速
		hy2_2();//逆180 逐渐加速
		hy2();	//逆360 逐渐加速（渐亮）
		hy3(); // 逆360 （渐灭）
		shansuo();//闪烁
		
		hy1();// 逆90 亮，灭
		jc(); // 四叶草
		hy4(); //顺90 亮，灭
		jc(); // 四叶草
		shansuo(); 
		
		lzxy();	// 亮中下移
		jh();	//	交换2个灯
		lzsy();//	亮中上移
		jh();	 
		
//		hy5(); //对称二个亮上下移动(暗中上下移)
		sxb();//上下半
		shansuo();
		js(); //结束 (调用hy2_3)
	}
}