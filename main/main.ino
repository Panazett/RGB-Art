/*
  RGB Spectacle

  Controlls the lighting modes, rotation of motor and interprets inputs as different
  colour modes and further settings.

  The circuit:
  - NA

  created 10 Aug 2017
  by David de Vigne
*/

int mode = 0;

// Pins
int RGB1red = 9;
int RGB1green = 10;
int RGB1blue = 11;
int buttonPin = 2;
int motorPin = 3;

// Cycles
int buttonState = 0;
int lastPress;

int firstCycle = true;

int spectrum1;
int spectrum2;
int spectrum3;
int led;

int breath;

int c1;
int c2;
int c3;

int lastPick;
int pick;

//Touch Sensor Setup
int capSensePin = 5;
int touchedCutoff = 60;

void setup() {
  Serial.begin(9600);
  
  pinMode(RGB1red, OUTPUT);
  pinMode(RGB1green, OUTPUT);
  pinMode(RGB1blue, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(motorPin, OUTPUT);
}

void loop() {
  Serial.println(readCapacitivePin(capSensePin));
 
  // Changing Colour Modes and Button Input Checker
  buttonState = readCapacitivePin(capSensePin); // Touch Sensor
  //buttonState = digitalRead(buttonPin); // Push Button
  buttonCheck();

  //Serial.println(mode); // Debug
  if(mode > 0){
    analogWrite(motorPin, 150);
  } else {
    analogWrite(motorPin, 0);
  }

  // Spectrum Cycling (Mode 1)
  
  if (mode == 1){
    if(firstCycle){
      colourFade();
      led = 1;
    }
    if (led == 1){
      spectrum1 ++;
      if(spectrum3 > 0){
        spectrum3 --;
      }
      if (spectrum1 >= 255){
        led = 2;
      }
    } else if (led == 2){
      spectrum2 ++;
      spectrum1 --;
      if (spectrum2 >= 255){
        led = 3;
      }
    } else if (led == 3){
      spectrum3 ++;
      spectrum2 --;
      if (spectrum3 >= 255){
        led = 1;
      }
    }
    setColor(spectrum1, spectrum2, spectrum3);
    firstCycle = false;
    delay(5);



    // Breathing (Mode 2)
    
  } else if (mode == 2) {
    if(firstCycle){
      colourFade();
      led = 1;
      breath = 0;
      colourPicker();
    }
    if(led == 1){
      breath ++;
      if(breath >= 255){
        led = 2;
      }
    } else if (led == 2){
      breath --;
      if(breath <= 0){
        led = 1;
        colourPicker();
      }
    }
   spectrum1 = map(breath, 0, 255, 0, c1);
   spectrum2 = map(breath, 0, 255, 0, c2);
   spectrum3 = map(breath, 0, 255, 0, c3);
   setColor(spectrum1, spectrum2, spectrum3);
   firstCycle = false;
   delay(7);



   // Stars (Mode 3)
   
  } else if (mode == 3) {
    if(firstCycle){
      colourFade();
      breath = 0;
    }
    if(breath >= 150){
      colourPicker2();
      setColor(spectrum1, spectrum2, spectrum3);
      delay(15);
      spectrum1 = 0;
      spectrum2 = 0;
      spectrum3 = 0;
      setColor(0, 0, 0);
      breath = 0;
    }
    breath ++;
    firstCycle = false;
  }
}

// Function to set colour for led when called

void setColor(int red, int green, int blue) {
  analogWrite(RGB1red, red);
  analogWrite(RGB1green, green);
  analogWrite(RGB1blue, blue);  
}

// Randomly select colour for use in the map function

void colourPicker(){
  pick = random(1,9);
  while(pick == lastPick){
    pick = random(1,9);
  }
  lastPick = pick;
  if(pick == 1) { // Red
    c1 = 255;
    c2 = 0;
    c3 = 0;
  } else if (pick == 2) { // Green
    c1 = 0;
    c2 = 255;
    c3 = 0;
  } else if (pick == 3) { // Blue
    c1 = 0;
    c2 = 0;
    c3 = 255;
  } else if (pick == 4) { // Orange
    c1 = 255;
    c2 = 60;
    c3 = 0;
  } else if (pick == 5) { // Purple
    c1 = 80;
    c2 = 0;
    c3 = 80;
  } else if (pick == 6) { // White
    c1 = 255;
    c2 = 255;
    c3 = 255;
  } else if (pick == 7) { // Cyan
    c1 = 0;
    c2 = 255;
    c3 = 255;
  } else if (pick == 8) { // Pink
    c1 = 255;
    c2 = 0;
    c3 = 147;
  } 
}

// Randomly select colour

void colourPicker2(){
  pick = random(1,9);
  while(pick == lastPick){
    pick = random(1,9);
  }
  lastPick = pick;
  if(pick == 1) { // Red
    spectrum1 = 255;
    spectrum2 = 0;
    spectrum3 = 0;
  } else if (pick == 2) { // Green
    spectrum1 = 0;
    spectrum2 = 255;
    spectrum3 = 0;
  } else if (pick == 3) { // Blue
    spectrum1 = 0;
    spectrum2 = 0;
    spectrum3 = 255;
  } else if (pick == 4) { // Orange
    spectrum1 = 255;
    spectrum2 = 60;
    spectrum3 = 0;
  } else if (pick == 5) { // Purple
    spectrum1 = 80;
    spectrum2 = 0;
    spectrum3 = 80;
  } else if (pick == 6) { // White
    spectrum1 = 255;
    spectrum2 = 255;
    spectrum3 = 255;
  } else if (pick == 7) { // Cyan
    spectrum1 = 0;
    spectrum2 = 255;
    spectrum3 = 255;
  } else if (pick == 8) { // Pink
    spectrum1 = 255;
    spectrum2 = 0;
    spectrum3 = 147;
  } 
}

// Fade Colour when the colour mode is changed

void colourFade(){
  while(spectrum1 > 0 || spectrum2 > 0 || spectrum3 > 0){
        if(spectrum1 > 0){
          spectrum1 --;
        }
        if(spectrum2 > 0){
          spectrum2 --;
        }
        if(spectrum3 > 0){
          spectrum3 --;
        }
        setColor(spectrum1, spectrum2, spectrum3);
        delay(2);
  }
}

// Check function to correctly cycle through colour modes

void buttonCheck(){
  if (buttonState == 1) {
    if (lastPress != buttonState){
      mode ++;
      firstCycle = true;
      if(mode >= 4){
        mode = 0;
      }
    }
  }
  lastPress = buttonState;
}


// This code was obtained and studied from, https://www.instructables.com/id/Turn-a-pencil-drawing-into-a-capacitive-sensor-for/
// This function calulates the capacitance by the feild generated from the human touch and calculates wheather the return should be true or false
// I did not write this myself, but used a tutorial to help understand how it is achieved

uint8_t readCapacitivePin(int pinToMeasure){

  volatile uint8_t* port;
  volatile uint8_t* ddr;
  volatile uint8_t* pin;

  byte bitmask;
  if ((pinToMeasure >= 0) && (pinToMeasure <= 7)){
    port = &PORTD;
    ddr = &DDRD;
    bitmask = 1 << pinToMeasure;
    pin = &PIND;
  }
  if ((pinToMeasure > 7) && (pinToMeasure <= 13)){
    port = &PORTB;
    ddr = &DDRB;
    bitmask = 1 << (pinToMeasure - 8);
    pin = &PINB;
  }
  if ((pinToMeasure > 13) && (pinToMeasure <= 19)){
    port = &PORTC;
    ddr = &DDRC;
    bitmask = 1 << (pinToMeasure - 13);
    pin = &PINC;
  }
  // Discharge the pin first by setting it low and output
  *port &= ~(bitmask);
  *ddr  |= bitmask;
  delay(1);
  // Make the pin an input WITHOUT the internal pull-up on
  *ddr &= ~(bitmask);
  // Now see how long the pin to get pulled up
  int cycles = 16000;
  for(int i = 0; i < cycles; i++){
    if (*pin & bitmask){
      cycles = i;
      break;
    }
  }
  *port &= ~(bitmask);
  *ddr  |= bitmask;
  
  return cycles;
}