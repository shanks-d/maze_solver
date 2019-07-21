#include"sra.c"

int mpos=0,prempos=0;
int motor1_value = 0;//left motor is m1
int motor2_value = 0;
unsigned int opt = 310;//310
float kp = 5;//8
float kd = 1.0;//4.0
float pid;
float p = 0,d = 0;
int flag_6 = 0, flag_7 = 0, left = 0, right = 0, check = 0, dir_count = 0, start = 0, dir_pass = 0;
int LR = 0, LF = 0, RF = 0, LFR = 0, LT = 0, Plus = 0, End = 0, flag = 0, flag_L = 0, flag_R = 0, flag_F = 0, flag_END = 0, flag_start = 0, flag_line = 0;
long int len_count = 0;
char dir[4]={'N','E','S','W'}, Dir = 'N';

int i = 0, j = 0,k = 0, len_6 = 0;// len_9 = 0;
int x_map = 16, y_map = 14, x_end = 0,y_end = 0;//16//14
char map[27][27][2];
int track[20];

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
	for(i=0; i<27; i++)//29
	{
		for(j=0; j<27; j++)//29
		{
			for(k=0; k<2; k++)
			{
				map[i][j][k]='\0';
			}
		}
	}
	map[16][14][0]='a';
}

void write_map()
{
	uint16_t eeprom_addr=0x0000;
	eeprom_write_word(eeprom_addr,x_end);
	eeprom_addr+=2;
	eeprom_write_word(eeprom_addr,y_end);
	eeprom_addr+=2;
	for(int I=0;I<27;I++)
	{
		for(int J=0;J<27;J++)
		{
			eeprom_write_byte(eeprom_addr,map[I][J][0]);
			eeprom_addr+=0x0001;
		}
	}
	lcd_clear();lcd_write_string_xy(4,0,"MAP DONE");

	/*unsigned char eeprom_addr2=0x0000;
	eeprom_write_word(eeprom_addr2,k);
	eeprom_addr2++;
	for(int a=k;a>=0;a--)
	{
		eeprom_addr2++;
		eeprom_write_word(eeprom_addr2,track[a]);
		eeprom_addr2++;
	}
	lcd_write_string_xy(3,1,"TRACK DONE");*/
	if(flag_END==2)
	{	
		while(1)
		{
			bot_stop();
		}
	}
}

void read_map()
{
	uint16_t eeprom_addr=0x0000;
	x_end=eeprom_read_word(eeprom_addr);
	eeprom_addr+=2;
	y_end=eeprom_read_word(eeprom_addr);
	eeprom_addr+=2;
	for(int I=0;I<27;I++)
	{
		for(int J=0;J<27;J++)
		{
			map[I][J][0]=eeprom_read_byte(eeprom_addr);
			eeprom_addr+=0x0001;
		}
	}
	lcd_clear();lcd_write_string_xy(4,0,"MAP READ");
}

void plan_path()
{
	int num = 98, count1 = 0, count2 = 0, i = 0;
	int x1[10], y1[10], x2[10], y2[10];

	x_map = 16;y_map = 14;
	if(map[x_map+1][y_map][0]=='('&&map[x_map+2][y_map][0]=='a')
	{
		map[x_map+2][y_map][0]=(char)num;
		x1[count1]=x_map+2;y1[count1]=y_map;
		count1++;
	}
	if(map[x_map][y_map-1][0]=='('&&map[x_map][y_map-2][0]=='a')
	{
		map[x_map][y_map-2][0]=(char)num;
		x1[count1]=x_map;y1[count1]=y_map-2;
		count1++;
	}
	if(map[x_map-1][y_map][0]=='('&&map[x_map-2][y_map][0]=='a')
	{
		map[x_map-2][y_map][0]=(char)num;
		x1[count1]=x_map-2;y1[count1]=y_map;
		count1++;
	}
	if(map[x_map][y_map+1][0]=='('&&map[x_map][y_map+2][0]=='a')
	{
		map[x_map][y_map+2][0]=(char)num;
		x1[count1]=x_map;y1[count1]=y_map+2;
		count1++;
	}
	while(map[x_end][y_end][0]=='a')
	{
		num++;i=0;
		while(count1!=0)
		{
			if(map[x1[i]+1][y1[i]][0]=='('&&map[x1[i]+2][y1[i]][0]=='a')
			{
				map[x1[i]+2][y1[i]][0]=(char)num;
				x2[count2]=x1[i]+2;y2[count2]=y1[i];
				count2++;
			}
			if(map[x1[i]][y1[i]-1][0]=='('&&map[x1[i]][y1[i]-2][0]=='a')
			{
				map[x1[i]][y1[i]-2][0]=(char)num;
				x2[count2]=x1[i];y2[count2]=y1[i]-2;
				count2++;
			}
			if(map[x1[i]-1][y1[i]][0]=='('&&map[x1[i]-2][y1[i]][0]=='a')
			{
				map[x1[i]-2][y1[i]][0]=(char)num;
				x2[count2]=x1[i]-2;y2[count2]=y1[i];
				count2++;
			}
			if(map[x1[i]][y1[i]+1][0]=='('&&map[x1[i]][y1[i]+2][0]=='a')
			{
				map[x1[i]][y1[i]+2][0]=(char)num;
				x2[count2]=x1[i];y2[count2]=y1[i]+2;
				count2++;
			}
			i++;count1--;
		}
		num++;i=0;
		while(count2!=0)
		{
			if(map[x2[i]+1][y2[i]][0]=='('&&map[x2[i]+2][y2[i]][0]=='a')
			{
				map[x2[i]+2][y2[i]][0]=(char)num;
				x1[count1]=x2[i]+2;y1[count1]=y2[i];
				count1++;
			}
			if(map[x2[i]][y2[i]-1][0]=='('&&map[x2[i]][y2[i]-2][0]=='a')
			{
				map[x2[i]][y2[i]-2][0]=(char)num;
				x1[count1]=x2[i];y1[count1]=y2[i]-2;
				count1++;
			}
			if(map[x2[i]-1][y2[i]][0]=='('&&map[x2[i]-2][y2[i]][0]=='a')
			{
				map[x2[i]-2][y2[i]][0]=(char)num;
				x1[count1]=x2[i]-2;y1[count1]=y2[i];
				count1++;
			}
			if(map[x2[i]][y2[i]+1][0]=='('&&map[x2[i]][y2[i]+2][0]=='a')
			{
				map[x2[i]][y2[i]+2][0]=(char)num;
				x1[count1]=x2[i];y1[count1]=y2[i]+2;
				count1++;
			}
			i++;count2--;
		}
	}
	map[16][14][0]='a';
	lcd_clear();lcd_write_string_xy(2,0,"MAP");lcd_write_string_xy(7,0,"FILLED");
	lcd_write_int_xy(2,1,x_end,2);lcd_write_int_xy(5,1,y_end,2);
	num=(int)map[x_end][y_end][0];
	x_map=x_end;y_map=y_end;k=0;num--;
	while(x_map!=16||y_map!=14)//num!=97 and k--;
	{
		if(map[x_map+1][y_map][0]=='('&&map[x_map+2][y_map][0]==(char)num)
		{
			track[k]=3;num--;x_map+=2;k++;
		}
		else if(map[x_map][y_map-1][0]=='('&&map[x_map][y_map-2][0]==(char)num)
		{
			track[k]=4;num--;y_map-=2;k++;
		}
		else if(map[x_map-1][y_map][0]=='('&&map[x_map-2][y_map][0]==(char)num)
		{
			track[k]=1;num--;x_map-=2;k++;
		}
		else if(map[x_map][y_map+1][0]=='('&&map[x_map][y_map+2][0]==(char)num)
		{
			track[k]=2;num--;y_map+=2;k++;
		}
	}
	k-=2;Dir = 'N';lcd_clear();lcd_write_string_xy(2,1,"PATH");lcd_write_string_xy(7,1,"PLANNED");
	//write_track();
}

