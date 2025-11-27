struct LED
{
  bool on = false;
  String mode = "off";
  unsigned long dur = 0;
  unsigned long last = 0;
  int bright = 0;
  int dir = 1; // 1  creste, -1  scade
  bool state = false;

  // pentru PWM
  unsigned long cycle = 5;
  unsigned long fadeDelay = 20;
};

LED ledArray[14];
String serialBuffer = "";
bool ready = false;

void setup() {
  Serial.begin(9600);
  for(int i = 0; i < 14; i++)
  {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }
  Serial.println("Sistem pregatit pentru comenzi.");
}

void loop() {
  if(ready)
  {
    runCmd(serialBuffer);
    serialBuffer = "";
    ready = false;
  }

  unsigned long t = millis();
  for(int i = 0; i < 14; i++)
  {
    if(ledArray[i].on)
      updateLED(i, ledArray[i], t);
  }
}

void serialEvent()
{
  while(Serial.available())
  {
    char c = (char)Serial.read();
    if(c == '\n' || c == '\r')
      ready = true;
    else
      serialBuffer += c;
  }
}

void runCmd(String cmd)
{
  cmd.trim();
  if(cmd.length() == 0) return;

  int c1 = cmd.indexOf(',');
  int c2 = cmd.indexOf(',', c1 + 1);
  if(c1 == -1 || c2 == -1)
  {
    Serial.println("Comanda invalida");
    return;
  }

  String pins = cmd.substring(0, c1);
  String m = cmd.substring(c1 + 1, c2);
  unsigned long d = cmd.substring(c2 + 1).toInt();

  int dash = pins.indexOf('-');
  int s, e;
  if(dash != -1)
  {
    s = pins.substring(0, dash).toInt();
    e = pins.substring(dash + 1).toInt();
  }
  else
    s = e = pins.toInt();

  for(int i = s; i <= e; i++)
  {
    if(i < 0 || i >= 14) continue;

    pinMode(i, OUTPUT);
    ledArray[i].on = true;
    ledArray[i].mode = m;
    ledArray[i].dur = d;
    ledArray[i].last = millis();
    ledArray[i].bright = (m == "fadeout") ? 255 : 0;
    ledArray[i].dir = 1;
    ledArray[i].state = false;

    Serial.print("LED ");
    Serial.print(i);
    Serial.print(" -> ");
    Serial.print(m);
    Serial.print(", ");
    Serial.print(d);
    Serial.println(" ms");
  }
}

bool isPWM(int pin)
{
  return (pin == 9 || pin == 10 || pin == 11);
}

void updateLED(int pin, LED &led, unsigned long now)
{
  if(led.mode == "on") digitalWrite(pin, HIGH);
  else if(led.mode == "off") digitalWrite(pin, LOW);
  else if(led.mode == "blink")
  {
    if(now - led.last >= led.dur / 2)
    {
      led.state = !led.state;
      digitalWrite(pin, led.state ? HIGH : LOW);
      led.last = now;
    }
  }
  else if(led.mode == "fadein" || led.mode == "fadeout" || led.mode == "fadeinout")
  {
    unsigned long step = led.dur / 255;
    if(step < 1) step = 1;

    if(now - led.last >= step)
    {
      led.last = now;

      if(led.mode == "fadein")
      {
        led.bright++;
        if(led.bright > 255) led.bright = 0;
      }
      else if(led.mode == "fadeout")
      {
        led.bright--;
        if(led.bright < 0) led.bright = 255;
      }
      else if(led.mode == "fadeinout")
      {
        led.bright += led.dir;
        if(led.bright >= 255) { led.bright = 255; led.dir = -1; }
        if(led.bright <= 0)   { led.bright = 0;   led.dir = 1;  }
      }
    }

    unsigned long tPWM = now % led.cycle;
    unsigned long tOn = (led.bright * led.cycle) / 255;
    digitalWrite(pin, tPWM < tOn ? HIGH : LOW);
  }
}
