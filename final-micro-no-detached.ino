#include <DIYables_IRcontroller.h>
#include <Servo.h>

#define IR_RECEIVER_PIN 9

void smoothMove(Servo &servo, int startAngle, int endAngle, int stepDelay = 10) {
  int step = (startAngle < endAngle) ? 1 : -1;

  for (int angle = startAngle; angle != endAngle; angle += step) {
    servo.write(angle);
    delay(stepDelay); // small pause between steps
  }

  servo.write(endAngle); // ensure final position is set
}




// Ultrasonic pins
const int trigPin = 19;
const int echoPin = 18;

// Distance value
long duration;


int readDistanceCM() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  return duration * 0.034 / 2; // Convert to centimeters
}

unsigned long lastDistanceCheck = 0;
const unsigned long distanceInterval = 500;
int distance = 0;



// Declare servos with readable names (based on pin numbers)
Servo s_2, s_3, s_4, s_5, s_6, s_7, s_8, s_10, s_11, s_12, s_14, s_15, s_16, s_17, s_22;

// Pin definitions
const int pin_2 = 2;
const int pin_3 = 3;
const int pin_4 = 4;
const int pin_5 = 5;
const int pin_6 = 6;
const int pin_7 = 7;
const int pin_8 = 8;

const int pin_10 = 10;
const int pin_11 = 11;
const int pin_12 = 12;
const int pin_14 = 14;
const int pin_15 = 15;
const int pin_16 = 16;
const int pin_17 = 17;
const int pin_22 = 22;

DIYables_IRcontroller_17 irController(IR_RECEIVER_PIN, 200);

// State tracking
int currentMode = -1;
unsigned long lastStepTime = 0;
int sequenceStep = 0;

void setup() {
  Serial.begin(9600);
  irController.begin();

  // Attach all servos and center them
  s_2.attach(pin_2);   s_2.write(90);
  s_3.attach(pin_3);   s_3.write(90);
  s_4.attach(pin_4);   s_4.write(90);
  s_5.attach(pin_5);   s_5.write(90);
  s_6.attach(pin_6);   s_6.write(90);
  s_7.attach(pin_7);   s_7.write(90);
  s_8.attach(pin_8);   s_8.write(90);


  s_10.attach(pin_10); s_10.write(90);
  s_11.attach(pin_11); s_11.write(90);
  s_12.attach(pin_12); s_12.write(90);
  s_14.attach(pin_14); s_14.write(90);
  s_15.attach(pin_15); s_15.write(90);
  s_16.attach(pin_16); s_16.write(90);
  s_17.attach(pin_17); s_17.write(90);
  s_22.attach(pin_22); s_22.write(90);

pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);


  Serial.println("System ready. Press a key (1–8) to activate a mode, 0 to stop.");
}

void loop() {
  // Read IR remote key

  Key17 key = irController.getKey();
  if (key != Key17::NONE) {
    switch (key) {
      case Key17::KEY_0: switchMode(0); break;
      case Key17::KEY_1: switchMode(1); break;
      case Key17::KEY_2: switchMode(2); break;
      case Key17::KEY_3: switchMode(3); break;
      case Key17::KEY_4: switchMode(4); break;
      case Key17::KEY_5: switchMode(5); break;
      case Key17::KEY_6: switchMode(6); break;
      case Key17::KEY_7: switchMode(7); break;
      case Key17::KEY_8: switchMode(8); break;
      case Key17::KEY_9: switchMode(9); break;
      
      case Key17::KEY_STAR:  switchMode(10); break;
      case Key17::KEY_SHARP:  switchMode(11); break;
      case Key17::KEY_UP:    switchMode(12); break;
      case Key17::KEY_DOWN:  switchMode(13); break;
      case Key17::KEY_LEFT:  switchMode(14); break;
      case Key17::KEY_RIGHT: switchMode(15); break;
      case Key17::KEY_OK:    switchMode(16); break;
    }
  }

  // Execute mode behavior
  switch (currentMode) {
    case 1: mode1_behavior(); break;
    case 2: mode2_behavior(); break;
    case 3: mode3_behavior(); break;
    case 4: mode4_behavior(); break;
    case 5: mode5_behavior(); break;
    case 6: mode6_behavior(); break;
    case 7: mode7_behavior(); break;
    case 8: mode8_behavior(); break;
    case 9: mode9_behavior(); break;
    case 10: mode10_behavior(); break;
    case 11: mode11_behavior(); break;
    case 12: mode12_behavior(); break;
    case 13: mode13_behavior(); break;
    case 14: mode14_behavior(); break;
    case 15: mode15_behavior(); break;
    case 16: mode16_behavior(); break;
    // Add more modes here
  }
}