void pass_turns()
{
	if(len_6==1)
	{
		k--;
	}
	/*else if(len_9==1)
	{
		k-=2;
	}*/
	lcd_write_int_xy(0,1,k,2);
	switch(track[k])
	{
		case 1:
		switch(Dir)
		{
			case 'S':flag_line=0;correct_turn();len_count=0;break;
			case 'W':left=1;break;
			case 'E':right=1;break;
			default:break;
		}break;
		
		case 2:
		switch(Dir)
		{
			case 'W':flag_line=0;correct_turn();len_count=0;break;
			case 'N':left=1;break;
			case 'S':right=1;break;
			default:break;
		}break;
		
		case 3:
		switch(Dir)
		{
			case 'N':flag_line=0;correct_turn();len_count=0;break;
			case 'E':left=1;break;
			case 'W':right=1;break;
			default:break;
		}break;
		
		case 4:
		switch(Dir)
		{
			case 'E':flag_line=0;correct_turn();len_count=0;break;
			case 'S':left=1;break;
			case 'N':right=1;break;
			default:break;
		}break;
		
		default:break;
	}
	k--;
}

void update_map()
{
	if(len_count<850)//850
	{
		dir_pass=1;
	}
	else if(len_count>849&&len_count<1850)//849//2100
	{
		dir_pass=0;len_6=0;//len_9=0;
		if(flag_END!=2)
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
			}
			else if(Dir=='E')
			{
				map[x_map][y_map+1][0]='(';
				map[x_map][y_map+2][0]='a';
				if(map[x_map][y_map+1][1]=='\0')
				{
					if(start!=0)
					{
						map[x_map][y_map+1][1]='a';
					}
					else if(start==0)
					{
						map[x_map][y_map+1][1]='b';
					}
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
					if(start!=0)
					{
						map[x_map+1][y_map][1]='a';
					}
					else if(start==0)
					{
						map[x_map+1][y_map][1]='b';
					}
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
					if(start!=0)
					{
						map[x_map][y_map-1][1]='a';
					}
					else if(start==0)
					{
						map[x_map][y_map-1][1]='b';
					}
				}
				else if(map[x_map][y_map-1][1]=='a')
				{
					map[x_map][y_map-1][1]='b';
				}
				y_map-=2;
			}
		}lcd_clear();lcd_write_string_xy(15,0,"3");
	}
	else if(len_count>1849&&len_count<3050)//2099//3300
	{
		dir_pass=0;len_6=1;//len_9=0;
		if(flag_END!=2)
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
						map[x_map-1][y_map][1]='b';
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
					if(start!=0)
					{
						map[x_map][y_map+1][1]='a';
					}
					else if(start==0)
					{
						map[x_map][y_map+1][1]='b';
					}
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
					if(start!=0)
					{
						map[x_map][y_map+1][1]='a';
					}
					else if(start==0)
					{
						map[x_map][y_map+1][1]='b';
					}
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
					if(start!=0)
					{
						map[x_map+1][y_map][1]='a';
					}
					else if(start==0)
					{
						map[x_map+1][y_map][1]='b';
					}
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
					if(start!=0)
					{
						map[x_map+1][y_map][1]='a';
					}
					else if(start==0)
					{
						map[x_map+1][y_map][1]='b';
					}
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
					if(start!=0)
					{
						map[x_map][y_map-1][1]='a';
					}
					else if(start==0)
					{
						map[x_map][y_map-1][1]='b';
					}
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
					if(start!=0)
					{
						map[x_map][y_map-1][1]='a';
					}
					else if(start==0)
					{
						map[x_map][y_map-1][1]='b';
					}
				}
				else if(map[x_map][y_map-1][1]=='a')
				{
					map[x_map][y_map-1][1]='b';
				}
				y_map-=2;
			}
		}lcd_clear();lcd_write_string_xy(15,0,"6");
	}
	/*else if(len_count>3299)
	{
		dir_pass=0;len_9=1;len_6=0;
		if(flag_END!=2)
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
						map[x_map-1][y_map][1]='b';
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
					if(start!=0)
					{
						map[x_map][y_map+1][1]='a';
					}
					else if(start==0)
					{
						map[x_map][y_map+1][1]='b';
					}
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
					if(start!=0)
					{
						map[x_map][y_map+1][1]='a';
					}
					else if(start==0)
					{
						map[x_map][y_map+1][1]='b';
					}
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
					if(start!=0)
					{
						map[x_map][y_map+1][1]='a';
					}
					else if(start==0)
					{
						map[x_map][y_map+1][1]='b';
					}
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
					if(start!=0)
					{
						map[x_map+1][y_map][1]='a';
					}
					else if(start==0)
					{
						map[x_map+1][y_map][1]='b';
					}
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
					if(start!=0)
					{
						map[x_map+1][y_map][1]='a';
					}
					else if(start==0)
					{
						map[x_map+1][y_map][1]='b';
					}
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
					if(start!=0)
					{
						map[x_map+1][y_map][1]='a';
					}
					else if(start==0)
					{
						map[x_map+1][y_map][1]='b';
					}
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
					if(start!=0)
					{
						map[x_map][y_map-1][1]='a';
					}
					else if(start==0)
					{
						map[x_map][y_map-1][1]='b';
					}
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
					if(start!=0)
					{
						map[x_map][y_map-1][1]='a';
					}
					else if(start==0)
					{
						map[x_map][y_map-1][1]='b';
					}
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
					if(start!=0)
					{
						map[x_map][y_map-1][1]='a';
					}
					else if(start==0)
					{
						map[x_map][y_map-1][1]='b';
					}
				}
				else if(map[x_map][y_map-1][1]=='a')
				{
					map[x_map][y_map-1][1]='b';
				}
				y_map-=2;
			}
		}lcd_clear();lcd_write_string_xy(15,0,"9");
	}*/
}

