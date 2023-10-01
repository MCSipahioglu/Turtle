#line 1 "D:/_Projects/TURTLE.v1/Simulations/Simulation 2 - SD & LCD & TEMP/Simulation2-Code/Simulation2.c"
#line 12 "D:/_Projects/TURTLE.v1/Simulations/Simulation 2 - SD & LCD & TEMP/Simulation2-Code/Simulation2.c"
sfr sbit LCD_EN_Direction at TRISA1_bit;
sfr sbit LCD_RS_Direction at TRISA2_bit;
sfr sbit SWITCH_TRIS at TRISA4_bit;
sfr sbit Mmc_Chip_Select_Direction at TRISA5_bit;





sfr sbit LCD_D4_Direction at TRISD4_bit;
sfr sbit LCD_D5_Direction at TRISD5_bit;
sfr sbit LCD_D6_Direction at TRISD6_bit;
sfr sbit LCD_D7_Direction at TRISD7_bit;




sfr sbit LCD_EN at RA1_bit;
sfr sbit LCD_RS at RA2_bit;
sfr sbit SWITCH at RA4_bit;
sfr sbit Mmc_Chip_Select at LATA5_bit;





sfr sbit LCD_D4 at RD4_bit;
sfr sbit LCD_D5 at RD5_bit;
sfr sbit LCD_D6 at RD6_bit;
sfr sbit LCD_D7 at RD7_bit;






unsigned int temp;
int i=0;
int j=0;
int error;
char Temperature[] = "00";




void setup_IO() {
 SWITCH_TRIS=1;
}

void setup_Init() {
 ADCON0=0b00000001;
 ADCON1=0b00001110;
 ADCON2=0b10000010;
#line 75 "D:/_Projects/TURTLE.v1/Simulations/Simulation 2 - SD & LCD & TEMP/Simulation2-Code/Simulation2.c"
 ADC_Init();
 Lcd_Init();
 Lcd_Cmd(_LCD_CURSOR_OFF);
 Lcd_Cmd(_LCD_CLEAR);
 Lcd_Out(1,1,"Hello");
 Delay_ms(1000);
}



void sd_demo(){
 temp = (ADC_Read(0))*500/1023;
 Mmc_Fat_Append();
 Temperature[0]='0'+ (temp/10)% 10;
 Temperature[1]='0'+ temp%10;
 Mmc_Fat_Write(Temperature,2);
}

void temp_lcd_demo(){
 if(i>j){
 Lcd_Init();
 Lcd_Cmd(_LCD_CURSOR_OFF);
 i=j;
 }

 temp = (ADC_Read(0))*500/1023;
 lcd_out(1, 1, "Temperature:");
 Temperature[0]='0'+ (temp/10)% 10;
 Temperature[1]='0'+ temp%10;
 Lcd_out(2,1, Temperature);
 Delay_ms(1000);
}




void main() {

 setup_Init();
 setup_IO();
 while(1){
 if (SWITCH==0){
 Lcd_cmd(_LCD_TURN_OFF);
 if(i==j)
 i++;
 }
 else
 temp_lcd_demo();



 }
}
