
_setup_IO:

;Simulation2.c,57 :: 		void setup_IO() {                 //Setup_IO will assign input and output pins. (Output 0, Input 1)
;Simulation2.c,58 :: 		SWITCH_TRIS=1;               //INPUTS
	BSF         TRISA4_bit+0, BitPos(TRISA4_bit+0) 
;Simulation2.c,59 :: 		}
L_end_setup_IO:
	RETURN      0
; end of _setup_IO

_setup_Init:

;Simulation2.c,61 :: 		void setup_Init() {               //Setup_Init will initialize anything that requires initialization.
;Simulation2.c,62 :: 		ADCON0=0b00000001;           //ADC on for analog channel 0.
	MOVLW       1
	MOVWF       ADCON0+0 
;Simulation2.c,63 :: 		ADCON1=0b00001110;           //VDD and GND are Analog reference. Only the 0th channel open. We should use the analog channels starting from AN0 since we can't turn for example AN5 on without turning AN0-4 on.
	MOVLW       14
	MOVWF       ADCON1+0 
;Simulation2.c,64 :: 		ADCON2=0b10000010;           //Right justified, slowest.
	MOVLW       130
	MOVWF       ADCON2+0 
;Simulation2.c,75 :: 		ADC_Init();                  //Initialize Analog to Digital Conversion.
	CALL        _ADC_Init+0, 0
;Simulation2.c,76 :: 		Lcd_Init();                  //Initializes LCD pins & LCD.
	CALL        _Lcd_Init+0, 0
;Simulation2.c,77 :: 		Lcd_Cmd(_LCD_CURSOR_OFF);    //Turns the Cursor off.
	MOVLW       12
	MOVWF       FARG_Lcd_Cmd_out_char+0 
	CALL        _Lcd_Cmd+0, 0
;Simulation2.c,78 :: 		Lcd_Cmd(_LCD_CLEAR);         //Clears the screen. (More commands can be looked up from the library.)
	MOVLW       1
	MOVWF       FARG_Lcd_Cmd_out_char+0 
	CALL        _Lcd_Cmd+0, 0
;Simulation2.c,79 :: 		Lcd_Out(1,1,"Hello");
	MOVLW       1
	MOVWF       FARG_Lcd_Out_row+0 
	MOVLW       1
	MOVWF       FARG_Lcd_Out_column+0 
	MOVLW       ?lstr1_Simulation2+0
	MOVWF       FARG_Lcd_Out_text+0 
	MOVLW       hi_addr(?lstr1_Simulation2+0)
	MOVWF       FARG_Lcd_Out_text+1 
	CALL        _Lcd_Out+0, 0
;Simulation2.c,80 :: 		Delay_ms(1000);
	MOVLW       26
	MOVWF       R11, 0
	MOVLW       94
	MOVWF       R12, 0
	MOVLW       110
	MOVWF       R13, 0
L_setup_Init0:
	DECFSZ      R13, 1, 1
	BRA         L_setup_Init0
	DECFSZ      R12, 1, 1
	BRA         L_setup_Init0
	DECFSZ      R11, 1, 1
	BRA         L_setup_Init0
	NOP
;Simulation2.c,81 :: 		}
L_end_setup_Init:
	RETURN      0
; end of _setup_Init

_sd_demo:

