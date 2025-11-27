byte cifre[] =
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

byte segmente[] = {31, 47, 79, 143};

unsigned long timeDuration[10];
int values[10];

void setup ()
{
  pinMode(4,OUTPUT); // LATCH
  pinMode(7,OUTPUT); // CLOCK
  pinMode(8,OUTPUT); // DATA

  Serial.begin(9600);
  randomSeed(analogRead(A0));

  Serial.println("=== TEST DE REACTIE ===");
  Serial.println("Introdu numarul afisat pe display cat mai repede!");
  delay(2000);
}

void afiseazaValoare(byte val, byte seg)
{
  digitalWrite(4,LOW);
  shiftOut(8, 7, LSBFIRST, val);
  shiftOut(8, 7, LSBFIRST, seg);
  digitalWrite(4,HIGH);
}

void afiseazaNumar(int nr)
{
  int digs[4];
  for(int i=3; i>=0; i--)
  {
    digs[i] = nr % 10;
    nr /= 10;
  }

  for(int t=0; t<50; t++)
    for(int j=0; j<4; j++)
      afiseazaValoare(cifre[digs[j]], segmente[j]);
}

void loop()
{
  for(int k=0; k<10; k++)
  {
    int nr = random(1000,10000);
    values[k] = nr;
    unsigned long start = millis();

    bool corect = false;

    while(!corect)
    {
      afiseazaNumar(nr);

      if(Serial.available())
      {
        String intrare = Serial.readStringUntil('\n');
        intrare.trim();

        if(intrare.toInt() == nr)
        {
          corect = true;
          unsigned long end = millis();
          timeDuration[k] = end - start;

          Serial.print("Corect! Timp incercarea ");
          Serial.print(k+1);
          Serial.print(": ");
          Serial.print(timeDuration[k]);
          Serial.println(" ms");
        }
        else
        {
          Serial.println("Gresit! Mai incearca.");
        }
      }
    }

    delay(500);
  }

  unsigned long minT = timeDuration[0], maxT = timeDuration[0], sum = 0;
  int minIdx=0, maxIdx=0;

  for(int i=0; i<10; i++)
  {
    if(timeDuration[i]<minT) { minT=timeDuration[i]; minIdx=i; }
    if(timeDuration[i]>maxT) { maxT=timeDuration[i]; maxIdx=i; }
    sum += timeDuration[i];
  }

  float medie = sum / 10.0;

  Serial.println("\n=== REZULTATE ===");
  Serial.print("Timp minim: "); Serial.print(minT);
  Serial.print(" ms (pentru valoarea "); Serial.print(values[minIdx]); Serial.println(")");
  Serial.print("Timp maxim: "); Serial.print(maxT);
  Serial.print(" ms (pentru valoarea "); Serial.print(values[maxIdx]); Serial.println(")");
  Serial.print("Timp mediu: "); Serial.print(medie); Serial.println(" ms");

  while(true); // oprește programul
}
