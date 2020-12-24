#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,1);  // Устанавливаем дисплей
void setup()
{
  lcd.init();                     
  lcd.backlight();// Включаем подсветку дисплея
  lcd.setCursor(0, 0);
  lcd.print("Hello!");
}
void loop()
{
  // Устанавливаем курсор на вторую строку и нулевой символ.
  lcd.setCursor(0, 0);
  // Выводим на экран количество секунд с момента запуска ардуины
  lcd.print(millis()/1000);
}