;Simulation2.c,85 :: 		void sd_demo(){
;Simulation2.c,86 :: 		temp = (ADC_Read(0))*500/1023;      //Read Temperature from Analog Channel 5.
	CLRF        FARG_ADC_Read_channel+0 
	CALL        _ADC_Read+0, 0
	MOVLW       244
	MOVWF       R4 
	MOVLW       1
	MOVWF       R5 
	CALL        _Mul_16X16_U+0, 0
	MOVLW       255
	MOVWF       R4 
	MOVLW       3
	MOVWF       R5 
	CALL        _Div_16X16_U+0, 0
	MOVF        R0, 0 
	MOVWF       _temp+0 
	MOVF        R1, 0 
	MOVWF       _temp+1 
;Simulation2.c,87 :: 		Mmc_Fat_Append();
	CALL        _Mmc_Fat_Append+0, 0
;Simulation2.c,88 :: 		Temperature[0]='0'+ (temp/10)% 10;  //Tens of the temperature
	MOVLW       10
	MOVWF       R4 
	MOVLW       0
	MOVWF       R5 
	MOVF        _temp+0, 0 
	MOVWF       R0 
	MOVF        _temp+1, 0 
	MOVWF       R1 
	CALL        _Div_16X16_U+0, 0
	MOVLW       10
	MOVWF       R4 
	MOVLW       0
	MOVWF       R5 
	CALL        _Div_16X16_U+0, 0
	MOVF        R8, 0 
	MOVWF       R0 
	MOVF        R9, 0 
	MOVWF       R1 
	MOVF        R0, 0 
	ADDLW       48
	MOVWF       _Temperature+0 
;Simulation2.c,89 :: 		Temperature[1]='0'+ temp%10;  //Ones of the temperature
	MOVLW       10
	MOVWF       R4 
	MOVLW       0
	MOVWF       R5 
	MOVF        _temp+0, 0 
	MOVWF       R0 
	MOVF        _temp+1, 0 
	MOVWF       R1 
	CALL        _Div_16X16_U+0, 0
	MOVF        R8, 0 
	MOVWF       R0 
	MOVF        R9, 0 
	MOVWF       R1 
	MOVF        R0, 0 
	ADDLW       48
	MOVWF       _Temperature+1 
;Simulation2.c,90 :: 		Mmc_Fat_Write(Temperature,2);
	MOVLW       _Temperature+0
	MOVWF       FARG_Mmc_Fat_Write_fdata+0 
	MOVLW       hi_addr(_Temperature+0)
	MOVWF       FARG_Mmc_Fat_Write_fdata+1 
	MOVLW       2
	MOVWF       FARG_Mmc_Fat_Write_len+0 
	MOVLW       0
	MOVWF       FARG_Mmc_Fat_Write_len+1 
	CALL        _Mmc_Fat_Write+0, 0
;Simulation2.c,91 :: 		}
L_end_sd_demo:
	RETURN      0
; end of _sd_demo

_temp_lcd_demo:

;Simulation2.c,93 :: 		void temp_lcd_demo(){
;Simulation2.c,94 :: 		if(i>j){
	MOVLW       128
	XORWF       _j+1, 0 
	MOVWF       R0 
	MOVLW       128
	XORWF       _i+1, 0 
	SUBWF       R0, 0 
	BTFSS       STATUS+0, 2 
	GOTO        L__temp_lcd_demo12
	MOVF        _i+0, 0 
	SUBWF       _j+0, 0 
L__temp_lcd_demo12:
	BTFSC       STATUS+0, 0 
	GOTO        L_temp_lcd_demo1
;Simulation2.c,95 :: 		Lcd_Init();       //When the SWITCH is off i goes 1 up. When the SWITCH goes back on this void initiates, LCD must be reinitialized to work.
	CALL        _Lcd_Init+0, 0
;Simulation2.c,96 :: 		Lcd_Cmd(_LCD_CURSOR_OFF);    //Reinitialize the LCD and equate i to j. When the SWITCH goes back off i can go up 1 more. And we can reinitialize again in this if.
	MOVLW       12
	MOVWF       FARG_Lcd_Cmd_out_char+0 
	CALL        _Lcd_Cmd+0, 0
;Simulation2.c,97 :: 		i=j;
	MOVF        _j+0, 0 
	MOVWF       _i+0 
	MOVF        _j+1, 0 
	MOVWF       _i+1 
;Simulation2.c,98 :: 		}
L_temp_lcd_demo1:
;Simulation2.c,100 :: 		temp = (ADC_Read(0))*500/1023;      //Read Temperature from ADC
	CLRF        FARG_ADC_Read_channel+0 
	CALL        _ADC_Read+0, 0
	MOVLW       244
	MOVWF       R4 
	MOVLW       1
	MOVWF       R5 
	CALL        _Mul_16X16_U+0, 0
	MOVLW       255
	MOVWF       R4 
	MOVLW       3
	MOVWF       R5 
	CALL        _Div_16X16_U+0, 0
	MOVF        R0, 0 
	MOVWF       _temp+0 
	MOVF        R1, 0 
	MOVWF       _temp+1 
;Simulation2.c,101 :: 		lcd_out(1, 1, "Temperature:");
	MOVLW       1
	MOVWF       FARG_Lcd_Out_row+0 
	MOVLW       1
	MOVWF       FARG_Lcd_Out_column+0 
	MOVLW       ?lstr2_Simulation2+0
	MOVWF       FARG_Lcd_Out_text+0 
	MOVLW       hi_addr(?lstr2_Simulation2+0)
	MOVWF       FARG_Lcd_Out_text+1 
	CALL        _Lcd_Out+0, 0
;Simulation2.c,102 :: 		Temperature[0]='0'+ (temp/10)% 10;  //Tens of the temperature
	MOVLW       10
	MOVWF       R4 
	MOVLW       0
	MOVWF       R5 
	MOVF        _temp+0, 0 
	MOVWF       R0 
	MOVF        _temp+1, 0 
	MOVWF       R1 
	CALL        _Div_16X16_U+0, 0
	MOVLW       10
	MOVWF       R4 
	MOVLW       0
	MOVWF       R5 
	CALL        _Div_16X16_U+0, 0
	MOVF        R8, 0 
	MOVWF       R0 
	MOVF        R9, 0 
	MOVWF       R1 
	MOVF        R0, 0 
	ADDLW       48
	MOVWF       _Temperature+0 
;Simulation2.c,103 :: 		Temperature[1]='0'+ temp%10;  //Ones of the temperature
	MOVLW       10
	MOVWF       R4 
	MOVLW       0
	MOVWF       R5 
	MOVF        _temp+0, 0 
	MOVWF       R0 
	MOVF        _temp+1, 0 
	MOVWF       R1 
	CALL        _Div_16X16_U+0, 0
	MOVF        R8, 0 
	MOVWF       R0 
	MOVF        R9, 0 
	MOVWF       R1 
	MOVF        R0, 0 
	ADDLW       48
	MOVWF       _Temperature+1 
;Simulation2.c,104 :: 		Lcd_out(2,1, Temperature);
	MOVLW       2
	MOVWF       FARG_Lcd_Out_row+0 
	MOVLW       1
	MOVWF       FARG_Lcd_Out_column+0 
	MOVLW       _Temperature+0
	MOVWF       FARG_Lcd_Out_text+0 
	MOVLW       hi_addr(_Temperature+0)
	MOVWF       FARG_Lcd_Out_text+1 
	CALL        _Lcd_Out+0, 0
;Simulation2.c,105 :: 		Delay_ms(1000);
	MOVLW       26
	MOVWF       R11, 0
	MOVLW       94
	MOVWF       R12, 0
	MOVLW       110
	MOVWF       R13, 0
L_temp_lcd_demo2:
	DECFSZ      R13, 1, 1
	BRA         L_temp_lcd_demo2
	DECFSZ      R12, 1, 1
	BRA         L_temp_lcd_demo2
	DECFSZ      R11, 1, 1
	BRA         L_temp_lcd_demo2
	NOP
;Simulation2.c,106 :: 		}
L_end_temp_lcd_demo:
	RETURN      0
