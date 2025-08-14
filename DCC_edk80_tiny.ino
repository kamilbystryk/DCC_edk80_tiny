//47, 48 mapowanie f13
//49, 50 mapowanie f14
//51, 52 mapowanie f15
//53  - konfiguracja:
//[0] działanie F0,
//[1] aktywacja kontaktronu [1],
//[2] mapowanie f0
//[3] domyslny schemat
//[4]16 zapis końcówek z kontaktronu
//[5]32 kontkatron obsluguje oswietlenie
//future tryb kontaktronu [f13-15]
//54 - PWM value
//58, 59 mapowanie f0
//60, 61 default state
//62 - techniczne, brak dcc (dla == 0)
/*
   todo:
   reset fabryczny zworka
*/


/*
  f0 - oswietlenie
  f1-10 - przedzialy
  f11,f12 - koncowki
  f13-f15 - oswietlenie mapowane
  f16 - deaktywacja kontaktronu
*/

#include <Bounce.h>
#include <NmraDcc.h>
#include <EEPROM.h>

#define WithEnds true

bool reedLight = false;
bool savekontaktron;
bool savekontaktron2 = false;
int mod = 3;

bool DCCactive = false;
bool DCactive = false;
bool DCCdefaultSchema = false;
long lastbutton = 0;
const byte            Version     =                1; // Major version in CV 7

bool bylrst = false;

void(* resetFunc) (void) = 0;

boolean cv53_0 = false; //tryb F1
boolean cv53_1 = false; //aktywacja kontaktronu

int PWMtemp = 255;
int PWMvalue = 0;
int PWMvalue2 = 0;
int PWMvalue3 = 0;
byte locoH;
byte locoL;
byte locoS;
byte f13cv47L = 255;
byte f13cv48H = 255;
byte f14cv49L = 255;
byte f14cv50H = 255;
byte f15cv51L = 255;
byte f15cv52H = 255;
byte f0cv60L = 1;
byte cv120 = 1;
byte cv121 = 2;
byte cv122 = 4;
byte cv123 = 0;
byte cv124 = 0;
byte cv125 = 0;
byte cv126 = 0;
byte cv127 = 0;
byte cv128 = 0;
byte cv129 = 0;
byte cv130 = 0;
byte cv131 = 0;
byte cv132 = 0;
byte cv133 = 0;
byte cv140 = 0;
byte cv141 = 0;
byte cv142 = 0;
byte number53 = 0;

bool kon1 = false;
bool kon2 = false;
bool kon3 = false;
bool konend = false;

boolean out1 = false;

boolean f0 = false;
boolean f1 = false;
boolean f2 = false;
boolean f3 = false;
boolean f4 = false;
boolean f5 = false;
boolean f6 = false;
boolean f7 = false;
boolean f8 = false;
boolean f9 = false;
boolean f10 = false;
boolean f11 = false;
boolean f12 = false;
boolean f13 = false;
//produkcja:


int ACT = 0;
int NACT = 1;
#define DCC_PIN     2 //HW=4
boolean test = true;
#define DccAckPin 3 //HW=5
#define FunctionPin0 6//czesc wspolna //HW=8
#define FunctionPin11 7 //koncowka 1 //HW=9
#define FunctionPin12 8 //koncowka 2 //HW=11
#define FunctionPin20 0
#define FunctionPin21 1


const int Input1 = 10; //HW=13

Bounce bouncer = Bounce( Input1, 50 );

int myAddress;
int myAddressLong = 444;
bool isAddressLong = false;

struct CVPair
{
  uint16_t  CV;
  uint8_t   Value;
};

