#include "./Group50/Group50.h"
int step_x, step_y;
int count = 0;
int sign = 0;
TCHAR buffer[10];
uint8_t retry = 0;

unsigned int Task_Delay[NumOfTask]; 

BLTDev bltDevList;


char sendData[1024];
char linebuff[1024];

char* redata;
uint16_t len;

static uint8_t hc05_role=0;
unsigned long count_2 = 0;

char hc05_mode[10]="SLAVE";
char hc05_name[30]="HC05_SLAVE";
char hc05_nameCMD[40];
char disp_buff[200];

extern uint8_t Ov7725_vsync;

extern void jpgDisplay(char *pic_name);

extern __IO uint16_t prescaler[4];
extern int  pulse_count_1;
extern int  pulse_count_2;
extern FATFS fs;				


extern FIL fnew;
extern char speed[10];              
extern UINT fnum;            					 
extern char ReadBuffer[10];
extern BYTE WriteBuffer[];
extern strType_XPT2046_Coordinate coordinate;

extern FRESULT res_sd;                
extern OV7725_MODE_PARAM cam_mode;

float speed_1, speed_2 = 0;

void slice_str(const char * str, char * buffer, size_t start, size_t end)
{
    size_t j = 0;
    for ( size_t i = start; i <= end; ++i ) {
        buffer[j++] = str[i];
    }
    buffer[j] = 0;
}

void Delayus_3(int duration)
{
		while(duration--) 
		{
			int i=0x02;				
			while(i--)
			__asm("nop");
		}
}

void ILI9341_DrawFourSquare(void){
	ILI9341_DrawRectangle(100, 50, 40, 40, 0);
	ILI9341_DrawRectangle(50, 100, 40, 40, 0);
	ILI9341_DrawRectangle(150, 100, 40, 40, 0);
	ILI9341_DrawRectangle(100, 150, 40, 40, 0);
	ILI9341_DrawRectangle(180, 20, 40, 40, 0);
	ILI9341_DrawRectangle(20, 200, 40, 40, 0);
	ILI9341_DrawRectangle(180, 200, 40, 40, 0);
	ILI9341_DrawRectangle(20, 250,40, 40, 0);
	ILI9341_DrawRectangle(180, 250, 40, 40, 0);
	
	
	ILI9341_DispString_EN(110,60,"+Y");
	ILI9341_DispString_EN(60,110,"-X");
	ILI9341_DispString_EN(160,110,"+X");
	ILI9341_DispString_EN(110,160,"-Y");
	ILI9341_DispString_EN(190,30,"<-");
	ILI9341_DispString_EN(30,210,"UP");
	ILI9341_DispString_EN(185,210,"DOWN");
	ILI9341_DispString_EN(35,260,"+");
	ILI9341_DispString_EN(195,260,"-");
}

void ILI9341_DrawMainMenu(){
	ILI9341_DrawRectangle(50, 50, 120, 40, 0);
	ILI9341_DrawRectangle(50, 120, 120, 40, 0);
	ILI9341_DrawRectangle(50, 190, 120, 40, 0);
	ILI9341_DrawRectangle(50, 260, 120, 40, 0);
	ILI9341_DispString_EN(60,60,"Camera Mode");
	ILI9341_DispString_EN(60,130,"Control Mode");
	ILI9341_DispString_EN(60,200,"USB Mode");
	ILI9341_DispString_EN(60,270,"SD Card File");
}

