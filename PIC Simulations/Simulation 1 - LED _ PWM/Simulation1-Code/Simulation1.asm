
_setup_IO:

;Simulation1.c,61 :: 		void setup_IO() {                 //Setup_IO will assign input and output pins. (Output 0, Input 1)
;Simulation1.c,62 :: 		STBY_R_TRIS=0;               //OUTPUTS
	BCF         TRISA2_bit+0, BitPos(TRISA2_bit+0) 
;Simulation1.c,63 :: 		STBY_L_TRIS=0;
	BCF         TRISA3_bit+0, BitPos(TRISA3_bit+0) 
;Simulation1.c,64 :: 		PWM_R_F_TRIS=0;
	BCF         TRISB0_bit+0, BitPos(TRISB0_bit+0) 
;Simulation1.c,65 :: 		PWM_R_B_TRIS=0;
	BCF         TRISB1_bit+0, BitPos(TRISB1_bit+0) 
;Simulation1.c,66 :: 		PWM_L_F_TRIS=0;
	BCF         TRISB2_bit+0, BitPos(TRISB2_bit+0) 
;Simulation1.c,67 :: 		PWM_L_B_TRIS=0;
	BCF         TRISB3_bit+0, BitPos(TRISB3_bit+0) 
;Simulation1.c,68 :: 		LED1_TRIS=0;
	BCF         TRISB4_bit+0, BitPos(TRISB4_bit+0) 
;Simulation1.c,69 :: 		LED2_TRIS=0;
	BCF         TRISB5_bit+0, BitPos(TRISB5_bit+0) 
;Simulation1.c,70 :: 		DIR_R_F_1_TRIS=0;
	BCF         TRISC0_bit+0, BitPos(TRISC0_bit+0) 
;Simulation1.c,71 :: 		DIR_R_F_2_TRIS=0;
	BCF         TRISC1_bit+0, BitPos(TRISC1_bit+0) 
;Simulation1.c,72 :: 		DIR_R_B_1_TRIS=0;
	BCF         TRISC2_bit+0, BitPos(TRISC2_bit+0) 
;Simulation1.c,73 :: 		DIR_R_B_2_TRIS=0;
	BCF         TRISC3_bit+0, BitPos(TRISC3_bit+0) 
;Simulation1.c,74 :: 		DIR_L_F_1_TRIS=0;
	BCF         TRISD0_bit+0, BitPos(TRISD0_bit+0) 
;Simulation1.c,75 :: 		DIR_L_F_2_TRIS=0;
	BCF         TRISD1_bit+0, BitPos(TRISD1_bit+0) 
;Simulation1.c,76 :: 		DIR_L_B_1_TRIS=0;
	BCF         TRISD2_bit+0, BitPos(TRISD2_bit+0) 
;Simulation1.c,77 :: 		DIR_L_B_2_TRIS=0;
	BCF         TRISD3_bit+0, BitPos(TRISD3_bit+0) 
;Simulation1.c,78 :: 		}
L_end_setup_IO:
	RETURN      0
; end of _setup_IO

_setup_Init:

;Simulation1.c,80 :: 		void setup_Init() {               //Setup_Init will initialize anything that requires initialization.
;Simulation1.c,81 :: 		PTCON0=0b00000000;           //1:1 Post Scale, PWM input clock=Osc/4, Free Running Mode.
	CLRF        PTCON0+0 
;Simulation1.c,82 :: 		PTCON1=0b10000000;           //PWM time base is on, counting up. Least significant 6 bits arenWt used.
	MOVLW       128
	MOVWF       PTCON1+0 
;Simulation1.c,83 :: 		PWMCON0=0b00111111;          //PWM0-PWM3 (RB0-RB3) are in PWM mode, even pins are in complementary mode. (PWM0 to PWM3 are independent regardless.)
	MOVLW       63
	MOVWF       PWMCON0+0 
;Simulation1.c,84 :: 		PWMCON1=0b00000001;          //Special Event Registers somewhat default, Updates are enabled and synchronized to time base.
	MOVLW       1
	MOVWF       PWMCON1+0 
;Simulation1.c,85 :: 		PTMRH=0b00000000;            //PTMR keeps track of current base time in the pwm cycle, initialize to 0.
	CLRF        PTMRH+0 
;Simulation1.c,86 :: 		PTMRL=0b00000000;            //PTMR will be resetting when it reaches PTPER. (period)
	CLRF        PTMRL+0 
;Simulation1.c,87 :: 		PTPERH=0b00000001;           //PTPER=(T_Pwm*f_osc)/(4*PTMRPS) -1. PTMRPS=Prescaler=1.
	MOVLW       1
	MOVWF       PTPERH+0 
;Simulation1.c,88 :: 		PTPERL=0b11111111;
	MOVLW       255
	MOVWF       PTPERL+0 
;Simulation1.c,91 :: 		ANSEL0=0x00;
	CLRF        ANSEL0+0 
;Simulation1.c,92 :: 		ANSEL1=0x00;
	CLRF        ANSEL1+0 
;Simulation1.c,93 :: 		}
L_end_setup_Init:
	RETURN      0
; end of _setup_Init

_pwm_demo:

