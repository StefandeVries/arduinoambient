// midi shield
// button test pullup resistor

const int leftPot = A0;

const int rightButton = 2;
const int middleButton = 3;
const int leftButton = 4;

const int led = 13;

const byte lowestNote = 36; // C2
const byte highestNote = 84; // C6

bool toLearn = true;
byte result;

struct intervalPair
{
  byte first;
  byte second;
};

intervalPair pairs[10];

// stores absolute note values
// for MIDI purposes
byte lastNotes[3];

void setup()
{
  pinMode(led, OUTPUT);
  pinMode(leftButton, INPUT_PULLUP); // enables pull up resistor for button 2
  pinMode(middleButton, INPUT_PULLUP); // etc.
  pinMode(rightButton, INPUT_PULLUP);

  prepareLearn();

  Serial.begin(31250); // MIDI baudrate is unusual
  digitalWrite(led, HIGH);
  wait();
}

void loop()
{
  if (toLearn)
  {
    learn();
  }
  else
  {
    execute();
  }
}

void learn()
{
  int i;
  for (i = 0; i < 10; )
  {
    buildHarmony();

    result = evalHarmony();
    if (result == 0)
    {
      saveToPairs(i);      
      // this way, we will have ten approved harmonies
      // in the pairs array
      i++;
    }
    else if (result == 2)
    {
      break;
    }
  }

  toLearn = false;
}


void buildHarmony()
{
  //   Pick three notes at least an octave (12 half steps) away from each other
  int i;
  
  lastNotes[0] = random(lowestNote, lowestNote + 12);
  lastNotes[1] = random(lastNotes[0] + 12, lastNotes[0] + 24);
  lastNotes[2] = random(lastNotes[1] + 12, lastNotes[1] + 24);

  for (i = 0; i < 3; i++)
  {
    send_note(lastNotes[i], 80); // ..and have the synth output them
  }

  return;
}

byte evalHarmony()
{
  byte r;
  while (true)
  {
    delay(500);
    if (digitalRead(leftButton) == LOW) // if harmony sounds alright
    {
      r = 0;
      break;
    }
    else if (digitalRead(middleButton) == LOW) // if harmony sounds crappy
    {
      r = 1; // Nothing is done with this specific return value, but just to be future proof
      break;
    }
    else if (digitalRead(rightButton) == LOW) // exit learn() immediately, go to execute() (for now)
    {
      r = 2;
      break;
    }
  }

  killMIDI(); // make sure the previous chord is gone
  delay(150); // synthesizer patch should have some kind of release value for smooth transition

  return r;
}

// pairs are saved with no regard to octaves
// and bass notes to enable a scale-based
// approach later on
void saveToPairs(int i)
{
    pairs[i].first = lastNotes[1] - lastNotes[0];
    pairs[i].first = lastNotes[2] - lastNotes[1];
}

void execute()
{
  killMIDI();
  wait();
}


/*
   Helper functions in working order.
   Should not edit this, will be moved to header file.
*/

void initLastNotes()
{
  int i;
  for (i = 0; i < 3; i++)
  {
    lastNotes[i] = 0;
  }
}

void initPairs()
{
  int i;
  for (i = 0; i < 10; i++)
  {
    pairs[i].first = pairs[i].second = 0;
  }
}

void killMIDI()
{
  // kill all sound on channel 1
  Serial.flush();
  Serial.write(0xB0); // channel 1
  Serial.write(0x78);
  Serial.write(0x00);
}

void endChord()
{
   // TODO: implement key off for all lastNotes
   // for smoother transitions
}

void prepareLearn()
{
  // make sure there is no sound output
  // and all the necessary arrays are initialized
  killMIDI();
  initLastNotes();
  initPairs();
}

byte checkRange(byte note)
{
  if (note >= lowestNote && note <= highestNote)
  {
    return note;
  }
  else
  {
    return 0;
  }
}

void send_note(unsigned char note, unsigned char velocity)
{
  Serial.write(0x90); // Dec 144, note on for channel 1
  Serial.write(note);
  Serial.write(velocity);
}

void wait()
{
  // behavior where the LED blinks quickly
  // and a button push is required to carry on
  while (digitalRead(leftButton) == HIGH)
  {
    digitalWrite(led, HIGH);
    delay(150);
    digitalWrite(led, LOW);
    delay(150);
  }
  digitalWrite(led, LOW);
}








