#include <LiquidCrystal_I2C.h>

const int col = 16;
const int row = 2;

//LiquidCrystal_I2C lcd(address,column,row)  
LiquidCrystal_I2C lcd(0x27,col,row);  

void setup()
{
lcd.init(); //inisialisasi led
lcd.backlight();  // set backlight /no
lcd.setCursor(0,0); // cursor
lcd.print("dandi ganteng");  //munculkan text
}

void loop()
{
  
}
