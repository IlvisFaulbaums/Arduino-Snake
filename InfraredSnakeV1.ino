#include <Arduino.h>
#include <TM1637Display.h>
#include <IRremote.h>
// Module connection pins (Digital Pins)
#define CLK 2
#define DIO 3

int RECV_PIN = 11; // define input pin on Arduino
IRrecv irrecv(RECV_PIN);
decode_results results; // decode_results class is defined in IRremote.h

///ideja: varbuut skaitiit noietos soljus!
//kad beidzas punkti - autoresets!
//button pins:
const int buttonPinRed = 2;
const int buttonPinOrange = 3;
const int buttonPinYellow = 4;
const int buttonPinGreen = 5;
// Variables will change:
int RedButtonState = 0;         // current state of the button
int OrangeButtonState = 0;
int YellowButtonState = 0;
int GreenButtonState = 0;

long lastButtonStateRed = 0;     // previous state of the button
long lastButtonStateOrange = 0;
long lastButtonStateYellow = 0;
long lastButtonStateGreen = 0;

int foodA = 0;
int foodF = 0;
int foodB = 0;
int foodG = 0;
int foodE = 0;
int foodC = 0;
int foodD = 0;
unsigned char dot[] = {128, 0, 0, 0};
long randomDotSegment = 0;
//int dot[1] = 0;
//int dot[2] = 0;
//int dot[3] = 0;
int points = 10; //starting points
//| foodB | foodG | foodE | foodC | foodD

//     A
//    ---
// F |   | B
//    -G-
// E |   | C
//    --- .   // . = 128
//     D

// The amount of time (in milliseconds) between tests
#define TEST_DELAY   2000
int horizontalMovement = 0;
int verticalMovement = 0;
int Segment = 0;
int upperHalf = 0;
String Segments = "";
int line[] = {1, 0, 0, 0};
int posX = 0; // current position on horizontal line

int incomingByte = 0;
int mvmt = 0;
int pushedColor = 0;
int movementX = 1;
int movementY = 1;
int lnNmbr = 0;
double buttonState = 0;
double lastButtonState = 0;
const int signalDelay = 200; // bija 400 un gaaja labi
uint8_t SEG_LINE_0[] = {0x0, 0x0, 0x0, 0x0};
String digit[] = {" --> \n", "|   | \n", " --- \n", "|   | \n", " --- \n"}; //cipars 8}
uint8_t SEG_LINE_HACK[] = {1 | 0 | 128, 0x0, 0x0, 0x0};
TM1637Display display(CLK, DIO);

void setup()
{
//  int fs = fullSegment();
  
//  addSegment();
  irrecv.enableIRIn(); // Start the receiver
  pinMode(buttonPinRed, INPUT_PULLUP);
  pinMode(buttonPinOrange, INPUT_PULLUP);
  pinMode(buttonPinYellow, INPUT_PULLUP);
  pinMode(buttonPinGreen, INPUT_PULLUP);
  Serial.begin(9600);
  Serial.println("Snake All Segments and 4 control buttons");
  //  Serial.println(Segments);
  Serial.println("Use these keys to control the snake: 'a', 's', 'd', 'w' and after every key, press 'Enter'!");
  display.setBrightness(15);
  Serial.println(SEG_A); //1
  Serial.println(SEG_B); //2
  Serial.println(SEG_C); //4
  Serial.println(SEG_D); //8
  Serial.println(SEG_E); //16
  Serial.println(SEG_F); //32
  Serial.println(SEG_G); //64
  Serial.println("punkta integers = 128;"); // . = 128
  Serial.println(0x0);   //0
  Serial.println(0xff);  //255
//        pushedColor = 115; //down
//Shiis rindas pievienoju, lai vareetu redzeet pirmos segmentus jau no saakuma
        int fS;
        fS = fullSegment();
        addSegment();  //Shii ir loti svariiga rinda!!!!!!!!!
}

