#include <stdio.h>
#include <reg52.h>
/*
* ?¡ã??¡ê??-¦Ì?¡Á¨®¨®¨°¡Áa
* ?¨¬2a??¨¬?
* ¨ºy?Y¡ä¡é¡ä?-¨ºy¡Á¨¦
* ¡À¨¦¨¤¨²¡êo
	¡Á¨®¨º?¡¤¡§?¨°¡êo¡Á¨®-?¡ã-¨®¨°-o¨®
	3?¨º???0????¡ê???3¡¥1o???¡ê??¨¬2a??¨¬?¡ê??????t??char¨ºy¡Á¨¦maze??0,0??¦Ì¨ª????1 
	¡¤??¨°¦Ì¡Â¡Áa¡ê?3¡¥?¨°¨®D?¡¤¦Ì?¡¤??¨°?¡ê????¡¤??¨°¡À?¨¢?dirction
	for ?¡ã??¨°???¡ê???????wall???¡ã?????a¨¤¡ä?¡ä¡¤??¨°?¡ê
		
* ?¡ã?¡¤¡êo
* 3?¡ä¨¬¡êo
*/

struct coordinate
{
  int cX;
  int cY;
};

struct coordinate crossStack[24];



sfr  P4=0xe8;
sbit A0=P4^0;
sbit A1=P2^0;
sbit A2=P2^7;
sbit irR1 = P2^1;
sbit irR2 = P2^2;
sbit irR3 = P2^3;
sbit irR4 = P2^4;
sbit irR5 = P2^5;
sbit tubel1=P4^2;
sbit tubel2=P4^3;
sbit Buzzer = P2^4;
bit irC1=0,irC2=0,irR=0;
char  go[] = {0x19,0x38,0x2c,0x64,0x46,0xc2,0x83,0x91};
char  back[] = {0x91,0x83,0xc2,0x46,0x64,0x2c,0x38,0x19};
char i1,j1,i2;





unsigned char	coY=0,coX=0,step1=0,step2=15;											//¨¨???D?3¦Ì¡Á?¡À¨º
unsigned char	dir=0,													//D?3¦Ì¦Ì¡À?¡ã???¨°¦Ì?????¡¤??¨°
				top=0,													//???D?a????¨ºy¡ê??¡ä¡Á?o¨®¨°????a????¨®|¦Ì?¨ºy¡Á¨¦??¡À¨º+1
				cX=0,cY=0;

unsigned char 	maze[8][8]={0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0};										//¡äy¨¬?2a??1?
/*unsigned char	realMaze[8][8]={0x0d,0x05,0x03,0x0d,0x05,0x01,0x03,0x0b,
															0x09,0x05,0x00,0x05,0x05,0x06,0x0a,0x0a,
															0x0a,0x0d,0x04,0x01,0x05,0x01,0x04,0x06,
															0x0a,0x0d,0x05,0x00,0x05,0x04,0x01,0x03,
															0x0c,0x05,0x03,0x08,0x05,0x07,0x0a,0x0a,
															0x09,0x01,0x04,0x04,0x01,0x03,0x0a,0x0e,
															0x0a,0x0a,0x0d,0x05,0x02,0x08,0x04,0x07,
															0x0e,0x0c,0x05,0x05,0x06,0x0c,0x05,0x07,};*/	//?¡ê?a??¨º¦Ì??1?

unsigned char 	wall[4][3]={0x08,0x01,0x02, 
							0x01,0x02,0x04,
							0x02,0x04,0x08,
							0x04,0x08,0x01};  							//[dir][¡Á¨®0/?D1/¨®¨°2/]¡ê?

void Delay(int x)		
{
  char i, j;
	for(x;x>0;x--)
	{
			i = 2;
			j = 199;
			do
			{
				while (--j);
			} while (--i);
	}
}
void MOUSE_IR_ON(int a) 		 
{	
	A0=(a)&0x01;
	A1=(a)&0x02;
	A2=(a)&0x04;
}
void setTime2(unsigned int us)
{
	TH2=(65536-us)/256;
	RCAP2H=(65536-us)/256;
  TL2=(65536-us)%256;
	RCAP2L=(65536-us)%256;
}
void initTime2()
{
	EA=1;
	ET2=1;
	setTime2(5000);
	TR2=1;
}

