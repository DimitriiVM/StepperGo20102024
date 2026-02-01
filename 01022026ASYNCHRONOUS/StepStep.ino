/* 
#AUTHOR Дмитрий Витальевич Мельник
 #MAIL i.x.c.o.n@yandex.ru 
#LICENSE Использование  только  для не коммерческих проектов, без упоминания автора. При использовании автор не несет ни какой отвественности, и не дает ни какой гарантии работы. Не ведет поддержку или исправление ошибок.  
*/
#define VERSION "0.02"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "GyverStepper2.h"

#define PINONESTEPDIR 34
#define PINTWOSTEPDIR 26
#define PINTREESTEPDIR 5
#define STEPONE 35
#define STEPTWO 33
#define STEPTREE 6
#define StepTurn 200

GStepper2< STEPPER2WIRE> stepperX(StepTurn, STEPONE, PINONESTEPDIR);
GStepper2< STEPPER2WIRE> stepperY(StepTurn, STEPTWO, PINTWOSTEPDIR);
GStepper2< STEPPER2WIRE> stepperZ(StepTurn, STEPTREE, PINTREESTEPDIR);

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

uint32_t timer = 0;
uint32_t timer1 = 0;

char MSG[64];
int ZiseMSG = 0;

void setup() 
{
  Serial.begin(115200);
  Serial.setTimeout(20);
  Serial.println("ESP32 START OK!");
  stepperX.setMaxSpeed(400);
  stepperY.setMaxSpeed(400);
  stepperZ.setMaxSpeed(400);

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
  stepperX.tick();
  stepperY.tick();
  stepperZ.tick();

  if(millis() - timer >= 100)
  {
    timer = millis();
    SetDisplay();
  }

  if(millis() - timer1 >= 50)
  {
    timer1 = millis();
    if(Serial.available() > 0)
    {
      ZiseMSG = Serial.readBytes(MSG, 64);
      SerialRead();
      ClearMSG();
    }

  }
  
}

void SetDisplay()
{
  display.clearDisplay();
  display.setCursor(36, 8);
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.println(stepperX.pos);
  display.setCursor(36, 24);
  display.println(stepperY.pos);
  display.setCursor(36, 40);
  display.println(stepperZ.pos);
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
  if(NumberMotor == 0)
  {
    stepperX.setTarget(stepperX.pos + Step);//Задаем позицию куда двигаться
  }
  else if(NumberMotor == 1)
  {
    stepperY.setTarget(stepperY.pos + Step);//Задаем позицию куда двигаться
  }
  else if(NumberMotor == 2)
  {
    stepperZ.setTarget(stepperZ.pos + Step);//Задаем позицию куда двигаться
  }
}