void line_track(void)
{
	if(flag_line==0)
	{
		opt=310;
		if(bit_is_clear(PINA,5)&&bit_is_set(PINA,6))
		{
			opt=310;kp=5;kd=1.0;
		}
		else if((bit_is_clear(PINA,5)&&bit_is_clear(PINA,6))||(bit_is_set(PINA,5)&&bit_is_clear(PINA,6)))
		{
			opt=290;kp=4;kd=1.0;
		}
		else if(bit_is_set(PINA,5)&&bit_is_clear(PINA,6))
		{
			opt=280;kp=5;kd=1.5;
		}
	}
	check_sensors();
	bot_forward();
	switch(sensorbyte)
	{
		case 0b0110:mpos = 0; break;//sensor numbering starts from right
		
		case 0b0100:mpos = 1; break;
		
		case 0b0010:mpos = -1; break;
		
		case 0b1100:mpos = 3; break;
		
		case 0b0011:mpos = -3; break;
		
		case 0b1110:if(flag_7==1){mpos = -3;}else{if(bit_is_clear(PINA,5)&&bit_is_set(PINA,6)){mpos=-4;}else mpos = 5;} break;//6//4
		
		case 0b0111:if(flag_6==1){mpos = 3;}else {if(bit_is_set(PINA,5)&&bit_is_clear(PINA,6)){mpos=4;}else mpos = -5;} break;//6//4
		
		case 0b1000:mpos = 6; break;
		
		case 0b0001:mpos = -6; break;
		
		case 0b0000:if(bit_is_set(PIND,6)&&bit_is_set(PINA,4)&&bit_is_set(PINA,5)&&bit_is_set(PINA,6)&&bit_is_set(PINA,7)&&bit_is_set(PIND,7)&&flag_END!=2&&len_count>1000){lcd_clear();lcd_write_string_xy(2,0,"WRITING MAP");bot_backward();bot_brake();write_map();D_U_turn();}break;
		
		default:mpos=0; break;
	}
	if(check==0)
	{check_turns();}
	
	p = mpos * kp;
	d = kd * (mpos-prempos);
	pid = p + d;
	motor1_value = opt + pid;//left motor is m1
	motor2_value = opt - pid;
	set_pwm1a(motor1_value);
	set_pwm1b(motor2_value);
	prempos=mpos;len_count++;
	delay_microsec(60);
}

