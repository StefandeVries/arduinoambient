// midi shield
// button test pullup resistor

int leftPot = A0;
int rightPot = A1;

int rightButton = 2;
int middleButton = 3;
int leftButton = 4;

int led = 13;
int start = 36; // C2
byte play = 1;
byte multiplier = 1;
int i;

void panic()
{
  // kill all sound on channel 1
  Serial.flush();
  Serial.write(0xB0); // channel 1
  Serial.write(0x78);
  Serial.write(0x00);
}

void send_midi(unsigned char command, unsigned char note, unsigned char velocity)
{
  Serial.write(command);
  Serial.write(note);
  Serial.write(velocity);
}

void setup()
{
  pinMode(led, OUTPUT);
  pinMode(leftButton, INPUT_PULLUP); // enables pull up resistor for button 2
  pinMode(middleButton, INPUT_PULLUP); // etc.
  pinMode(rightButton, INPUT_PULLUP);

  Serial.begin(31250);
  digitalWrite(led, HIGH);
  while(digitalRead(leftButton) == HIGH);
}

void loop()
{
  digitalWrite(led, LOW);
  play = 0;
  for (i = 0; i < 13; i++)
  {
    send_midi(144, 60+i, 80);
    send_midi(144, 60+i+6, 80);
    delay(500);
    send_midi(144, 60+i, 0);
    send_midi(144, 60+i+6, 0);
    delay(200);
  }
  if(play == 0)
  {
     stuck(); 
  }
}

void stuck()
{
   while(true); 
}