void switchMode(int newMode) {
  Serial.print("Switching to mode: ");
  Serial.println(newMode);

  // Reset sequence
  currentMode = (newMode == 0) ? -1 : newMode;
  sequenceStep = 0;
  lastStepTime = millis();

  // Optionally: reset servos to neutral
  if (newMode == 0) {
    s_2.write(90);
    s_3.write(90);
    s_4.write(90);
    s_5.write(90);
    s_10.write(90);
    s_11.write(90);
    s_12.write(90);
    s_14.write(90);
    Serial.println("All servos centered.");
  }
}

// === MODE 1 ===
// Sequential motion across servos
void mode1_behavior() {
  // Read distance every 500ms
  if (millis() - lastDistanceCheck >= distanceInterval) {
    distance = readDistanceCM();
    lastDistanceCheck = millis();
  }

  // Exit walking if distance is within the stop range
  if (distance >= 10 && distance <= 30) {
    return;
  }

  // Continue walking every 1500ms
  unsigned long interval = 1500;
  if (millis() - lastStepTime > interval) {
    switch (sequenceStep) {
      case 0:
        s_2.write(93); s_3.write(45); s_4.write(125); s_5.write(96);
        s_10.write(91); s_11.write(70); s_12.write(115); s_14.write(94);
        s_6.write(0); s_7.write(90); s_8.write(150);
        s_15.write(130); s_16.write(90); s_17.write(30);
        s_22.write(100);
        break;

      case 1:
        s_2.write(101); s_3.write(110); s_4.write(65); s_5.write(96);
        s_10.write(92); s_11.write(135); s_12.write(60); s_14.write(94);
        s_6.write(50); s_7.write(90); s_8.write(150);
        s_15.write(180); s_16.write(90); s_17.write(30);
        s_22.write(80);
        break;
    }

    sequenceStep = (sequenceStep + 1) % 2;
    lastStepTime = millis();
  }
}


// === MODE 2 ===
// Simultaneous motion of s_5, s_10, s_11
void mode2_behavior() {
  unsigned long interval = 1000; // 1 second per step

  if (millis() - lastStepTime > interval) {
    switch (sequenceStep) {
      case 0:
    s_2.write(101); 
    s_3.write(140); 
    s_4.write(150); 
    s_5.write(96);

    s_10.write(91); 
    s_11.write(40); 
    s_12.write(30); 
    s_14.write(94);

    s_6.write(50); //right
    s_7.write(90);
    s_8.write(25);

    s_15.write(130); //left
    s_16.write(90);
    s_17.write(150);

    s_22.write(90);

        break;
            case 1:
    s_2.write(101); 
    s_3.write(140); 
    s_4.write(150); 
    s_5.write(96);

    s_10.write(91); 
    s_11.write(40); 
    s_12.write(30); 
    s_14.write(94);

    s_6.write(120); //right
    s_7.write(90);
    s_8.write(180);

    s_15.write(130); //left
    s_16.write(90);
    s_17.write(150);

    s_22.write(120);
            break;
  case 2:

    s_2.write(101); 
    s_3.write(140); 
    s_4.write(150); 
    s_5.write(96);

    s_10.write(91); 
    s_11.write(40); 
    s_12.write(30); 
    s_14.write(94);

    s_6.write(50); //right
    s_7.write(90);
    s_8.write(25);

    s_15.write(130); //left
    s_16.write(90);
    s_17.write(150);

 s_22.write(90);


  break;

case 3:

    s_2.write(101); 
    s_3.write(140); 
    s_4.write(150); 
    s_5.write(96);

    s_10.write(91); 
    s_11.write(40); 
    s_12.write(30); 
    s_14.write(94);

    s_6.write(50); //right
    s_7.write(90);
    s_8.write(25);

    s_15.write(60); //left
    s_16.write(90);
    s_17.write(0);

     s_22.write(60);

  break;




   
    }

    sequenceStep = (sequenceStep + 1) % 4;
    lastStepTime = millis();
  }
}