void loop() {
  ////////
  if (irrecv.decode(&results)) {
    
    buttonState = results.value;
    if (buttonState != lastButtonState) {
      if (buttonState == 650140) {
        Serial.println("down");
        pushedColor = 115; //down
        int fS;
        fS = fullSegment();
        addSegment();  //Shii ir loti svariiga rinda!!!!!!!!!
        delay(signalDelay); // small delay to prevent reading errors
      }
      if (buttonState == 125852) {
        Serial.println("up");
        pushedColor = 119;
        int fs = fullSegment();
        addSegment();  //Shii ir loti svariiga rinda!!!!!!!!!
        delay(signalDelay); // small delay to prevent reading errors
      }
      if (buttonState == 912284) {
        Serial.println("right");
        pushedColor = 100;
        int fs = fullSegment();
        addSegment();  //Shii ir loti svariiga rinda!!!!!!!!!
        delay(signalDelay); // small delay to prevent reading errors
      }
      if (buttonState == 387996) {
        Serial.println("left");
        pushedColor = 97;
        int fs = fullSegment();
        addSegment();  //Shii ir loti svariiga rinda!!!!!!!!!
        delay(signalDelay); // small delay to prevent reading errors
      }


      irrecv.resume();
    }
    addSegment();  //Shii ir loti svariiga rinda!!!!!!!!!
  }
}

