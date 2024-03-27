#include <reg52.h>
#include "mouse.h"
#define  uchar unsigned char
#define  uint unsigned int
#define N 8
/* * * * * * * * * * * * * * * * * 
 *������������
 */
sbit tube1 = P4^3;
sbit tube2 = P4^2;
sbit beep = P3^7;
/* * * * * * * * * * * * * * * * * *
 *���ݵ�Ԫ
 */
//ԭ��תȦ
uchar code run1[] = {0x11,0x33,0x22,0x66,0x44,0xcc,0x88,0x99,0x11};
//ֱ��
uchar code run2[] = {0x11,0x39,0x28,0x6c,0x44,0xc6,0x82,0x93,0x11};
//�����
uchar code table[] = {0xc0,0xcf,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e,0xff};
//��ͼ
uchar map[8][8] = {0};
//λ������
uchar location[2] = {0,0};
//��·����Ϣ
uchar stack[17][2] = {0};

char top = -1;
uchar direction = 0;
void recall();//���� 
void voice();
uchar pass(uchar way);


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
	uchar i,j,k,l = 21;
	while(l--){
		for(i=0;i<5;i++)
		{
			for(j=8;j>0;j--)
			{
				P1 = run2[j];
				delayms(3);
			}
		}
			if((irLU==1) && (irC == 0))
			{
				//voice();
				  for(k = 0 ;k < 3; k++)
				{
					for(j=0;j<8;j++)
					{
						P1 = run1[j];
						delayms(3);
					}
				}
				
			}
			if((irRU==1) && (irC == 0))
			{   
				//voice();
				  for(k = 0;k < 3; k++)
				{
					for(j=8;j>0;j--)
					{
						P1 = run1[j];
						delayms(3);
					}
				}
			}
		}
}

/*
 *��ͷ
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

/*
 *��ת
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
 *��ת
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

/*
 *�����
 */
////�У��� ��ʾ
void now_x(uchar num)
{
	tube2 = 0;
	tube1 = 1;
	P0 = table[num];
	delayms(2);
	tube1 = 0;
}
void now_y(uchar num)
{
	tube1 = 0;
	tube2 = 1;
	P0 = table[num];
	delayms(2);
	tube2 = 0;
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
		delayms(500);
		go_straight();
		delayms(1000);
	}
	else if(!irC)
	{
		go_straight();
		delayms(500);
	
	}
	else if(!irL)
	{
		turn_left();
		delayms(500);
		go_straight();
		delayms(100);
	}
	else
	{
		back();
		delayms(500);
		go_straight();
		delayms(500);
		voice();
		delayms(100);
	}
	  
}
void ir_test(){ // ����ĳ������ĺ���
   
        if(irL||irR||irC){
            beep = 0;
        }else{
            beep = 1;
        }
   
}
/* * * * * * * * * * * * * * * 
 * ��һ���� �����Թ�
 * ��ͼ��ʼ��
 */
// void init_map()
// {
// 	uchar i,j;
// 	//�����ջ
// 	top++;
// 	stack[top][0] = location[0];
// 	stack[top][1] = location[1];
// 	for(i=0;i<8;i++)
// 	{
// 		for(j=0;j<8;j++)
// 		{
// 			map[i][j] = 0xff;
// 		}
// 	}
// }
	
 /*
 *��¼��ǰλ����Ϣ
 */
void save()
{ 	unsigned char temp = 0,count = 0,temp1 = 0;
		switch(direction)//��ͼ��Ϣ�ļ�¼* * * * ���ν���˵�Ԫ��
		{
			case 0:temp+=irC; temp<<=1 ; temp+=irR; temp<<=2; temp+=irL; 
				   map[location[0]][location[1]] &= (0xf0+temp); map[location[0]][location[1]] &= 0xdf; 
				   break;
			case 1:temp+=irL; temp<<=1 ; temp+=irC; temp<<=1; temp+=irR;
				   temp<<=1; map[location[0]][location[1]] &= (0xf0+temp); map[location[0]][location[1]] &= 0xef; 
				   break;
			case 2:temp+=irL; temp<<=1 ; temp+=irC; temp<<=1; temp+=irR; 
			       map[location[0]][location[1]] &= (0xf0+temp); map[location[0]][location[1]] &= 0x7f; 
				   break;
			case 3:temp+=irR; temp<<=2 ; temp+=irL; temp<<=1; temp+=irC; 
			       map[location[0]][location[1]] &= (0xf0+temp); map[location[0]][location[1]] &= 0xbf; 
				   break;
		}
		if(location[0] == 0 && location[1] == 0)//是否为起点
		map[0][0] = 0xf7;
		//�ж��Ƿ�Ϊ��·��
		if((!irC) && pass(0))
			count++;
		if((!irR) && pass(1))
			count++;
		if((!irL) && pass(3))
			count++;
		if(count>=2)//��¼��·����Ϣ
		{
			top++;
			stack[top][0] = location[0];
			stack[top][1] = location[1];
		}
}	
/*
 *��鵱ǰҪ�ߵķ����Ƿ��߹�
 *�߹�Ϊ0������Ϊ1
 */