void Camera_Mode(){
	ILI9341_Clear(0,0,ILI9341_LESS_PIXEL,ILI9341_MORE_PIXEL);
	ILI9341_DispString_EN ( 40, 60, "Camera Mode" );
	ILI9341_DispString_EN ( 40, 80, "Press K1 to capture" );
	ILI9341_DispString_EN ( 40, 100, "Press K2 to return" );
	Delayus_3(5000000);
	
	
	OV7725_GPIO_Config();
	
	LED_BLUE;
	while(OV7725_Init() != SUCCESS)
	{
		retry++;
		if(retry>5)
		{
			ILI9341_DispStringLine_EN(LINE(2),"No OV7725 module detected!");
			while(1);
		}
	}


	OV7725_Special_Effect(cam_mode.effect);
	OV7725_Light_Mode(cam_mode.light_mode);
	OV7725_Color_Saturation(cam_mode.saturation);
	OV7725_Brightness(cam_mode.brightness);
	OV7725_Contrast(cam_mode.contrast);
	OV7725_Special_Effect(cam_mode.effect);
	OV7725_Window_Set(cam_mode.cam_sx,
														cam_mode.cam_sy,
														cam_mode.cam_width,
														cam_mode.cam_height,
														cam_mode.QVGA_VGA);
	
	
	
	ILI9341_DispStringLine_EN(LINE(2),"OV7725 initialize success!");
	
	Ov7725_vsync = 0;
	ILI9341_Clear(0,0,ILI9341_LESS_PIXEL,ILI9341_MORE_PIXEL);
	ILI9341_GramScan( cam_mode.lcd_scan );
	while(1){
		if( Ov7725_vsync == 2 )
		{
			FIFO_PREPARE;			
			ImagDisp(cam_mode.lcd_sx,
								cam_mode.lcd_sy,
								cam_mode.cam_width,
								cam_mode.cam_height);
			
			Ov7725_vsync = 0;

		}
		
		if( Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON  )
		{		
			static uint8_t name_count = 0;
			char name[40];
			
			name_count++;
			sprintf(name,"0:photo_%d.bmp",name_count);

			LED_BLUE;
			
			ILI9341_GramScan ( cam_mode.lcd_scan );			
			
			if(Screen_Shot(0,0,LCD_X_LENGTH,LCD_Y_LENGTH,name) == 0)
			{
				LED_GREEN;
			}
			else
			{
				LED_RED;
			}
		}
		if( Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN) == KEY_ON  )
		{
			ILI9341_GramScan(6);
			ILI9341_Clear(0,0,ILI9341_LESS_PIXEL,ILI9341_MORE_PIXEL);
			//count= 0;
			return;	
		}
	}
}


void Bluetooth_Control(){
	ILI9341_Clear(0,0,ILI9341_LESS_PIXEL,ILI9341_MORE_PIXEL);
	ILI9341_DispString_EN( 40, 20,"BLUETOOTH MODE"  );
	ILI9341_DispString_EN( 40, 40,"Press K2 to return"  );
	if(HC05_Init() == 0)
	{
		ILI9341_DispString_EN ( 40, 60, "HC05 module detected!" );

	}
	else
	{
		ILI9341_DispString_EN ( 20, 60, "No HC05 module detected!"  );
		ILI9341_DispString_EN ( 5, 80, "Please check the hardware connection and reset the system." );

		while(1);
	}
	HC05_Send_CMD("AT+RESET\r\n",1);	
	delay_ms(800);
	
	HC05_Send_CMD("AT+ORGL\r\n",1);
	delay_ms(200);
	
	HC05_Send_CMD("AT+INIT\r\n",1);
	HC05_Send_CMD("AT+CLASS=0\r\n",1);
	HC05_Send_CMD("AT+INQM=1,9,48\r\n",1);
	
	sprintf(hc05_nameCMD,"AT+NAME=%s\r\n",hc05_name);
	HC05_Send_CMD(hc05_nameCMD,1);

	sprintf(disp_buff,"Device name:%s",hc05_name);
	ILI9341_DispString_EN( 30, 80, disp_buff );

	while(1){
		if( Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN) == KEY_ON  )
		{
			ILI9341_Clear(0,0,ILI9341_LESS_PIXEL,ILI9341_MORE_PIXEL);
			break;
		}
		if(Task_Delay[2]==0 && !IS_HC05_CONNECTED() ) 
		{
					HC05_Send_CMD("AT+INQ\r\n",1);
					delay_ms(1000);
					HC05_Send_CMD("AT+INQC\r\n",1);

					ILI9341_Clear(0,80,240,240);
						
					Task_Delay[2]=2000;
		}
		sprintf(disp_buff,"Device name:%s",hc05_name);
		ILI9341_DispString_EN( 30, 80, disp_buff );		
		
		if(Task_Delay[0]==0 && IS_HC05_CONNECTED())  
		{
				ILI9341_DispString_EN( 40, 240,"Send +X, -X, +Y, -Y"  );
				ILI9341_DispString_EN( 40, 260,"and UP, DOWN"  );
				ILI9341_DispString_EN( 40, 280,"To Control the Pen"  );
				uint16_t linelen;
				LCD_SetColors(YELLOW,BLACK);
			
				ILI9341_Clear(0,80,240,20);
				ILI9341_DispString_EN( 40, 100,"Bluetooth connected!"  );


				redata = get_rebuff(&len); 
				linelen = get_line(linebuff,redata,len);
			
				if(linelen<200 && linelen != 0)
				{
					
					LCD_SetColors(YELLOW,BLACK);

					if(strcmp(redata,"AT+LED=RED")==0)
					{
						LED_RED;						
						HC05_SendString("+LED1:RED\r\nOK\r\n");	
						
						ILI9341_Clear(0,100,240,20);						
						ILI9341_DispString_EN ( 15, 120, "receive cmd: AT+LED1=RED" );
					}
					else if(strcmp(redata,"AT+LED=GREEN")==0)
					{
						LED_GREEN;
						HC05_SendString("+LED1:GREEN\r\nOK\r\n");
						
						ILI9341_Clear(0,100,240,20);
						ILI9341_DispString_EN ( 15, 120, "receive cmd: AT+LED=GREEN" );
					}else if(strncmp(redata,"+X",2)==0 || strncmp(redata,"+x",2)==0){
						ILI9341_Clear(0,160,240,20);
						ILI9341_DispString_EN ( 40, 160, redata );
						len = strlen(redata);
						if (len > 3){
							slice_str(redata, buffer, 3, len-4);
							Motor_Move_X(atoi(buffer));
						}
						Motor_Move_X(10);
					}else if(strncmp(redata,"-X",2)==0 || strncmp(redata,"-x",2)==0){
						ILI9341_Clear(0,160,240,20);
						ILI9341_DispString_EN ( 40, 160, redata );
						len = strlen(redata);
						if (len > 3){
							slice_str(redata, redata, 3, len-4);
							Motor_Move_X(-atoi(redata));
						}
						Motor_Move_X(-10);
					}else if(strncmp(redata,"+Y",2)==0 || strncmp(redata,"+y",2)==0){
						ILI9341_Clear(0,160,240,20);
						ILI9341_DispString_EN ( 40, 160, redata );
						len = strlen(redata);
						if (len > 3){
							slice_str(redata, redata, 3, len-4);
							Motor_Move_Y(atoi(redata));
						}
						Motor_Move_Y(10);
					}else if(strncmp(redata,"-Y",2)==0 || strncmp(redata,"-y",2)==0){
						ILI9341_Clear(0,160,240,20);
						ILI9341_DispString_EN ( 40, 160, redata );
						len = strlen(redata);
						if (len > 3){
							slice_str(redata, redata, 3, len-4);
							Motor_Move_Y(-atoi(redata));
						}
						Motor_Move_Y(-10);
					}else if(strcmp(redata,"UP") ==0 || strcmp(redata,"up")==0){
						LED_PURPLE;
						ILI9341_Clear(0,160,240,20);
						TIM_SetCompare1(GENERAL_TIMx,50);
						Delayus_3(500000);
						TIM_SetCompare1(GENERAL_TIMx,150);
						LED_BLUE;
					}else if(strcmp(redata,"DOWN") ==0|| strcmp(redata,"down")==0){
						LED_PURPLE;
						ILI9341_Clear(0,160,240,20);
						TIM_SetCompare1(GENERAL_TIMx,250);
						Delayus_3(500000);
						TIM_SetCompare1(GENERAL_TIMx,150);
						LED_BLUE;
					}else{
						ILI9341_Clear(0,100,240,20);
						ILI9341_DispString_EN ( 40, 160, "Pls Check Input!" );
					}
					
					clean_rebuff();
				}
			Task_Delay[0]=500;
		}
	}
}

