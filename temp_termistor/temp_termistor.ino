// DON'T FORGET TO CONNECT TO 3.3V NOT 5V
// ALSO DON'T FORGET TO CONNECT AREF TO 3.3V

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

void setup(void) {

Serial.begin(9600);

analogReference(EXTERNAL);

}

void loop(void) {


}