; end of _temp_lcd_demo

_main:

;Simulation2.c,111 :: 		void main() {
;Simulation2.c,113 :: 		setup_Init();
	CALL        _setup_Init+0, 0
;Simulation2.c,114 :: 		setup_IO(); //IO setup must be done after initialization, since the proper digital/analog initialization of the ADCON register is needed before assigning a digital input characteristic to the SWITCH. (Program doesn't work if setup_IO is above setup_Init)
	CALL        _setup_IO+0, 0
;Simulation2.c,115 :: 		while(1){
L_main3:
;Simulation2.c,116 :: 		if (SWITCH==0){         //We won't be using the SWITCH for debugging, rather we will demonstrate that we can use the SWITCH by turning off the LCD with it.
	BTFSC       RA4_bit+0, BitPos(RA4_bit+0) 
	GOTO        L_main5
;Simulation2.c,117 :: 		Lcd_cmd(_LCD_TURN_OFF);
	MOVLW       8
	MOVWF       FARG_Lcd_Cmd_out_char+0 
	CALL        _Lcd_Cmd+0, 0
;Simulation2.c,118 :: 		if(i==j)  //Make i go 1 up when the SWITCH is off.
	MOVF        _i+1, 0 
	XORWF       _j+1, 0 
	BTFSS       STATUS+0, 2 
	GOTO        L__main14
	MOVF        _j+0, 0 
	XORWF       _i+0, 0 
L__main14:
	BTFSS       STATUS+0, 2 
	GOTO        L_main6
;Simulation2.c,119 :: 		i++;
	INFSNZ      _i+0, 1 
	INCF        _i+1, 1 
L_main6:
;Simulation2.c,120 :: 		}
	GOTO        L_main7
L_main5:
;Simulation2.c,122 :: 		temp_lcd_demo();
	CALL        _temp_lcd_demo+0, 0
L_main7:
;Simulation2.c,126 :: 		}
	GOTO        L_main3
;Simulation2.c,127 :: 		}
L_end_main:
	GOTO        $+0
; end of _main