void Touch_Control(){
	ILI9341_Clear(0,0,ILI9341_LESS_PIXEL,ILI9341_MORE_PIXEL);
	while(1){
		XPT2046_TouchEvenHandler(&coordinate);
		ILI9341_DrawFourSquare();
		sprintf(speed, "speed: %d", prescaler[0]);
		ILI9341_DispString_EN(80,260,"            ");
		ILI9341_DispString_EN(80,260,speed);
		if((coordinate.x > 100 && coordinate.x <140)&&(coordinate.y > 50 && coordinate.y<90)){
			GPIO_SetBits(TB6600_ENA1_PORT,TB6600_ENA1_PIN);
			GPIO_SetBits(TB6600_DIR1_PORT,TB6600_DIR1_PIN);
			GPIO_SetBits(TB6600_ENA2_PORT,TB6600_ENA2_PIN);
			GPIO_ResetBits(TB6600_DIR2_PORT,TB6600_DIR2_PIN);
			ILI9341_DispString_EN(100,100,"+Y");
		}else if((coordinate.x > 50 && coordinate.x <90)&&(coordinate.y > 100 && coordinate.y <140)){
			GPIO_SetBits(TB6600_ENA1_PORT,TB6600_ENA1_PIN);
			GPIO_ResetBits(TB6600_DIR1_PORT,TB6600_DIR1_PIN);
			GPIO_SetBits(TB6600_ENA2_PORT,TB6600_ENA2_PIN);
			GPIO_ResetBits(TB6600_DIR2_PORT,TB6600_DIR2_PIN);
			ILI9341_DispString_EN(100,100,"-X");
		}else if((coordinate.x > 150 && coordinate.x <190)&&(coordinate.y > 100 && coordinate.y <140)){
			GPIO_SetBits(TB6600_ENA1_PORT,TB6600_ENA1_PIN);
			GPIO_SetBits(TB6600_DIR1_PORT,TB6600_DIR1_PIN);
			GPIO_SetBits(TB6600_ENA2_PORT,TB6600_ENA2_PIN);
			GPIO_SetBits(TB6600_DIR2_PORT,TB6600_DIR2_PIN);
			ILI9341_DispString_EN(100,100,"+X");
		}else if((coordinate.x > 100 && coordinate.x <140)&&(coordinate.y > 150 && coordinate.y <190)){
			GPIO_SetBits(TB6600_ENA1_PORT,TB6600_ENA1_PIN);
			GPIO_ResetBits(TB6600_DIR1_PORT,TB6600_DIR1_PIN);
			GPIO_SetBits(TB6600_ENA2_PORT,TB6600_ENA2_PIN);
			GPIO_SetBits(TB6600_DIR2_PORT,TB6600_DIR2_PIN);
			ILI9341_DispString_EN(100,100,"-Y");
		}else if((coordinate.x > 180 && coordinate.x <220)&&(coordinate.y > 20 && coordinate.y < 60)){
			ILI9341_Clear(0,0,ILI9341_LESS_PIXEL,ILI9341_MORE_PIXEL);
			return;
		}else if((coordinate.x > 20 && coordinate.x <60)&&(coordinate.y > 200 && coordinate.y < 240)){
			TIM_SetCompare1(GENERAL_TIMx,50);
			Delayus_3(200000);
			TIM_SetCompare1(GENERAL_TIMx,150);
		}else if((coordinate.x > 180 && coordinate.x <220)&&(coordinate.y > 200 && coordinate.y < 240)){
			TIM_SetCompare1(GENERAL_TIMx,250);
			Delayus_3(200000);
			TIM_SetCompare1(GENERAL_TIMx,150);
		}else if((coordinate.x > 20 && coordinate.x <60)&&(coordinate.y > 250 && coordinate.y < 290)){
			prescaler[0] -= 100;
			prescaler[1] -= 100;
			if(prescaler[0] < 650){
				prescaler[0] = 650;
			}
			if(prescaler[1] < 650){
				prescaler[1] = 650;
			}
			Delayus_3(500000);
		}else if((coordinate.x > 180 && coordinate.x <220)&&(coordinate.y > 250 && coordinate.y < 290)){
			prescaler[0] += 100;
			prescaler[1] += 100;
			if(prescaler[0] > 3500){
				prescaler[0] = 3500;
			}
			if(prescaler[1] > 3500){
				prescaler[1] = 3500;
			}
			Delayus_3(500000);
		}else{
			ILI9341_DispString_EN(100,100,"NA");
			GPIO_ResetBits(TB6600_ENA1_PORT,TB6600_ENA1_PIN);
			GPIO_ResetBits(TB6600_ENA2_PORT,TB6600_ENA2_PIN);
		}
	}
}

