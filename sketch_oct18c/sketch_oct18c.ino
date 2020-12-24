// значение 'другого' резистора

#define SERIESRESISTOR 10000

// к какому пину подключается термистор

#define THERMISTORPIN A0

void setup(void) {

Serial.begin(9600);

}

void loop(void) {

float reading;

reading = analogRead(THERMISTORPIN);

Serial.print("Analog reading ");

Serial.println(reading);

// преобразуем полученные значения в сопротивление

reading = (1023 / reading) - 1;

reading = SERIESRESISTOR / reading;

Serial.print("Thermistor resistance ");

Serial.println(reading);

delay(1000);

}
