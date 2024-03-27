#include <reg52.h>
#include "mouse.h"
#define  uchar unsigned char
#define  uint unsigned int
sbit tube1 = P4^3;
sbit tube2 = P4^2;
sbit beep = P3^7;
uchar number = 0;
//ԭ��תȦ
uchar code run1[] = {0x11,0x33,0x22,0x66,0x44,0xcc,0x88,0x99,0x11};
//ֱ��
uchar code run2[] = {0x11,0x39,0x28,0x6c,0x44,0xc6,0x82,0x93,0x11};
//�����
uchar code table[] = {0xc0,0xcf,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e,0xff};
void xiuzheng();
/*
 *��ʱ1ms
 */
void delayms(uint n)
{
	uchar i,j;
	for(j=n;j>0;j--)
	for(i=112;i>0;i--);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *�ƶ���Ԫ
 * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *ֱ��
 */
void go_straight()
{
	uchar j,i;
	for(j=0;j<105;j++)//һȦ
	{
		for(i=8;i>0;i--)
		{
			P1 = run2[i];
			delayms(2);
		}
	}
}

/*
 *��ͷ
 */
void back()
{
	uchar i,j;
	for(i=0;i<101;i++)
	{
		for(j=0;j<8;j++)
		{
			P1 = run1[j];
			delayms(2);
		}
	}
	
}

/*
 *��ת
 */
void turn_right()
{
	uchar i,j;
	for(i=0;i<51;i++)
	{
		for(j=0;j<8;j++)
		{
			P1 = run1[j];
			delayms(2);
		}
	}
	
}

/*
 *��ת
 */
void turn_left()
{
	uchar i,j;
	for(i=0;i<51;i++)
	{
		for(j=8;j>0;j--)
		{
			P1 = run1[j];
			delayms(2);
		}
	}
	
}
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *���赥Ԫ
 * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *������
 */
void voice()
{
	  uchar j = 0;
		for(j = 10;j>0;j--){
			beep = ~beep;
			delayms(10);
		}
		for(j = 100;j>0;j--){
			beep = 1;
			delayms(10);
		}
}
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *��������Ԫ
 * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *���ⷢ��
 */
void mouse_ir_on(uchar group_no)
{
	A0 = (group_no) & 0x01;
	A1 = (group_no) & 0x02;
	A2 = (group_no) & 0x04;
}

/*
 *����T2�Զ���װ�ؼĴ����ͼ�����
 */
void setTime2(uint us)
{
	TH2 = (65536 - us) % 256;
	RCAP2H = (65536 - us) / 256;
	TL2 = (65536 -us) % 256;
	RCAP2L = (65536 - us) % 256;
}
/*
 *��ʱ��������ʼ��
 */
void initTime2()
{
	EA = 1;
	ET2 = 1;
	setTime2(5000);//����5ms�ж�һ��
	TR2 = 1;
}
/*
 *���ϲ���
 */
void test()
{
	if(!irR)
	{
		turn_right();
		delayms(100);
		//go_straight();
		xiuzheng();
		delayms(100);
	}
	else if(!irC)
	{
		//go_straight();
		xiuzheng();
		delayms(100);
	}
	else if(!irL)
	{
		turn_left();
		delayms(100);
		//go_straight();
		xuizheng();
		delayms(100);
	}
	else
	{
		back();
		delayms(100);
		//go_straight();
		xiuzheng();
		delayms(100);
		voice();
		delayms(100);
	}
	  
}
void ir_test(){ // ����ĳ������ĺ���
   
        if(irLU||irRU){
            beep = 0;
        }else{
            beep = 1;
        }
   
}
/*
 *�����
 */
void display(uchar num)
{
	uchar a,b;
	a = num / 10;
	b = num % 10;
	
	tube2 = 0;
	tube1 = 1;
	P0 = table[a];
	delayms(2);
	tube1 = 0;
	
	tube1 = 0;
	tube2 = 1;
	P0 = table[b];
	delayms(2);
	tube2 = 0;
}
void xiuzheng()//��һ������
{
	uchar i,j,k,l = 20;
	while(l--){
		for(i=0;i<5;i++)
		{
			for(j=8;j>0;j--)
			{
				P1 = run2[j];
				delayms(1);
			}
		}
			if(irLU==1 && irC == 0)
			{
					voice();
				  for(k = 0 ;k < 3; k++)
				{
					for(j=0;j<8;j++)
					{
						P1 = run1[j];
						delayms(2);
					}
				}
				
			}
			if(irRU==1 && irC == 0)
			{   
					voice();
				  for(k = 0;k < 3; k++)
				{
					for(j=8;j>0;j--)
					{
						P1 = run1[j];
						delayms(2);
					}
				}
			}
		}
}
void main()
{
	
	initTime2();
	
	while(1)
	{
		//test();
		ir_test();
	}
	
	
}
void time2() interrupt 5{ // TIM2�жϷ�����
    static bit ir = 0;
    static unsigned char n=1;
    TF2 = 0;
    if(!ir)	{
        mouse_ir_on(n-1);
    }else{
        if(n==1){
            if(irR1)	irC=0;
            else			irC=1;
        }else if(n==2){
            if(irR2)	irLU=0;
            else			irLU=1;
        }else if(n==3){
            if(irR3)	irL=0;
            else			irL=1;
        }else if(n==4){
            if(irR4)	irR=0;
            else			irR=1;
        }else if(n==5){
            if(irR5)	irRU=0;
            else			irRU=1;
        }
    }
    if(ir)	n++;
    if(n>5)	n=1;
    ir=~ir;
}
