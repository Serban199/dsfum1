void setup ()
{
  pinMode(4,OUTPUT); // pinul pentru latch (blocare)
  pinMode(7,OUTPUT); // pin pentru sincronizarea ceasului
  pinMode(8,OUTPUT); // pin pentru transmiterea datelor
}
 
void loop()
{
  byte valCifre[]=
  {
    0b00000011, // 0
    0b11110011, // 1
    0b00100101, // 2
    0b01100001, // 3
    0b11010001, // 4
    0b01001001, // 5
    0b00001001, // 6
    0b11100011, // 7
    0b00000001, // 8
    0b01000001  // 9
  };

  for(int seg=0; seg<4; seg++)
    for(int cif=0; cif<10; cif++)
    {
      afiseazaValoare(valCifre[cif], segmente[seg]);
      delay(1000);
    }

}
 
void afiseazaValoare(byte cifra, byte segment)
{
  digitalWrite(4,LOW);
  shiftOut(8, 7, LSBFIRST, cifra);
  shiftOut(8, 7, LSBFIRST, segment);
  digitalWrite(4,HIGH);
}