void check_random()
{
	if(flag_END!=2)
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
				else if(map[x_map][y_map-1][1]=='b'&&map[x_map-1][y_map][1]=='\0')
				{
					flag=1;flag_F=1;
				}
				else if(map[x_map][y_map-1][1]=='\0'&&map[x_map-1][y_map][1]=='b')
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
				else if(map[x_map][y_map+1][1]=='b'&&map[x_map-1][y_map][1]=='\0')
				{
					flag=1;flag_F=1;
				}
				else if(map[x_map][y_map+1][1]=='\0'&&map[x_map-1][y_map][1]=='b')
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
				else if(map[x_map][y_map-1][1]=='b'&&map[x_map][y_map+1][1]=='\0')
				{
					flag=1;flag_R=1;
				}
				else if(map[x_map][y_map-1][1]=='\0'&&map[x_map][y_map+1][1]=='b')
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
				if((map[x_map][y_map-1][1]=='a'&&map[x_map-1][y_map][1]=='\0'&&map[x_map][y_map+1][1]=='\0')||(map[x_map][y_map-1][1]=='b'&&map[x_map-1][y_map][1]=='a'&&map[x_map][y_map+1][1]=='a')||(map[x_map][y_map-1][1]=='b'&&map[x_map-1][y_map][1]=='\0'&&map[x_map][y_map+1][1]=='\0'))
				{
					flag=0;LFR=0;RF=1;
				}
				else if((map[x_map][y_map-1][1]=='\0'&&map[x_map-1][y_map][1]=='a'&&map[x_map][y_map+1][1]=='\0')||(map[x_map][y_map-1][1]=='a'&&map[x_map-1][y_map][1]=='b'&&map[x_map][y_map+1][1]=='a')||(map[x_map][y_map-1][1]=='\0'&&map[x_map-1][y_map][1]=='b'&&map[x_map][y_map+1][1]=='\0'))
				{
					flag=0;LFR=0;LR=1;
				}
				else if((map[x_map][y_map-1][1]=='\0'&&map[x_map-1][y_map][1]=='\0'&&map[x_map][y_map+1][1]=='a')||(map[x_map][y_map-1][1]=='a'&&map[x_map-1][y_map][1]=='a'&&map[x_map][y_map+1][1]=='b')||(map[x_map][y_map-1][1]=='\0'&&map[x_map-1][y_map][1]=='\0'&&map[x_map][y_map+1][1]=='b'))
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
				
				else if((map[x_map][y_map-1][1]=='\0'&&map[x_map-1][y_map][1]=='a'&&map[x_map][y_map+1][1]=='b')||(map[x_map][y_map-1][1]=='\0'&&map[x_map-1][y_map][1]=='b'&&map[x_map][y_map+1][1]=='a'))
				{
					flag=1;flag_L=1;
				}
				else if((map[x_map][y_map-1][1]=='a'&&map[x_map-1][y_map][1]=='\0'&&map[x_map][y_map+1][1]=='b')||(map[x_map][y_map-1][1]=='b'&&map[x_map-1][y_map][1]=='\0'&&map[x_map][y_map+1][1]=='a'))
				{
					flag=1;flag_F=1;
				}
				else if((map[x_map][y_map-1][1]=='a'&&map[x_map-1][y_map][1]=='b'&&map[x_map][y_map+1][1]=='\0')||(map[x_map][y_map-1][1]=='b'&&map[x_map-1][y_map][1]=='a'&&map[x_map][y_map+1][1]=='\0'))
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
				else if(map[x_map-1][y_map][1]=='b'&&map[x_map][y_map+1][1]=='\0')
				{
					flag=1;flag_F=1;
				}
				else if(map[x_map-1][y_map][1]=='\0'&&map[x_map][y_map+1][1]=='b')
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
				else if(map[x_map+1][y_map][1]=='b'&&map[x_map][y_map+1][1]=='\0')
				{
					flag=1;flag_F=1;
				}
				else if(map[x_map+1][y_map][1]=='\0'&&map[x_map][y_map+1][1]=='b')
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
				else if(map[x_map-1][y_map][1]=='b'&&map[x_map+1][y_map][1]=='\0')
				{
					flag=1;flag_R=1;
				}
				else if(map[x_map-1][y_map][1]=='\0'&&map[x_map+1][y_map][1]=='b')
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
				if((map[x_map-1][y_map][1]=='a'&&map[x_map][y_map+1][1]=='\0'&&map[x_map+1][y_map][1]=='\0')||(map[x_map-1][y_map][1]=='b'&&map[x_map][y_map+1][1]=='a'&&map[x_map+1][y_map][1]=='a')||(map[x_map-1][y_map][1]=='b'&&map[x_map][y_map+1][1]=='\0'&&map[x_map+1][y_map][1]=='\0'))
				{
					flag=0;LFR=0;RF=1;
				}
				else if((map[x_map-1][y_map][1]=='\0'&&map[x_map][y_map+1][1]=='a'&&map[x_map+1][y_map][1]=='\0')||(map[x_map-1][y_map][1]=='a'&&map[x_map][y_map+1][1]=='b'&&map[x_map+1][y_map][1]=='a')||(map[x_map-1][y_map][1]=='\0'&&map[x_map][y_map+1][1]=='b'&&map[x_map+1][y_map][1]=='\0'))
				{
					flag=0;LFR=0;LR=1;
				}
				else if((map[x_map-1][y_map][1]=='\0'&&map[x_map][y_map+1][1]=='\0'&&map[x_map+1][y_map][1]=='a')||(map[x_map-1][y_map][1]=='a'&&map[x_map][y_map+1][1]=='a'&&map[x_map+1][y_map][1]=='b')||(map[x_map-1][y_map][1]=='\0'&&map[x_map][y_map+1][1]=='\0'&&map[x_map+1][y_map][1]=='b'))
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
				
				else if((map[x_map-1][y_map][1]=='\0'&&map[x_map][y_map+1][1]=='a'&&map[x_map+1][y_map][1]=='b')||(map[x_map-1][y_map][1]=='\0'&&map[x_map][y_map+1][1]=='b'&&map[x_map+1][y_map][1]=='a'))
				{
					flag=1;flag_L=1;
				}
				else if((map[x_map-1][y_map][1]=='a'&&map[x_map][y_map+1][1]=='\0'&&map[x_map+1][y_map][1]=='b')||(map[x_map-1][y_map][1]=='b'&&map[x_map][y_map+1][1]=='\0'&&map[x_map+1][y_map][1]=='a'))
				{
					flag=1;flag_F=1;
				}
				else if((map[x_map-1][y_map][1]=='a'&&map[x_map][y_map+1][1]=='b'&&map[x_map+1][y_map][1]=='\0')||(map[x_map-1][y_map][1]=='b'&&map[x_map][y_map+1][1]=='a'&&map[x_map+1][y_map][1]=='\0'))
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
				else if(map[x_map][y_map+1][1]=='b'&&map[x_map+1][y_map][1]=='\0')
				{
					flag=1;flag_F=1;
				}
				else if(map[x_map][y_map+1][1]=='\0'&&map[x_map+1][y_map][1]=='b')
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
				else if(map[x_map][y_map-1][1]=='b'&&map[x_map+1][y_map][1]=='\0')
				{
					flag=1;flag_F=1;
				}
				else if(map[x_map][y_map-1][1]=='\0'&&map[x_map+1][y_map][1]=='b')
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
				else if(map[x_map][y_map+1][1]=='b'&&map[x_map][y_map-1][1]=='\0')
				{
					flag=1;flag_R=1;
				}
				else if(map[x_map][y_map+1][1]=='\0'&&map[x_map][y_map-1][1]=='b')
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
				if((map[x_map][y_map+1][1]=='a'&&map[x_map+1][y_map][1]=='\0'&&map[x_map][y_map-1][1]=='\0')||(map[x_map][y_map+1][1]=='b'&&map[x_map+1][y_map][1]=='a'&&map[x_map][y_map-1][1]=='a')||(map[x_map][y_map+1][1]=='b'&&map[x_map+1][y_map][1]=='\0'&&map[x_map][y_map-1][1]=='\0'))
				{
					flag=0;LFR=0;RF=1;
				}
				else if((map[x_map][y_map+1][1]=='\0'&&map[x_map+1][y_map][1]=='a'&&map[x_map][y_map-1][1]=='\0')||(map[x_map][y_map+1][1]=='a'&&map[x_map+1][y_map][1]=='b'&&map[x_map][y_map-1][1]=='a')||(map[x_map][y_map+1][1]=='\0'&&map[x_map+1][y_map][1]=='b'&&map[x_map][y_map-1][1]=='\0'))
				{
					flag=0;LFR=0;LR=1;
				}
				else if((map[x_map][y_map+1][1]=='\0'&&map[x_map+1][y_map][1]=='\0'&&map[x_map][y_map-1][1]=='a')||(map[x_map][y_map+1][1]=='a'&&map[x_map+1][y_map][1]=='a'&&map[x_map][y_map-1][1]=='b')||(map[x_map][y_map+1][1]=='\0'&&map[x_map+1][y_map][1]=='\0'&&map[x_map][y_map-1][1]=='b'))
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
				
				else if((map[x_map][y_map+1][1]=='\0'&&map[x_map+1][y_map][1]=='a'&&map[x_map][y_map-1][1]=='b')||(map[x_map][y_map+1][1]=='\0'&&map[x_map+1][y_map][1]=='b'&&map[x_map][y_map-1][1]=='a'))
				{
					flag=1;flag_L=1;
				}
				else if((map[x_map][y_map+1][1]=='a'&&map[x_map+1][y_map][1]=='\0'&&map[x_map][y_map-1][1]=='b')||(map[x_map][y_map+1][1]=='b'&&map[x_map+1][y_map][1]=='\0'&&map[x_map][y_map-1][1]=='a'))
				{
					flag=1;flag_F=1;
				}
				else if((map[x_map][y_map+1][1]=='a'&&map[x_map+1][y_map][1]=='b'&&map[x_map][y_map-1][1]=='\0')||(map[x_map][y_map+1][1]=='b'&&map[x_map+1][y_map][1]=='a'&&map[x_map][y_map-1][1]=='\0'))
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
				else if(map[x_map+1][y_map][1]=='b'&&map[x_map][y_map-1][1]=='\0')
				{
					flag=1;flag_F=1;
				}
				else if(map[x_map+1][y_map][1]=='\0'&&map[x_map][y_map-1][1]=='b')
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
				else if(map[x_map-1][y_map][1]=='b'&&map[x_map][y_map-1][1]=='\0')
				{
					flag=1;flag_F=1;
				}
				else if(map[x_map-1][y_map][1]=='\0'&&map[x_map][y_map-1][1]=='b')
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
				else if(map[x_map+1][y_map][1]=='b'&&map[x_map-1][y_map][1]=='\0')
				{
					flag=1;flag_R=1;
				}
				else if(map[x_map+1][y_map][1]=='\0'&&map[x_map-1][y_map][1]=='b')
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
				if((map[x_map+1][y_map][1]=='a'&&map[x_map][y_map-1][1]=='\0'&&map[x_map-1][y_map][1]=='\0')||(map[x_map+1][y_map][1]=='b'&&map[x_map][y_map-1][1]=='a'&&map[x_map-1][y_map][1]=='a')||(map[x_map+1][y_map][1]=='b'&&map[x_map][y_map-1][1]=='\0'&&map[x_map-1][y_map][1]=='\0'))
				{
					flag=0;LFR=0;RF=1;
				}
				else if((map[x_map+1][y_map][1]=='\0'&&map[x_map][y_map-1][1]=='a'&&map[x_map-1][y_map][1]=='\0')||(map[x_map+1][y_map][1]=='a'&&map[x_map][y_map-1][1]=='b'&&map[x_map-1][y_map][1]=='a')||(map[x_map+1][y_map][1]=='\0'&&map[x_map][y_map-1][1]=='b'&&map[x_map-1][y_map][1]=='\0'))
				{
					flag=0;LFR=0;LR=1;
				}
				else if((map[x_map+1][y_map][1]=='\0'&&map[x_map][y_map-1][1]=='\0'&&map[x_map-1][y_map][1]=='a')||(map[x_map+1][y_map][1]=='a'&&map[x_map][y_map-1][1]=='a'&&map[x_map-1][y_map][1]=='b')||(map[x_map+1][y_map][1]=='\0'&&map[x_map][y_map-1][1]=='\0'&&map[x_map-1][y_map][1]=='b'))
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
				
				else if((map[x_map+1][y_map][1]=='\0'&&map[x_map][y_map-1][1]=='a'&&map[x_map-1][y_map][1]=='b')||(map[x_map+1][y_map][1]=='\0'&&map[x_map][y_map-1][1]=='b'&&map[x_map-1][y_map][1]=='a'))
				{
					flag=1;flag_L=1;
				}
				else if((map[x_map+1][y_map][1]=='a'&&map[x_map][y_map-1][1]=='\0'&&map[x_map-1][y_map][1]=='b')||(map[x_map+1][y_map][1]=='b'&&map[x_map][y_map-1][1]=='\0'&&map[x_map-1][y_map][1]=='a'))
				{
					flag=1;flag_F=1;
				}
				else if((map[x_map+1][y_map][1]=='a'&&map[x_map][y_map-1][1]=='b'&&map[x_map-1][y_map][1]=='\0')||(map[x_map+1][y_map][1]=='b'&&map[x_map][y_map-1][1]=='a'&&map[x_map-1][y_map][1]=='\0'))
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
	else
	{
		flag=4;
	}
}