//MODE 3
void mode3_behavior() {
  
  unsigned long interval = 1200; // 1 second per step
   
  if (millis() - lastStepTime > interval) {
    switch (sequenceStep) {
    
      case 0:
   s_2.write(111); 
    s_3.write(90); 
    s_4.write(85); 
    s_5.write(96);

    s_10.write(101); 
    s_11.write(89); 
    s_12.write(89); 
    s_14.write(94);

    s_6.write(180); //right
    s_7.write(130);
    s_8.write(180);

    s_15.write(0); //left
    s_16.write(130);
    s_17.write(0);


        break;
              case 1:
   s_2.write(89); 
    s_3.write(90); 
    s_4.write(85); 
    s_5.write(96);

    s_10.write(79); 
    s_11.write(89); 
    s_12.write(89); 
    s_14.write(94);

    s_6.write(180); //right
    s_7.write(50);
    s_8.write(180);

    s_15.write(0); //left
    s_16.write(50);
    s_17.write(0);


        break;

    }

    sequenceStep = (sequenceStep + 1) % 2;
    lastStepTime = millis();
  }
}

//MODE 4
void mode4_behavior() {
  unsigned long interval = 1000; // 1 second per step

  if (millis() - lastStepTime > interval) {
    switch (sequenceStep) {
      case 0:
  s_2.write(101); 
    s_3.write(90); 
    s_4.write(85); 
    s_5.write(96);

    s_10.write(91); 
    s_11.write(89); 
    s_12.write(89); 
    s_14.write(94);

    s_6.write(180); //right
    s_7.write(90);
    s_8.write(180);

    s_15.write(0); //left
    s_16.write(90);
    s_17.write(0);
    delay(1000);
        break;
      
    }
    sequenceStep = (sequenceStep + 1) % 1;
    lastStepTime = millis();
  }
}

//MODE 5
void mode5_behavior() {
  unsigned long interval = 1000; // 1 second per step

  if (millis() - lastStepTime > interval) {
    switch (sequenceStep) {
      case 0:
    s_2.write(101); 
    s_3.write(90); 
    s_4.write(85); 
    s_5.write(96);

    s_10.write(91); 
    s_11.write(89); 
    s_12.write(89); 
    s_14.write(94);

   s_6.write(20); //right
    s_7.write(90);
    s_8.write(180);

    s_15.write(160); //left
    s_16.write(90);
    s_17.write(0);
    delay(1000);
        break;
      
    }
    sequenceStep = (sequenceStep + 1) % 1;
    lastStepTime = millis();
  }
}

