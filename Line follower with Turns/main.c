#include"sra.c"

int mpos=0,prempos=0;
unsigned int opt = 290;//270
float kp = 10;//13
float kd = 0.00;//1.2
float pid;
float p = 0,d = 0;
int flag_6 = 0, flag_7 = 0, left = 0, right = 0, check = 0, dir_count = 0, start = 0;
int LR = 0, LF = 0, RF = 0, LFR = 0, LT = 0, Plus = 0, End = 0, flag = 0, flag_L = 0, flag_R = 0, flag_F = 0, flag_END = 0;
long int len_count = 0;
char dir[4]={'N','E','S','W'}, Dir = 'N';

int i = 0, j = 0,k = 0;
int x_map = 18, y_map = 9, x_end = 0, y_end = 0;
char map[19][19][2];

void update_dir()
{
	if(dir_count==-1)
	{
		dir_count=3;
	}
	else if(dir_count==4)
	{
		dir_count=0;
	}
	else if(dir_count==5)
	{
		dir_count=1;
	}
	else if(dir_count==-2)
	{
		dir_count=2;
	}
	Dir=dir[dir_count];
}

void start_map()
{
	for(i=0; i<19; i++)
{
	for(j=0; j<19; j++)
	{
		for(k=0; k<2; k++)
		{
			map[i][j][k]='\0';//0
		}
	}
}
map[18][9][0]='a';//2
}

void update_map()
{
	if(len_count>800&&len_count<3200)
	{
		if(Dir=='N')
		{
			map[x_map-1][y_map][0]='(';
			map[x_map-2][y_map][0]='a';
			if(map[x_map-1][y_map][1]=='\0')
			{
				if(start!=0)
				{
					map[x_map-1][y_map][1]='a';
				}
				else if(start==0)
				{
					map[x_map-1][y_map][1]='b';start=1;
				}
			}
			else if(map[x_map-1][y_map][1]=='a')
			{
				map[x_map-1][y_map][1]='b';
			}
			x_map-=2;
		}
		else if(Dir=='E')
		{
			map[x_map][y_map+1][0]='(';
			map[x_map][y_map+2][0]='a';
			if(map[x_map][y_map+1][1]=='\0')
			{
				map[x_map][y_map+1][1]='a';
			}
			else if(map[x_map][y_map+1][1]=='a')
			{
				map[x_map][y_map+1][1]='b';
			}
			y_map+=2;
		}
		else if(Dir=='S')
		{
			map[x_map+1][y_map][0]='(';
			map[x_map+2][y_map][0]='a';
			if(map[x_map+1][y_map][1]=='\0')
			{
				map[x_map+1][y_map][1]='a';
			}
			else if(map[x_map+1][y_map][1]=='a')
			{
				map[x_map+1][y_map][1]='b';
			}
			x_map+=2;
		}
		else if(Dir=='W')
		{
			map[x_map][y_map-1][0]='(';
			map[x_map][y_map-2][0]='a';
			if(map[x_map][y_map-1][1]=='\0')
			{
				map[x_map][y_map-1][1]='a';
			}
			else if(map[x_map][y_map-1][1]=='a')
			{
				map[x_map][y_map-1][1]='b';
			}
			y_map-=2;
		}
		lcd_clear();lcd_write_string_xy(15,1,"3");
	}
	else if(len_count>3200&&len_count<5200)
	{
		if(Dir=='N')
		{
			map[x_map-1][y_map][0]='(';
			map[x_map-2][y_map][0]='a';
			if(map[x_map-1][y_map][1]=='\0')
			{
				if(start!=0)
				{
					map[x_map-1][y_map][1]='a';
				}
				else if(start==0)
				{
					map[x_map-1][y_map][1]='b';
				}
			}
			else if(map[x_map-1][y_map][1]=='a')
			{
				map[x_map-1][y_map][1]='b';
			}
			x_map-=2;
			map[x_map-1][y_map][0]='(';
			map[x_map-2][y_map][0]='a';
			if(map[x_map-1][y_map][1]=='\0')
			{
				if(start!=0)
				{
					map[x_map-1][y_map][1]='a';
				}
				else if(start==0)
				{
					map[x_map-1][y_map][1]='b';start=1;
				}
			}
			else if(map[x_map-1][y_map][1]=='a')
			{
				map[x_map-1][y_map][1]='b';
			}
			x_map-=2;
		}
		else if(Dir=='E')
		{
			map[x_map][y_map+1][0]='(';
			map[x_map][y_map+2][0]='a';
			if(map[x_map][y_map+1][1]=='\0')
			{
				map[x_map][y_map+1][1]='a';
			}
			else if(map[x_map][y_map+1][1]=='a')
			{
				map[x_map][y_map+1][1]='b';
			}
			y_map+=2;
			map[x_map][y_map+1][0]='(';
			map[x_map][y_map+2][0]='a';
			if(map[x_map][y_map+1][1]=='\0')
			{
				map[x_map][y_map+1][1]='a';
			}
			else if(map[x_map][y_map+1][1]=='a')
			{
				map[x_map][y_map+1][1]='b';
			}
			y_map+=2;
		}
		else if(Dir=='S')
		{
			map[x_map+1][y_map][0]='(';
			map[x_map+2][y_map][0]='a';
			if(map[x_map+1][y_map][1]=='\0')
			{
				map[x_map+1][y_map][1]='a';
			}
			else if(map[x_map+1][y_map][1]=='a')
			{
				map[x_map+1][y_map][1]='b';
			}
			x_map+=2;
			map[x_map+1][y_map][0]='(';
			map[x_map+2][y_map][0]='a';
			if(map[x_map+1][y_map][1]=='\0')
			{
				map[x_map+1][y_map][1]='a';
			}
			else if(map[x_map+1][y_map][1]=='a')
			{
				map[x_map+1][y_map][1]='b';
			}
			x_map+=2;
		}
		else if(Dir=='W')
		{
			map[x_map][y_map-1][0]='(';
			map[x_map][y_map-2][0]='a';
			if(map[x_map][y_map-1][1]=='\0')
			{
				map[x_map][y_map-1][1]='a';
			}
			else if(map[x_map][y_map-1][1]=='a')
			{
				map[x_map][y_map-1][1]='b';
			}
			y_map-=2;
			map[x_map][y_map-1][0]='(';
			map[x_map][y_map-2][0]='a';
			if(map[x_map][y_map-1][1]=='\0')
			{
				map[x_map][y_map-1][1]='a';
			}
			else if(map[x_map][y_map-1][1]=='a')
			{
				map[x_map][y_map-1][1]='b';
			}
			y_map-=2;
		}
		lcd_clear();lcd_write_string_xy(15,1,"6");
	}
}

