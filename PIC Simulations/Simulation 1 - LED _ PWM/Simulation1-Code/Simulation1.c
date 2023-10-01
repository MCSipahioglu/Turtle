/*
  PIC Proof of Concept for LED & PWM, for ME331 F2020.
  Simulated with PIC18F4431.
  PWM duty cycle gets smaller and smaller until it resets, LEDs blink opposite to each other.
  by: M. Çaðatay Sipahioðlu
*/
//----------------------------------MACROS--------------------------------------
#define HIGH_BYTE(x) ((unsigned char)(((x)>>8)&0xFF))
#define LOW_BYTE(x) ((unsigned char)((x)&0xFF))

//------------------------------------PINS--------------------------------------
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


sfr sbit STBY_R at RA2_bit;            //Ports RA2-RA3 control motor standby.
sfr sbit STBY_L at RA3_bit;


sfr sbit PWM_R_F at RB0_bit;           //Ports RB0-RB1 control right front and back motor pwm.
sfr sbit PWM_R_B at RB1_bit;
sfr sbit PWM_L_F at RB2_bit;           //Ports RB2-RB3 control left front and back motor pwm.
sfr sbit PWM_L_B at RB3_bit;
sfr sbit LED1 at RB4_bit;              //Ports RB4-RB5 control the 2 LEDs.
sfr sbit LED2 at RB5_bit;


sfr sbit DIR_R_F_1 at RC0_bit;         //Ports RC0-1 control right front motor direction.
sfr sbit DIR_R_F_2 at RC1_bit;
sfr sbit DIR_R_B_1 at RC2_bit;         //Ports RC2-3 control right back motor direction.
sfr sbit DIR_R_B_2 at RC3_bit;


sfr sbit DIR_L_F_1 at RD0_bit;         //Ports RD0-1 control left front motor direction.
sfr sbit DIR_L_F_2 at RD1_bit;
sfr sbit DIR_L_B_1 at RD2_bit;         //Ports RD2-3 control left back motor direction.
sfr sbit DIR_L_B_2 at RD3_bit;


//------------------------------------SETUP-------------------------------------
void setup_IO() {                 //Setup_IO will assign input and output pins. (Output 0, Input 1)
     STBY_R_TRIS=0;               //OUTPUTS
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

void setup_Init() {               //Setup_Init will initialize anything that requires initialization.
     PTCON0=0b00000000;           //1:1 Post Scale, PWM input clock=Osc/4, Free Running Mode.
     PTCON1=0b10000000;           //PWM time base is on, counting up. Least significant 6 bits arenWt used.
     PWMCON0=0b00111111;          //PWM0-PWM3 (RB0-RB3) are in PWM mode, even pins are in complementary mode. (PWM0 to PWM3 are independent regardless.)
     PWMCON1=0b00000001;          //Special Event Registers somewhat default, Updates are enabled and synchronized to time base.
     PTMRH=0b00000000;            //PTMR keeps track of current base time in the pwm cycle, initialize to 0.
     PTMRL=0b00000000;            //PTMR will be resetting when it reaches PTPER. (period)
     PTPERH=0b00000001;           //PTPER=(T_Pwm*f_osc)/(4*PTMRPS) -1. PTMRPS=Prescaler=1.
     PTPERL=0b11111111;
     //PWM WORKING PRINCIPAL: PTPER sets the period. PTMR linearly reaches that period. While PDC>PTMR output will be high.
     //So PTMR starts from 0, duty cycle starts, when PTMR==PDC duty cycle ends. Bigger the PDC, longer the duty cycle.
     ANSEL0=0x00;                 //Analog multiplexes should be turned off otherwise there will arise problems.
     ANSEL1=0x00;
}

//----------------------------------FUNCTIONS-----------------------------------

void pwm_demo(){
     int pwm=0;                   //Pwm demo by changing the pwm signal and seeing the output from the PROTEUS oscillator.
     STBY_R=1;                    //STBY should be high for the motors to work.
     STBY_L=1;                    //Set an initial direction for the motors.
     DIR_R_F_1=1;
     DIR_R_F_2=0;
     DIR_R_B_1=0;
     DIR_R_B_2=1;
     DIR_L_F_1=1;
     DIR_L_F_2=0;
     DIR_L_B_1=0;
     DIR_L_B_2=1;
     //PDC0 assigns for PWM0,PWM1
     //PDC1 assigns for PWM2,PWM3.
     //Since we are working in complementary mode, assigning PC1 will turn the whole left piece the same speed.

    for(pwm=1;pwm<4096;pwm++){
        PDC0H=HIGH_BYTE(4096-pwm);
        PDC0L=LOW_BYTE(4096-pwm);
        PDC1H=HIGH_BYTE(4096-pwm);
        PDC1L=LOW_BYTE(4096-pwm);
        Delay_us(10);
    }
}

//------------------------------------MAIN--------------------------------------
void main() {
     setup_IO();
     setup_Init();

     LED1=1; //Initialize the leds for the led demo, and simulate in the main loop.
     LED2=0;

     while(1){

          pwm_demo();
          LED1= !LED1;
          LED2= !LED2;
    }
}