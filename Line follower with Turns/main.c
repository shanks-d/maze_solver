#include"sra.c"

int mpos=0,prempos=0;
unsigned int opt = 270;
float kp = 13;
float kd = 1.2;
float pid;
float p = 0,d = 0;
int flag_6 = 0, flag_7 = 0, left = 0, right = 0, check = 0, dir_count = 0;
int Left = 0, Right = 0, Forward = 0, LT = 0, Plus = 0, End = 0;
long int len_count = 0;
char dir[4]={'N','E','S','W'}, Dir = 'N';
int LSR = 0, RSL = 1, SRL = 0;

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

void update_map()
{
	lcd_clear();
	if(len_count>1000&&len_count<3200)
	{
		if(Dir=='N')
		{
			lcd_write_string_xy(7,0,"N");
		}
		else if(Dir=='E')
		{
			lcd_write_string_xy(7,0,"E");
		}
		else if(Dir=='S')
		{
			lcd_write_string_xy(7,0,"S");
		}
		else if(Dir=='W')
		{
			lcd_write_string_xy(7,0,"W");
		}
		lcd_write_string_xy(15,1,"3");
	}
	else if(len_count>3700&&len_count<5200)
	{
		if(Dir=='N')
		{
			lcd_write_string_xy(7,0,"N");
		}
		else if(Dir=='E')
		{
			lcd_write_string_xy(7,0,"E");
		}
		else if(Dir=='S')
		{
			lcd_write_string_xy(7,0,"S");
		}
		else if(Dir=='W')
		{
			lcd_write_string_xy(7,0,"W");
		}
		lcd_write_string_xy(15,1,"6");
	}
	else if(len_count>5700&&len_count<7200)
	{
		if(Dir=='N')
		{
			lcd_write_string_xy(7,0,"N");
		}
		else if(Dir=='E')
		{
			lcd_write_string_xy(7,0,"E");
		}
		else if(Dir=='S')
		{
			lcd_write_string_xy(7,0,"S");
		}
		else if(Dir=='W')
		{
			lcd_write_string_xy(7,0,"W");
		}
		lcd_write_string_xy(15,1,"9");
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
		
		case 0b0000:if(bit_is_set(PIND,6)&&bit_is_set(PINA,4)&&bit_is_set(PINA,5)&&bit_is_set(PINA,6)&&bit_is_set(PINA,7)&&bit_is_set(PIND,7)){U_turn();} break;
		
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

void pass_turns()
{
	if(LSR==1)
	{//LSR
	if(Left==1)
	{left=1;}
	else if(Forward==1)
	{opt=270;len_count=0;lcd_clear();}
	else if(Right==1)
	{right=1;}
	}
	
	else if(RSL==1)
	{//RSL
	if(Right==1)
	{right=1;}
	else if(Forward==1)
	{opt=270;len_count=0;lcd_clear();}
	else if(Left==1)
	{left=1;}
	}
	
	else if(SRL==1)
	{//SRL
	if(Forward==1)
	{opt=270;len_count=0;lcd_clear();}
	else if(Right==1)
	{right=1;}
	else if(Left==1)
	{left=1;}
	}
}

void mt_order()
{
	LSR=0;RSL=0;SRL=0;
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
					if(bit_is_clear(PIND,7)){Plus=1;break;}//PLUS condition is HERE
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
					Left=1;Forward=1;Right=0;
					pass_turns();
					flag_6=0;
					if(bit_is_set(PINA,4))
					{while(bit_is_set(PINA,4))
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
					Left=1;Right=1;Forward=1;LT=1;
					pass_turns();
					if(left==1){left_L();}
					else if(right==1){right_L();}
				}
				else if(End==1){lcd_clear();lcd_write_string_xy(4,0,"END ZONE");bot_brake();set_pwm1a(400);set_pwm1b(400);flick();delay_sec(1000);}
			}
			
			else if(bit_is_set(PIND,6)&&bit_is_clear(PIND,7))
			{
				opt=245;
				flag_7=1;
				while(bit_is_clear(PIND,7))
				{
					line_track();
					if(bit_is_clear(PIND,6)){Plus=1;break;}//PLUS condition is HERE
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
					Left=0;Forward=1;Right=1;
					pass_turns();
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
					Left=1;Right=1;Forward=1;LT=1;
					pass_turns();
					if(left==1){left_L();}
					else if(right==1){right_L();}
				}
				else if(End==1){lcd_clear();lcd_write_string_xy(4,0,"END ZONE");bot_brake();set_pwm1a(400);set_pwm1b(400);flick();delay_sec(1000);}
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
				Left=1;Right=1;Forward=0;LT=1;
				pass_turns();
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
		set_pwm1a(270);//9
		set_pwm1b(270);//9
	}
	while(bit_is_set(PINA,5))
	{
		set_pwm1a(260);
		set_pwm1b(260);
	}
	while(bit_is_set(PINA,6))//clear
	{
		set_pwm1a(250);//6
		set_pwm1b(250);//6
	}
	bot_brake();
	set_pwm1a(400);
	set_pwm1b(400);
	_delay_ms(150);
	left=0;check=0;opt=270;flag_6=0;flag_7=0,Left=0,Forward=0,Right=0;LT=0;Plus=0;len_count=0;dir_count--;
	update_dir();lcd_clear();
}

void right_L()
{
	lcd_write_string_xy(6,1,"RIHT");
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
	while(bit_is_set(PINA,6))//5
	{
		set_pwm1a(260);
		set_pwm1b(270);
	}
	while(bit_is_set(PINA,5))//clear6
	{
		set_pwm1a(255);//5
		set_pwm1b(260);//55
	}
	bot_brake();
	set_pwm1a(400);
	set_pwm1b(400);
	_delay_ms(150);
	right=0;check=0;opt=270;flag_6=0;flag_7=0,Left=0,Forward=0,Right=0;LT=0;Plus=0;len_count=0;dir_count++;
	update_dir();lcd_clear();
}

void U_turn()
{
	update_map();
	while(bit_is_set(PIND,6))//setA4
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
	update_dir();lcd_clear();
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

	while(1)
	{			
		if(pressed_switch2())
		{
			lcd_write_string_xy(5,0,"START");
			_delay_ms(500);
			lcd_clear();
			while(1)
			{
				line_track();
				LED&=0b11110000;
				LED|=sensorbyte;
			}
		}
	}
	return 0;
}