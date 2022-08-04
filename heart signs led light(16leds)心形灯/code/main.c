#include <REG51.H>
#include <intrins.H>
void delay(unsigned int x){while(x--);}
void display()	
{	
	unsigned char i_0,i_1,i_2,i_3;
/***依次流水***/
	for(i_0 = 0;i_0 < 7;i_0++)
	{
		if(i_0 == 0)
		{
			P1 = ~0x01;delay(40000);
		}
		P1 = _crol_(P1,1);delay(40000);
	}
	P1 = 0xff;
	for(i_1 = 0;i_1 < 7;i_1++)
	{
		if(i_1 == 0)	
		{
			P2 = ~0x01;delay(40000);
		}
		P2 = _crol_(P2,1);delay(40000);
	}
	P2 = 0xff;
	delay(40000);
/***依次点亮***/
	for(i_2 = 0;i_2 < 7;i_2++)
	{
		if(i_2 == 0)
		{
		P1 = ~0x01;
		}delay(30000);
		P1 = _crol_(P1,1) - 0x01;
	}
	delay(30000);	
	for(i_3 = 0;i_3 < 7;i_3++)
	{
		if(i_3 == 0)
		{
			P2 = ~0x01;
		}delay(30000);
		P2 = _crol_(P2,1) - 0x01;
	}
	delay(20000);
	P1 = 0Xff;P2 = 0Xff;
	delay(20000);
}
void main()
{
	while(1)
	{
		display();
	}
}