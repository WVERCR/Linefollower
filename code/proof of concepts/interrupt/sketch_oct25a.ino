const int Drukknop = 2;
const int Led = 12;
bool toestand_Led = LOW;

void setup() {
  
  pinMode(Drukknop, INPUT_PULLUP);
  pinMode(Led, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(Drukknop), aan, CHANGE);
}
void loop() {
   digitalWrite(Led, toestand_Led);
}

void aan() {
  toestand_Led = !toestand_Led;
 
}