void line_track(void)
{
	check_sensors();
	bot_forward();
	switch(sensorbyte)
	{
		case 0b0110:mpos = 0; break;//sensor numbering starts from right
		
		case 0b0100:mpos = 1; break;
		
		case 0b0010:mpos = -1; break;
		
		case 0b1100:mpos = 2; break;
		
		case 0b0011:mpos = -2; break;
		
		case 0b1110:if(flag_7==1){mpos = -6;}else mpos = 4; break;
		
		case 0b0111:if(flag_6==1){mpos = 6;}else mpos = -4; break;
		
		case 0b1000:mpos = 6; break;
		
		case 0b0001:mpos = -6; break;
		
		case 0b0000:if(bit_is_set(PIND,6)&&bit_is_set(PINA,4)&&bit_is_set(PINA,5)&&bit_is_set(PINA,6)&&bit_is_set(PINA,7)&&bit_is_set(PIND,7)){D_U_turn();} break;
		
		default:mpos=0; break;
	}
	if(check==0)
	{check_turns();}

	p = mpos * kp;
	d = kd * (mpos-prempos);
	pid = p + d;
	int motor1_value = opt + pid;//left motor is m1
	int motor2_value = opt - pid;
	set_pwm1a(motor1_value);
	set_pwm1b(motor2_value);
	prempos=mpos;len_count++;
	delay_microsec(10);
}

