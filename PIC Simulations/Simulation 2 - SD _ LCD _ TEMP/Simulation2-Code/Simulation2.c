/*
  PIC Proof of Concept for SD Card, LCD, Temperature Sensor, for ME331 F2020.
  Simulated with PIC18F4550.
  Temperature data is written both on the LCD and the SD Card.
  This program uses the following MikroC libraries: ADC, LCD, SPI, Mmc, Mmc_FAT16.
  It can be seen that this program uses about 1kB of RAM which the 18F4431 didn't have.
  by: M. Çaðatay Sipahioðlu
*/


//------------------------------------PINS--------------------------------------
sfr sbit LCD_EN_Direction at TRISA1_bit;
sfr sbit LCD_RS_Direction at TRISA2_bit;          //LCD Pins must be named _Direction in order to work with the library.
sfr sbit SWITCH_TRIS at TRISA4_bit;
sfr sbit Mmc_Chip_Select_Direction at TRISA5_bit; //MMC_CS Pin must be named _Direction in order to work with the library.

//sfr sbit SD_MISO_TRIS at TRISB0bit;
//sfr sbit SD_CLK_TRIS at TRISB1_bit;
//sfr sbit SD_MOSI_TRIS at TRISC7_bit;

sfr sbit LCD_D4_Direction at TRISD4_bit;
sfr sbit LCD_D5_Direction at TRISD5_bit;
sfr sbit LCD_D6_Direction at TRISD6_bit;
sfr sbit LCD_D7_Direction at TRISD7_bit;

//sfr sbit TEMP_TRIS at TRISE0_bit;    //Doesn't need to be defined thanks to ADC library.


sfr sbit LCD_EN at RA1_bit;          //LCD bits must be defined with these names for the library.
sfr sbit LCD_RS at RA2_bit;
sfr sbit SWITCH at RA4_bit;              //Debugging Switch
sfr sbit Mmc_Chip_Select at LATA5_bit;   //CS pin must be named like this in order to work with the library.

//sfr sbit SD_MISO at RB0_bit;
//sfr sbit SD_CLK at RB1_bit;            //Since there is only 1 SPI port. MISO, CLK and MOSI pins will be constant and don't need defining.
//sfr sbit SD_MOSI at RC7_bit;

sfr sbit LCD_D4 at RD4_bit;            //Ports RD4-7 control the LCD along with RA0-RA1.
sfr sbit LCD_D5 at RD5_bit;
sfr sbit LCD_D6 at RD6_bit;
sfr sbit LCD_D7 at RD7_bit;

//sfr sbit TEMP at RE0_bit;             //Port RE0 connects the temperature sensor. (LM35) Doesn't need to be named, ADC library works straight with the Analog channel number.



 //--------------------------------VARIABLES-------------------------------------
unsigned int temp;
int i=0;   //Counters to understand if the SWITCH has been turned off.
int j=0;
int error;
char Temperature[] = "00";


//------------------------------------SETUP-------------------------------------

void setup_IO() {                 //Setup_IO will assign input and output pins. (Output 0, Input 1)
     SWITCH_TRIS=1;               //INPUTS
}

void setup_Init() {               //Setup_Init will initialize anything that requires initialization.
     ADCON0=0b00000001;           //ADC on for analog channel 0.
     ADCON1=0b00001110;           //VDD and GND are Analog reference. Only the 0th channel open. We should use the analog channels starting from AN0 since we can't turn for example AN5 on without turning AN0-4 on.
     ADCON2=0b10000010;           //Right justified, slowest.

       /*
     SPI1_Init();                 //Initialize SPI protocol
     SPI1_Init_Advanced(_SPI_MASTER_OSC_DIV64, _SPI_DATA_SAMPLE_MIDDLE, _SPI_CLK_IDLE_LOW, _SPI_LOW_2_HIGH);
     Delay_us(10);
     error = Mmc_Init();
     MMC_Fat_Init();              //Initialize MMC Fat Systems
     SPI1_Init_Advanced(_SPI_MASTER_OSC_DIV4, _SPI_DATA_SAMPLE_MIDDLE, _SPI_CLK_IDLE_LOW, _SPI_LOW_2_HIGH);
     Mmc_Fat_Assign("Data.TXT",0xA0);
         */
     ADC_Init();                  //Initialize Analog to Digital Conversion.
     Lcd_Init();                  //Initializes LCD pins & LCD.
     Lcd_Cmd(_LCD_CURSOR_OFF);    //Turns the Cursor off.
     Lcd_Cmd(_LCD_CLEAR);         //Clears the screen. (More commands can be looked up from the library.)
     Lcd_Out(1,1,"Hello");
     Delay_ms(1000);
}

//----------------------------------FUNCTIONS-----------------------------------

void sd_demo(){
  temp = (ADC_Read(0))*500/1023;      //Read Temperature from Analog Channel 5.
  Mmc_Fat_Append();
  Temperature[0]='0'+ (temp/10)% 10;  //Tens of the temperature
  Temperature[1]='0'+ temp%10;  //Ones of the temperature
  Mmc_Fat_Write(Temperature,2);
}

void temp_lcd_demo(){
  if(i>j){
   Lcd_Init();       //When the SWITCH is off i goes 1 up. When the SWITCH goes back on this void initiates, LCD must be reinitialized to work.
   Lcd_Cmd(_LCD_CURSOR_OFF);    //Reinitialize the LCD and equate i to j. When the SWITCH goes back off i can go up 1 more. And we can reinitialize again in this if.
   i=j;
  }
  
  temp = (ADC_Read(0))*500/1023;      //Read Temperature from ADC
  lcd_out(1, 1, "Temperature:");
  Temperature[0]='0'+ (temp/10)% 10;  //Tens of the temperature
  Temperature[1]='0'+ temp%10;  //Ones of the temperature
  Lcd_out(2,1, Temperature);
  Delay_ms(1000);
}



//------------------------------------MAIN--------------------------------------
void main() {

     setup_Init();
     setup_IO(); //IO setup must be done after initialization, since the proper digital/analog initialization of the ADCON register is needed before assigning a digital input characteristic to the SWITCH. (Program doesn't work if setup_IO is above setup_Init)
     while(1){
          if (SWITCH==0){         //We won't be using the SWITCH for debugging, rather we will demonstrate that we can use the SWITCH by turning off the LCD with it.
          Lcd_cmd(_LCD_TURN_OFF);
          if(i==j)  //Make i go 1 up when the SWITCH is off.
          i++;
          }
          else
          temp_lcd_demo();

          //sd_demo();

    }
}