const int sensor[] = { A5, A4, A3, A2, A1, A0};
int Sensorwaarde[6];

void setup() {
  
Serial.begin(9600);
}

void loop() {
  
  Serial.print("waarde sensoren: ");
    for (int i = 0; i < 6; i++)
    {
      Sensorwaarde[i] = analogRead(sensor[i]);
      Serial.print(Sensorwaarde[i]);
      Serial.print(" ");
    }
    
   Serial.println();
   delay(500);
}

  