void check_random()
{
	switch(Dir)
	{
		case 'N':
		if(LF==1)
		{
			if((map[x_map][y_map-1][1]=='a'&&map[x_map-1][y_map][1]=='\0')||(map[x_map][y_map-1][1]=='b'&&map[x_map-1][y_map][1]=='a'))
			{
				flag=1;flag_F=1;
			}
			else if((map[x_map][y_map-1][1]=='\0'&&map[x_map-1][y_map][1]=='a')||(map[x_map][y_map-1][1]=='a'&&map[x_map-1][y_map][1]=='b'))
			{
				flag=1;flag_L=1;
			}
			else if(map[x_map][y_map-1][1]=='b'&&map[x_map-1][y_map][1]=='b')
			{
				flag=2;
			}
		}
		else if(RF==1)
		{
			if((map[x_map][y_map+1][1]=='a'&&map[x_map-1][y_map][1]=='\0')||(map[x_map][y_map+1][1]=='b'&&map[x_map-1][y_map][1]=='a'))
			{
				flag=1;flag_F=1;
			}
			else if((map[x_map][y_map+1][1]=='\0'&&map[x_map-1][y_map][1]=='a')||(map[x_map][y_map+1][1]=='a'&&map[x_map-1][y_map][1]=='b'))
			{
				flag=1;flag_R=1;
			}
			else if(map[x_map][y_map+1][1]=='b'&&map[x_map-1][y_map][1]=='b')
			{
				flag=2;
			}
		}
		else if(LR==1)
		{
			if((map[x_map][y_map-1][1]=='a'&&map[x_map][y_map+1][1]=='\0')||(map[x_map][y_map-1][1]=='b'&&map[x_map][y_map+1][1]=='a'))
			{
				flag=1;flag_R=1;
			}
			else if((map[x_map][y_map-1][1]=='\0'&&map[x_map][y_map+1][1]=='a')||(map[x_map][y_map-1][1]=='a'&&map[x_map][y_map+1][1]=='b'))
			{
				flag=1;flag_L=1;
			}
			else if(map[x_map][y_map-1][1]=='b'&&map[x_map][y_map+1][1]=='b')
			{
				flag=2;
			}
		}
		else if(LFR==1)
		{
			if((map[x_map][y_map-1][1]=='a'&&map[x_map-1][y_map][1]=='\0'&&map[x_map][y_map+1][1]=='\0')||(map[x_map][y_map-1][1]=='b'&&map[x_map-1][y_map][1]=='a'&&map[x_map][y_map+1][1]=='a'))
			{
				flag=0;LFR=0;RF=1;
			}
			else if((map[x_map][y_map-1][1]=='\0'&&map[x_map-1][y_map][1]=='a'&&map[x_map][y_map+1][1]=='\0')||(map[x_map][y_map-1][1]=='a'&&map[x_map-1][y_map][1]=='b'&&map[x_map][y_map+1][1]=='a'))
			{
				flag=0;LFR=0;LR=1;
			}
			else if((map[x_map][y_map-1][1]=='\0'&&map[x_map-1][y_map][1]=='\0'&&map[x_map][y_map+1][1]=='a')||(map[x_map][y_map-1][1]=='a'&&map[x_map-1][y_map][1]=='a'&&map[x_map][y_map+1][1]=='b'))
			{
				flag=0;LFR=0;LF=1;
			}
			
			else if((map[x_map][y_map-1][1]=='\0'&&map[x_map-1][y_map][1]=='a'&&map[x_map][y_map+1][1]=='a')||(map[x_map][y_map-1][1]=='a'&&map[x_map-1][y_map][1]=='b'&&map[x_map][y_map+1][1]=='b'))
			{
				flag=1;flag_L=1;
			}
			else if((map[x_map][y_map-1][1]=='a'&&map[x_map-1][y_map][1]=='\0'&&map[x_map][y_map+1][1]=='a')||(map[x_map][y_map-1][1]=='b'&&map[x_map-1][y_map][1]=='a'&&map[x_map][y_map+1][1]=='b'))
			{
				flag=1;flag_F=1;
			}
			else if((map[x_map][y_map-1][1]=='a'&&map[x_map-1][y_map][1]=='a'&&map[x_map][y_map+1][1]=='\0')||(map[x_map][y_map-1][1]=='b'&&map[x_map-1][y_map][1]=='b'&&map[x_map][y_map+1][1]=='a'))
			{
				flag=1;flag_R=1;
			}
			
			else if(map[x_map][y_map-1][1]=='b'&&map[x_map-1][y_map][1]=='b'&&map[x_map][y_map+1][1]=='b')
			{
				flag=2;
			}
		}break;
		
		case 'E':
		if(LF==1)
		{
			if((map[x_map-1][y_map][1]=='a'&&map[x_map][y_map+1][1]=='\0')||(map[x_map-1][y_map][1]=='b'&&map[x_map][y_map+1][1]=='a'))
			{
				flag=1;flag_F=1;
			}
			else if((map[x_map-1][y_map][1]=='\0'&&map[x_map][y_map+1][1]=='a')||(map[x_map-1][y_map][1]=='a'&&map[x_map][y_map+1][1]=='b'))
			{
				flag=1;flag_L=1;
			}
			else if(map[x_map-1][y_map][1]=='b'&&map[x_map][y_map+1][1]=='b')
			{
				flag=2;
			}
		}
		else if(RF==1)
		{
			if((map[x_map+1][y_map][1]=='a'&&map[x_map][y_map+1][1]=='\0')||(map[x_map+1][y_map][1]=='b'&&map[x_map][y_map+1][1]=='a'))
			{
				flag=1;flag_F=1;
			}
			else if((map[x_map+1][y_map][1]=='\0'&&map[x_map][y_map+1][1]=='a')||(map[x_map+1][y_map][1]=='a'&&map[x_map][y_map+1][1]=='b'))
			{
				flag=1;flag_R=1;
			}
			else if(map[x_map+1][y_map][1]=='b'&&map[x_map][y_map+1][1]=='b')
			{
				flag=2;
			}
		}
		else if(LR==1)
		{
			if((map[x_map-1][y_map][1]=='a'&&map[x_map+1][y_map][1]=='\0')||(map[x_map-1][y_map][1]=='b'&&map[x_map+1][y_map][1]=='a'))
			{
				flag=1;flag_R=1;
			}
			else if((map[x_map-1][y_map][1]=='\0'&&map[x_map+1][y_map][1]=='a')||(map[x_map-1][y_map][1]=='a'&&map[x_map+1][y_map][1]=='b'))
			{
				flag=1;flag_L=1;
			}
			else if(map[x_map-1][y_map][1]=='b'&&map[x_map+1][y_map][1]=='b')
			{
				flag=2;
			}
		}
		else if(LFR==1)
		{
			if((map[x_map-1][y_map][1]=='a'&&map[x_map][y_map+1][1]=='\0'&&map[x_map+1][y_map][1]=='\0')||(map[x_map-1][y_map][1]=='b'&&map[x_map][y_map+1][1]=='a'&&map[x_map+1][y_map][1]=='a'))
			{
				flag=0;LFR=0;RF=1;
			}
			else if((map[x_map-1][y_map][1]=='\0'&&map[x_map][y_map+1][1]=='a'&&map[x_map+1][y_map][1]=='\0')||(map[x_map-1][y_map][1]=='a'&&map[x_map][y_map+1][1]=='b'&&map[x_map+1][y_map][1]=='a'))
			{
				flag=0;LFR=0;LR=1;
			}
			else if((map[x_map-1][y_map][1]=='\0'&&map[x_map][y_map+1][1]=='\0'&&map[x_map+1][y_map][1]=='a')||(map[x_map-1][y_map][1]=='a'&&map[x_map][y_map+1][1]=='a'&&map[x_map+1][y_map][1]=='b'))
			{
				flag=0;LFR=0;LF=1;
			}
			
			else if((map[x_map-1][y_map][1]=='\0'&&map[x_map][y_map+1][1]=='a'&&map[x_map+1][y_map][1]=='a')||(map[x_map-1][y_map][1]=='a'&&map[x_map][y_map+1][1]=='b'&&map[x_map+1][y_map][1]=='b'))
			{
				flag=1;flag_L=1;
			}
			else if((map[x_map-1][y_map][1]=='a'&&map[x_map][y_map+1][1]=='\0'&&map[x_map+1][y_map][1]=='a')||(map[x_map-1][y_map][1]=='b'&&map[x_map][y_map+1][1]=='a'&&map[x_map+1][y_map][1]=='b'))
			{
				flag=1;flag_F=1;
			}
			else if((map[x_map-1][y_map][1]=='a'&&map[x_map][y_map+1][1]=='a'&&map[x_map+1][y_map][1]=='\0')||(map[x_map-1][y_map][1]=='b'&&map[x_map][y_map+1][1]=='b'&&map[x_map+1][y_map][1]=='a'))
			{
				flag=1;flag_R=1;
			}
			
			else if(map[x_map-1][y_map][1]=='b'&&map[x_map][y_map+1][1]=='b'&&map[x_map+1][y_map][1]=='b')
			{
				flag=2;
			}
		}break;
		
		case 'S':
		if(LF==1)
		{
			if((map[x_map][y_map+1][1]=='a'&&map[x_map+1][y_map][1]=='\0')||(map[x_map][y_map+1][1]=='b'&&map[x_map+1][y_map][1]=='a'))
			{
				flag=1;flag_F=1;
			}
			else if((map[x_map][y_map+1][1]=='\0'&&map[x_map+1][y_map][1]=='a')||(map[x_map][y_map+1][1]=='a'&&map[x_map+1][y_map][1]=='b'))
			{
				flag=1;flag_L=1;
			}
			else if(map[x_map][y_map+1][1]=='b'&&map[x_map+1][y_map][1]=='b')
			{
				flag=2;
			}
		}
		else if(RF==1)
		{
			if((map[x_map][y_map-1][1]=='a'&&map[x_map+1][y_map][1]=='\0')||(map[x_map][y_map-1][1]=='b'&&map[x_map+1][y_map][1]=='a'))
			{
				flag=1;flag_F=1;
			}
			else if((map[x_map][y_map-1][1]=='\0'&&map[x_map+1][y_map][1]=='a')||(map[x_map][y_map-1][1]=='a'&&map[x_map+1][y_map][1]=='b'))
			{
				flag=1;flag_R=1;
			}
			else if(map[x_map][y_map-1][1]=='b'&&map[x_map+1][y_map][1]=='b')
			{
				flag=2;
			}
		}
		else if(LR==1)
		{
			if((map[x_map][y_map+1][1]=='a'&&map[x_map][y_map-1][1]=='\0')||(map[x_map][y_map+1][1]=='b'&&map[x_map][y_map-1][1]=='a'))
			{
				flag=1;flag_R=1;
			}
			else if((map[x_map][y_map+1][1]=='\0'&&map[x_map][y_map-1][1]=='a')||(map[x_map][y_map+1][1]=='a'&&map[x_map][y_map-1][1]=='b'))
			{
				flag=1;flag_L=1;
			}
			else if(map[x_map][y_map+1][1]=='b'&&map[x_map][y_map-1][1]=='b')
			{
				flag=2;
			}
		}
		else if(LFR==1)
		{
			if((map[x_map][y_map+1][1]=='a'&&map[x_map+1][y_map][1]=='\0'&&map[x_map][y_map-1][1]=='\0')||(map[x_map][y_map+1][1]=='b'&&map[x_map+1][y_map][1]=='a'&&map[x_map][y_map-1][1]=='a'))
			{
				flag=0;LFR=0;RF=1;
			}
			else if((map[x_map][y_map+1][1]=='\0'&&map[x_map+1][y_map][1]=='a'&&map[x_map][y_map-1][1]=='\0')||(map[x_map][y_map+1][1]=='a'&&map[x_map+1][y_map][1]=='b'&&map[x_map][y_map-1][1]=='a'))
			{
				flag=0;LFR=0;LR=1;
			}
			else if((map[x_map][y_map+1][1]=='\0'&&map[x_map+1][y_map][1]=='\0'&&map[x_map][y_map-1][1]=='a')||(map[x_map][y_map+1][1]=='a'&&map[x_map+1][y_map][1]=='a'&&map[x_map][y_map-1][1]=='b'))
			{
				flag=0;LFR=0;LF=1;
			}
			
			else if((map[x_map][y_map+1][1]=='\0'&&map[x_map+1][y_map][1]=='a'&&map[x_map][y_map-1][1]=='a')||(map[x_map][y_map+1][1]=='a'&&map[x_map+1][y_map][1]=='b'&&map[x_map][y_map-1][1]=='b'))
			{
				flag=1;flag_L=1;
			}
			else if((map[x_map][y_map+1][1]=='a'&&map[x_map+1][y_map][1]=='\0'&&map[x_map][y_map-1][1]=='a')||(map[x_map][y_map+1][1]=='b'&&map[x_map+1][y_map][1]=='a'&&map[x_map][y_map-1][1]=='b'))
			{
				flag=1;flag_F=1;
			}
			else if((map[x_map][y_map+1][1]=='a'&&map[x_map+1][y_map][1]=='a'&&map[x_map][y_map-1][1]=='\0')||(map[x_map][y_map+1][1]=='b'&&map[x_map+1][y_map][1]=='b'&&map[x_map][y_map-1][1]=='a'))
			{
				flag=1;flag_R=1;
			}
			
			else if(map[x_map][y_map+1][1]=='b'&&map[x_map+1][y_map][1]=='b'&&map[x_map][y_map-1][1]=='b')
			{
				flag=2;
			}
		}break;
		
		case 'W':
		if(LF==1)
		{
			if((map[x_map+1][y_map][1]=='a'&&map[x_map][y_map-1][1]=='\0')||(map[x_map+1][y_map][1]=='b'&&map[x_map][y_map-1][1]=='a'))
			{
				flag=1;flag_F=1;
			}
			else if((map[x_map+1][y_map][1]=='\0'&&map[x_map][y_map-1][1]=='a')||(map[x_map+1][y_map][1]=='a'&&map[x_map][y_map-1][1]=='b'))
			{
				flag=1;flag_L=1;
			}
			else if(map[x_map+1][y_map][1]=='b'&&map[x_map][y_map-1][1]=='b')
			{
				flag=2;
			}
		}
		else if(RF==1)
		{
			if((map[x_map-1][y_map][1]=='a'&&map[x_map][y_map-1][1]=='\0')||(map[x_map-1][y_map][1]=='b'&&map[x_map][y_map-1][1]=='a'))
			{
				flag=1;flag_F=1;
			}
			else if((map[x_map-1][y_map][1]=='\0'&&map[x_map][y_map-1][1]=='a')||(map[x_map-1][y_map][1]=='a'&&map[x_map][y_map-1][1]=='b'))
			{
				flag=1;flag_R=1;
			}
			else if(map[x_map-1][y_map][1]=='b'&&map[x_map][y_map-1][1]=='b')
			{
				flag=2;
			}
		}
		else if(LR==1)
		{
			if((map[x_map+1][y_map][1]=='a'&&map[x_map-1][y_map][1]=='\0')||(map[x_map+1][y_map][1]=='b'&&map[x_map-1][y_map][1]=='a'))
			{
				flag=1;flag_R=1;
			}
			else if((map[x_map+1][y_map][1]=='\0'&&map[x_map-1][y_map][1]=='a')||(map[x_map+1][y_map][1]=='a'&&map[x_map-1][y_map][1]=='b'))
			{
				flag=1;flag_L=1;
			}
			else if(map[x_map+1][y_map][1]=='b'&&map[x_map-1][y_map][1]=='b')
			{
				flag=2;
			}
		}
		else if(LFR==1)
		{
			if((map[x_map+1][y_map][1]=='a'&&map[x_map][y_map-1][1]=='\0'&&map[x_map-1][y_map][1]=='\0')||(map[x_map+1][y_map][1]=='b'&&map[x_map][y_map-1][1]=='a'&&map[x_map-1][y_map][1]=='a'))
			{
				flag=0;LFR=0;RF=1;
			}
			else if((map[x_map+1][y_map][1]=='\0'&&map[x_map][y_map-1][1]=='a'&&map[x_map-1][y_map][1]=='\0')||(map[x_map+1][y_map][1]=='a'&&map[x_map][y_map-1][1]=='b'&&map[x_map-1][y_map][1]=='a'))
			{
				flag=0;LFR=0;LR=1;
			}
			else if((map[x_map+1][y_map][1]=='\0'&&map[x_map][y_map-1][1]=='\0'&&map[x_map-1][y_map][1]=='a')||(map[x_map+1][y_map][1]=='a'&&map[x_map][y_map-1][1]=='a'&&map[x_map-1][y_map][1]=='b'))
			{
				flag=0;LFR=0;LF=1;
			}
			
			else if((map[x_map+1][y_map][1]=='\0'&&map[x_map][y_map-1][1]=='a'&&map[x_map-1][y_map][1]=='a')||(map[x_map+1][y_map][1]=='a'&&map[x_map][y_map-1][1]=='b'&&map[x_map-1][y_map][1]=='b'))
			{
				flag=1;flag_L=1;
			}
			else if((map[x_map+1][y_map][1]=='a'&&map[x_map][y_map-1][1]=='\0'&&map[x_map-1][y_map][1]=='a')||(map[x_map+1][y_map][1]=='b'&&map[x_map][y_map-1][1]=='a'&&map[x_map-1][y_map][1]=='b'))
			{
				flag=1;flag_F=1;
			}
			else if((map[x_map+1][y_map][1]=='a'&&map[x_map][y_map-1][1]=='a'&&map[x_map-1][y_map][1]=='\0')||(map[x_map+1][y_map][1]=='b'&&map[x_map][y_map-1][1]=='b'&&map[x_map-1][y_map][1]=='a'))
			{
				flag=1;flag_R=1;
			}
			
			else if(map[x_map+1][y_map][1]=='b'&&map[x_map][y_map-1][1]=='b'&&map[x_map-1][y_map][1]=='b')
			{
				flag=2;
			}
		}break;
		
		default:break;
	}
}

