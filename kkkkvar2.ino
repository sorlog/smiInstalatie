#include <IRremote.h>

const byte releuPin = 6;
const int speakerPin = 4;
const int receiver = 2;

IRrecv irrecv(receiver);
decode_results results;


volatile int mod = 0;
volatile int track = 0; 
volatile int n;



const int length[5] = {73, 69, 71, 29, 26}; 

const char * notes[5] = \
 { 
 "ggagsed deggsgg ggagsed deggsgg DCbCDbCbabCabagabgagsgasgsesgeseddeggsgg ", \
 "ddaagfedcdefga ddaagfedcdefga avgavCDagfdefgfgavaagfedfedgfgavCDagfed" , \
 "cfffgagavCavafggfcfffgagavCavafggffaCDCvagfgavCcfagfccfffgagavCavafggf " , \
 "faagfvvagaCCbCaDCvagfeagffef " , \
 "eeeeeeegcde fffffeeeeddedg"};
 
const int beats[5][77] = {
{2,2,1,1,1,1,4,2,2,2,2,2,2,4,2,2,2,2,1,1,1,1,4,2,2,2,2,2,2,4,2,2,3,1,1,1,1,1,3,1,1,1,1,1,3,1,1,1,1,1,3,1,1,1,1,1
,3,1,1,1,1,1,3,1,2,2,2,2,2,2,4,2,2 }, \
{2,2,2,2,2,2,2,2,2,2,2,2,2,4,2,2,2,2,2,2,2,2,2,2,2,2,2,2,4,2,2,2,2,2,2,2,2,2,2,2,2,2,2,4,2,2,4,2,2,2,2,2,2,4,1,1
,2,4,2,2,2,2,2,2,2,2,2,2,8 } , \
{2,2,2,2,2,1,1,1,1,2,2,2,1,1,2,2,6,2,2,2,2,2,1,1,1,1,2,2,2,1,1,2,2,6,1,1,3,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,4,4,2,2
,2,2,1,1,1,1,2,2,2,1,1,2,2,6,2 } , \
{ 2,3,1,2,2,2,2,2,2,2,2,2,2,6,2,3,1,2,2,2,2,2,2,2,2,2,2,6,2 } , \ 
{ 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2}};

const int tempo[5] = {150,150,150,150, 300}; 


void playTone(int tone, int duration)
{
  for (long i = 0; i < duration * 1000L; i += tone * 2)
  {
    digitalWrite(speakerPin, HIGH);
    delayMicroseconds(tone);
  
    digitalWrite(speakerPin, LOW);
    delayMicroseconds(tone);
 }
}

void playNote(char note, int duration)
{
 const char names[] = { 'c', 'd', 'e', 'f', 's', 'g', 'a', 'v', 'b', 'C', 'D', 'E' }; 
 const int tones[] = { 1915, 1700, 1519, 1432, 1352, 1275, 1136, 1073, 1014, 956, 852, 758 };
 
 for (int i = 0; i < 12; i++)
  if (names[i] == note) playTone(tones[i], duration);
}


void setup()
{ 
 irrecv.enableIRIn();
 pinMode(releuPin, OUTPUT);
 pinMode(speakerPin, OUTPUT);

 attachInterrupt(0, irIN, HIGH); //RISING
 Serial.begin(9600);
}


void loop()
{  
 if (mod!=0)
 {
   for (n = 0; n < length[track]; n++)
    {  
      if (mod==0) break;

      if(n % 2 == 0)
        digitalWrite(releuPin,HIGH);
      else
        digitalWrite(releuPin, LOW);

      if (mod==0) break;

     if (notes[track][n] == ' ') delay(beats[track][n] * tempo[track]); 
     else playNote(notes[track][n], beats[track][n] * tempo[track]);

     if (mod==0) break;
   }
 delay(tempo[track] / 2);
 }
}

void irIN()
{
 if (irrecv.decode(&results)) {
    irrecv.decode(&results); 
    Serial.print(results.value, HEX);
    if(  results.value == 0x2ADE08A3  || results.value == 0x20DF8877) {
      if(mod == 0)
        Serial.println("  --  Start");
      else
        Serial.println("  --  Stop");
      startStop();
    }
    if(results.value == 0x7F613A3B  || results.value == 0x20DF48B7) {
      if(mod == 0) 
        Serial.println(" -- Song is stop : cann t next");
    
      else {
        Serial.println("  --  Next Song");
        nextSong();
      }
    }
 
    if(results.value == 0x1A46B33F || results.value == 0x20DFC837) {
      if(mod == 0) 
        Serial.println(" -- Song is stop : cann t last");
   
      else {
        Serial.println("  --  Last Song");
      lastSong();
      }
    } 
 irrecv.resume();
 }
}

void startStop()
{
 mod=!mod;
 _delay_ms(400);
}

void nextSong()
{
 track++;
 track = (track % 4);
 n = 0; 
 _delay_ms(400);
}

void lastSong()
{
 if(track == 0)
  track = 3;
 else
  track --;
 n = 0; 
 _delay_ms(400);
}
