/* 
#AUTHOR Дмитрий Витальевич Мельник
 #MAIL i.x.c.o.n@yandex.ru 
#LICENSE Использование  только  для не коммерческих проектов, без упоминания автора. При использовании автор не несет ни какой отвественности, и не дает ни какой гарантии работы. Не ведет поддержку или исправление ошибок.  
*/
#define VERSION "0.01"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define PINONESTEPDIR 34
#define PINTWOSTEPDIR 26
#define PINTREESTEPDIR 5
#define STEPONE 35
#define STEPTWO 33
#define STEPTREE 6

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int CountStepOne = 0;
int CountStepTwo = 0;
int CountStepTree = 0;

uint32_t timer = 0;
uint32_t timer1 = 0;
uint32_t timer2 = 0;

char MSG[64];
int ZiseMSG = 0;

void setup() 
{
  Serial.begin(115200);
  Serial.println("ESP32 START OK!");
  pinMode(PINONESTEPDIR, OUTPUT);
  pinMode(PINTWOSTEPDIR, OUTPUT);
  pinMode(PINTREESTEPDIR, OUTPUT);
  pinMode(STEPONE, OUTPUT);
  pinMode(STEPTWO, OUTPUT);
  pinMode(STEPTREE, OUTPUT);

  digitalWrite(PINONESTEPDIR, HIGH);
  digitalWrite(PINTWOSTEPDIR, HIGH);
  digitalWrite(PINTREESTEPDIR, HIGH);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) 
  {
    Serial.println(F("SSD1306 allocation failed"));
  }
  display.clearDisplay();
  display.setCursor(36, 12);
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.println("ESP32");
  display.setCursor(36, 32);
  display.println("START");
  display.display();
  delay(1000);
}

void loop() 
{

  if(millis() - timer >= 100)
  {
    timer = millis();
    SetDisplay();
  }

  if(millis() - timer1 >= 1000)
  {
    timer1 = millis();
  }

  if(millis() - timer2 >= 50)
  {
    timer2 = millis();
    if(Serial.available() > 0)
    {
      ZiseMSG = Serial.readBytes(MSG, 64);
      SerialRead();
      ClearMSG();
    }

  }
  
}

void StepOneDrive(int step)
{
  for(int i = 0; i < step; i++)
  {
    digitalWrite(STEPONE, HIGH);
    delay(1);
    digitalWrite(STEPONE, LOW);
    delay(1);
  }
  bool TempStepOneDrive = digitalRead(PINONESTEPDIR);
  if(TempStepOneDrive == false)
  {
    step = step * -1;
  }
  CountStepOne = CountStepOne + step;
}

void StepTwoDrive(int step)
{
  for(int i = 0; i < step; i++)
  {
    digitalWrite(STEPTWO, HIGH);
    delay(1);
    digitalWrite(STEPTWO, LOW);
    delay(1);
  }
  bool TempStepTwoDrive = digitalRead(PINTWOSTEPDIR);
  if(TempStepTwoDrive == false)
  {
    step = step * -1;
  }
  CountStepTwo = CountStepTwo + step;
}

void StepTreeDrive(int step)
{
  for(int i = 0; i < step; i++)
  {
    digitalWrite(STEPTREE, HIGH);
    delay(1);
    digitalWrite(STEPTREE, LOW);
    delay(1);
  }
  bool TempStepTreeDrive = digitalRead(PINTREESTEPDIR);
  if(TempStepTreeDrive == false)
  {
    step = step * -1;
  }
  CountStepTree = CountStepTree + step;
}

void SetDisplay()
{
  display.clearDisplay();
  display.setCursor(36, 8);
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.println(CountStepOne);
  display.setCursor(36, 24);
  display.println(CountStepTwo);
  display.setCursor(36, 40);
  display.println(CountStepTree);
  display.display();
}

void ClearMSG()
{
  for(int  i = 0; i < 64; i++)
  {
    MSG[i] = 0x00;
  }
}

void SerialRead()
{
  int NumberMotor = 0;
  int Step = 0;
  bool Direction = false;
  if(MSG[0] == 'X')
  {
    NumberMotor = 0;
  }
  else if(MSG[0] == 'Y')
  {
    NumberMotor = 1;
  }
  else if(MSG[0] == 'Z')
  {
    NumberMotor = 2;
  }
  else
  {
    NumberMotor = -1;
  }
  Step = atoi(MSG + 1);
  if(Step > 0)
  {
    Direction = true;
  }
  else
  {
    Direction = false;
    Step = Step * -1;
  }
  if(NumberMotor == 0)
  {
    if(Direction)
    {
      digitalWrite(PINONESTEPDIR, HIGH);
    }
    else
    {
      digitalWrite(PINONESTEPDIR, LOW);
    }
    StepOneDrive(Step);
  }
  else if(NumberMotor == 1)
  {
    if(Direction)
    {
      digitalWrite(PINTWOSTEPDIR, HIGH);
    }
    else
    {
      digitalWrite(PINTWOSTEPDIR, LOW);
    }
    StepTwoDrive(Step);
  }
  else if(NumberMotor == 2)
  {
    if(Direction)
    {
      digitalWrite(PINTREESTEPDIR, HIGH);
    }
    else
    {
      digitalWrite(PINTREESTEPDIR, LOW);
    }
    StepTreeDrive(Step);
  }
}
