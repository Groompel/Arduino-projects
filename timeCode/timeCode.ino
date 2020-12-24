#include <iarduino_RTC.h>

iarduino_RTC time(RTC_DS1302,10,13,12);

void setup() {
    delay(300);
    Serial.begin(9600);
    time.begin();
    time.settime(10,45,17,14,11,20,6);  // 0  сек, 51 мин, 21 час, 27, октября, 2015 года, вторник
}
void loop(){
    if(millis()%1000==0){ // если прошла 1 секунда
      Serial.println(time.gettime("d-m-Y, H:i:s, D")); // выводим время
      delay(1); // приостанавливаем на 1 мс, чтоб не выводить время несколько раз за 1мс
    }
}