CVPair FactoryDefaultCVs [] =
{
  // The CV Below defines the Short DCC Address
  {CV_MULTIFUNCTION_PRIMARY_ADDRESS, DEFAULT_MULTIFUNCTION_DECODER_ADDRESS},

  // These two CVs define the Long DCC Address
  {CV_MULTIFUNCTION_EXTENDED_ADDRESS_MSB, CALC_MULTIFUNCTION_EXTENDED_ADDRESS_MSB(DEFAULT_MULTIFUNCTION_DECODER_ADDRESS)},
  {CV_MULTIFUNCTION_EXTENDED_ADDRESS_LSB, CALC_MULTIFUNCTION_EXTENDED_ADDRESS_LSB(DEFAULT_MULTIFUNCTION_DECODER_ADDRESS)},

  // ONLY uncomment 1 CV_29_CONFIG line below as approprate
  //  {CV_29_CONFIG,                                      0}, // Short Address 14 Speed Steps
  {CV_29_CONFIG,                       CV29_F0_LOCATION}, // Short Address 28/128 Speed Steps
  //{CV_29_CONFIG, CV29_EXT_ADDRESSING | CV29_F0_LOCATION}, // Long  Address 28/128 Speed Steps

  {7, Version},
  {47, 85},
  {48, 55},
  {49, 170},
  {50, 170},
  {51, 51},
  {52, 51},
  {53, 0},
  {54, 255},
  {120, 1},
  {121, 2},
  {122, 4},
  {123, 0},
  {124, 0},
  {125, 0},
  {126, 0},
  {127, 0},
  {128, 0},
  {129, 0},
  {130, 0},
  {131, 2},
  {132, 4},
  {133, 0},
  {134, 0},
  {135, 0},
  {136, 0},
  {137, 0},
  {138, 0},
  {139, 0},
  {140, 0},
  {141, 0},
  {142, 0},
  {233, 255},
  {234, 255},
};

NmraDcc  Dcc ;

uint8_t FactoryDefaultCVIndex = 0;


// Uncomment this line below to force resetting the CVs back to Factory Defaults
//uint8_t FactoryDefaultCVIndex = sizeof(FactoryDefaultCVs)/sizeof(CVPair);

void notifyCVResetFactoryDefault()
{
  // Make FactoryDefaultCVIndex non-zero and equal to num CV's to be reset
  // to flag to the loop() function that a reset to Factory Defaults needs to be done
  FactoryDefaultCVIndex = sizeof(FactoryDefaultCVs) / sizeof(CVPair);
};

// Uncomment the #define below to print all Speed Packets
//#define NOTIFY_DCC_SPEED
#ifdef  NOTIFY_DCC_SPEED
void notifyDccSpeed( uint16_t Addr, DCC_ADDR_TYPE AddrType, uint8_t Speed, DCC_DIRECTION Dir, DCC_SPEED_STEPS SpeedSteps )
{
  if (test)
  {
    /*Serial.print("notifyDccSpeed: Addr: ");
      Serial.print(Addr, DEC);
      Serial.print( (AddrType == DCC_ADDR_SHORT) ? "-S" : "-L" );
      Serial.print(" Speed: ");
      Serial.print(Speed, DEC);
      Serial.print(" Steps: ");
      Serial.print(SpeedSteps, DEC);
      Serial.print(" Dir: ");
      Serial.println( (Dir == DCC_DIR_FWD) ? "Forward" : "Reverse" );*/
  }
};
#endif




// Uncomment the #define below to print all DCC Packets
//#define NOTIFY_DCC_MSG


