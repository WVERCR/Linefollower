# Gebruiksaanwijzing

**opladen / vervangen batterijen**

uitleg over het opladen of vervangen van de batterijen De batterijen zijn zeer makkelijk te vervangen. Het zijn AA batterijen die je makkelijk kan kopen in supermarkten. De batterijen gaan ook lang mee.

**draadloze communicatie
verbinding maken**

uitleg over het verbinden van de robot met laptop / smartphone Om connectie te maken met de wagen gebruiken we de app 'Arduino Bluetooth Control' deze werkt wel enkel via android. Om connectie te maken moeten we de bluetooth inschakelen en de HC-05 module via bluetooth zoeken. Eens we verbonden zijn blijft dit zolang de Linefollower niet uitvalt. Wanneer de connectie verbroken is moeten we bovenaan rechts klikken en deze verbindt snel opnieuw.

**commando's**

debug [on/off] : ingestelde parameters bekijken start : start de linefollower stop : stopt de linefollower set cycle [µs] : het instellen van de cycle time set power [0..255] : instellen snelheid/power van de motoren set diff [0..1] : diff regelen. Dit is om de snelheid van de wielen te regelen in de bochten. Hogere diff is versnellen in de bochten, lagere diff is vertragen set kp [0..] : Kp regelen is hoe sterk de auto zich corrigeert om in het midden van de lijn te rijden. Bij een te hoge kp schokt hij van links naar rechts set ki [0..] integrerende correctie set kd [0..] : dit is de foutverandering, kijkt naar vorige cyclus en zal zo de fout bijregelen calibrate black : commando om zwart te kalibreren calibrate white : commando om wit te kalibreren

**kalibratie**

uitleg kalibratie
Om de sensor te kalibreren plaatsen we de sensor in een volledig zwart vlak, daarnaa typ je calibrate black. Hierna doen we hetzelfde in een wit vlak en typen we calibrate white. Indien je op een nieuw parcours rijdt doe je dit best even opnieuw.

**settings**

De robot rijdt stabiel met volgende parameters: Cycle 2000, Power 75, diff 0.5 , kp 10, ki 0, kd 0
