#include <Muca.h>

Muca muca;


String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete



void setup() {
  Serial.begin(115200);
  muca.init(); // useInterrupt ne fonctionne pas bien
  // muca.useRaw = true;
  // muca.setGain(100);
  //  muca.autocal();
  muca.printInfo();
  muca.testconfig();
  muca.printInfo();

  inputString.reserve(200);

}


void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}



void loop() {




  // print the string when a newline arrives:
  if (stringComplete) {
    //  Serial.println(inputString);

    int *RevertInt = getDelimeters(inputString, ":");

    muca.setConfig(byte(RevertInt[0]), byte(RevertInt[1]), byte(RevertInt[2]), byte(RevertInt[3]));

    Serial.println("Received");
    muca.printInfo();

    // clear the string:
    inputString = "";
    stringComplete = false;

  }

  GetTouch();
  delay(5);
}





int *getDelimeters(String DelString, String Delby) {
  static int Delimeters[4];//important!!! now much delimeters;
  int i = 0;
  while (DelString.indexOf(Delby) >= 0) {
    int delim = DelString.indexOf(Delby);
    Delimeters[i] = (DelString.substring(0, delim)).toInt();
    DelString = DelString.substring(delim + 1, DelString.length());
    i++;
    if (DelString.indexOf(Delby) == -1) {
      Delimeters[i] = DelString.toInt();
    }
  }
  return Delimeters;
}



void GetTouch() {
  if (muca.updated()) {
    // Serial.print("NumTouches:"); Serial.println(muca.getNumberOfTouches());

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
    if ( muca.getNumberOfTouches() != 0) Serial.println("");
  }
}


int frameCount = 0;
float fps = 0.0F;
float t = 0.0F;
float prevtt = 0.0F;

void GetFPS()
{
  frameCount++;
  t += millis() - prevtt;
  if (t > 1000.0f)
  {
    fps = frameCount;
    frameCount = 0;
    t = 0;
  }
  prevtt = millis();
  Serial.println(fps);
}