int fullSegment() {
  int r;
  mvmt = pushedColor;
  //optional:
  /*
    Serial.println("\nEnter movement:");
  */
  //  mvmt = Serial.read();
  //optional:
  /*
    Serial.println("Movement: " + mvmt);
  */
  if (mvmt == 100) {//right = 100 (d) Yellow)
    {

      if (digit[1] == "↑   | \n") {
        r = refactor(0, " --> \n");
      } else if (digit[3] == "↑   | \n") {
        r = refactor(2, " --> \n");
      } else if (digit[3] == "↓   | \n") {
        r = refactor(4, " --> \n");
      } else if (digit[1] == "↓   | \n") {
        r = refactor(2, " --> \n");
      } else if (digit[0] == " --> \n") {
        r = refactor(0, " --> \n");
        Segment = Segment + 1;
      } else if (digit[2] == " --> \n") {
        r = refactor(2, " --> \n");
        Segment = Segment + 1;
      } else if (digit[4] == " --> \n") {
        r = refactor(4, " --> \n");
        Segment = Segment + 1;
      } else if (digit[1] == "|   ↓ \n") {
        r = refactor(2, " --> \n");
        Segment = Segment + 1;
      } else if (digit[3] == "|   ↓ \n") {
        r = refactor(4, " --> \n");
        Segment = Segment + 1;
      } else if (digit[1] == "|   ↑ \n") {
        r = refactor(0, " --> \n");
        Segment = Segment + 1;
      } else if (digit[3] == "|   ↑ \n") {
        r = refactor(2, " --> \n");
        Segment = Segment + 1;
      }

    }

  }
  if (mvmt == 97) { //left = 97 (a) //green
    if (digit[1] == "|   ↓ \n") {
      r = refactor(2, " <-- \n");
    } else if (digit[3] == "|   ↓ \n") {
      r = refactor(4, " <-- \n");
    } else if (digit[3] == "|   ↑ \n") {
      r = refactor(2, " <-- \n");
    } else if (digit[1] == "|   ↑ \n") {
      r = refactor(0, " <-- \n");
    } else if (digit[0] == " <-- \n") {
      if (Segment == 0) {
        Serial.println("tu ej arpus zonas!");
        Segment = 0;
      } else if (Segment > 0) {
        r = refactor(0, " <-- \n");
        Segment = Segment - 1;
      }
    } else if (digit[2] == " <-- \n") {
      if (Segment == 0) {
        Serial.println("tu ej arpus zonas!");
        Segment = 0;
      } else if (Segment > 0) {
        r = refactor(2, " <-- \n");
        //      r = refactor(2, " <-- \n");
        //      Segment = Segment - 1;;
        Segment = Segment - 1;
      }

      //
    } else if (digit[4] == " <-- \n") {
      //      ghjhj
      if (Segment == 0) {
        Serial.println("tu ej arpus zonas!");
        Segment = 0;
      } else if (Segment > 0) {
        //        r = refactor(2, " <-- \n");
        //      r = refactor(2, " <-- \n");
        //      Segment = Segment - 1;;
        r = refactor(4, " <-- \n");
        Segment = Segment - 1;
      }



      //      r = refactor(4, " <-- \n");
      //      Segment = Segment - 1;
    } else if (digit[1] == "↓   | \n") {
      if (Segment == 0) { //ja ir nultais segments, tad ir iznjeemums, ka neko nedara
        Serial.println("Tu ej aarpus zonas!");
        Segment = 0;
      } else if (Segment > 0) {
        r = refactor(2, " <-- \n");
        Segment = Segment - 1;
      }
    } else if (digit[3] == "↓   | \n") {
      if (Segment == 0) { //ja ir nultais segments, tad ir iznjeemums, ka neko nedara
        Serial.println("Tu ej aarpus zonas!");
        Segment = 0;
      } else if (Segment > 0) {
        r = refactor(4, " <-- \n");
        Segment = Segment - 1;
      }
      //      }
      //      r = refactor(4, " <-- \n");
      //      Segment = Segment - 1;
    } else if (digit[1] == "↑   | \n") {
      //asdasd

      if (Segment == 0) { //ja ir nultais segments, tad ir iznjeemums, ka neko nedara
        Serial.println("Tu ej aarpus zonas!");
        Segment = 0;
      } else if (Segment > 0) {
        //      r = refactor(4, " <-- \n");
        //      Segment = Segment - 1;
        r = refactor(0, " <-- \n");
        Segment = Segment - 1;
      }



    } else if (digit[3] == "↑   | \n") {

      //asd

      if (Segment == 0) { //ja ir nultais segments, tad ir iznjeemums, ka neko nedara
        Serial.println("Tu ej aarpus zonas!");
        Segment = 0;
      } else if (Segment > 0) {
        //      r = refactor(4, " <-- \n");
        //      Segment = Segment - 1;
        r = refactor(2, " <-- \n");
        Segment = Segment - 1;
      }





      //      r = refactor(2, " <-- \n");
      //      Segment = Segment - 1;

    }
  }
  if (mvmt == 115) { ////down = 115 (s) //red
    if (digit[0] == " --> \n") {
      r = refactor(1, "|   ↓ \n");
    } else if (digit[1] == "|   ↓ \n") {
      r = refactor(3, "|   ↓ \n");
    } else if (digit[2] == " --> \n") {
      r = refactor(3, "|   ↓ \n");
    } else if (digit[2] == " <-- \n") {
      r = refactor(3, "↓   | \n");
    } else if (digit[0] == " <-- \n") {
      r = refactor(1, "↓   | \n");
    } else if (digit[1] == "↓   | \n") {
      r = refactor(3, "↓   | \n");
    }
  }
  if (mvmt == 119) { //up = 119 //orange
    //    verticalMovement = verticalMovement + 1;
    //    if (verticalMovement == 2) {
    //      verticalMovement = 0;
    //    }
    if (digit[2] == " <-- \n") {
      r = refactor(1, "↑   | \n");
    } else if (digit[4] == " <-- \n") {
      /*
        //      dfg
        // shobriid neiet apaksejais horizontalais segments uz augshu - kaut kas to blokjee!
        if (Segment == 0) { //ja ir nultais segments, tad ir iznjeemums, ka neko nedara
        Serial.println("Tu ej aarpus zonas!");
        Segment = 0;
        } else if (Segment > 0) {
        //      r = refactor(4, " <-- \n");
        //      Segment = Segment - 1;
        r = refactor(2, " <-- \n");
        Segment = Segment - 1;
        }

      */

      r = refactor(3, "↑   | \n"); //originaalais kods!
    } else if (digit[3] == "↑   | \n") {
      r = refactor(1, "↑   | \n");
    } else if (digit[4] == " --> \n") {
      r = refactor(3, "|   ↑ \n");
    } else if (digit[3] == "|   ↑ \n") {
      r = refactor(1, "|   ↑ \n");
    } else if (digit[2] == " --> \n") {
      r = refactor(1, "|   ↑ \n");
    }
  }

  //This is for testing, optional:
  /*
    for (int q = 0; q < 5; q++) {
      Serial.println(digit[q]);
    }
  */
  //  if (mvmt == "") {
  //    Serial.println("now must move to previous made direction(saved variable)");
  //  }

  if (Segment == 4) {
    Segment = 3;
  }
  if (Segment < 0) {
    Segment = 0;

  }


  //  addSegment();
  //  delay(0);
}