void lightNumber(int x,int num)
{
	char Number[] = {0xC0,0xCF,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};
	switch(x)
	{
		case 1:tubel1=1;tubel2=0;break;
		case 2:tubel1=0;tubel2=1;break;
	}
	P0=Number[num];
	Delay(5);
}
unsigned char check(int flag);
void run(bit f)
{
	if(f) 
	{
		for(j1=0;j1<110;j1++)
			{
				for(i1=0;i1<8;i1++)
						{
							P1=(go[i1] );
							Delay(3);
						}
						if(j1%20==0)
						{
							
							if(check(4)&&(!check(0)))
							{
								//lightNumber(1,8);
								//lightNumber(2,8);
								for(i2=0;i2<3;i2++)
								{
									for(i1=0;i1<8;i1++)
									{
										P1=(go[i1]|0xf0);
										P1=P1&(back[i1]|0x0f);
										Delay(6);
									}
								}
								j1-=1;
								Delay(200);
							}
						}
						if(j1%25==0)
							{
							if(check(1)&&(!check(0)))
							{
								//lightNumber(1,7);
								//lightNumber(2,7);
								for(i2=0;i2<3;i2++)
								{
									for(i1=0;i1<8;i1++)
									{
										P1=(back[i1]|0xf0);
										P1=P1&(go[i1]|0x0f);
										Delay(6);
									}	
									
								}
								j1-=1;
								Delay(200);
							 }		
							}
												
					}

	}
}




void goForward ()
{
	switch (dir)
	{
	case 0:coY--;break;
	case 1:coX++;break;
	case 2:coY++;break;
	case 3:coX--;break;
	default:
		break;
	}
	run(1);
} 
void goBack ()
{
	switch (dir)
	{
	case 0:coY++;break;
	case 1:coX--;break;
	case 2:coY--;break;
	case 3:coX++;break;
	default:
		break;
	}
} 
void turnLeft() 
{
	switch (dir)
	{
	case 0:dir=3;break;
	case 1:dir=0;break;
	case 2:dir=1;break;
	case 3:dir=2;break;
	default:
		break;
	}
	for(j1=0;j1<52;j1++)
				{
					for(i1=0;i1<8;i1++)
							{
							P1=(go[i1]|0xf0);
							P1=P1&(back[i1]|0x0f);
							Delay(3);
							}
				}
}
void turnRight() 
{
	switch (dir)
	{
	case 0:dir=1;break;
	case 1:dir=2;break;
	case 2:dir=3;break;
	case 3:dir=0;break;
	default:
		break;
	}
	for(j1=0;j1<52;j1++)
				{
					for(i1=0;i1<8;i1++)
							{
							P1=(back[i1]|0xf0);
							P1=P1&(go[i1]|0x0f);
							Delay(3);
							}
				}
}
void turnBack()
{
	switch (dir)
	{
	case 0:dir=2;break;
	case 1:dir=3;break;
	case 2:dir=0;break;
	case 3:dir=1;break;
	default:
		break;
	}
	for(j1=0;j1<54;j1++)
				{
					for(i1=0;i1<8;i1++)
							{
							P1=(back[i1]|0xf0);
							P1=P1&(go[i1]|0x0f);
							Delay(3);
							}
				}
	for(j1=0;j1<54;j1++)
				{
					for(i1=0;i1<8;i1++)
							{
							P1=(back[i1]|0xf0);
							P1=P1&(go[i1]|0x0f);
							Delay(3);
							}
				}
}