;Simulation1.c,97 :: 		void pwm_demo(){
;Simulation1.c,98 :: 		int pwm=0;                   //Pwm demo by changing the pwm signal and seeing the output from the PROTEUS oscillator.
	CLRF        pwm_demo_pwm_L0+0 
	CLRF        pwm_demo_pwm_L0+1 
;Simulation1.c,99 :: 		STBY_R=1;                    //STBY should be high for the motors to work.
	BSF         RA2_bit+0, BitPos(RA2_bit+0) 
;Simulation1.c,100 :: 		STBY_L=1;                    //Set an initial direction for the motors.
	BSF         RA3_bit+0, BitPos(RA3_bit+0) 
;Simulation1.c,101 :: 		DIR_R_F_1=1;
	BSF         RC0_bit+0, BitPos(RC0_bit+0) 
;Simulation1.c,102 :: 		DIR_R_F_2=0;
	BCF         RC1_bit+0, BitPos(RC1_bit+0) 
;Simulation1.c,103 :: 		DIR_R_B_1=0;
	BCF         RC2_bit+0, BitPos(RC2_bit+0) 
;Simulation1.c,104 :: 		DIR_R_B_2=1;
	BSF         RC3_bit+0, BitPos(RC3_bit+0) 
;Simulation1.c,105 :: 		DIR_L_F_1=1;
	BSF         RD0_bit+0, BitPos(RD0_bit+0) 
;Simulation1.c,106 :: 		DIR_L_F_2=0;
	BCF         RD1_bit+0, BitPos(RD1_bit+0) 
;Simulation1.c,107 :: 		DIR_L_B_1=0;
	BCF         RD2_bit+0, BitPos(RD2_bit+0) 
;Simulation1.c,108 :: 		DIR_L_B_2=1;
	BSF         RD3_bit+0, BitPos(RD3_bit+0) 
;Simulation1.c,113 :: 		for(pwm=1;pwm<4096;pwm++){
	MOVLW       1
	MOVWF       pwm_demo_pwm_L0+0 
	MOVLW       0
	MOVWF       pwm_demo_pwm_L0+1 
L_pwm_demo0:
	MOVLW       128
	XORWF       pwm_demo_pwm_L0+1, 0 
	MOVWF       R0 
	MOVLW       128
	XORLW       16
	SUBWF       R0, 0 
	BTFSS       STATUS+0, 2 
	GOTO        L__pwm_demo9
	MOVLW       0
	SUBWF       pwm_demo_pwm_L0+0, 0 
L__pwm_demo9:
	BTFSC       STATUS+0, 0 
	GOTO        L_pwm_demo1
;Simulation1.c,114 :: 		PDC0H=HIGH_BYTE(4096-pwm);
	MOVF        pwm_demo_pwm_L0+0, 0 
	SUBLW       0
	MOVWF       R3 
	MOVF        pwm_demo_pwm_L0+1, 0 
	MOVWF       R4 
	MOVLW       16
	SUBFWB      R4, 1 
	MOVF        R4, 0 
	MOVWF       R0 
	MOVLW       0
	BTFSC       R4, 7 
	MOVLW       255
	MOVWF       R1 
	MOVLW       255
	ANDWF       R0, 0 
	MOVWF       R1 
	MOVF        R1, 0 
	MOVWF       PDC0H+0 
;Simulation1.c,115 :: 		PDC0L=LOW_BYTE(4096-pwm);
	MOVLW       255
	ANDWF       R3, 0 
	MOVWF       R0 
	MOVF        R0, 0 
	MOVWF       PDC0L+0 
;Simulation1.c,116 :: 		PDC1H=HIGH_BYTE(4096-pwm);
	MOVF        R1, 0 
	MOVWF       PDC1H+0 
;Simulation1.c,117 :: 		PDC1L=LOW_BYTE(4096-pwm);
	MOVF        R0, 0 
	MOVWF       PDC1L+0 
;Simulation1.c,118 :: 		Delay_us(10);
	MOVLW       16
	MOVWF       R13, 0
L_pwm_demo3:
	DECFSZ      R13, 1, 1
	BRA         L_pwm_demo3
	NOP
;Simulation1.c,113 :: 		for(pwm=1;pwm<4096;pwm++){
	INFSNZ      pwm_demo_pwm_L0+0, 1 
	INCF        pwm_demo_pwm_L0+1, 1 
;Simulation1.c,119 :: 		}
	GOTO        L_pwm_demo0
L_pwm_demo1:
;Simulation1.c,120 :: 		}
L_end_pwm_demo:
	RETURN      0
; end of _pwm_demo

_main:

;Simulation1.c,123 :: 		void main() {
;Simulation1.c,124 :: 		setup_IO();
	CALL        _setup_IO+0, 0
;Simulation1.c,125 :: 		setup_Init();
	CALL        _setup_Init+0, 0
;Simulation1.c,127 :: 		LED1=1; //Initialize the leds for the led demo, and simulate in the main loop.
	BSF         RB4_bit+0, BitPos(RB4_bit+0) 
;Simulation1.c,128 :: 		LED2=0;
	BCF         RB5_bit+0, BitPos(RB5_bit+0) 
;Simulation1.c,130 :: 		while(1){
L_main4:
;Simulation1.c,132 :: 		pwm_demo();
	CALL        _pwm_demo+0, 0
;Simulation1.c,133 :: 		LED1= !LED1;
	BTG         RB4_bit+0, BitPos(RB4_bit+0) 
;Simulation1.c,134 :: 		LED2= !LED2;
	BTG         RB5_bit+0, BitPos(RB5_bit+0) 
;Simulation1.c,135 :: 		}
	GOTO        L_main4
;Simulation1.c,136 :: 		}
L_end_main:
	GOTO        $+0
; end of _main