void addSegment() {
  //  int upperHalf = 0;
  //  segm[] = {SEG_A, SEG_B, SEG_G, SEG_F};
  //  upperSegment:
  //  if (verticalMovement == 0) { //upper segment part:
  if (digit[0] == " --> \n" || digit[0] == " <-- \n") {
    //    uint8_t SEG_LINE_A[] = {SEG_A, 0x0, 0x0, 0x0};
    uint8_t SEG_LINE_A[][4] = {{(unsigned char) (SEG_A | dot[0]), dot[1], dot[2], dot[3]}, {dot[0], (unsigned char) (SEG_A | dot[1]), dot[2], dot[3]}, {dot[0], dot[1], (unsigned char) (SEG_A | dot[2]), dot[3]}, {dot[0], dot[1], dot[2], (unsigned char) (SEG_A | dot[3])}};
    //        uint8_t SEG_LINE_A[][4] = {{SEG_A | foodF | foodB | foodG | foodE | foodC | foodD | dot[1], 0x0, 0x0, 0x0}, {0x0, SEG_A, 0x0, 0x0}, {0x0, 0x0, SEG_A, 0x0}, {0x0, 0x0, 0x0, SEG_A}};
    display.setSegments(SEG_LINE_A[Segment]);
  } else if (digit[1] == "↑   | \n" || digit[1] == "↓   | \n") {
    //    uint8_t SEG_LINE_F[] = {SEG_F, 0x0, 0x0, 0x0};
    uint8_t SEG_LINE_F[][4] = {{(unsigned char) (SEG_F | dot[0]), dot[1], dot[2], dot[3]}, {dot[0], (unsigned char)(SEG_F | dot[1]), dot[2], dot[3]}, {dot[0], dot[1], (unsigned char) (SEG_F | dot[2]), dot[3]}, {dot[0], dot[1], dot[2], (unsigned char)(SEG_F | dot[3])}};
    display.setSegments(SEG_LINE_F[Segment]);
  } else if (digit[1] == "|   ↑ \n" || digit[1] == "|   ↓ \n") {
    //    uint8_t SEG_LINE_B[] = {SEG_B, 0x0, 0x0, 0x0};
    uint8_t SEG_LINE_B[][4] = {{(unsigned char)(SEG_B | dot[0]), dot[1], dot[2], dot[3]}, {dot[0], (unsigned char)(SEG_B | dot[1]), dot[2], dot[3]}, {dot[0], dot[1], (unsigned char)(SEG_B | dot[2]), dot[3]}, {dot[0], dot[1], dot[2], (unsigned char) (SEG_B | dot[3])}};
    display.setSegments(SEG_LINE_B[Segment]);
  } else if (digit[2] == " --> \n" || digit[2] == " <-- \n") {
    //    uint8_t SEG_LINE_G[] = {SEG_G, 0x0, 0x0, 0x0};
    uint8_t SEG_LINE_G[][4] = {{(unsigned char)(SEG_G | dot[0]), dot[1], dot[2], dot[3]}, {dot[0], (unsigned char) (SEG_G | dot[1]), dot[2], dot[3]}, {dot[0], dot[1], (unsigned char) (SEG_G | dot[2]), dot[3]}, {dot[0], dot[1], dot[2], (unsigned char) (SEG_G | dot[3])}};
    display.setSegments(SEG_LINE_G[Segment]);
  } else if (digit[3] == "↑   | \n" || digit[3] == "↓   | \n") {
    //    uint8_t SEG_LINE_E[] = {SEG_E, 0x0, 0x0, 0x0};
    uint8_t SEG_LINE_E[][4] = {{(unsigned char)(SEG_E | dot[0]), dot[1], dot[2], dot[3]}, {dot[0], (unsigned char) (SEG_E | dot[1]), dot[2], dot[3]}, {dot[0], dot[1], (unsigned char) (SEG_E | dot[2]), dot[3]}, {dot[0], dot[1], dot[2], (unsigned char) (SEG_E | dot[3])}};
    display.setSegments(SEG_LINE_E[Segment]);
  } else if (digit[3] == "|   ↑ \n" || digit[3] == "|   ↓ \n") {
    //    uint8_t SEG_LINE_C[] = {SEG_C, 0x0, 0x0, 0x0};
    uint8_t SEG_LINE_C[][4] = {{(unsigned char) (SEG_C | dot[0]), dot[1], dot[2], dot[3]}, {dot[0], (unsigned char) (SEG_C | dot[1]), dot[2], dot[3]}, {dot[0], dot[1], (unsigned char)(SEG_C | dot[2]), dot[3]}, {dot[0], dot[1], dot[2], (unsigned char) (SEG_C | dot[3])}};
    display.setSegments(SEG_LINE_C[Segment]);
  } else if (digit[4] == " --> \n" || digit[4] == " <-- \n") {
    //    uint8_t SEG_LINE_D[] = {SEG_D, 0x0, 0x0, 0x0};
    uint8_t SEG_LINE_D[][4] = {{(unsigned char)(SEG_D | dot[0]), dot[1], dot[2], dot[3]}, {dot[0], (unsigned char)(SEG_D | dot[1]), dot[2], dot[3]}, {dot[0], dot[1], (unsigned char)(SEG_D | dot[2]), dot[3]}, {dot[0], dot[1], dot[2], (unsigned char)(SEG_D | dot[3])}};
    if (Segment == randomDotSegment && dot[Segment] == (unsigned char) 128) {
      dot[Segment] = (unsigned char) 0;
      foodCatched();

    }

    display.setSegments(SEG_LINE_D[Segment]);

  }
  if (digit[3] == "|   ↓ \n" && Segment == randomDotSegment && dot[Segment] == 128) {
    uint8_t SEG_LINE_D[][4] = {{(unsigned char)(SEG_D | dot[0]), dot[1], dot[2], dot[3]}, {dot[0], (unsigned char)(SEG_D | dot[1]), dot[2], dot[3]}, {dot[0], dot[1], (unsigned char)(SEG_D | dot[2]), dot[3]}, {dot[0], dot[1], dot[2], (unsigned char)(SEG_D | dot[3])}};
    Serial.println("from digit[3]!!!");
    dot[Segment] = 0;
    foodCatched();
//    display.setSegments(SEG_LINE_D[Segment]); //shii rinda trauceeja iet uz leju panjemot punktu
  }
  //Serial.println(verticalMovement);
}