void random_turns()
{
	if(dir_pass==0)
	{
		check_random();
	}
	else if(dir_pass==1)
	{
		flag = 3;
	}
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
					flag_line=0;correct_turn();len_count=0;lcd_write_int_xy(5,0,x_map,2);lcd_write_int_xy(8,0,y_map,2);//lcd_clear();
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
					flag_line=0;correct_turn();len_count=0;lcd_write_int_xy(5,0,x_map,2);lcd_write_int_xy(8,0,y_map,2);//lcd_clear();
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
						left=1;lcd_write_string_xy(15,1,"L");
					}
					
					else if((len_count/2)%2==0)
					{
						right=1;lcd_write_string_xy(15,1,"R");
					}
				}
				
				else if(len_count%2==1)
				{
					flag_line=0;correct_turn();len_count=0;lcd_write_string_xy(15,1,"S");lcd_write_int_xy(5,0,x_map,2);lcd_write_int_xy(8,0,y_map,2);//lcd_clear();
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
				flag_line=0;correct_turn();len_count=0;lcd_write_int_xy(5,0,x_map,2);lcd_write_int_xy(8,0,y_map,2);//lcd_clear();
			}
		}break;
		case 2:
		{
			flag_END++;
			if(flag_END==2)
			{
				bot_brake();set_pwm1a(400);set_pwm1b(400);lcd_clear();lcd_write_string_xy(4,0,"COMPLETE");
				lcd_write_int_xy(5,1,x_end,2);lcd_write_int_xy(8,1,y_end,2);delay_sec(1);bot_stop();
				lcd_clear();lcd_write_string_xy(2,0,"WRITING MAP");write_map();
			}
			else if(LF==1)
			{
				lcd_clear();lcd_write_string_xy(2,0,"WRITING MAP");bot_backward();bot_brake();write_map();R_U_turn();
			}
			else if(RF==1)
			{
				lcd_clear();lcd_write_string_xy(2,0,"WRITING MAP");bot_backward();bot_brake();write_map();L_U_turn();
			}
			else if(LR==1)
			{
				lcd_clear();lcd_write_string_xy(2,0,"WRITING MAP");bot_backward();bot_brake();write_map();T_U_turn();
			}
			else if(LFR==1)
			{
				lcd_clear();lcd_write_string_xy(2,0,"WRITING MAP");bot_backward();bot_brake();write_map();P_U_turn();
			}
		}break;
		case 3:
		{
			if(LF==1)
			{
				left=1;
			}
			else if(RF==1)
			{
				right=1;
			}
		}break;
		case 4: pass_turns();break;
		default:break;
	}
	LR = 0; LF = 0; RF = 0; LFR = 0; flag = 0; flag_L = 0; flag_F = 0; flag_R = 0;Plus = 0;
}