unsigned char check(int flag)
{
	MOUSE_IR_ON(flag);
	Delay(10);
	switch (flag)
		{
			case 0:
							if(irR1){}
							else{return 1;} break;//forward
			case 1:
							if(irR2){}
							else{return 1;} break;
		  case 2:
							if(irR3){}
							else{return 1;} break;//left
		  case 3:
							if(irR4){}
							else{Delay(50);return 1;} break;//right
			case 4:
							if(irR5){}
							else{return 1;} break;
		}
}
int checkLeft(unsigned char * x)
{
	
	
	if(check(2))
	{
		(*x)=(*x)|wall[dir][0];
		return 0;
	}
		
	else
		return 2;
}
int checkFront(unsigned char * x)
{
	
	Delay(50);
	if(check(0))
	{
		(*x)=(*x)|wall[dir][1];
		return 0;
	}
	switch(dir)
	{
		case 0: if((maze[coY-1][coX])!=0)
				{
				//printf("??¨°???¨°??-¡Á?1y¡ê??¡ä¡Á¡Â?a??¡ê?2???¨ºy?Y");
				return 0;
				}break;
		case 1: if((maze[coY][coX+1])!=0)
				{
				//printf("??¨°???¨°??-¡Á?1y¡ê??¡ä¡Á¡Â?a??¡ê?2???¨ºy?Y");
				return 0;
				}break;
		case 2: if((maze[coY+1][coX])!=0)
				{
				//printf("??¨°???¨°??-¡Á?1y¡ê??¡ä¡Á¡Â?a??¡ê?2???¨ºy?Y");
				return 0;
				}break;
		case 3: if((maze[coY][coX-1])!=0)
				{
				//printf("??¨°???¨°??-¡Á?1y¡ê??¡ä¡Á¡Â?a??¡ê?2???¨ºy?Y");
				return 0;
				}break;					 
    }
	return 3;
}
int checkRight(unsigned char * x)
{
	
	Delay(50);
	if(check(3))
	{
		(*x)=(*x)|wall[dir][2];
		return 0;
	}
	
	else
	{;
	 return 4;
	}
}
void recordRoad(unsigned char * x)
{
	unsigned char origin =0;
	if(((*x)&0xf0)>0)
	return;
	switch (dir)
	{
	case 0:(*x)=(*x)|0x40;break;
	case 1:(*x)=(*x)|0x80;break;
	case 2:(*x)=(*x)|0x10;break;
	case 3:(*x)=(*x)|0x20;break;
	default:
		break;
	}

}
int checkWall(unsigned char * x)
{
	recordRoad(x);
	return checkRight(x)+checkLeft(x)+checkFront(x);
}
void adjustDir(int choice)
{
	if(choice==3)
	{
		//printf("¡Á¨®¨®¨°¨®D??¡ê??¡ã¡¤??T??¡ê?¡À¡ê3?¡¤??¨°\n"); 
		return;
	}
	
	if ( choice==2)
	{
		//printf("¨°¨°¨®D????¡Á¨®¡Áa\n"); 
		turnLeft();
		return;
	}
	if ( choice==4)
	{
		//printf("¨°¨°¨®D????¨®¨°¡Áa\n");
		turnRight();
		return;
	}
}
void recordCrossing()
{
	int crossChoice=0;
	switch(dir)
	{
		case 0:
				//printf("3¡¥?¨°?a%d¨º¡À¦Ì?2¨ª?¡¤????\n",dir);
				if((maze[coY][coX]&0x08)==0)
				{
					if((maze[coY][coX-1])==0)
					crossChoice+=2; 
					
				}
				if((maze[coY][coX]&0x01)==0)
				{
					if((maze[coY-1][coX])==0)
						crossChoice+=3;
						//printf("?¡¤?¨²2?¡Áa¡ê?3¡¥?¨°?a%d",dir); 
				}
				if((maze[coY][coX]&0x02)==0)
				{
					if((maze[coY][coX+1])==0)
						crossChoice+=4;
						//printf("?¡¤?¨²¨®¨°¡Áa¡ê?3¡¥?¨°?a%d",dir); 
				}break;
		case 1:
				//printf("3¡¥?¨°?a%d¨º¡À¦Ì?2¨ª?¡¤????\n",dir);
				if((maze[coY][coX]&0x01)==0)
				{
					if((maze[coY-1][coX])==0)
					crossChoice+=2; 
						//printf("?¡¤?¨²¡Á¨®¡Áa¡ê?3¡¥?¨°?a%d",dir);
				}
				if((maze[coY][coX]&0x02)==0)
				{
					//printf("2¨ª?¡¤?????¨¬2a¨®???¡ê??¡ã¡¤??T??\n");
					if((maze[coY][coX+1])==0)
					{
						crossChoice+=3;
						
					}	
				}
				if((maze[coY][coX]&0x04)==0)
				{
					if((maze[coY+1][coX])==0)
						crossChoice+=4;
						//printf("?¡¤?¨²¨®¨°¡Áa¡ê?3¡¥?¨°?a%d",dir); 
				}break;
		case 2:
				//printf("3¡¥?¨°?a%d¨º¡À¦Ì?2¨ª?¡¤????\n",dir);
				if((maze[coY][coX]&0x02)==0)
				{
					if((maze[coY][coX+1])==0)
					{
						crossChoice+=2; 
					}
				}
				if((maze[coY][coX]&0x04)==0)
				{
					if((maze[coY+1][coX])==0)
						crossChoice+=3;
						//printf("?¡¤?¨²2?¡Áa¡ê?3¡¥?¨°?a%d",dir); 
				}
				if((maze[coY][coX]&0x08)==0)
				{
					if((maze[coY][coX-1])==0)
						crossChoice+=4;
						//printf("?¡¤?¨²¨®¨°¡Áa¡ê?3¡¥?¨°?a%d",dir); 
				}break;
		case 3:
				//printf("3¡¥?¨°?a%d¨º¡À¦Ì?2¨ª?¡¤????\n",dir);
			 	if((maze[coY][coX]&0x04)==0)
				{
					if((maze[coY+1][coX])==0)
					crossChoice+=2; 
						//printf("?¡¤?¨²¡Á¨®¡Áa¡ê?3¡¥?¨°?a%d",dir);
				}
				if((maze[coY][coX]&0x08)==0)
				{
					if((maze[coY][coX-1])==0)
						crossChoice+=3;
						//printf("?¡¤?¨²2?¡Áa¡ê?3¡¥?¨°?a%d",dir); 
				}
				if((maze[coY][coX]&0x01)==0)
				{
					if((maze[coY-1][coX])==0)
						crossChoice+=4;
						//printf("?¡¤?¨²¨®¨°¡Áa¡ê?3¡¥?¨°?a%d",dir); 
				}break;
			default:
				crossChoice=15;
			break;
	}
	if(crossChoice==2||crossChoice==3||crossChoice==4)
	{
		//lightNumber(1,9);
	}
	if(crossChoice==3||crossChoice==7)
	{
		//printf("?¡¤?¨²2?¡Áa¡ê?3¡¥?¨°?a%d\n",dir);
		//crossChoice=0;
	}
	if(crossChoice==2||crossChoice==5||crossChoice==6||crossChoice==9)
	{
		turnLeft();
		//printf("?¡¤?¨²¡Á¨®¡Áa¡ê?3¡¥?¨°?a%d\n",dir); 
		//crossChoice=0;
	}
	if(crossChoice==4)
	{
		turnRight();
		//printf("?¡¤?¨²¨®¨°¡Áa¡ê?3¡¥?¨°?a%d\n",dir); 
		//crossChoice=0;
	}
	//printf("??2¨ª?¡¤?¨²¡ä|¦Ì?crossChoice?a%d\n",crossChoice);
	
	if(crossChoice==5||crossChoice==6||crossChoice==7||crossChoice==9)
	{
		//printf("2¨ª?¡¤?¨²?D??3?¡ä¨ª¡ê???¦Ì??¨´¡ä?¨¨???\n"); 
		top++;
		crossStack[top].cX=coX;
		crossStack[top].cY=coY;
		//printf("??2¨ª?¡¤?¨²??%d,%d??¡¤?¨¨????D,¡ä?¨º¡À??top?a%d\n",coX,coY,top);
	}
	crossChoice=0; 
	
}
void reBackCrossing()
{
	crossStack[top].cX=0;
	crossStack[top].cY=0;	
	top--;
}

