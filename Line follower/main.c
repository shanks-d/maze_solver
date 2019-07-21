#include"sra.c"

int mpos=0,prempos=0, motor1_value=0, motor2_value=0;
unsigned int opt = 270;
float kp = 13;//33
float ki = 0.00;
float kd = 1.2;//1
float pid;
float p = 0,i = 0,d = 0;

void line_track(void)
{
	check_sensors();
	bot_forward();
	switch(sensorbyte)
	{
		case 0b0110:mpos=0; break;//sensor numbering starts from right
		
		case 0b0100:mpos = 1; break;
		
	case 0b0010:mpos = -1; break;
		
		case 0b1100:mpos = 3; break;
		
		case 0b0011:mpos = -3; break;
		
		case 0b1110:mpos = 4; break;
		
		case 0b0111:mpos = -4; break;
		
		case 0b1000:mpos = 6; break;
		
		case 0b0001:mpos = -6; break;
		
		case 0b0000: if(bit_is_set(PIND,6)&&bit_is_set(PINA,4)&&bit_is_set(PINA,5)&&bit_is_set(PINA,6)&&bit_is_set(PINA,7)&&bit_is_set(PIND,7)){lcd_clear();lcd_write_string_xy(1,0,"SHIT SHIT SHIT");lcd_write_string_xy(1,1,"SHIT SHIT SHIT");} break;
		
		default: break;
	}
	
	p = mpos * kp;
	i += mpos;
	d = kd * (mpos-prempos);
	pid = p + ki*i + d;
	motor1_value = opt + pid;//left motor is m1
	motor2_value = opt - pid;
	set_pwm1a(motor1_value);
	set_pwm1b(motor2_value);
	prempos=mpos;
	delay_microsec(10);
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
	lcd_write_int_xy(7,0,kd*10,2);
	lcd_write_int_xy(12,0,ki*100,2);
	//lcd_write_int_xy(7,1,opt,3);
	
	while(1)
	{			
		/*if(pressed_switch3())
		{
			while(1)
			{
				check_sensors();
				LED&=0b11110000;
				LED|=sensorbyte;
				delay_microsec(20);
			}
			
		}*/
		
		if(pressed_switch2())
		{
			while(1)
			{
				line_track();
				LED&=0b11110000;
				LED|=sensorbyte;
			}
		}
		
		if(pressed_switch3())
		{
			kp++;
			lcd_write_int_xy(2,0,kp,2);
			_delay_ms(150);
		}
		
		if(pressed_switch0())
		{
			ki-=0.01;
			lcd_write_int_xy(12,0,-ki*100,2);
			_delay_ms(150);
		}
		
		if(pressed_switch1())
		{
			ki+=0.01;
			lcd_write_int_xy(12,0,ki*100,2);
			_delay_ms(150);
		}
	}
	return 0;
}