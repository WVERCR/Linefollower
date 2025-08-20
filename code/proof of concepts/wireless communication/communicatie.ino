#define ledPin 13
char state;

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  Serial.begin(9600);    // Voor debug via USB
  Serial1.begin(9600);   // Voor communicatie met HC-05 via pin 0/1
  Serial.println("Bluetooth klaar");
}

void loop() {
  if (Serial1.available()) {
    state = Serial1.read();
    Serial.print("Ontvangen via Bluetooth: ");
    Serial.println(state);

    if (state == '1') {
      digitalWrite(ledPin, HIGH);
      Serial1.println("LED: ON");
    }
    else if (state == '0') {
      digitalWrite(ledPin, LOW);
      Serial1.println("LED: OFF");
    }
  }
}