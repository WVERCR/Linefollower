// LED start/stop via interrupt
const int ledPin = 13;      // LED op pin 13
const int knopPin = 2;      // Drukknop op pin 2 (interrupt pin)

volatile bool ledStatus = false;      // LED status (aan/uit)
unsigned long laatsteInterrupt = 0;   // Voor debounce

void setup() {
  pinMode(ledPin, OUTPUT);              // Zet LED pin als output
  pinMode(knopPin, INPUT_PULLUP);       // Gebruik interne pull-up weerstand

  // Koppel interrupt: bij FALLING (knop indrukken → GND)
  attachInterrupt(digitalPinToInterrupt(knopPin), toggleLED, FALLING);
}

void loop() {
  digitalWrite(ledPin, ledStatus);  // Zet LED aan of uit op basis van status
}

// Interrupt Service Routine (ISR)
void toggleLED() {
  // Debounce: negeer triggers die binnen 200 ms vallen
  unsigned long huidigeTijd = millis();
  if (huidigeTijd - laatsteInterrupt > 200) {
    ledStatus = !ledStatus;                    // Toggle LED status
    laatsteInterrupt = huidigeTijd;
  }
}