void setup()
{
  pinMode(FunctionPin0, OUTPUT);

  pinMode(FunctionPin11, OUTPUT);
  pinMode(FunctionPin12, OUTPUT);
  pinMode(Input1, INPUT_PULLUP);
  pinMode(FunctionPin20, OUTPUT);
  pinMode(FunctionPin21, OUTPUT);

  analogWrite(FunctionPin0, 0);

  digitalWrite(FunctionPin11, ACT);
  digitalWrite(FunctionPin12, ACT);
  digitalWrite(FunctionPin20, NACT);
  digitalWrite(FunctionPin21, NACT);
  if (test)
  {
    Serial.begin(9600);
  }
  uint8_t maxWaitLoops = 255;
  while (!Serial && maxWaitLoops--)
    delay(20);
  if (test)
  {
    Serial.println("Bee 120a");
  }

  // Configure the DCC CV Programing ACK pin for an output
  pinMode( DccAckPin, OUTPUT );
  digitalWrite( DccAckPin, LOW );

  // Setup which External Interrupt, the Pin it's associated with that we're using and enable the Pull-Up
  // Many Arduino Cores now support the digitalPinToInterrupt() function that makes it easier to figure out the
  // Interrupt Number for the Arduino Pin number, which reduces confusion.
  pinMode(DCC_PIN, DCC_PIN);


#ifdef digitalPinToInterrupt
  Dcc.pin(DCC_PIN, 0);
#else
  Dcc.pin(0, DCC_PIN, 1);
#endif

  // Call the main DCC Init function to enable the DCC Receiver
  //Dcc.init( MAN_ID_DIY, 10, CV29_ACCESSORY_DECODER | CV29_OUTPUT_ADDRESS_MODE, 0 );

  Dcc.init( MAN_ID_DIY, 10, FLAGS_MY_ADDRESS_ONLY, 0 );
  //Dcc.setCV(29, 2);
  CVinit();

  // Uncomment to force CV Reset to Factory Defaults
  //notifyCVResetFactoryDefault();
  if (!digitalRead(Input1))
  {
    delay(1000);
    if (!digitalRead(Input1))
    {
      delay(1000);
      if (!digitalRead(Input1))
      {
        //if (test)
        {
          Serial.println("FACTORY RESET");
          DriveAll(true, true);
          delay(100);
          DriveAll(false, true);
          delay(100);
          DriveAll(true, true);
          delay(100);
          DriveAll(false, true);
          delay(100);
          DriveAll(true, true);
          delay(100);
          DriveAll(false, true);
          delay(100);
        }
        FactoryDefaultCVIndex = sizeof(FactoryDefaultCVs) / sizeof(CVPair);
      }
    }
  }
  //schemat domyślny
  number53 = Dcc.getCV(53);
  if (bitRead(number53, 3) == 1)
  {
    f0cv60L = Dcc.getCV(60);
    DCCdefaultSchema = true;
    if (bitRead(f0cv60L, 0)) analogWrite(FunctionPin0, PWMvalue);
    if (bitRead(f0cv60L, 1)) digitalWrite( FunctionPin11, NACT );
    if (bitRead(f0cv60L, 2)) digitalWrite( FunctionPin12, NACT );
  }
  if (bitRead(number53, 4) == 1)
  {
    savekontaktron = true;
    Serial.println("init kontaktron");
    mod = Dcc.getCV(63);
    Serial.println(Dcc.getCV(63));
    if (WithEnds)
    {
      if (Dcc.getCV(63) == 1)
      {
        digitalWrite(FunctionPin11, ACT);
        digitalWrite(FunctionPin12, ACT);
        analogWrite( FunctionPin0, 0);
      }
      if (Dcc.getCV(63) == 2)
      {
        digitalWrite(FunctionPin11, NACT);
        digitalWrite(FunctionPin12, ACT);
        analogWrite( FunctionPin0, 0);
      }
      if (Dcc.getCV(63) == 3)
      {
        digitalWrite(FunctionPin11, ACT);
        digitalWrite(FunctionPin12, NACT);
        analogWrite( FunctionPin0, 0);
      }
      if (Dcc.getCV(63) == 4)
      {
        digitalWrite(FunctionPin11, ACT);
        digitalWrite(FunctionPin12, ACT);
        analogWrite( FunctionPin0, PWMvalue);
      }
      if (Dcc.getCV(63) == 5)
      {
        digitalWrite(FunctionPin11, NACT);
        digitalWrite(FunctionPin12, ACT);
        analogWrite( FunctionPin0, PWMvalue);
      }
      if (Dcc.getCV(63) == 6)
      {
        digitalWrite(FunctionPin11, ACT);
        digitalWrite(FunctionPin12, NACT);
        analogWrite( FunctionPin0, PWMvalue);
      }
    }
    else
    {
      if (mod == 1 || mod == 3 || mod == 5)
      {
        if (reedLight)
        {
          analogWrite( FunctionPin0, PWMvalue);
          f0 = true;
        }
      }
      if (mod == 2 || mod == 4 || mod == 6)
      {
        if (reedLight)
        {
          analogWrite( FunctionPin0, 0);
          f0 = false;
        }
      }
    }
  }
  else
  {
    savekontaktron = false;
  }
  if ( Dcc.getCV(62) == 0)
  {
    DCactive = true;
    DriveAll(true, false);
  }
  cv120 = Dcc.getCV(120);
  cv121 = Dcc.getCV(121);
  cv122 = Dcc.getCV(122);
  cv123 = Dcc.getCV(123);
  cv124 = Dcc.getCV(124);
  cv140 = Dcc.getCV(140);
  cv141 = Dcc.getCV(141);
  cv142 = Dcc.getCV(142);

  locoH = Dcc.getCV(230);
  locoL = Dcc.getCV(231);
  locoS = Dcc.getCV(232);
  Serial.print("H=");
  Serial.println(locoH);
  Serial.print("L=");
  Serial.println(locoL);
}