void returnWay()
{
	//printf("?¨°?¨¤??¨¦¨²¡ê??a¨º????Y\n");
	turnBack();
	//printf("???Y¦Ì¨²¨°?2?¡ê?¡Áa¨ª¡¤\n");
	while((coX != crossStack[top].cX)||(coY != crossStack[top].cY))
	{
		
		switch(dir)
		{
		case 0:
				//printf("3¡¥?¨°?a%d¨º¡À¦Ì????Y\n",dir);
				if((maze[coY][coX]&0x10)>0)
				{
						//printf("???Y2?¡Áa¡ê?3¡¥?¨°?a%d",dir);
				}
				if((maze[coY][coX]&0x80)>0)
				{
					turnLeft();
					//printf("???Y¡Á¨®¡Áa¡ê?3¡¥?¨°?a%d",dir); 
				}
				if((maze[coY][coX]&0x20)>0)
				{
					turnRight();
					//printf("?¡¤?¨²¨®¨°¡Áa¡ê?3¡¥?¨°?a%d",dir); 
				}break;
		case 1:
				//printf("3¡¥?¨°?a%d¨º¡À¦Ì????Y\n",dir);
				if((maze[coY][coX]&0x20)>0)
				{
					//printf("???Y2?¡Áa¡ê?3¡¥?¨°?a%d",dir);
				}
				if((maze[coY][coX]&0x10)>0)
				{
					turnLeft();
					//printf("???Y¡Á¨®¡Áa¡ê?3¡¥?¨°?a%d",dir); 
				}
				if((maze[coY][coX]&0x40)>0)
				{
					turnRight();
					//printf("???Y¨®¨°¡Áa¡ê?3¡¥?¨°?a%d",dir); 
				}break;
		case 2:
				//printf("3¡¥?¨°?a%d¨º¡À¦Ì????Y\n",dir);
				if((maze[coY][coX]&0x40)>0)
				{
					//printf("???Y2?¡Áa¡ê?3¡¥?¨°?a%d",dir);
				}
				else if((maze[coY][coX]&0x20)>0)
				{
					turnLeft();
					//printf("???Y¡Á¨®¡Áa¡ê?3¡¥?¨°?a%d",dir); 
				}
				else if((maze[coY][coX]&0x80)>0)
				{
					turnRight();
					//printf("???Y¨®¨°¡Áa¡ê?3¡¥?¨°?a%d",dir); 
				}
				else 
				{
					//printf("3???¡ä¨ª?¨®¡ê?2??a¦Ì¨¤¨ª¨´??¡Áa¡ê?3¡¥?¨°?a%d",dir); 
				}break;
		case 3:
				//printf("3¡¥?¨°?a%d¨º¡À¦Ì????Y\n",dir);
				if((maze[coY][coX]&0x80)>0)
				{
						//printf("???Y2?¡Áa¡ê?3¡¥?¨°?a%d",dir);
				}
				if((maze[coY][coX]&0x40)>0)
				{
					turnLeft();
					//printf("???Y¡Á¨®¡Áa¡ê?3¡¥?¨°?a%d",dir); 
				}
				if((maze[coY][coX]&0x10)>0)
				{
					turnRight();
					//printf("?¡¤?¨²¨®¨°¡Áa¡ê?3¡¥?¨°?a%d",dir); 
				}break;
		}
		goForward();
		//printf("¦Ì¡À?¡ã???Yo¨®¡Á?¡À¨º??%d,%d??",coX,coY);
		if(coX>10||coY>10)															//1¨º???¨¬2aDD 
		{
			return; 
		} 
	}

	top--;
	
}
void comeBack()
	{
		top=1;
		returnWay();
		turnBack();
	} 
