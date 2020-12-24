

unsigned long long l_time;
String color = "r";

void setup() {
  // put your setup code here, to run once:
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  l_time = millis();
  Serial.begin(9600);
}


void red() {
  digitalWrite(7, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(4, LOW);
  digitalWrite(5, HIGH);
}

void blue() {
  digitalWrite(7, LOW);
  digitalWrite(6, HIGH);
  digitalWrite(4, LOW);
  digitalWrite(5, HIGH);
}

void green() {
  digitalWrite(7, LOW);
  digitalWrite(6, LOW);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
}

void changeColor() {
  if (color == "r") {
    color = "g";
    green();
    Serial.println("green");
  } else if (color == "g") {
    color = "b";
    blue();
    Serial.println("blue");
  } else if (color == "b") {
    color = "r";
    red();
    Serial.println("red");
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if (millis() - l_time > 1000) {
    l_time = millis();

    changeColor();
  }


  
}
