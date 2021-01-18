#include <SD.h>
#include <SPI.h>
const int buttonPin = 7;
const int ledpin = 2;

int rawX;
int rawY;
int rawZ;

unsigned long startTime; // timing variables
unsigned long stopTime;
unsigned long diff;

int conv = 1;
String sta = String("test");
String en = String(".txt");
String buf = String();

// variables will change:
int buttonState = LOW;         // variable for reading the pushbutton status
int newState = LOW;
int hold = 0;
int CS_PIN = 4;
unsigned long tim = 0;
File file;
char strconv[16];

void setup() {
  Serial.begin(115200);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
  pinMode(ledpin, OUTPUT);
  initializeSD();
}

void loop() {
  int len;
  // read the state of the pushbutton value:
  newState = digitalRead(buttonPin);
  if (newState == HIGH & buttonState == LOW){
    if (hold == 0){
      startTime = millis();
      openFile("track.txt");
      conv = file.read()-48;
      closeFile;
      
      SD.remove("track.txt");
      tim = 0;
      hold = 1;
      buf = sta + conv + en;
      conv = conv+1;
      
      createFile("track.txt");
      itoa(conv, strconv, 10);
      writeToFile(strconv);
      closeFile();
      
      len = buf.length();
      char nom[len+1];
      strcpy(nom, buf.c_str());
      createFile(nom);
      digitalWrite(ledpin, HIGH);
      startTime = millis();
    }
    else if (hold == 1){
      stopTime = millis();
      diff = stopTime-startTime;
      hold = 0;
      digitalWrite(ledpin, LOW);
      file.println("number of points: ");
      file.println(tim);
      file.println("time elapsed: ");
      file.println(diff);
      closeFile();
    }
  }
  
  buttonState = newState;
  if (hold == 1) {
    rawX = analogRead(A0);
    rawY = analogRead(A1);
    rawZ = analogRead(A2);
    //float scaledX, scaledY, scaledZ; // Scaled values for each axis
    //scaledX = map(rawX, 0, 675, -300, 300);
    file.println(String(rawX)+"\n"+String(rawY)+"\n"+String(rawZ));
    tim = tim+1;
  }
}

void initializeSD()
{
  Serial.println("Initializing SD card...");
  pinMode(CS_PIN, OUTPUT);

  if (SD.begin())
  {
    Serial.println("SD card is ready to use.");
  } else
  {
    Serial.println("SD card initialization failed");
    return;
  }
}

int createFile(char filename[])
{
  file = SD.open(filename, FILE_WRITE);

  if (file)
  {
    Serial.println("File created successfully.");
    return 1;
  } else
  {
    Serial.println("Error while creating file.");
    return 0;
  }
}

int writeToFile(char text[])
{
  if (file)
  {
    file.println(text);
    Serial.println("Writing to file: ");
    Serial.println(text);
    return 1;
  } else
  {
    Serial.println("Couldn't write to file");
    return 0;
  }
}

void closeFile()
{
  if (file)
  {
    file.close();
    Serial.println("File closed");
  }
}

int openFile(char filename[])
{
  file = SD.open(filename);
  if (file)
  {
    Serial.println("File opened with success!");
    return 1;
  } else
  {
    Serial.println("Error opening file...");
    return 0;
  }
}

String readLine()
{
  String received = "";
  char ch;
  while (file.available())
  {
    ch = file.read();
    if (ch == '\n')
    {
      return String(received);
    }
    else
    {
      received += ch;
    }
  }
  return "";
}
