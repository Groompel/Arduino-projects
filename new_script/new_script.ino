#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);


int val;
unsigned long lastTime;

// к какому аналоговому пину мы подключены

#define THERMISTORPIN A0

// сопротивление при 25 градусах по Цельсию

#define THERMISTORNOMINAL 10000

// temp. для номинального сопротивления (практически всегда равна 25 C)

#define TEMPERATURENOMINAL 25

// сколько показаний используем для определения среднего значения

#define NUMSAMPLES 5

// бета коэффициент термистора (обычно 3000-4000)

#define BCOEFFICIENT 3950

// сопротивление второго резистора

#define SERIESRESISTOR 10000

int samples[NUMSAMPLES];

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Fuck u");

  lastTime = 0;
  
  // Bluetooth setup
  Serial.begin(9600);
  
  // For rele
  pinMode(4, OUTPUT);

  // For thermistor
  analogReference(EXTERNAL); 
}

void loop() {
  
  if (Serial.available()) {
    val = Serial.read();
    if (val == 1) {
      digitalWrite(4, LOW);
    } else if (val == 0) {
      digitalWrite(4, HIGH);
    }
  }

  
  if (millis() - lastTime >= 1000) {
 
    lastTime = millis();

  uint8_t i;
  
  float average;
  
  // сводим показания в вектор с небольшой задержкой между снятием показаний
  
  for (i=0; i< NUMSAMPLES; i++) {
  
  samples[i] = analogRead(THERMISTORPIN);
  
  delay(10);
  
  }
  
  // рассчитываем среднее значение
  
  average = 0;
  
  for (i=0; i< NUMSAMPLES; i++) {
  
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
    
  Serial.print("Average analog reading ");
  
  Serial.println(average);
  
  Serial.print("Thermistor resistance ");
  
  Serial.println(average);
  
     Serial.print("Temperature ");


     Serial.print(steinhart);


     Serial.println(" *C");  

  }
}