void random_turns()
{
	check_random();
	switch(flag)
	{
		case 0:
		{
			flag_END=0;
			if(LF==1)
			{
				if(len_count%2==0)
				{
					left=1;
				}
				else if(len_count%2==1)
				{
					opt=290;len_count=0;lcd_clear();
				}
			}
			else if(RF==1)
			{
				if(len_count%2==0)
				{
					right=1;
				}
				else if(len_count%2==1)
				{
					opt=290;len_count=0;lcd_clear();
				}
			}
			else if(LR==1)
			{
				if(len_count%2==0)
				{
					left=1;
				}
				else if(len_count%2==1)
				{
					right=1;
				}
			}
			else if(LFR==1)
			{
				if(len_count%2==0)
				{
					if((len_count/2)%2==1)
					{
						left=1;
					}
				}
				else if(len_count%2==0)
				{
					if((len_count/2)%2==0)
					{
						right=1;
					}
				}
				else if(len_count%2==1)
				{
					opt=290;len_count=0;lcd_clear();
				}
			}
		}break;
		case 1:
		{
			flag_END=0;
			if(flag_L==1)
			{
				left=1;
			}
			else if(flag_R==1)
			{
				right=1;
			}
			else if(flag_F==1)
			{
				opt=290;len_count=0;lcd_clear();
			}
		}break;
		case 2:
		{
			flag_END++;
			if(flag_END==2)
			{
				bot_brake();set_pwm1a(400);set_pwm1b(400);lcd_clear();lcd_write_string_xy(6,0,"FUCK");delay_sec(1000);
			}
			else if(LF==1)
			{
				R_U_turn();
			}
			else if(RF==1)
			{
				L_U_turn();
			}
			else if(LR==1)
			{
				T_U_turn();
			}
			else if(LFR==1)
			{
				P_U_turn();
			}
		}break;
		default:break;
	}
	LR = 0; LF = 0; RF = 0; LFR = 0; flag = 0; flag_L = 0; flag_F = 0; flag_R = 0;
}

