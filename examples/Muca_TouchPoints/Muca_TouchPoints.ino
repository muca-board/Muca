#include <Muca.h>

Muca muca;

void setup() {
  Serial.begin(115200);
  muca.init(true);
 // muca.skipLine(TX,(const short[]) {18,19,20,21}, 4);
  
  // height: 93mm  - 155
  // width:  70mm  - 90
  muca.setResolution(930, 700);
  // muca.setResolution(1550, 900);
 // muca.printAllRegisters();
 muca.setReportRate(6);

}


void loop() {
  GetTouchSimple();
  delay(5);
}


void GetTouchSimple() {
  if (muca.updated()) {
    for (int i = 0; i < muca.getNumberOfTouches(); i++) {
      if (i != 0)Serial.print("|");
      Serial.print(muca.getTouch(i).id); Serial.print(":");
      Serial.print(muca.getTouch(i).flag); Serial.print(":");
      Serial.print(muca.getTouch(i).x); Serial.print(":");
      Serial.print(muca.getTouch(i).y); Serial.print(":");
      Serial.print(muca.getTouch(i).weight);
    }
    Serial.println("|");
  }
}


void GetTouch() {
  if (muca.updated()) {
    Serial.print("NTouch:"); Serial.print(muca.getNumberOfTouches());Serial.print("\t");
    for (int i = 0; i < muca.getNumberOfTouches(); i++) {
      Serial.print("Touch ");
      Serial.print(i);
      Serial.print("\tx:");
      Serial.print(muca.getTouch(i).x);
      Serial.print("\ty:");
      Serial.print(muca.getTouch(i).y);
      Serial.print("\tid:");
      Serial.print(muca.getTouch(i).id);
      Serial.print("\tweight:");
      Serial.print(muca.getTouch(i).weight);
      Serial.print(" |\t");
    }
    Serial.println("");
  }
}


/*
   Serial Event
*/

char incomingMsg[20];

void serialEvent() {
  int charsRead;
  while (Serial.available() > 0) {
    charsRead = Serial.readBytesUntil('\n', incomingMsg, sizeof(incomingMsg) - 1);
    incomingMsg[charsRead] = '\0';  // Make it a string
    if (incomingMsg[0] == 'a')  {
      muca.autocal();
    }
    else if (incomingMsg[0] == 'i') {
      Serial.print("CURRENT\t");
      muca.printInfo();
    }
    else if (incomingMsg[0] == 'g') {
      Gain();
    }
    else if (incomingMsg[0] == 'z') {
      muca.printAllRegisters();
    }
    else if (incomingMsg[0] == 'r') {
      Resolution();
    }
    else {
      Settings();
    }
  }
}

void Resolution() {
  Serial.print("Received:"); Serial.println(incomingMsg);
  char *str;
  char *p = incomingMsg;
  byte i = 0;
  unsigned short x, y;
  while ((str = strtok_r(p, ":", &p)) != NULL)  // Don't use \n here it fails
  {
    if (i == 1 )  x = atoi(str);
    if (i == 2 )  y = atoi(str);
    i++;
  }
  muca.setResolution(x, y);
  incomingMsg[0] = '\0'; // Clear array
}


void Gain() {
  Serial.print("Received:"); Serial.println(incomingMsg);
  char *str;
  char *p = incomingMsg;
  byte i = 0;
  while ((str = strtok_r(p, ":", &p)) != NULL)  // Don't use \n here it fails
  {
    if (i == 1 )  {
      muca.setGain(atoi(str));
    }
    i++;
  }
  incomingMsg[0] = '\0'; // Clear array
}


void Settings() {
  Serial.print("Received:"); Serial.println(incomingMsg);
  char *str;
  char *p = incomingMsg;
  int settings[4];
  byte i = 0;
  while ((str = strtok_r(p, ":", &p)) != NULL)  // Don't use \n here it fails
  {
    settings[i] = atoi(str);
    i++;
  }
  incomingMsg[0] = '\0'; // Clear array
  muca.setConfig(settings[0], settings[1], settings[2], settings[3]);
}