void Control_Mode(){
		ILI9341_Clear(0,0,ILI9341_LESS_PIXEL,ILI9341_MORE_PIXEL);
	
		while(1)
		{
			ILI9341_DrawRectangle(50, 50, 120, 40, 0);
			ILI9341_DrawRectangle(50, 180, 120, 40, 0);
			ILI9341_DrawRectangle(180, 20, 40, 40, 0);
			ILI9341_DispString_EN(60,60,"Touch Control");
			ILI9341_DispString_EN(60,190,"Bluetooth");
			ILI9341_DispString_EN(190,30,"<-");
			LCD_SetColors(RED,BLACK);
			XPT2046_TouchEvenHandler(&coordinate);
			
			if ((coordinate.x > 50 && coordinate.x <170)&&(coordinate.y > 50 && coordinate.y<90)){
				Touch_Control();
			}else if((coordinate.x > 50 && coordinate.x <170)&&(coordinate.y > 180 && coordinate.y < 220)){
				Bluetooth_Control();
			}else if((coordinate.x > 180 && coordinate.x <220)&&(coordinate.y > 20 && coordinate.y < 60)){
				ILI9341_Clear(0,0,ILI9341_LESS_PIXEL,ILI9341_MORE_PIXEL);
				return;
			}
		}
	}

	