void check_turns()
{
	check=1;
	if(bit_is_clear(PIND,6)||bit_is_clear(PIND,7))
	{
		update_map();
		if(bit_is_clear(PINA,5)||bit_is_clear(PINA,6))
		{
			if(bit_is_clear(PIND,6)&&bit_is_set(PIND,7))
			{
				opt=245;
				flag_6=1;
				while(bit_is_clear(PIND,6))
				{
					line_track();
					if(bit_is_clear(PIND,7)){Plus=1;break;}
				}
				if(Plus==1)
				{
					while(bit_is_set(PINA,4)||bit_is_set(PINA,7))
					{
						bot_forward();set_pwm1a(240);set_pwm1b(245);
						if(bit_is_set(PIND,6)&&bit_is_set(PIND,7)){End=-1;Plus=0;break;}
					}
					End++;Plus++;
				}
				if(Plus==0)
				{
					LF=1;random_turns();
					flag_6=0;
					if(bit_is_set(PINA,4))
					{while(bit_is_set(PINA,4))
					{line_track();}}
					check_turn_sensors();
					take_turns();
				}
				else if(Plus==1)
				{
					while(bit_is_set(PINA,4)||bit_is_set(PINA,7))
					{
						bot_forward();
						set_pwm1a(245);
						set_pwm1b(245);
					}
					LFR=1;LT=1;
					random_turns();
					if(left==1){left_L();}
					else if(right==1){right_L();}
				}
				else if(End!=0){lcd_clear();lcd_write_string_xy(4,0,"END ZONE");bot_brake();set_pwm1a(400);set_pwm1b(400);flick();delay_sec(1);E_U_turn();}
			}
			
			else if(bit_is_set(PIND,6)&&bit_is_clear(PIND,7))
			{
				opt=245;
				flag_7=1;
				while(bit_is_clear(PIND,7))
				{
					line_track();
					if(bit_is_clear(PIND,6)){Plus=1;break;}
				}
				if(Plus==1)
				{
					while(bit_is_set(PINA,4)||bit_is_set(PINA,7))
					{
						bot_forward();set_pwm1a(245);set_pwm1b(240);
						if(bit_is_set(PIND,6)&&bit_is_set(PIND,7)){End=-1;Plus=0;break;}
					}
					End++;Plus++;
				}
				if(Plus==0)
				{
					RF=1;
					random_turns();
					flag_7=0;
					if(bit_is_set(PINA,7))
					{while(bit_is_set(PINA,7))
					{line_track();}}
					check_turn_sensors();
					take_turns();
				}
				else if(Plus==1)
				{
					while(bit_is_set(PINA,4)||bit_is_set(PINA,7))//&&
					{
						bot_forward();
						set_pwm1a(245);
						set_pwm1b(245);
					}
					LFR=1;LT=1;
					random_turns();
					if(left==1){left_L();}
					else if(right==1){right_L();}
				}
				else if(End!=0){lcd_clear();lcd_write_string_xy(4,0,"END ZONE");bot_brake();set_pwm1a(400);set_pwm1b(400);flick();delay_sec(1);E_U_turn();}
			}
		}
		else
		{
			if(bit_is_clear(PIND,6)&&bit_is_clear(PIND,7))
			{
				NO_FRO_T:
				while(bit_is_set(PINA,4)||bit_is_set(PINA,7))//&&
				{
					bot_forward();
					set_pwm1a(245);
					set_pwm1b(245);
				}
				LR=1;LT=1;
				random_turns();
				if(left==1){left_L();}
				else if(right==1){right_L();}
			}
			
			else if(bit_is_clear(PIND,6)&&bit_is_set(PIND,7))
			{
				while(bit_is_clear(PIND,6))
				{
					bot_forward();
					set_pwm1a(245);//6
					set_pwm1b(250);//6
					if(bit_is_clear(PIND,7)){goto NO_FRO_T;}
				}LT=1;
				while(bit_is_set(PINA,4))
				{
					bot_forward();
					set_pwm1a(245);
					set_pwm1b(245);
				}
				left_L();
			}
			
			else if(bit_is_set(PIND,6)&&bit_is_clear(PIND,7))
			{
				while(bit_is_clear(PIND,7))
				{
					bot_forward();
					set_pwm1a(250);
					set_pwm1b(245);
					if(bit_is_clear(PIND,6)){goto NO_FRO_T;}
				}LT=1;
				while(bit_is_set(PINA,7))
				{
					bot_forward();
					set_pwm1a(245);
					set_pwm1b(245);
				}
				right_L();
			}
		}
	}
	check=0;
}

