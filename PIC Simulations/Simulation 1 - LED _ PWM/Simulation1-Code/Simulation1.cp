#line 1 "D:/_Projects/TURTLE.v1/Simulations/Simulation 1 - LED & PWM/Simulation1-Code/Simulation1.c"
#line 12 "D:/_Projects/TURTLE.v1/Simulations/Simulation 1 - LED & PWM/Simulation1-Code/Simulation1.c"
sfr sbit STBY_R_TRIS at TRISA2_bit;
sfr sbit STBY_L_TRIS at TRISA3_bit;


sfr sbit PWM_R_F_TRIS at TRISB0_bit;
sfr sbit PWM_R_B_TRIS at TRISB1_bit;
sfr sbit PWM_L_F_TRIS at TRISB2_bit;
sfr sbit PWM_L_B_TRIS at TRISB3_bit;
sfr sbit LED1_TRIS at TRISB4_bit;
sfr sbit LED2_TRIS at TRISB5_bit;


sfr sbit DIR_R_F_1_TRIS at TRISC0_bit;
sfr sbit DIR_R_F_2_TRIS at TRISC1_bit;
sfr sbit DIR_R_B_1_TRIS at TRISC2_bit;
sfr sbit DIR_R_B_2_TRIS at TRISC3_bit;


sfr sbit DIR_L_F_1_TRIS at TRISD0_bit;
sfr sbit DIR_L_F_2_TRIS at TRISD1_bit;
sfr sbit DIR_L_B_1_TRIS at TRISD2_bit;
sfr sbit DIR_L_B_2_TRIS at TRISD3_bit;


sfr sbit STBY_R at RA2_bit;
sfr sbit STBY_L at RA3_bit;


sfr sbit PWM_R_F at RB0_bit;
sfr sbit PWM_R_B at RB1_bit;
sfr sbit PWM_L_F at RB2_bit;
sfr sbit PWM_L_B at RB3_bit;
sfr sbit LED1 at RB4_bit;
sfr sbit LED2 at RB5_bit;


sfr sbit DIR_R_F_1 at RC0_bit;
sfr sbit DIR_R_F_2 at RC1_bit;
sfr sbit DIR_R_B_1 at RC2_bit;
sfr sbit DIR_R_B_2 at RC3_bit;


sfr sbit DIR_L_F_1 at RD0_bit;
sfr sbit DIR_L_F_2 at RD1_bit;
sfr sbit DIR_L_B_1 at RD2_bit;
sfr sbit DIR_L_B_2 at RD3_bit;



void setup_IO() {
 STBY_R_TRIS=0;
 STBY_L_TRIS=0;
 PWM_R_F_TRIS=0;
 PWM_R_B_TRIS=0;
 PWM_L_F_TRIS=0;
 PWM_L_B_TRIS=0;
 LED1_TRIS=0;
 LED2_TRIS=0;
 DIR_R_F_1_TRIS=0;
 DIR_R_F_2_TRIS=0;
 DIR_R_B_1_TRIS=0;
 DIR_R_B_2_TRIS=0;
 DIR_L_F_1_TRIS=0;
 DIR_L_F_2_TRIS=0;
 DIR_L_B_1_TRIS=0;
 DIR_L_B_2_TRIS=0;
}

void setup_Init() {
 PTCON0=0b00000000;
 PTCON1=0b10000000;
 PWMCON0=0b00111111;
 PWMCON1=0b00000001;
 PTMRH=0b00000000;
 PTMRL=0b00000000;
 PTPERH=0b00000001;
 PTPERL=0b11111111;


 ANSEL0=0x00;
 ANSEL1=0x00;
}



void pwm_demo(){
 int pwm=0;
 STBY_R=1;
 STBY_L=1;
 DIR_R_F_1=1;
 DIR_R_F_2=0;
 DIR_R_B_1=0;
 DIR_R_B_2=1;
 DIR_L_F_1=1;
 DIR_L_F_2=0;
 DIR_L_B_1=0;
 DIR_L_B_2=1;




 for(pwm=1;pwm<4096;pwm++){
 PDC0H= ((unsigned char)(((4096-pwm)>>8)&0xFF)) ;
 PDC0L= ((unsigned char)((4096-pwm)&0xFF)) ;
 PDC1H= ((unsigned char)(((4096-pwm)>>8)&0xFF)) ;
 PDC1L= ((unsigned char)((4096-pwm)&0xFF)) ;
 Delay_us(10);
 }
}


void main() {
 setup_IO();
 setup_Init();

 LED1=1;
 LED2=0;

 while(1){

 pwm_demo();
 LED1= !LED1;
 LED2= !LED2;
 }
}