void check_turns()
{
	check=1;
	if(bit_is_clear(PIND,6)||bit_is_clear(PIND,7))
	{
		update_map();
		if(bit_is_clear(PINA,5)||bit_is_clear(PINA,6))
		{
			start=1;
			if(bit_is_clear(PIND,6)&&bit_is_set(PIND,7))
			{
				bot_backward();
				bot_forward();
				opt=245;flag_line=1;
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
						bot_forward();set_pwm1a(245);set_pwm1b(245);
						if(bit_is_set(PIND,6)&&bit_is_set(PIND,7)){End=-1;Plus=0;break;}
					}
					End++;Plus++;
				}
				if(Plus==0)
				{
					flag_6=0;lcd_write_string_xy(14,1,"E");
					if(bit_is_set(PINA,4))
					{
						while(bit_is_set(PINA,4))
						{
							line_track();
						}
					}
					LF=1;random_turns();
					check_turn_sensors();
					take_turns();
				}
				else if(Plus==1)
				{
					lcd_write_string_xy(14,1,"P");
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
				else if(End!=0){lcd_clear();lcd_write_string_xy(4,0,"END ZONE");bot_brake();set_pwm1a(400);set_pwm1b(400);_delay_ms(500);E_U_turn();}
			}
			
			else if(bit_is_set(PIND,6)&&bit_is_clear(PIND,7))
			{
				bot_backward();
				bot_forward();
				opt=245;flag_line=1;
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
						bot_forward();set_pwm1a(245);set_pwm1b(245);
						if(bit_is_set(PIND,6)&&bit_is_set(PIND,7)){End=-1;Plus=0;break;}
					}
					End++;Plus++;
				}
				if(Plus==0)
				{
					flag_7=0;lcd_write_string_xy(14,1,"E");
					if(bit_is_set(PINA,7))
					{
						while(bit_is_set(PINA,7))
						{
							line_track();
						}
					}
					RF=1;
					random_turns();
					check_turn_sensors();
					take_turns();
				}
				else if(Plus==1)
				{
					lcd_write_string_xy(14,1,"P");
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
				else if(End!=0){lcd_clear();lcd_write_string_xy(4,0,"END ZONE");bot_brake();set_pwm1a(400);set_pwm1b(400);E_U_turn();}
			}
		}
		else
		{
			if(bit_is_clear(PIND,6)&&bit_is_clear(PIND,7))
			{
				NO_FRO_T:
				start=1;lcd_write_string_xy(14,1,"T");
				while(bit_is_set(PINA,4)&&bit_is_set(PINA,7))//||
				{
					bot_forward();
					set_pwm1a(250);
					set_pwm1b(250);
				}
				LR=1;LT=1;
				random_turns();
				if(left==1){left_L();}
				else if(right==1){right_L();}
			}
			
			else if(bit_is_clear(PIND,6)&&bit_is_set(PIND,7))
			{
				bot_backward();
				bot_brake();
				while(bit_is_clear(PIND,6))
				{
					bot_forward();
					set_pwm1a(245);
					set_pwm1b(250);//55
					if(bit_is_clear(PIND,7)){goto NO_FRO_T;}
				}LT=1;lcd_write_string_xy(14,1,"L");
				while(bit_is_set(PINA,4))
				{
					bot_forward();
					set_pwm1a(250);//45
					set_pwm1b(250);
				}
				if(len_6==1)
				{
					k--;
				}
				k--;
				left_L();
			}
			
			else if(bit_is_set(PIND,6)&&bit_is_clear(PIND,7))
			{
				bot_backward();
				bot_brake();
				while(bit_is_clear(PIND,7))
				{
					bot_forward();
					set_pwm1a(250);//55
					set_pwm1b(245);
					if(bit_is_clear(PIND,6)){goto NO_FRO_T;}
				}LT=1;lcd_write_string_xy(14,1,"L");
				while(bit_is_set(PINA,7))
				{
					bot_forward();
					set_pwm1a(250);
					set_pwm1b(250);//45
				}
				if(len_6==1)
				{
					k--;
				}
				k--;
				right_L();
			}
		}lcd_clear();
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