void Motor_Move_Y(int step){
	pulse_count_1 = 0;
	pulse_count_2 = 0;
	while(1){
		if(step<0){
			GPIO_SetBits(TB6600_ENA1_PORT,TB6600_ENA1_PIN);
			GPIO_ResetBits(TB6600_DIR1_PORT,TB6600_DIR1_PIN);
			GPIO_SetBits(TB6600_ENA2_PORT,TB6600_ENA2_PIN);
			GPIO_SetBits(TB6600_DIR2_PORT,TB6600_DIR2_PIN);
			if(pulse_count_1 >= - ONE_STEP * step && pulse_count_2 >= - ONE_STEP * step)
			{
				pulse_count_1 = 0;
				pulse_count_2 = 0;
				GPIO_ResetBits(TB6600_ENA1_PORT,TB6600_ENA1_PIN);
				GPIO_ResetBits(TB6600_ENA2_PORT,TB6600_ENA2_PIN);
				break;
			} 

		}else if (step>0){
			GPIO_SetBits(TB6600_ENA1_PORT,TB6600_ENA1_PIN);
			GPIO_SetBits(TB6600_DIR1_PORT,TB6600_DIR1_PIN);
			GPIO_SetBits(TB6600_ENA2_PORT,TB6600_ENA2_PIN);
			GPIO_ResetBits(TB6600_DIR2_PORT,TB6600_DIR2_PIN);
			if(pulse_count_1 >= ONE_STEP * step && pulse_count_2 >= ONE_STEP * step)
			{
				pulse_count_1 = 0;
				pulse_count_2 = 0;
				GPIO_ResetBits(TB6600_ENA1_PORT,TB6600_ENA1_PIN);
				GPIO_ResetBits(TB6600_ENA2_PORT,TB6600_ENA2_PIN);
				break;
			} 
		}else{
			break;
		}
	}
}
void Motor_Move_X(int step){
	pulse_count_1 = 0;
	pulse_count_2 = 0;
	while(1){
		if(step<0){
			GPIO_SetBits(TB6600_ENA1_PORT,TB6600_ENA1_PIN);
			GPIO_SetBits(TB6600_DIR1_PORT,TB6600_DIR1_PIN);
			GPIO_SetBits(TB6600_ENA2_PORT,TB6600_ENA2_PIN);
			GPIO_SetBits(TB6600_DIR2_PORT,TB6600_DIR2_PIN);
			if(pulse_count_1 >= - ONE_STEP * step && pulse_count_2 >= - ONE_STEP * step)
			{
				pulse_count_1 = 0;
				pulse_count_2 = 0;
				GPIO_ResetBits(TB6600_ENA1_PORT,TB6600_ENA1_PIN);
				GPIO_ResetBits(TB6600_ENA2_PORT,TB6600_ENA2_PIN);
				break;
			} 

		}else if (step>0){
			GPIO_SetBits(TB6600_ENA1_PORT,TB6600_ENA1_PIN);
			GPIO_ResetBits(TB6600_DIR1_PORT,TB6600_DIR1_PIN);
			GPIO_SetBits(TB6600_ENA2_PORT,TB6600_ENA2_PIN);
			GPIO_ResetBits(TB6600_DIR2_PORT,TB6600_DIR2_PIN);
			if(pulse_count_1 >= ONE_STEP * step && pulse_count_2 >= ONE_STEP * step)
			{
				pulse_count_1 = 0;
				pulse_count_2 = 0;
				GPIO_ResetBits(TB6600_ENA1_PORT,TB6600_ENA1_PIN);
				GPIO_ResetBits(TB6600_ENA2_PORT,TB6600_ENA2_PIN);
				break;
			} 
		}else{
			break;
		}
	}
}

void Set_Speed_X_times_Y(float rate){
	if(rate > 3500/650){
		prescaler[0] = 650;
		prescaler[1] = 3500;
	}else{
		prescaler[0] = (int)3500/rate;
		prescaler[1] = 3500;
	}
}
void Set_Speed_Y_times_X(float rate){
	if(rate > 3500/650){
		prescaler[1] = 650;
		prescaler[0] = 3500;
	}else{
		prescaler[1] = (int)3500/rate;
		prescaler[0] = 3500;
	}
}

void Motor_Move_1(int step){
	pulse_count_1 = 0;
	GPIO_SetBits(TB6600_ENA1_PORT,TB6600_ENA1_PIN);
	while(1){
		if(step>0){
			GPIO_SetBits(TB6600_DIR1_PORT,TB6600_DIR1_PIN);
		}else{
			GPIO_ResetBits(TB6600_DIR1_PORT,TB6600_DIR1_PIN);
		}
		if(pulse_count_1 >= ONE_STEP * step)
			{
				pulse_count_1 = 0;
				GPIO_ResetBits(TB6600_ENA1_PORT,TB6600_ENA1_PIN);
				break;
			}
	}
}
void Motor_Move_2(int step){
	pulse_count_2 = 0;
	GPIO_SetBits(TB6600_ENA2_PORT,TB6600_ENA2_PIN);
	while(1){
		if(step>0){
			GPIO_SetBits(TB6600_DIR2_PORT,TB6600_DIR2_PIN);
		}else{
			GPIO_ResetBits(TB6600_DIR2_PORT,TB6600_DIR2_PIN);
		}
		if(pulse_count_2 >= ONE_STEP * step)
			{
				pulse_count_2 = 0;
				GPIO_ResetBits(TB6600_ENA2_PORT,TB6600_ENA2_PIN);
				break;
			}
	}
}