void take_turns()
{
	switch(sensorbyte_T)
	{
		case 0b1110:if(left==1){left_L();}break;
		case 0b1100:if(left==1){left_L();}break;
		case 0b1010:if(left==1){left_L();}break;
		case 0b1000:if(left==1){left_L();}break;
		case 0b0111:if(right==1){right_L();}break;
		case 0b0101:if(right==1){right_L();}break;
		case 0b0011:if(right==1){right_L();}break;
		case 0b0001:if(right==1){right_L();}break;
		default:break;
	}
}

void left_L()
{
	lcd_write_string_xy(6,1,"LEFT");
	bot_brake();
	set_pwm1a(400);set_pwm1b(400);
	if(LT==1)
	{while(bit_is_clear(PINA,4))
	{
		bot_forward();
		set_pwm1a(245);
		set_pwm1b(245);
	}}
	while(bit_is_set(PIND,6))
	{
		bot_spot_left();
		set_pwm1a(270);
		set_pwm1b(270);
	}
	while(bit_is_set(PINA,5))
	{
		set_pwm1a(260);
		set_pwm1b(260);
	}
	while(bit_is_set(PINA,6))
	{
		set_pwm1a(260);//5
		set_pwm1b(260);//5
	}
	bot_brake();
	set_pwm1a(400);
	set_pwm1b(400);
	_delay_ms(150);
	left=0;check=0;opt=290;flag_6=0;flag_7=0,LT=0;Plus=0;len_count=0;dir_count--;
	update_dir();
}