void correct_turn()
{
	if(bit_is_set(PINA,6))
	{
		while(bit_is_set(PINA,6))
		{
			set_pwm1a(400);set_pwm1b(280);
			MOTOR1A=1;
			MOTOR1B=1;
			MOTOR2A=1;
			MOTOR2B=0;
		}
	}
}

void left_L()
{
	lcd_write_string_xy(6,1,"LEFT");lcd_write_int_xy(5,0,x_map,2);lcd_write_int_xy(8,0,y_map,2);
	if(LT==1)
	{while(bit_is_clear(PINA,4))
	{
		bot_forward();
		set_pwm1a(250);
		set_pwm1b(250);
	}}
	set_pwm1a(400);set_pwm1b(400);
	bot_backward();
	while(bit_is_set(PIND,6))
	{
		bot_spot_left();
		set_pwm1a(300);//300
		set_pwm1b(300);//300
	}
	while(bit_is_set(PINA,5))
	{
		set_pwm1a(280);//280
		set_pwm1b(280);//280
	}
	while(bit_is_set(PINA,6))
	{
		set_pwm1a(255);
		set_pwm1b(260);
	}
	bot_spot_right();
	set_pwm1a(400);
	set_pwm1b(400);
	bot_brake();
	_delay_ms(150);
	left=0;check=0;flag_6=0;flag_7=0,LT=0;Plus=0;len_count=0;flag_line=0;//opt=290;
	if(dir_pass==0)
	{
		dir_count--;update_dir();
	}
}

