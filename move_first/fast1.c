#include <reg52.h>
#include "mouse.h"
#define  uchar unsigned char
#define  uint unsigned int
	char way[17] = {0 ,0, 1, 1, 0 , 0 , 1 , 1 , 1 , 0 , 1 , 1 , 0 , 0 };//路径数据
	char top1 = 13;
	sbit beep = P3^7;
	//原地转圈
  uchar code run1[] = {0x11,0x33,0x22,0x66,0x44,0xcc,0x88,0x99,0x11};
  //直行
  uchar code run2[] = {0x11,0x39,0x28,0x6c,0x44,0xc6,0x82,0x93,0x11};
	uchar direction = 2;
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *传感器单元
 * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *红外发射
 */
void mouse_ir_on(uchar group_no)
{
	A0 = (group_no) & 0x01;
	A1 = (group_no) & 0x02;
	A2 = (group_no) & 0x04;
}

/*
 *设置T2自动重装载寄存器和计数器
 */
void setTime2(uint us)
{
	TH2 = (65536 - us) % 256;
	RCAP2H = (65536 - us) / 256;
	TL2 = (65536 -us) % 256;
	RCAP2L = (65536 - us) % 256;
}
/*
 *定时计数器初始化
 */
void initTime2()
{
	EA = 1;
	ET2 = 1;
	setTime2(5000);//设置5ms中断一次
	TR2 = 1;
}
	/*
 *延时1ms
 */
void delayms(uint n)
{
	uchar i,j;
	for(j=n;j>0;j--)
	for(i=112;i>0;i--);
}
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
	/*
 *冲刺
 */
void run_fast()
{
		uchar i,j,k,l = 21;
	while(l--){
		for(i=0;i<5;i++)
		{
			for(j=8;j>0;j--)
			{
				P1 = run2[j];
				delayms(2);
			}
		}
			if((irLU==1) && (irC == 0))
			{
//					voice();
				  for(k = 0 ;k < 3; k++)
				{
					for(j=0;j<8;j++)
					{
						P1 = run1[j];
						delayms(2);
					}
				}
				
			}
			if((irRU==1) && (irC == 0))
			{   
//					voice();
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

/*
 *右转
 */
void turn_right()
{
	uchar i,j;
	for(i=0;i<52;i++)
	{
		for(j=0;j<8;j++)
		{
			P1 = run1[j];
			delayms(2);
		}
	}
	direction = (direction + 1) % 4;
}
/*
 *左转
 */
void turn_left()
{
	uchar i,j;
	for(i=0;i<52;i++)
	{
		for(j=8;j>0;j--)
		{
			P1 = run1[j];
			delayms(2);
		}
	}
	direction = (direction + 3) % 4;
}
/*
 *掉头
 */
void back()
{
	uchar i,j;
	for(i=0;i<102;i++)
	{
		for(j=0;j<8;j++)
		{
			P1 = run1[j];
			delayms(2);
		}
	}
	direction = (direction + 2) % 4;
}
	
	/* * * * * * * * * * * * * 
 * 第三部分 冲刺
 * 冲刺
 */
void run()
{

	back();
	while(top1 > -1)
	{
		switch(direction){
			case 0:
				{
		
			switch(way[top1])
			{
				case 1: turn_right(); run_fast()  ; break;
				case 0: run_fast() ; break; 
			}
			break;
		}
			case 1:{
				
			
			switch(way[top1])
			{
				case 1: run_fast() ;break;
				case 0: turn_left(); run_fast() ; break; 
			}	
		    break;
		}
	   }
		top1--;
	}	

} 



void main(){
	initTime2();
	delayms(3);
	run();
	voice();
	voice();
	voice();
	while(1);
	
}

/*
 *中断服务函数
 */
void time2() interrupt 5{ // TIM2中断服务函数
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