uchar pass(uchar way)
{
		unsigned char x1,y1;
	  x1 = location[0];
	  y1 = location[1];
		switch(direction)
		{
			case 0: 
			{
				switch(way)
				{
					case 0: if(map[x1 - 1][y1] == 0xff) return 1;  return 0;//�� 
					case 1: if(map[x1][y1 + 1] == 0xff) return 1;  return 0;//�� 
					case 3: if(map[x1][y1 - 1] == 0xff) return 1;  return 0;//�� 
				}
				
			}
			case 1: 
			{
				switch(way)
				{
					case 0: if(map[x1][y1+1] == 0xff) return 1;  return 0;//��
					case 1: if(map[x1+1][y1] == 0xff) return 1;  return 0;//��
					case 3: if(map[x1-1][y1] == 0xff) return 1;  return 0;//�� 
				}
			}
			case 2: 
			{
				switch(way)
				{
					case 0: if(map[x1+1][y1] == 0xff) return 1;  return 0;//�� 
					case 1: if(map[x1][y1-1] == 0xff) return 1;  return 0;//��
					case 3: if(map[x1][y1+1] == 0xff) return 1;  return 0;//�� 
				}
			}
			case 3: 
			{
				switch(way)
				{
					case 0: if(map[x1][y1-1] == 0xff) return 1; return 0;//�� 
					case 1: if(map[x1-1][y1] == 0xff) return 1; return 0;//��
					case 3: if(map[x1+1][y1] == 0xff) return 1; return 0;//�� 
				}
			}
		}
}
/*
 * * ���µ�ǰλ����Ϣ
 * �� �� �� �� 0 1 2 3
 */
void move()
{
	switch(direction)
	{
		case 0: {
			location[0] += 1;
			break;
		}
		case 1:{
		  location[1] += 1;
		  break;
	  }
		case 2:{
			location[0] -= 1;
			break;
		}
		case 3:{
			location[1] -= 1;
			break;
		}
	}
}	
/*
 * ����
 */
void recall()
{
	unsigned char temp = 0,count = 0,temp1 = 0,i = 0; 
	temp = map[location[0]][location[1]];
	while(1)
	{
		temp = map[location[0]][location[1]];
		if(!((temp >>= 4) & 0x01))//��
		{
			switch(direction)
			{
				case 0: turn_left(); delayms(100); go_straight();delayms(100); break;
				case 1: back();delayms(100);go_straight();delayms(100); break;
				case 2: turn_right(); delayms(100); go_straight();delayms(100); break;
				case 3: go_straight(); delayms(100); break;
			}
			move(); 
			temp = map[location[0]][location[1]];
		}
		//��ʾ��ǰ����
	  now_x(location[0]);
	  now_y(location[1]);
		if(location[0] == stack[top][0] && location[1] == stack[top][1])
			break;
	  	
		
		temp = map[location[0]][location[1]];
		if(!((temp >>= 5) & 0x01))//��
		{
			switch(direction)
			{
				case 0: back();delayms(100);go_straight();delayms(100); break;
				case 1: turn_right(); delayms(100);go_straight();delayms(100);break;
				case 2: go_straight(); delayms(100); break;
				case 3: turn_left();delayms(100);go_straight();delayms(100);break;
			}
			move();
			temp = map[location[0]][location[1]];
		}
		//��ʾ��ǰ����
	  now_x(location[0]);
	  now_y(location[1]);
		if(location[0] == stack[top][0] && location[1] == stack[top][1])
			break;
		
		temp = map[location[0]][location[1]];
		if(!((temp >>= 6) & 0x01))//��
		{
			switch(direction)
			{
				case 0: turn_right();delayms(100);go_straight();delayms(100);break;
				case 1: go_straight();delayms(100); break;
				case 2: turn_left();delayms(100);go_straight();delayms(100); break;
				case 3: back();delayms(100);go_straight();delayms(100); break;
			}
			 move();
			temp = map[location[0]][location[1]];
		}
		//��ʾ��ǰ����
	  now_x(location[0]);
	  now_y(location[1]);
		if(location[0] == stack[top][0] && location[1] == stack[top][1])
			break;
		
		temp = map[location[0]][location[1]];
		if(!((temp >>= 7) & 0x01))//��
		{
			switch(direction)
			{
				case 0: go_straight(); delayms(100); break;
				case 1: turn_left();delayms(100);go_straight();delayms(100);  break;
				case 2: back();delayms(100);go_straight();delayms(100);  break;
				case 3: turn_right();delayms(100); go_straight();delayms(100);break;
			}
			move(); 
			temp = map[location[0]][location[1]];
		}
		//��ʾ��ǰ����
	  now_x(location[0]);
	  now_y(location[1]);
		if(location[0] == stack[top][0] && location[1] == stack[top][1])
			break;
	}
	top--;//��ջ 
}
/* 
 *�����Թ�
 */
void find()
{
	  //��ʾ��ǰ����
	  now_x(location[0]);
	  now_y(location[1]); 
	  //�ȴ����
//	  delayms(2);
	  //�жϵ�ǰλ���Ƿ�Ϊ��һ�ε���
	  if(map[location[0]][location[1]] == 0xff)
	  save();
	  
		  if(irR==0&&pass(1))//�ұ��޵�����δ�߹�
			{
				turn_right();
				delayms(100);
				go_straight();
				delayms(500);
				move();
			}
			else if(irC==0&&pass(0))//�ұ��е��壬ǰ������δ�߹�
			{
				go_straight();
				delayms(500);
				move();
			}
			else if(irL==0&&pass(3))//�ұ��е���ǰ���У������
			{
				turn_left();
				delayms(100);
				go_straight();
				delayms(500);
				move();
			}
			else//����ͬ
			{
				recall();
			}

}


void main()
{
//	initTime2();//��ʼ����ʱ��
//	while(1){
//		//test();
//		ir_test();
//	}
	uchar i,j;
	initTime2();//��ʼ����ʱ��
	//准备
	top++;
	stack[top][0] = location[0];
	stack[top][1] = location[1];
	for(i=0;i<8;i++)
	{
		for(j=0;j<8;j++)
		{
			map[i][j] = 0xff;
		}
	}
	while(top >= 0)
	{
		delayms(500);
		find();//������ͼ
	}

}
/*
 *�жϷ�����
 */
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