void right_L()
{
	lcd_write_string_xy(6,1,"RIGHT");lcd_write_int_xy(5,0,x_map,2);lcd_write_int_xy(8,0,y_map,2);
	if(LT==1)
	{while(bit_is_clear(PINA,7))
	{
		bot_forward();
		set_pwm1a(250);
		set_pwm1b(250);
	}}
	set_pwm1a(400);set_pwm1b(400);
	bot_backward();
	while(bit_is_set(PIND,7))
	{
		bot_spot_right();
		set_pwm1a(300);//300
		set_pwm1b(305);//305
	}
	while(bit_is_set(PINA,6))
	{
		set_pwm1a(280);//280
		set_pwm1b(285);//285
	}
	while(bit_is_set(PINA,5))
	{
		set_pwm1a(255);
		set_pwm1b(255);
	}
	bot_spot_left();
	set_pwm1a(400);
	set_pwm1b(400);
	bot_brake();
	_delay_ms(100);
	correct_turn();
	right=0;check=0;flag_6=0;flag_7=0,LT=0;Plus=0;len_count=0;flag_line=0;//opt=290;
	if(dir_pass==0)
	{
		dir_count++;update_dir();
	}
}

void D_U_turn()
{
	update_map();
	lcd_write_string_xy(6,1,"D - U");lcd_write_int_xy(5,0,x_map,2);lcd_write_int_xy(8,0,y_map,2);
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
	lcd_write_string_xy(6,1,"L - U");
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
	check=0;len_count=0;dir_count-=2;flag_line=0;//opt=290;
	update_dir();
}

void R_U_turn()
{
	lcd_write_string_xy(6,1,"R - U");
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
	check=0;len_count=0;dir_count-=2;flag_line=0;//opt=290;
	update_dir();
}

void T_U_turn()
{
	lcd_write_string_xy(6,1,"T - U");
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
	check=0;len_count=0;dir_count-=2;flag_line=0;//opt=290;
	update_dir();
}

void E_U_turn()
{
	if(flag_END!=2)
	{
		bot_backward();bot_brake();x_end=x_map;y_end=y_map;
		lcd_clear();lcd_write_string_xy(2,0,"WRITING MAP");
		write_map();
		lcd_write_string_xy(6,1,"E - U");
		bot_backward();
		set_pwm1a(400);set_pwm1b(400);
		while(bit_is_clear(PINA,5))
		{
			bot_spot_left();
			set_pwm1a(310);
			set_pwm1b(320);
		}
		while(bit_is_clear(PIND,7))
		{
			bot_spot_left();
			set_pwm1a(310);//275
			set_pwm1b(320);//285
		}
		while(bit_is_set(PINA,5))
		{
			bot_spot_left();
			set_pwm1a(275);
			set_pwm1b(285);
		}
		while(bit_is_set(PINA,6))
		{
			bot_spot_left();
			set_pwm1a(250);
			set_pwm1b(260);
		}
		bot_spot_right();
		set_pwm1a(400);
		set_pwm1b(400);
		check=0;len_count=0;dir_count-=2;flag_line=0;//opt=290;
		update_dir();
	}
	else 
	{flag_END=3;}
}

void P_U_turn()
{
	lcd_write_string_xy(6,1,"P - U");
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
	check=0;len_count=0;dir_count-=2;flag_line=0;//opt=290;
	update_dir();
}

void init_devices(void)
{
	port_init();
	adc_init();
	bot_motion_init();
	lcd_init(underline);
	lcd_clear();
	//switch_init();
	DDRD=0b00110010;
	PORTD=0xFF;
	pwm1_init();
}

int main(void)
{
	init_devices();
	lcd_write_string_xy(3,0,"D2 - START");lcd_write_string_xy(3,1,"D0 - PATH");
	//lcd_write_int_xy(1,0,kp,2);
	//lcd_write_int_xy(5,0,kd*10,2);
	//lcd_write_int_xy(6,1,opt,3);
	while(1)
	{
		if(pressed_switch2())
		{
			lcd_clear();lcd_write_string_xy(5,0,"START");
			start_map();
			_delay_ms(500);
			lcd_clear();
			while(flag_END!=2)
			{
				line_track();
				LED&=0b11110000;
				LED|=sensorbyte;
			}
		}
		if(bit_is_clear(PIND,0))
		{
			lcd_clear();
			_delay_ms(100);
			read_map();
			plan_path();
			flag_END=2;
			lcd_clear();
			for(int t=k;t>=0;t--)
			{
				lcd_write_int_xy(k-t,0,track[t],1);
			}
			lcd_write_string_xy(2,1,"D2 TO START");
			while(bit_is_set(PIND,2));
			
			lcd_clear();_delay_ms(500);
			while(flag_END==2)
			{
				line_track();
				LED&=0b11110000;
				LED|=sensorbyte;
			}
			bot_stop();lcd_write_string_xy(4,0,"END ZONE");
			while(1)
			{
				flick();
			}
		}
		
		/*if(pressed_switch2())
		{
			_delay_ms(150);
			while(1)
			{
				line_track();
				LED&=0b11110000;
				LED|=sensorbyte;
			}
		}
		if(pressed_switch0())
		{
			_delay_ms(150);
			kp++;
			lcd_write_int_xy(1,0,kp,2);
		}
		/*if(pressed_switch0())
		{
			_delay_ms(150);
			kd+=0.1;
			lcd_write_int_xy(5,0,kd*10,2);
		}*/
	}
	return 0;
}