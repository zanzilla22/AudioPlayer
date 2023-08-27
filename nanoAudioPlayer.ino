#include <LiquidCrystal.h>

#define scrollInput A0
#define playPin 2

#define speakerPin 11

//Liquid Crystal Pins
#define RS 8
#define E 7

#define DB4 6
#define DB5 5
#define DB6 4
#define DB7 3

LiquidCrystal lcd(RS, E, DB4, DB5, DB6, DB7);

float maxPot = 921;
String audiofiles[] = {"one", "deux", "tres", "arbaa", "cinqaro"};
int numberOfFiles = sizeof(audiofiles)/ sizeof(audiofiles[0]);

int selectedFile = 0;
int previousFile = 0;

bool playingAudio = false;

bool previousButtonState = LOW;
bool currentButtonState = LOW;
void setup()
{
  lcd.begin(16, 2);
  Serial.begin(9600);
  Serial.println(numberOfFiles);
}

void loop()
{
  
  if(playingAudio) {
    lcd.setCursor(0, 0);
    lcd.print("  Now Playing:  ");
  } else {
    lcd.setCursor(0, 0);
    lcd.print(" Selected Track ");
  }
  //analogWrite(speakerPin, 1);
  //Serial.println(analogRead(scrollInput));
  int potRatio = int(analogRead(scrollInput) / maxPot * 100);
  int interval = int(100 / numberOfFiles);
  
  previousButtonState = currentButtonState;
  currentButtonState = digitalRead(playPin);
  bool buttonClicked = previousButtonState != currentButtonState && currentButtonState == HIGH;

  
  previousFile = selectedFile;
  for (int i = 0; i < numberOfFiles; i++)
  {
    if (potRatio >= i * interval && potRatio < (i + 1) * interval)
    {
      selectedFile = i;
      break;
    }
  }
  
  if(buttonClicked && !playingAudio) {
    playingAudio = true;
    Serial.print("play audio: ");
    Serial.println(audiofiles[selectedFile]);
    //play the audio
  }
  else if((previousFile != selectedFile || buttonClicked) && playingAudio) {
    playingAudio = false;
    Serial.println("stopAudio");
    //stop the audio
  }
  if(previousFile != selectedFile) {
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor((8 - int(audiofiles[selectedFile].length()/2)), 1);
    lcd.print(audiofiles[selectedFile]);
  }
}
