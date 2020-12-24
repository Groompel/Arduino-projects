#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <iarduino_RTC.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
iarduino_RTC time(RTC_DS1302, 10, 13, 12);

Servo servo;

int val;
float temp;
unsigned long long lastTime = 0;
unsigned long long lastServoTime = 0;

#define RELE 8 // реле

#define THERMISTORPIN A0 // сопротивление при 25 градусах по Цельсию

#define THERMISTORNOMINAL 10000 // temp. для номинального сопротивления (практически всегда равна 25 C)

#define TEMPERATURENOMINAL 25 // сколько показаний используем для определения среднего значения

#define NUMSAMPLES 5 // бета коэффициент термистора (обычно 3000-4000)

#define BCOEFFICIENT 3950 // сопротивление второго резистора

#define SERIESRESISTOR 10000

int samples[NUMSAMPLES];

//  Определяем системное время:                           // Время загрузки скетча.
const char* strM = "JanFebMarAprMayJunJulAugSepOctNovDec"; // Определяем массив всех вариантов текстового представления текущего месяца.
const char* sysT = __TIME__;                              // Получаем время компиляции скетча в формате "SS:MM:HH".
const char* sysD = __DATE__;                              // Получаем дату  компиляции скетча в формате "MMM:DD:YYYY", где МММ - текстовое представление текущего месяца, например: Jul.
//  Парсим полученные значения sysT и sysD в массив i:    // Определяем массив «i» из 6 элементов типа int, содержащий следующие значения: секунды, минуты, часы, день, месяц и год компиляции скетча.
const int i[6] {(sysT[6] - 48) * 10 + (sysT[7] - 48), (sysT[3] - 48) * 10 + (sysT[4] - 48), (sysT[0] - 48) * 10 + (sysT[1] - 48), (sysD[4] - 48) * 10 + (sysD[5] - 48), ((int)memmem(strM, 36, sysD, 3) + 3 - (int)&strM[0]) / 3, (sysD[9] - 48) * 10 + (sysD[10] - 48)};
//

void setup() {

  lastTime = 0;

  // LED
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);

  // Bluetooth setup
  Serial.begin(9600);

  // For rele
  pinMode(RELE, OUTPUT);

  // For thermistor
  analogReference(EXTERNAL);

  // Real Time
  delay(300);
  Serial.begin(9600);
  time.begin();
  //    time.settime(10,52,18,14,11,20,6);  // 0  сек, 51 мин, 21 час, 27, октября, 2015 года, вторник
  time.settime(i[0], i[1], i[2], i[3], i[4], i[5]);    // Устанавливаем время в модуль: i[0] сек, i[1] мин, i[2] час, i[3] день, i[4] месяц, i[5] год, без указания дня недели.

  servo.attach(9);


  // LCD

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Starting...");
  delay(2000);
}


void led_red() {
  digitalWrite(7, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(4, LOW);
  digitalWrite(5, HIGH);
}

void led_blue() {
  digitalWrite(7, LOW);
  //  digitalWrite(6, HIGH);
  analogWrite(6, 150);
  digitalWrite(4, LOW);
  digitalWrite(5, HIGH);
}

void led_green() {
  digitalWrite(7, LOW);
  digitalWrite(6, LOW);
  digitalWrite(4, HIGH);
  //  digitalWrite(5, HIGH);
  analogWrite(5, 255);
}


void calculate_temp() {
  uint8_t i;

  float average;

  // сводим показания в вектор с небольшой задержкой между снятием показаний

  for (i = 0; i < NUMSAMPLES; i++) {

    samples[i] = analogRead(THERMISTORPIN);

    delay(10);

  }
  // рассчитываем среднее значение

  average = 0;

  for (i = 0; i < NUMSAMPLES; i++) {

    average += samples[i];

  }

  average /= NUMSAMPLES;


  // конвертируем значение в сопротивление

  average = 1023 / average - 1;

  average = SERIESRESISTOR / average;


  float steinhart;

  steinhart = average / THERMISTORNOMINAL; // (R/Ro)

  steinhart = log(steinhart); // ln(R/Ro)

  steinhart /= BCOEFFICIENT; // 1/B * ln(R/Ro)

  steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)

  steinhart = 1.0 / steinhart; // инвертируем

  steinhart -= 273.15; // конвертируем в градусы по Цельсию

  temp = steinhart;

}

void get_time() {
  //Real Time

  if (millis() - lastServoTime >= 1000) {
    time.gettime("d-m-Y, H:i:s, D");
    lastServoTime = millis();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Time: ");
    lcd.print(time.Hours);
    lcd.print(":");
    lcd.print(time.minutes);
    lcd.print(":");
    lcd.print(time.seconds);
    lcd.setCursor(0, 1);
    lcd.print("Temp: ");
    lcd.print(temp);
    


    if ((time.Hours == 10 && time.minutes == 0 && time.seconds == 0)) {
      servo.write(0);
      delay(500);
      servo.write(180);
    } else if (time.Hours == 22 && time.minutes == 00 && time.seconds == 0 ) {
      servo.write(0);
      delay(500);
      servo.write(180);
    } else {
      servo.write(180);
    }

    if (time.Hours > 10 && time.Hours < 22) {
      digitalWrite(RELE, LOW);
    } else {
      digitalWrite(RELE, HIGH);
    }

  }


}

void loop() {

  if (Serial.available()) {
    val = Serial.read();
    if (val == 1) {
      digitalWrite(RELE, LOW);
    } else if (val == 0) {
      digitalWrite(RELE, HIGH);
    } else if (val == 2) {
      Serial.println(temp);
    }
  }


  get_time();

  calculate_temp();

  if (temp <= 23) {
    led_blue();
  } else if (temp > 23 && temp < 27) {
    led_green();
  } else if (temp > 27) {
    led_red();
  }

}
