/* 
#AUTHOR Дмитрий Витальевич Мельник
 #MAIL i.x.c.o.n@yandex.ru 
#LICENSE Использование  только  для не коммерческих проектов, без упоминания автора. При использовании автор не несет ни какой отвественности, и не дает ни какой гарантии работы. Не ведет поддержку или исправление ошибок.  
*/
#define PINPOT A0
#define PINDIR 2
#define PINSTEP 3

int countpot = 0;
int summstep = 0;

void setup() {
  Serial.begin(115200);
  pinMode(PINPOT, INPUT);
  pinMode(PINDIR, OUTPUT);
  pinMode(PINSTEP, OUTPUT);

  digitalWrite(PINDIR, HIGH);
  digitalWrite(PINSTEP, LOW);

  Serial.println("Start ok!");

}

void loop() {
  countpot = analogRead(PINPOT);
  countpot = map(countpot, 0, 1023, 0, 200);
  Serial.println(countpot);

  if(countpot > summstep)
  {    
    digitalWrite(PINDIR, HIGH);
    int stepnow = countpot - summstep;
    summstep = countpot;
    for(int i = 0; i < stepnow; i++)
    {
      digitalWrite(PINSTEP, HIGH);
      delay(1);
      digitalWrite(PINSTEP, LOW);
      delay(1);
    }
  }
  if(countpot < summstep)
  {    
    digitalWrite(PINDIR, LOW);
    int stepnow = summstep - countpot;
    summstep = countpot;
    for(int i = 0; i < stepnow; i++)
    {
      digitalWrite(PINSTEP, HIGH);
      delay(1);
      digitalWrite(PINSTEP, LOW);
      delay(1);
    }
  }
}