void Motor_Move_XY(int x, int y){
	speed_1 = -x+y;
	speed_2 = -x-y;
	if(speed_1 == 0){
		// Move Motor 2 only
		pulse_count_2 = 0;
		while(1){
			GPIO_SetBits(TB6600_ENA2_PORT,TB6600_ENA2_PIN);
			if (speed_2 > 0){
				GPIO_SetBits(TB6600_DIR2_PORT,TB6600_DIR2_PIN);
			}else{
				GPIO_ResetBits(TB6600_DIR2_PORT,TB6600_DIR2_PIN);
			}
			if(pulse_count_2 >= ONE_STEP * (int)fabs(speed_2))
			{
				pulse_count_2 = 0;
				GPIO_ResetBits(TB6600_ENA2_PORT,TB6600_ENA2_PIN);
				break;
			} 
		}
	}else if(speed_2 == 0){
		// Move Motor 1 only
		pulse_count_1 = 0;
		while(1){
			GPIO_SetBits(TB6600_ENA1_PORT,TB6600_ENA1_PIN);
			if (speed_1 > 0){
				GPIO_SetBits(TB6600_DIR1_PORT,TB6600_DIR1_PIN);
			}else{
				GPIO_ResetBits(TB6600_DIR1_PORT,TB6600_DIR1_PIN);
			}
			if(pulse_count_1 >= ONE_STEP * (int)fabs(speed_1))
			{
				pulse_count_1 = 0;
				GPIO_ResetBits(TB6600_ENA1_PORT,TB6600_ENA1_PIN);
				break;
			} 
		}
	}else if ((fabs(speed_1)/fabs(speed_2) > 3500/650) || (fabs(speed_2)/fabs(speed_1) > 3500/650)){
		if(abs(x) > abs(y)){
			Motor_Move_XY(x*abs(y)/abs(x),y);
			Motor_Move_X(x-x*abs(y)/abs(x));
		}else{
			Motor_Move_XY(x,abs(x)*y/abs(y));
			Motor_Move_Y(y-abs(x)*y/abs(y));
		}
	}else if(fabs(speed_1)>fabs(speed_2)){
		// Set the speed of motor 2 to be 1 
		Set_Speed_X_times_Y(fabs(speed_1)/fabs(speed_2));
		pulse_count_1 = 0;
		pulse_count_2 = 0;
		while(1){
			GPIO_SetBits(TB6600_ENA1_PORT,TB6600_ENA1_PIN);
			GPIO_SetBits(TB6600_ENA2_PORT,TB6600_ENA2_PIN);
			if (speed_1 > 0){
				GPIO_SetBits(TB6600_DIR1_PORT,TB6600_DIR1_PIN);
			}else{
				GPIO_ResetBits(TB6600_DIR1_PORT,TB6600_DIR1_PIN);
			}
			if (speed_2 > 0){
				GPIO_SetBits(TB6600_DIR2_PORT,TB6600_DIR2_PIN);
			}else{
				GPIO_ResetBits(TB6600_DIR2_PORT,TB6600_DIR2_PIN);
			}
			if(pulse_count_2 >= ONE_STEP * (int)fabs(speed_2))
			{
				pulse_count_1 = 0;
				pulse_count_2 = 0;
				GPIO_ResetBits(TB6600_ENA1_PORT,TB6600_ENA1_PIN);
				GPIO_ResetBits(TB6600_ENA2_PORT,TB6600_ENA2_PIN);
				break;
			}
		}		
	}else if(fabs(speed_1)<fabs(speed_2)){
		// Set the speed of motor 1 to be 1 
		Set_Speed_Y_times_X(fabs(speed_2)/fabs(speed_1));
		pulse_count_1 = 0;
		pulse_count_2 = 0;
		while(1){
			GPIO_SetBits(TB6600_ENA1_PORT,TB6600_ENA1_PIN);
			GPIO_SetBits(TB6600_ENA2_PORT,TB6600_ENA2_PIN);
			if (speed_1 > 0){
				GPIO_SetBits(TB6600_DIR1_PORT,TB6600_DIR1_PIN);
			}else{
				GPIO_ResetBits(TB6600_DIR1_PORT,TB6600_DIR1_PIN);
			}
			if (speed_2 > 0){
				GPIO_SetBits(TB6600_DIR2_PORT,TB6600_DIR2_PIN);
			}else{
				GPIO_ResetBits(TB6600_DIR2_PORT,TB6600_DIR2_PIN);
			}
			if(pulse_count_1 >= ONE_STEP * (int)fabs(speed_1))
			{
				pulse_count_1 = 0;
				pulse_count_2 = 0;
				GPIO_ResetBits(TB6600_ENA1_PORT,TB6600_ENA1_PIN);
				GPIO_ResetBits(TB6600_ENA2_PORT,TB6600_ENA2_PIN);
				break;
			} 
		}
	}
}