//MODE 6
void mode6_behavior() {
  unsigned long interval = 1000; // 1 second per step

  if (millis() - lastStepTime > interval) {
    switch (sequenceStep) {
      case 0:


    s_2.write(101); 
    s_3.write(90); 
    s_4.write(50); //0 up
    s_5.write(96);

    s_10.write(91); 
    s_11.write(89); 
    s_12.write(130); //180 up
    s_14.write(94);

    s_6.write(0); //right
    s_7.write(90);
    s_8.write(90);

    s_15.write(180); //left
    s_16.write(90);
    s_17.write(90);

    

        break;

              case 1:


    s_2.write(101); 
    s_3.write(90); 
    s_4.write(110); 
    s_5.write(96);

    s_10.write(91); 
    s_11.write(89); 
    s_12.write(70); 
    s_14.write(94);

    s_6.write(90); //right
    s_7.write(90);
    s_8.write(90);

    s_15.write(90); //left
    s_16.write(90);
    s_17.write(90);

    

        break;
      
    }
    sequenceStep = (sequenceStep + 1) % 2;
    lastStepTime = millis();
  }
}

//MODE 7
void mode7_behavior() {
  unsigned long interval = 1000; // 1 second per step

  if (millis() - lastStepTime > interval) {
    switch (sequenceStep) {
      case 0:
        s_10.write(91);
        break;
      
    }
    sequenceStep = (sequenceStep + 1) % 6;
    lastStepTime = millis();
  }
}

//MODE 8
void mode8_behavior() {
  unsigned long interval = 1000; // 1 second per step

  if (millis() - lastStepTime > interval) {
    switch (sequenceStep) {
      case 0:
        s_10.write(91);
        break;
      
    }
    sequenceStep = (sequenceStep + 1) % 6;
    lastStepTime = millis();
  }
}

//MODE 9
void mode9_behavior() {
  unsigned long interval = 1000; // 1 second per step

  if (millis() - lastStepTime > interval) {
    switch (sequenceStep) {
      case 0:
        s_10.write(91);
        break;
      
    }
    sequenceStep = (sequenceStep + 1) % 6;
    lastStepTime = millis();
  }
}


//MODE 10
void mode10_behavior() {
  unsigned long interval = 1000; // 1 second per step

  if (millis() - lastStepTime > interval) {
    switch (sequenceStep) {
      case 0:
        s_10.write(91);
        break;
      
    }
    sequenceStep = (sequenceStep + 1) % 6;
    lastStepTime = millis();
  }
}

//MODE 11
void mode11_behavior() {
  unsigned long interval = 1000; // 1 second per step

  if (millis() - lastStepTime > interval) {
    switch (sequenceStep) {
      case 0:
        s_10.write(91);
        break;
      
    }
    sequenceStep = (sequenceStep + 1) % 6;
    lastStepTime = millis();
  }
}

//MODE 12 UP
void mode12_behavior() { //UP
  unsigned long interval = 1500; // 1 second per step

  if (millis() - lastStepTime > interval) {
    switch (sequenceStep) {

     case 0:
    s_2.write(93); // center
  s_3.write(45);
  s_4.write(125);
  s_5.write(96);

  s_10.write(91);
  s_11.write(70);
  s_12.write(115);
  s_14.write(94);

    s_6.write(0); //right
    s_7.write(90);
    s_8.write(150);

    s_15.write(130); //left
    s_16.write(90);
    s_17.write(30);
    
     s_22.write(100);
          break;
     case 1:

  s_2.write(101); // center
  s_3.write(110);
  s_4.write(65);
  s_5.write(96);

  s_10.write(92);
  s_11.write(135);
  s_12.write(60);
  s_14.write(94);

  
    s_6.write(50); //right
    s_7.write(90);
    s_8.write(150);

    s_15.write(180); //left
    s_16.write(90);
    s_17.write(30);

       s_22.write(80);
    
        break;
    
    
      
    }
    sequenceStep = (sequenceStep + 1) % 2;
    lastStepTime = millis();
  }
}