int b1;
int kk = 0;

void loop()
{
  if (savekontaktron2)
  {
    /*if (!kon2 && !kon3)
      {
      kk = 0;
      }
      else*/
    {
      kk++;
      delay(1);
      if (kk == 10000)
      {
        Serial.println("save kontaktron:");
        Serial.println(mod);
        savekontaktron2 = false;
        if (Dcc.getCV(63) != mod) Dcc.setCV(63, mod);
        /*if (!kon2 && !kon3)
          {
          if (!f0)
          {
            if (Dcc.getCV(63) != 0) Dcc.setCV(63, 0);
            Serial.println("0");
          }
          else
          {
            if (Dcc.getCV(63) != 3) Dcc.setCV(63, 3);
            Serial.println("3");
          }
          }
          if (kon2 && !kon3)
          {
          if (!f0)
          {
          if (Dcc.getCV(63) != 1) Dcc.setCV(63, 1);
          Serial.println("1");
          }
          else
          {
            if (Dcc.getCV(63) != 4) Dcc.setCV(63, 4);
            Serial.println("4");
          }
          }
          else if (kon3)
          {
          if (!f0)
          {
          if (Dcc.getCV(63) != 2) Dcc.setCV(63, 2);
          Serial.println("2");
          }
          else
          {
            if (Dcc.getCV(63) != 5) Dcc.setCV(63, 5);
            Serial.println("5");
          }
          }*/
      }
    }
  }
  if (PWMvalue != PWMtemp)
  {
    if (test)
    {
      Serial.print("PWM = ");
      Serial.println(PWMvalue);
    }
    if (f0)
    {
      analogWrite(FunctionPin0, PWMvalue);
    }
    PWMtemp = PWMvalue;
  }
  if (cv53_1)
  {
    bouncer.update();
    b1 = bouncer.read();
  }
  if (bouncer.fallingEdge()  && cv53_1)
  {
    delay(50);
    if (!digitalRead(Input1) && ((millis() - lastbutton) > 1000))
    {
      lastbutton = millis();
      ProceedReedSwitch();
    }
  }

  // You MUST call the NmraDcc.process() method frequently from the Arduino loop() function for correct library operation
  Dcc.process();

  if ( FactoryDefaultCVIndex && Dcc.isSetCVReady())
  {
    FactoryDefaultCVIndex--; // Decrement first as initially it is the size of the array
    Dcc.setCV( FactoryDefaultCVs[FactoryDefaultCVIndex].CV, FactoryDefaultCVs[FactoryDefaultCVIndex].Value);
    bylrst = true;
  }
  /*if (FactoryDefaultCVIndex == 0 && bylrst == true)
    {
    bylrst == false;
    if (test)
    {
      Serial.println("RESET AFTER FACTORY SET");
    }
    resetFunc();
    }*/
}
