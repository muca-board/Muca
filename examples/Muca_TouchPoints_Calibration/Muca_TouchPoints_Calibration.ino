#include <Muca.h>

Muca muca;

void setup() {
  Serial.begin(115200);
  muca.init(); // useInterrupt ne fonctionne pas bien
}


void loop() {
  GetTouch();
  delay(5);
}


void GetTouch() {
  if (muca.updated()) {
    Serial.print("NTouch:"); Serial.print(muca.getNumberOfTouches());
    Serial.print("\t");
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
    else {
      Settings();
    }
  }
}

void Gain() {
  Serial.print("Received:"); Serial.println(incomingMsg);
  char *str;
  char *p = incomingMsg;
  byte i = 0;
  while ((str = strtok_r(p, ":", &p)) != NULL)  // Don't use \n here it fails
  {
    if(i==1 )  {
       muca.setGain(atoi(str), true);
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