//MODE 13 DOWN
void mode13_behavior() { // DOWN
  unsigned long interval = 500; // second per step

  if (millis() - lastStepTime > interval) {
    switch (sequenceStep) {
         case 0:
      s_2.write(101); // center
      s_3.write(55);
      s_4.write(90);
      s_5.write(96);

      s_10.write(91);
      s_11.write(45);
      s_12.write(110);
      s_14.write(94);
          s_22.write(80);

     s_6.write(50); //right
    s_7.write(90);
    s_8.write(150);

    s_15.write(180); //left
    s_16.write(90);
    s_17.write(30);
              break;  

          case 1:
      s_2.write(101); // center
      s_3.write(130);
      s_4.write(70);
      s_5.write(96);

      s_10.write(91);
      s_11.write(125);
      s_12.write(89);
      s_14.write(94);
          s_22.write(100);
  s_6.write(0); //right
    s_7.write(90);
    s_8.write(150);

    s_15.write(130); //left
    s_16.write(90);
    s_17.write(30);
        break;
      
    }
    sequenceStep = (sequenceStep + 1) % 2;
    lastStepTime = millis();
  }
}

//MODE 14 LEFT
void mode14_behavior() { // LEFT
  unsigned long interval =500; // 1 second per step

  if (millis() - lastStepTime > interval) {
    switch (sequenceStep) {
      
      case 0:
  
      s_2.write(125);
  s_3.write(135);
  s_4.write(115);
  s_5.write(98);

  s_10.write(91);
  s_11.write(35);
  s_12.write(65);
  s_14.write(100);


    s_6.write(180); //right
    s_7.write(50);
    s_8.write(180);

    s_15.write(0); //left
    s_16.write(130);
    s_17.write(0);

          break;
       case 1:

   s_2.write(70);
  s_3.write(135);
  s_4.write(115);
  s_5.write(80);

  s_10.write(80);
  s_11.write(35);
  s_12.write(65);
  s_14.write(104);

      s_6.write(180); //right
    s_7.write(180);
    s_8.write(180);

    s_15.write(0); //left
    s_16.write(0);
    s_17.write(0);
  
        break;
      
      
    }
    sequenceStep = (sequenceStep + 1) % 2;
    lastStepTime = millis();
  }
}

//MODE 15 RIGHT
void mode15_behavior() { //RIGHT
  unsigned long interval = 500; // 1 second per step

  if (millis() - lastStepTime > interval) {
    switch (sequenceStep) {
      case 0:
  
  s_2.write(101);
  s_3.write(145);
  s_4.write(115);
  s_5.write(90);

  s_10.write(67);
  s_11.write(45);
  s_12.write(65); 
  s_14.write(88);

 
    s_6.write(180); //right
    s_7.write(50);
    s_8.write(180);

    s_15.write(0); //left
    s_16.write(130);
    s_17.write(0);

          break;
       case 1:

  s_2.write(130);
  s_3.write(145);
  s_4.write(115);
  s_5.write(86);

  s_10.write(125);
  s_11.write(45);
  s_12.write(65);
  s_14.write(110);

      s_6.write(180); //right
    s_7.write(180);
    s_8.write(180);

    s_15.write(0); //left
    s_16.write(0);
    s_17.write(0);
  
        break;
      
    }
    sequenceStep = (sequenceStep + 1) % 2;
    lastStepTime = millis();
  }
}

//MODE 16 OK
void mode16_behavior() { // OK
  unsigned long interval = 1000; // 1 second per step

  if (millis() - lastStepTime > interval) {
    switch (sequenceStep) {
      case 0:
      //legs
    s_2.write(101);
    s_3.write(90);
    s_4.write(85);
    s_5.write(96);

    s_10.write(91); 
    s_11.write(89); 
    s_12.write(89); 
    s_14.write(94);

        
    s_22.write(90);

    //arms

    s_6.write(90); //right
    s_7.write(90);
    s_8.write(90);

    s_15.write(90); //left
    s_16.write(90);
    s_17.write(90);


        break;
      
    }
    sequenceStep = (sequenceStep + 1) % 1;
    lastStepTime = millis();
  }
}