void findOneRoad2(unsigned char s)
{
	//printf("???%d?,?????2??,??????%d,%d?\n",s,coX,coY) ;	
	//printf("???????,,maze?[%X]????%d?\n",maze[coY][coX],maze[coY][coX]&0xf1) ;	
		if((maze[coY][coX]&0xf1)==0&&((maze[coY-1][coX]&0xf0)==0))
		{
			//printf("2[%d,%d]?????2\n",coX,coY);
			crossStack[step2].cX=coX;
			crossStack[step2].cY=coY-1;
			//maze[coY][coX]=(maze[coY][coX]|(s*16));
			step2++;
		}
		if((maze[coY][coX]&0xf2)==0&&((maze[coY][coX+1]&0xf0)==0))
		{
			//printf("2[%d,%d]?????2\n",coX,coY);
			crossStack[step2].cX=coX+1;
			crossStack[step2].cY=coY;
			//maze[coY][coX]=(maze[coY][coX]|(s*16));
			step2++;
		}
		if((maze[coY][coX]&0xf4)==0&&((maze[coY+1][coX]&0xf0)==0))
		{
			//printf("2[%d,%d]?????2\n",coX,coY);
			crossStack[step2].cX=coX;
			crossStack[step2].cY=coY+1;
			//maze[coY][coX]=(maze[coY][coX]|(s*16));
			step2++;
		}
		if((maze[coY][coX]&0xf8)==0&&((maze[coY][coX-1]&0xf0)==0))
		{
			//printf("2[%d,%d]?????2\n",coX,coY);
			crossStack[step2].cX=coX-1;
			crossStack[step2].cY=coY;
			//maze[coY][coX]=(maze[coY][coX]|(s*16));
			step2++;
		}
		maze[coY][coX]=(maze[coY][coX]|(s*16));
}
void findOneRoad1(unsigned char s)
{
	//printf("???%d?,?????1??,??????%d,%d?\n",s,coX,coY) ;
	if((maze[coY][coX]&0xf1)==0&&((maze[coY-1][coX]&0xf0)==0))
		{
			//printf("1[%d,%d]?????1\n",coX,coY);
			crossStack[step1].cX=coX;
			crossStack[step1].cY=coY-1;
			//maze[coY][coX]=(maze[coY][coX]|(s*16));
			step1++;
		}
		if((maze[coY][coX]&0xf2)==0&&((maze[coY][coX+1]&0xf0)==0))
		{
			//printf("1[%d,%d]?????1\n",coX,coY);
			crossStack[step1].cX=coX+1;
			crossStack[step1].cY=coY;
			//maze[coY][coX]=(maze[coY][coX]|(s*16));
			step1++;
		}
		if((maze[coY][coX]&0xf4)==0&&((maze[coY+1][coX]&0xf0)==0))
		{
			//printf("1[%d,%d]?????1\n",coX,coY);
			crossStack[step1].cX=coX;
			crossStack[step1].cY=coY+1;
			//maze[coY][coX]=(maze[coY][coX]|(s*16));
			step1++;
		}
		if((maze[coY][coX]&0xf8)==0&&((maze[coY][coX-1]&0xf0)==0))
		{
			//printf("1[%d,%d]?????1\n",coX,coY);
			crossStack[step1].cX=coX-1;
			crossStack[step1].cY=coY;
			//maze[coY][coX]=(maze[coY][coX]|(s*16));
			step1++;
		}
		maze[coY][coX]=(maze[coY][coX]|(s*16));
}