void right_L()
{
	lcd_write_string_xy(6,1,"RIGHT");
	bot_brake();
	set_pwm1a(400);set_pwm1b(400);
	if(LT==1)
	{while(bit_is_clear(PINA,7))
	{
		bot_forward();
		set_pwm1a(245);
		set_pwm1b(245);
	}}
	while(bit_is_set(PIND,7))
	{
		bot_spot_right();
		set_pwm1a(270);
		set_pwm1b(275);
	}
	while(bit_is_set(PINA,6))
	{
		set_pwm1a(260);
		set_pwm1b(270);
	}
	while(bit_is_set(PINA,5))
	{
		set_pwm1a(260);//55
		set_pwm1b(265);//60
	}
	bot_brake();
	set_pwm1a(400);
	set_pwm1b(400);
	_delay_ms(150);
	right=0;check=0;opt=290;flag_6=0;flag_7=0,LT=0;Plus=0;len_count=0;dir_count++;
	update_dir();
}

void D_U_turn()
{
	update_map();
	bot_backward();
	set_pwm1a(400);set_pwm1b(400);
	while(bit_is_set(PIND,6))
	{
		bot_spot_left();
		set_pwm1a(320);
		set_pwm1b(310);
	}
	while(bit_is_set(PINA,5))
	{
		set_pwm1a(270);
		set_pwm1b(260);
	}
	while(bit_is_set(PINA,6))
	{
		set_pwm1a(260);
		set_pwm1b(250);
	}
	bot_spot_right();
	set_pwm1a(400);
	set_pwm1b(400);
	len_count=0;dir_count-=2;
	update_dir();
}

