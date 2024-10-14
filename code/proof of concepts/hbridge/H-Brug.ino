void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(5, OUTPUT);//IN4
pinMode(6, OUTPUT);//IN3
pinMode(3, OUTPUT);//IN2
pinMode(11, OUTPUT);//IN1
}

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(5, HIGH);
digitalWrite(6, LOW);
digitalWrite(3, HIGH);
digitalWrite(11, LOW);

delay(2000);

digitalWrite(5, LOW);
digitalWrite(6, LOW);
digitalWrite(3, LOW);
digitalWrite(11, LOW);

delay(2000);

digitalWrite(5, LOW);
digitalWrite(6, HIGH);
digitalWrite(3, LOW);
digitalWrite(11, HIGH);

delay(2000);

digitalWrite(5, LOW);
digitalWrite(6, LOW);
digitalWrite(3, LOW);
digitalWrite(11, LOW);

delay(2000);
}
