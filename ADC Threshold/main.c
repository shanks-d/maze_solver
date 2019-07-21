#include"sra.c"
// threshold 55

void calc_threshold()
{
	int sensor_1[20], sensor_2[20], sensor_3[20], sensor_4[20];
	int white_1, white_2, white_3, white_4, black_1, black_2, black_3, black_4;
	int temp_white, temp_black, white, black;
	int count=50, i=0;
	
	while(count!=0)
	{
		lcd_write_int_xy(7,0,i,2);
		sensor_1[i] = adc_start(0);
		sensor_2[i] = adc_start(1);
		sensor_3[i] = adc_start(2);
		sensor_4[i] = adc_start(3);
		
		_delay_ms(100);
		i++;count--;
	}
	
	temp_white = sensor_1[0];
	temp_black = sensor_1[0];
	for(int j=1;j<20;j++)
	{
		if(temp_white>sensor_1[j])
		{
			temp_white = sensor_1[j];
		}
		if(temp_black<sensor_1[j])
		{
			temp_black = sensor_1[j];
		}
	}
	white_1 = temp_white;
	black_1 = temp_black;

	temp_white = sensor_2[0];
	temp_black = sensor_2[0];
	for(int j=1;j<20;j++)
	{
		if(temp_white>sensor_2[j])
		{
			temp_white = sensor_2[j];
		}
		if(temp_black<sensor_2[j])
		{
			temp_black = sensor_2[j];
		}
	}
	white_2 = temp_white;
	black_2 = temp_black;
	
	temp_white = sensor_3[0];
	temp_black = sensor_3[0];
	for(int j=1;j<20;j++)
	{
		if(temp_white>sensor_3[j])
		{
			temp_white = sensor_3[j];
		}
		if(temp_black<sensor_3[j])
		{
			temp_black = sensor_3[j];
		}
	}
	white_3 = temp_white;
	black_3 = temp_black;

	temp_white = sensor_4[0];
	temp_black = sensor_4[0];
	for(int j=1;j<20;j++)
	{
		if(temp_white>sensor_4[j])
		{
			temp_white = sensor_4[j];
		}
		if(temp_black<sensor_4[j])
		{
			temp_black = sensor_4[j];
		}
	}
	white_4 = temp_white;
	black_4 = temp_black;
	white = (int)(white_1 + white_2 + white_3 + white_4)/4;
	black = (int)(black_1 + black_2 + black_3 + black_4)/4;
	
	lcd_clear();
	lcd_write_int_xy(0,0,white_1,3);
	lcd_write_int_xy(4,0,white_2,3);
	lcd_write_int_xy(8,0,white_3,3);
	lcd_write_int_xy(12,0,white_4,3);
	lcd_write_int_xy(0,1,black_1,3);
	lcd_write_int_xy(4,1,black_2,3);
	lcd_write_int_xy(8,1,black_3,3);
	lcd_write_int_xy(12,1,black_4,3);
	delay_sec(5);
	lcd_clear();
	lcd_write_int_xy(0,0,white,3);
	lcd_write_int_xy(13,0,black,3);
	delay_sec(100);
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
		if(pressed_switch1())
		{
			calc_threshold();
		}
	}
	return 0;
}