void L_U_turn()
{
	bot_backward();
	set_pwm1a(400);set_pwm1b(400);
	while(bit_is_set(PIND,6))
	{
		bot_spot_left();
		set_pwm1a(320);
		set_pwm1b(310);
	}
	while(bit_is_set(PINA,5))
	{
		set_pwm1a(270);
		set_pwm1b(260);
	}
	while(bit_is_set(PINA,6))
	{
		set_pwm1a(260);
		set_pwm1b(250);
	}
	bot_spot_right();
	set_pwm1a(400);
	set_pwm1b(400);
	check=0;opt=290;len_count=0;dir_count-=2;
	update_dir();
}

void R_U_turn()
{
	bot_backward();
	set_pwm1a(400);set_pwm1b(400);
	while(bit_is_set(PIND,7))
	{
		bot_spot_left();
		set_pwm1a(320);
		set_pwm1b(310);
	}
	while(bit_is_set(PINA,6))
	{
		set_pwm1a(320);
		set_pwm1b(310);
	}
	while(bit_is_set(PIND,6))
	{
		set_pwm1a(320);
		set_pwm1b(310);
	}
	while(bit_is_set(PINA,5))
	{
		set_pwm1a(270);
		set_pwm1b(260);
	}
	while(bit_is_set(PINA,6))
	{
		set_pwm1a(260);
		set_pwm1b(250);
	}
	bot_spot_right();
	set_pwm1a(400);
	set_pwm1b(400);
	check=0;opt=290;len_count=0;dir_count-=2;
	update_dir();
}

void T_U_turn()
{
	bot_backward();
	set_pwm1a(400);set_pwm1b(400);
	while(bit_is_set(PINA,5))
	{
		bot_spot_left();
		set_pwm1a(320);
		set_pwm1b(310);
	}
	while(bit_is_set(PIND,6))
	{
		set_pwm1a(310);
		set_pwm1b(300);
	}
	while(bit_is_set(PINA,5))
	{
		set_pwm1a(270);
		set_pwm1b(260);
	}
	while(bit_is_set(PINA,6))
	{
		set_pwm1a(260);
		set_pwm1b(250);
	}
	bot_spot_right();
	set_pwm1a(400);
	set_pwm1b(400);
	check=0;opt=290;len_count=0;dir_count-=2;
	update_dir();
}

void E_U_turn()
{
	x_end=x_map;y_end=y_map;
	bot_backward();
	set_pwm1a(400);set_pwm1b(400);
	while(bit_is_clear(PINA,5))
	{
		bot_spot_left();
		set_pwm1a(320);
		set_pwm1b(310);
	}
	while(bit_is_set(PIND,6))
	{
		set_pwm1a(320);
		set_pwm1b(310);
	}
	while(bit_is_set(PINA,5))
	{
		set_pwm1a(270);
		set_pwm1b(260);
	}
	while(bit_is_set(PINA,6))
	{
		set_pwm1a(260);
		set_pwm1b(250);
	}
	bot_spot_right();
	set_pwm1a(400);
	set_pwm1b(400);
	check=0;opt=290;len_count=0;dir_count-=2;
	update_dir();
	update_dir();
}

void P_U_turn()
{
	bot_backward();
	set_pwm1a(400);set_pwm1b(400);
	while(bit_is_set(PIND,7))
	{
		bot_spot_left();
		set_pwm1a(320);
		set_pwm1b(310);
	}
	while(bit_is_set(PIND,6))
	{
		set_pwm1a(310);
		set_pwm1b(300);
	}
	while(bit_is_set(PIND,5))
	{
		set_pwm1a(310);
		set_pwm1b(300);
	}
	while(bit_is_set(PINA,5))
	{
		set_pwm1a(270);
		set_pwm1b(260);
	}
	while(bit_is_set(PINA,6))
	{
		set_pwm1a(260);
		set_pwm1b(250);
	}
	bot_spot_right();
	set_pwm1a(400);
	set_pwm1b(400);
	check=0;opt=290;len_count=0;dir_count-=2;
	update_dir();
}

void init_devices(void)
{
	port_init();
	adc_init();
	bot_motion_init();
	lcd_init(underline);
	lcd_clear();
	switch_init();
	pwm1_init();
}

int main(void)
{
	init_devices();
	lcd_write_int_xy(2,0,kp,2);
	lcd_write_int_xy(7,0,kd*100,2);

	while(1)
	{		
		if(pressed_switch2())
		{
			lcd_write_string_xy(5,1,"START");lcd_write_int_xy(5,0,x_map,2);lcd_write_int_xy(8,0,y_map,2);lcd_write_int_xy(0,1,Dir,2);
			start_map();
			_delay_ms(500);
			lcd_clear();
			while(1)
			{
				line_track();
				LED&=0b11110000;
				LED|=sensorbyte;
			}
		}
		if(pressed_switch0())
		{
			kp++;
			lcd_write_int_xy(2,0,kp,2);
			_delay_ms(150);
		}
		
		if(pressed_switch1())
		{
			kd+=0.01;
			lcd_write_int_xy(12,0,kd*100,2);
			_delay_ms(150);
		}
		
	}
	return 0;
}