void setRoad()
{
	//printf("?????[%d,%d]\n",coX,coY);
	while(coX!=0||coY!=0)
	{
		//printf("????%X??????%d\n",maze[coY][coX-1],maze[coY][coX-1]&0xf8);
		if((maze[coY][coX-1]&0xf0)==(dir-1)*16&&(maze[coY][coX]&0x08)==0)
		{
			//printf("??????????[%d,%d]\n",coX-1,coY);
			dir--;
			maze[coY][coX-1]=(maze[coY][coX-1]&0x0f);
			maze[coY][coX-1]=(maze[coY][coX-1]|0x20);
			coX--; 
			continue;
		}
		//printf("????%X??????%d\n",maze[coY-1][coX],maze[coY-1][coX]&0xf1);
		if((maze[coY-1][coX]&0xf0)==(dir-1)*16&&(maze[coY][coX]&0x01)==0)
		{
			//printf("??????????[%d,%d]\n",coX,coY-1);
			maze[coY-1][coX]=(maze[coY-1][coX]&0x0f);
			maze[coY-1][coX]=(maze[coY-1][coX]|0x40);
			coY--;
			dir--;
		}
	}
	dir=3;
	top=1;
	crossStack[1].cX=7;
	crossStack[1].cY=7;
	//printf("****??????,????****\n   ?????%d,%d?,dir?%d,top?%d",coX,coY,dir,top);
}
void findRoad()
{
	//printf("*****????*****\n[%d,%d]\n",coX,coY);
	findOneRoad1(1);
	
	for(dir=2;dir<15;dir++)
	{
		
		if(step1==0&&step2==15)
		{
			//printf("????,?????%d,%d?\n",coX,coY); 
		}
		while(step1>0)
		{
			step1--;
			coX=crossStack[step1].cX;
			coY=crossStack[step1].cY;
			//printf("????111,???%d,%d?,?????%d?\n",coX,coY,step1); 
			findOneRoad2(dir);
			
		}
		dir++;
		while(step2>15)
		{
			step2--;
			coX=crossStack[step2].cX;
			coY=crossStack[step2].cY;
			//printf("????222,???%d,%d?,?????%d?\n",coX,coY,step2); 
			findOneRoad1(dir);	
		}
		if(coX==7&&coY==7)
		{
			
			
			//printf("?????%d,%d?",coX,coY);
			break; 
		}
	
	}
	//printf("\n*****?????????,????,??????*****\n");
	setRoad();
	returnWay(); 
	
}
void main()
{   
	int i=3,choice =0,j=0,k=0;
	dir=1;
	//printf("¡¤?¨¨?D?3¦Ì¡ê??¨¬2a??¨¬?.\n");
	top=1;
	crossStack[top].cX=0;
	crossStack[top].cY=0;
	
	for(i=1;i<77;i++)
	{
		//printf("****¦Ì¨²%d¡ä??-?¡¤****\n???¨²¡Á?¡À¨º?a??%d,%d??,3¡¥?¨°¡êo%d,??¨º¦Ì??1??a%X\n",i,coX,coY,dir,realMaze[coY][coX]);
		lightNumber(1,coY);
		lightNumber(2,coX);
		if(coX>8)
			lightNumber(2,9);
		choice = checkWall(&maze[coY][coX]); 
		//printf("choice?a%d\n",choice); 
		switch (choice)
		{
		case 0:
				//printf("?¡ã¦Ì??¨¤?¡¤\n"); 
				returnWay();
				break;
		case 1:
				//printf("??¨¬??¨¬2a?¨¢1??a¨°?¡ê?¨°¨¦??3?¡ä¨ª\n"); 
				break;
		case 2:
		case 3:
		case 4:
				adjustDir(choice);
				//printf("?¡ã¦Ì?¦Ì£¤¨°??¡¤\n");
				goForward();
				//printf("     ?¡ã??¡ê?\n");
				break;
		case 5:
		case 6:
		case 7:
		case 9:
				//printf("D??¡é?a%X\n",maze[coY][coX]);
			 	recordCrossing();
			 	goForward();
			 	//printf("     ?¡ã??¡ê?\n");
				break;
				
		default:
			break;
		}
	}
	top=1;
	returnWay(); 
	for(j=0;j<8;j++)
		{
			for(k=0;k<8;k++)
			{
				maze[j][k]=(maze[j][k]&0x0f);
			}
		}
		maze[0][0]=(0x0d);
	dir=0;
	comeBack();
	findRoad();
}