void Execute_Command(int num){
	USART_Config();
	ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);
	LED_RED;
	count = 0;
	LED_BLUE;
	if(num == 1){
		f_open(&fnew, "0:/drawing_1.txt" ,FA_OPEN_EXISTING | FA_READ );
		ILI9341_GramScan ( 6 );
		LCD_Show_BMP(0,0,"0:1.bmp");  
	} else if (num == 2){
		f_open(&fnew, "0:/drawing_3.txt" ,FA_OPEN_EXISTING | FA_READ );
		LCD_Show_BMP(0,0,"0:3.bmp");  
	}else if (num == 3){
		ILI9341_GramScan( cam_mode.lcd_scan );
		f_open(&fnew, "0:/drawing_4.txt" ,FA_OPEN_EXISTING | FA_READ );
		LCD_Show_BMP(0,0,"0:photo_1.bmp");  
	}
		ILI9341_GramScan(6);
	Delayus_3(10000000);
	ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);
	if(res_sd == FR_OK)
	{
		LED_YELLOW;
		ILI9341_DispString_EN(20,20,"            ");
		ILI9341_DispString_EN(20,20,"drawing");
		TIM_SetCompare1(GENERAL_TIMx,160);
		while(f_gets(ReadBuffer, sizeof(ReadBuffer), &fnew) != 0){
			prescaler[0] = 3500;
			prescaler[1] = 3500;
		  XPT2046_TouchEvenHandler(&coordinate);
			ILI9341_DrawRectangle(180, 20, 40, 40, 0);
			ILI9341_DispString_EN(190,30,"<-");
			//Delayus_3(1000000);
			if((coordinate.x > 180 && coordinate.x <220)&&(coordinate.y > 20 && coordinate.y < 60)){
				ILI9341_Clear(0,0,ILI9341_LESS_PIXEL,ILI9341_MORE_PIXEL);
				break;
			}
			count++;
			sprintf(speed, "Loop %d", count);
			ILI9341_DispString_EN(80,60,"            ");
			ILI9341_DispString_EN(80,60,speed);
			
			if (count % 2 == 1){
				step_x = atoi(ReadBuffer);
				sprintf(speed, "Move X: %d", step_x);
				ILI9341_DispString_EN(80,140,"            ");
				ILI9341_DispString_EN(80,140,speed);
				Motor_Move_X(step_x);
			}else{	
				step_y = atoi(ReadBuffer);
				sprintf(speed, "Move Y: %d", step_y);
				ILI9341_DispString_EN(80,160,"            ");
				ILI9341_DispString_EN(80,160,speed);
				Motor_Move_Y(step_y);
			}
		}
	}
	else
	{
		LED_RED;
	}
	TIM_SetCompare1(GENERAL_TIMx,150);
	f_close(&fnew);
}

