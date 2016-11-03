#include <16f873a.h>
#define use_portb_lcd TRUE
#include <lcd.c>
#include <lcd.c>

#device adc = 10
#use delay(clock = 4M)
#fuses xt,protect,put,brownout,lvp,nowdt
#byte porta = 5
#byte portb = 6
#byte portc = 7
#bit  bot_1 = portc.2
#bit  bot_2 = portc.3
#bit  bot_3 = portc.4
#bit  buz = portc.1
#bit  cg = portc.0

int1 x=1,scape;
float temp;
signed int tempdsl;
float analise;
int c;

void main(){
   set_tris_a(0b1111111);
   set_tris_b(0);
   set_tris_c(0b1111100);
   porta = 0;
   portb = 0;
   portc = 0;
   setup_adc(adc_clock_internal);
   setup_adc_ports(ALL_ANALOG);
   set_adc_channel(0);
   lcd_init();
   
   while(true){
      
      lcd_putc("\f");
      analise=read_adc();
      temp=(analise*0.004887585532746823/0.01); 		//CALCULO
      delay_ms(20);
      lcd_gotoxy(1,1);lcd_putc("TEMPERATURA");
      lcd_gotoxy(3,2);printf(lcd_putc "%.1f C",temp);
      delay_ms(500);
      tempdsl = read_eeprom(1);				//Leitura da eeprom
      if(read_eeprom(1)){
      	if(temp < tempdsl ){
         cg = 1;
      }
      if(temp > tempdsl){
         cg = 0;
         for(c=0; c < 1;c++){
            buz = 1;
            delay_ms(200);
            buz = 0;
            delay_ms(200);
         }
      }
      
      }
      if(bot_1){  
         lcd_putc("\f");
         delay_ms(500);
         lcd_putc("CONTROLE");
         delay_ms(1000);
         scape=1;
         x = 1;
         while(scape==1){
            if(bot_1){
               c = 0;
               delay_ms(100);
               tempdsl = tempdsl + 1;
               while(bot_1){
                  c++;
                  delay_ms(50);
                  if(c > 20){
                     c=0;
                     scape=0;
                     lcd_putc("\f");
                     delay_ms(1000);
                  }
               }
               
            }
            if(bot_2){
               c = 0;
               delay_ms(100);
               tempdsl = tempdsl - 1;
               while(bot_2){
                  c++;
                  delay_ms(50);
                  if(c > 20){
                     c=0;
                     scape=0;
                     lcd_putc("\f");
                     delay_ms(1000);
                  }
               }
            }
            
            if(tempdsl < 0){
               tempdsl = -130;
            }
            if(tempdsl > 150){
               tempdsl = 0;
            }
            
            lcd_gotoxy(1,1);lcd_putc("TEMP. MAXIMA");
            lcd_gotoxy(1,2);printf(lcd_putc "%d C",tempdsl);
            while(scape == 0 && x == 1){
               lcd_putc("\f");
               delay_ms(1000);
               lcd_putc("GRAVANDO...");
               delay_ms(2000);
               lcd_putc("\f"); 
               write_eeprom(1,tempdsl);
               x=0;
               scape=0;
            }
         }
   }
}
}