void foodCatched() {
  points--;
  //  int dot[] = {128, 0, 0, 0};
  //  int dot[] = {0, 0, 0, 0};
  randomDotSegment = random(3);
  do {
    randomDotSegment = random(3);
  } while (randomDotSegment == Segment);


  Serial.println("next dotSegment will be: ");
  Serial.println(randomDotSegment);
  dot[randomDotSegment] = 128; //new dot!
  Serial.println("dot ate!");
  Serial.println("Points:");
  if (points > 0) {
    display.showNumberDec(points, false);
  }
  if (points <= 0) {
    const uint8_t SEG_END[] = {SEG_A | SEG_D | SEG_E | SEG_F | SEG_G, //E
                               SEG_C | SEG_E | SEG_G,                 //n
                               SEG_B | SEG_C | SEG_D | SEG_E | SEG_G | 128, //d .
                               0
                              }; //.
    //    while (1) {//
    display.setBrightness(8);
    display.setSegments(SEG_END);
    delay(1000);
    points = 10;
    display.setBrightness(15);
    //    }
  }
  Serial.print(points);
  delay(1000);
}
//  display.setSegments(SEG_LINE_0);



int refactor(int i, String arrow) {
  String originalDigits[] = {" --- \n", "|   | \n", " --- \n", "|   | \n", " --- \n"};
  //   Serial.println("int i = " + i);
  //   Serial.println("String arrow = " + arrow);
  for (int a = 0; a < 5; a++) {
    if (i == a) {
      digit[i] = arrow;
      Serial.println("i == a");
      //      Serial.println("i == a");
    }
    else if (i != a) {
      digit[a] = originalDigits[a];
      Serial.println("i != a");
    }
  }
}