void Execute_Command_Move_Together(int num){
	USART_Config();
	ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);
	LED_RED;
	count = 0;
	LED_BLUE;
	if(num == 1){
		f_open(&fnew, "0:/drawing_1.txt" ,FA_OPEN_EXISTING | FA_READ );
		ILI9341_GramScan ( 6 );
		LCD_Show_BMP(0,0,"0:1.bmp");  
	} else if (num == 2){
		f_open(&fnew, "0:/drawing_3.txt" ,FA_OPEN_EXISTING | FA_READ );
		LCD_Show_BMP(0,0,"0:3.bmp");  
	}else if (num == 3){
		ILI9341_GramScan( cam_mode.lcd_scan );
		f_open(&fnew, "0:/drawing.txt" ,FA_OPEN_EXISTING | FA_READ );
		LCD_Show_BMP(0,0,"0:photo_1.bmp");  
	}
		ILI9341_GramScan(6);
	Delayus_3(10000000);
	ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);
	if(res_sd == FR_OK)
	{
		LED_YELLOW;
		ILI9341_DispString_EN(20,20,"            ");
		ILI9341_DispString_EN(20,20,"drawing");
		TIM_SetCompare1(GENERAL_TIMx,155);
		while(f_gets(ReadBuffer, sizeof(ReadBuffer), &fnew) != 0){
			prescaler[0] = 3500;
			prescaler[1] = 3500;
		  XPT2046_TouchEvenHandler(&coordinate);
			ILI9341_DrawRectangle(180, 20, 40, 40, 0);
			ILI9341_DispString_EN(190,30,"<-");
			//Delayus_3(1000000);
			if((coordinate.x > 180 && coordinate.x <220)&&(coordinate.y > 20 && coordinate.y < 60)){
				ILI9341_Clear(0,0,ILI9341_LESS_PIXEL,ILI9341_MORE_PIXEL);
				break;
			}
			count++;
			sprintf(speed, "Loop %d", count);
			ILI9341_DispString_EN(80,60,"            ");
			ILI9341_DispString_EN(80,60,speed);
			
			if (count % 2 == 1){
				step_x = atoi(ReadBuffer);
			}else{
				sprintf(speed, "Move X: %d", step_x);
				ILI9341_DispString_EN(80,140,"            ");
				ILI9341_DispString_EN(80,140,speed);
				step_y = atoi(ReadBuffer);
				sprintf(speed, "Move Y: %d", step_y);
				ILI9341_DispString_EN(80,160,"            ");
				ILI9341_DispString_EN(80,160,speed);
				if(step_x == 0){
					Motor_Move_Y(step_y);
				}else if (step_y == 0){
					Motor_Move_X(step_x);
				}else{
					Motor_Move_XY(step_x, step_y);
				}
			}
		}
	}
	else
	{
		LED_RED;
	}
	TIM_SetCompare1(GENERAL_TIMx,150);
	f_close(&fnew);
}

void SD_Card_Mode(){
	ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);
	ILI9341_DispString_EN(10,10,"Choose the File");
	ILI9341_DispString_EN(10,30,"You Want To Draw");
	while(1){
		XPT2046_TouchEvenHandler(&coordinate);
		ILI9341_DrawRectangle(180, 20, 40, 40, 0);
		ILI9341_DispString_EN(190,30,"<-");
		if((coordinate.x > 180 && coordinate.x <220)&&(coordinate.y > 20 && coordinate.y < 60)){
			ILI9341_Clear(0,0,ILI9341_LESS_PIXEL,ILI9341_MORE_PIXEL);
			break;
		}
		ILI9341_DrawRectangle(50, 80, 120, 40, 0);
		ILI9341_DrawRectangle(50, 150, 120, 40, 0);
		ILI9341_DrawRectangle(50, 220, 120, 40, 0);
		ILI9341_DispString_EN(70,90,"Zealot");
		ILI9341_DispString_EN(70,160,"Zerling");
		ILI9341_DispString_EN(70,230,"Camera");
		if((coordinate.x > 50 && coordinate.x < 170)&&(coordinate.y > 80 && coordinate.y < 120)){
			ILI9341_Clear(0,0,ILI9341_LESS_PIXEL,ILI9341_MORE_PIXEL);
			Execute_Command_Move_Together(1);
		}else if((coordinate.x > 50 && coordinate.x <170)&&(coordinate.y > 150 && coordinate.y <190)){
			ILI9341_Clear(0,0,ILI9341_LESS_PIXEL,ILI9341_MORE_PIXEL);
			Execute_Command_Move_Together(2);
		}else if((coordinate.x > 50 && coordinate.x <170)&&(coordinate.y > 150 && coordinate.y < 260)){
			ILI9341_Clear(0,0,ILI9341_LESS_PIXEL,ILI9341_MORE_PIXEL);
			Execute_Command_Move_Together(3);
		}
	}
}

void USB_Delay(__IO uint32_t nCount)
{
	for(; nCount != 0; nCount--);
}

void USB_Mode(){
	ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);
	ILI9341_DispString_EN(10,10,"IN USB MODE");
	ILI9341_DispString_EN(10,30,"Press K2 to Return");
	Set_System();
	Set_USBClock();
	
	USB_Interrupts_Config();
	USB_Init();
 
	while (bDeviceState != CONFIGURED);
	while (1)
	{
		LED2_TOGGLE;
		if( Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN) == KEY_ON  )
		{
			ILI9341_Clear(0,0,ILI9341_LESS_PIXEL,ILI9341_MORE_PIXEL);
			NVIC_SystemReset();
		}
		USB_Delay(0x0FFFFF);
	}
}

