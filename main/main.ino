/**
________________________________________________________________________________________
________________________________________________________________________________________

                                      LIBRARY CODE
________________________________________________________________________________________
________________________________________________________________________________________
**/

#include <Servo.h>
#include <Wire.h>
#include <Adafruit_MCP23017.h>
#include <Adafruit_RGBLCDShield.h>

#define WHITE 0x7


/**
________________________________________________________________________________________
________________________________________________________________________________________

                                      INSTANCIATION
________________________________________________________________________________________
________________________________________________________________________________________
**/

// Instanciation éléments boîte

Servo servoArm, servoRoof;  // instanciation des servomoteurs
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();  // instanciation du LCD
int interruptInput = A0;         // assignation de la tension de l'interrupteur au pin A0
int interruptValue = 0;
int counterAction = 9;      // la première action est counterAction+1

// Instanciation éléments roues

const int enablePWN = 5;    // assignation du pin 5 en tant que enablePWN
const int in1 = 10;         // assignation du pin 10 en tant que IN1
const int in2 = 11;         // assignation du pin 11 en tant que IN2
                                                                                                                                                         
#define NOTE_C3  131
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_E4  330
#define NOTE_GS3 208
#define NOTE_F4  349

#define T2    2
#define T4    4
#define T8    8
#define T16   16
#define T24   24


int imperial_Tone_Melody[] = {NOTE_A3, NOTE_A3, NOTE_A3, NOTE_F3, NOTE_C4, NOTE_A3, NOTE_F3, NOTE_C4, NOTE_A3, NOTE_E4, NOTE_E4, NOTE_E4,
 NOTE_F4, NOTE_C4, NOTE_GS3, NOTE_F3, NOTE_C4, NOTE_A3};

int imperial_Tone_Notes_Duration[] = {T4, T4, T4, T24, T16, T4, T24, T16, T2, T4, T4, T4, T24, T16, T4, T24, T16, T2};

int pink_Panther_Melody[] = {NOTE_DS3, NOTE_E3, 0, NOTE_FS3, NOTE_G3, 0, NOTE_DS3, NOTE_E3, 0, NOTE_FS3, NOTE_G3, 0,
NOTE_C4, NOTE_B3, 0, NOTE_DS3, NOTE_E3, 0, NOTE_B3, NOTE_AS3, 0, NOTE_A3, NOTE_G3, NOTE_E3, NOTE_D3, NOTE_E3};
 
int pink_Panther_Notes_Duration[] = {T8, T8, T2, T8, T8, T2, T8, T8, T16, T8, T8, T16, T8, T8, T16, T8, T8, T16, T8, T2, T2, T16, T16, T16, T16, T2};

String emote[] = {"(^w^)" , "(^w^/)" , "(^w^|)" , "(-w-)" , "('-')" , "('o')" , "(u_u')" , "(u_u,)" , "(>_<)" , "(>o<)" , "(O_O)" , "(-_-')" , "(T_T)" , "(ToT)" , "(-.-)" , "(-o-)" , "(!_!)" , "(---)"};    // 18

void setup()
{
  Serial.begin(9600);        
  pinMode(interruptInput, INPUT); // initialise le pin A0 en tant qu'entrée
  servoArm.attach(9);             // relie le servo moteur au pin 9 de l'arduino
  servoArm.write(0);              // fixe l'inclinaison du moteur
  servoRoof.attach(8);            // relie le servo moteur au pin 8 de l'arduino
  servoRoof.write(0);             // fixe l'inclinaison du moteur
  lcd.begin(8,2);                 // fixe le début de l'écriture en coordonnées (8,2)
  printEmote(0);
}


/**
________________________________________________________________________________________
________________________________________________________________________________________

                                 GENERATION ACTION
________________________________________________________________________________________
________________________________________________________________________________________
**/

void loop()
{
  interruptValue = analogRead(interruptInput);  // lit la valeur analogique de interrupt
  Serial.println(interruptValue);
  if(interruptValue >= 1000)         // si cette valeur est > à 1000, soit = à 5V
  { 
    counterAction++;       // incrémentation d'un compteur
    switch(counterAction)
    {
      case 1 :  action1();   // extinction interrupteur x1 + ouverture capôt sans action
                break;
      case 2 :  action2();   // extinction interrupteur rapide, colérique
                break;
      case 3 :  action3();   // extinction selon les notes de la Panthère Rose
                break;
      case 4 :  action4();   // double feinte du bras suivi de l'extinction de l'interrupteur
                break;
      case 5 :  action5();   // extinction de l'interrupteur et intimidation de bras pour ne pas ré-enclencher l'interrupteur
                break;
      case 6 :  action6();   // le toit se ferme sur la main
                break;
      case 7 :  action7();   // feinte longue du bras, retour sans exctinction et fermeture du capot ; suivi d'une exctinction 
                break;
      case 8 :  action8();   // Star Wars
                break;
      case 9 :  action9();   // combat
                break;
      case 10 : action10();  // bonbons
                break;
      case 11 : action11();  // s'enerve et recule
                break;
      case 12 : action12();  // avance à chaque pression d'interrupteur
                break;             
      case 13 : action13();
                counterAction = 0;
                break;
      default : counterAction = 0;
                break;
                
    }
    reset();
  }
}


/**
________________________________________________________________________________________
________________________________________________________________________________________

                                  FONCTIONS DE CONTROLE
                                        DES SERVO
________________________________________________________________________________________
________________________________________________________________________________________
**/

void servoWriteAndWait(Servo servoChoose, int angle, int pause)
{
  servoChoose.write(angle);
  delay(pause);
}

void servoWriteAndCheck(Servo servoChoose, int angle, int pause)
{
  while(servoChoose.read() != angle)
    servoChoose.write(angle);
  if(pause > 0)
  delay(pause);
}

void controledMove(Servo servoChoose, int initAngle, int finalAngle, int increment, int pause)
{
  int angle = initAngle;
  while(servoChoose.read() != initAngle)
    servoChoose.write(initAngle);
  for(initAngle ; angle <= finalAngle ; angle += increment)
  {
    servoWriteAndWait(servoChoose,angle,pause);
  }
}

void inversedControledMove(Servo servoChoose, int initAngle, int finalAngle, int increment, int pause)
{
  int angle = initAngle;
  while(servoChoose.read() != initAngle)
  servoChoose.write(initAngle);
  for(initAngle ; angle > finalAngle ; angle -= increment)
  {
    servoWriteAndWait(servoChoose,angle,pause);
  }
}

void counterPush(int returnAngle)    // extinction interrupteur et position prête à réappuyer
{
  servoWriteAndWait(servoArm,146,150);
  servoWriteAndWait(servoArm,returnAngle,50);
}

void multiplePush(int repetition)    // déclenche x fois l'action appuyer sur l'interrupteur
{
  for(int i=0 ; i < repetition ; i++)
  {
    servoWriteAndWait(servoArm,110,150);
    servoWriteAndWait(servoArm,146,150); 
  }
}

void tiredRoof(int initAngle, int interAngle, int increment, int pause, int repetition)
{
  for(int i=0 ; i < repetition ; i++)
  {
    printEmote(6);
    inversedControledMove(servoRoof,initAngle,interAngle,increment,pause);
    printEmote(7);
    controledMove(servoRoof,interAngle,initAngle,increment,pause);
  }
}

void tiredRoof(int initAngle, int interAngle, int increment, int pause)
{
  printEmote(6);
  inversedControledMove(servoRoof,initAngle,interAngle,increment,pause);
  printEmote(7);
  controledMove(servoRoof,interAngle,initAngle,increment,pause);
}


/**
________________________________________________________________________________________
________________________________________________________________________________________

                               FONCTIONS DE CONTROLE
                                     DES ROUES
________________________________________________________________________________________
________________________________________________________________________________________
**/

/**
void wheelForward(int wheelSpeed)    // permet de faire avancer les roues dans le sens direct avec une vitesse comprise entre 1 et 9
{
  analogWrite(in1, 0);
  analogWrite(in2, 255);
  analogWrite(enablePWN, wheelSpeed);
}

void wheelBackward(int wheelSpeed)   // permet de faire avancer les roues dans le sens inverse avec une vitesse comprise entre 1 et 9
{
  analogWrite(in1, 255);
  analogWrite(in2, 0);
  analogWrite(enablePWN, wheelSpeed);
}
**/

void wheelsMove(int wheelSpeed)      // permet de faire avancer ou reculer les roues avec une vitesse comprise entre -9 et 9
{
  if(wheelSpeed == 0)
  {
    wheelStop();
  }
  else if(wheelSpeed > 0 && wheelSpeed <= 255)
  {
    analogWrite(in1, 0);
    analogWrite(in2, 255);
    analogWrite(enablePWN, wheelSpeed);
  }
  else if(wheelSpeed < 0 && wheelSpeed >= -255)
  {
    analogWrite(in1, 255);
    analogWrite(in2, 0);
    analogWrite(enablePWN, -wheelSpeed);
  }
}

void wheelStop()                     // permet d'arrêter les roues
{
  analogWrite(enablePWN, 0);
  analogWrite(in1, 0);
  analogWrite(in2, 0);
}


/**
________________________________________________________________________________________
________________________________________________________________________________________

                               FONCTIONS DE CONTROLE
                                     DU LCD
________________________________________________________________________________________
________________________________________________________________________________________
**/

void LCDPrintString(String myString, int time, boolean persist, int shiftTime)
{
  if(myString.length() <= 16)      // cas où la chaine de caractères serait de taille inférieure ou égale d'une ligne du LCD
  {
    lcd.setCursor(0,1);            // placement du curseur au début de l'écran
    lcd.print(myString);           // affichage
  }
  else
  {                                // cas pathologique où la chaîne de caractères est de taille supérieure à la taille de la ligne du LCD
    lcd.setCursor(1,1);            // placement du curseur au cran début + 1 de l'écran
    for(int i=0 ; i<15 ; i++)      // afficher une fois le début de la châine de caractères
    {
      lcd.print(myString[i]);
    }
    delay(1000);                   // délai afin de pouvoir lire le début
    for(int j=0 ; j<(myString.length())-15 ; j++)  // j est l'index du dernier caractère affiché à l'écran
    {
      lcd.setCursor(0,1);          // on se replace au tout début de la ligne
      for(int i=0 ; i<16 ; i++)    // pour chaque case de la ligne du LCD
      {
        lcd.print(myString[i+j]);  //   afficher le caractère décalé de une case à gauche pour chaque tour de boucle principal
      }
      if(shiftTime > 100 && shiftTime < 3000)  // possibilité de modifier le temps entre chaque décalage entre 100ms et 3s
        delay(shiftTime);
      else
        delay(100);                // par défaut 100ms
      if(j!=(myString.length())-16)
        clearRow(2);
    }
  }
  if(time > 0)
    delay(time);
  if(!persist)
    clearRow(2);
}

void clearRow(int number)
{
  lcd.setCursor(0,number-1);
  lcd.print("                ");
}

void printEmote(int number)
{
  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCursor(6,0);
  lcd.print(emote[number]);
}

void animateEmote(int emote1, int emote2, int time, int repetition)
{
  for(int i=0 ; i<repetition ; i++)
  {
    printEmote(emote1);
    delay(time);
    printEmote(emote2);
    delay(time);
  }
}


/**
________________________________________________________________________________________
________________________________________________________________________________________

                               FONCTIONS DE CONTROLE
                                  DU HAUT-PARLEUR
________________________________________________________________________________________
________________________________________________________________________________________
**/

void timer(int repetition)
{
  LCDPrintString("        "+repetition,1000,false,0);
  tone(3,1000,100);
  for(int i=repetition-1 ; i<=0 ; i--)
  {
    LCDPrintString("        "+i,900,false,0);
    tone(3,1000,100);
  }
  tone(3,800,400);
  LCDPrintString("        0",0,true,0);
}


/**
________________________________________________________________________________________
________________________________________________________________________________________

                                   LISTE DES ACTIONS
________________________________________________________________________________________
________________________________________________________________________________________
**/

void action1()
{
  printEmote(5);
  servoWriteAndWait(servoRoof,30,1500);
  servoWriteAndWait(servoArm,146,320);
}

void action2()
{
  printEmote(14);
  servoWriteAndWait(servoRoof,20,500);
  servoWriteAndWait(servoArm,135,320);
  servoWriteAndWait(servoArm,100,800);
  servoWriteAndWait(servoArm,146,320);
}

void action3()
{
  printEmote(5);
  int thisNote = 0;
  servoWriteAndWait(servoArm,60,200);
  for(int i=0 ; i<sizeof(pink_Panther_Melody)/2 ; i++)
  {
    int noteDuration = (500/pink_Panther_Notes_Duration[thisNote])*3;
    tone(3,pink_Panther_Melody[thisNote],noteDuration);

    if(i<=4)
    {
      servoWriteAndWait(servoRoof,(servoRoof.read())+5,0);
    }
    else
    {
      servoWriteAndWait(servoArm,(servoArm.read())+4,0);
    }
    
    int pauseBetweenNotes = noteDuration * 1.005;
    delay(pauseBetweenNotes);
    noTone(3);
    thisNote++;
  }
  servoWriteAndWait(servoArm,146,200);
}

void action4()
{
  printEmote(3);
  controledMove(servoRoof,0,25,2,50);
  for(int i=1 ; i <= 2 ; i++)
  {
    servoWriteAndWait(servoArm,130,300);
    servoWriteAndWait(servoArm,140,300);
    servoWriteAndWait(servoArm,0,500);
  } 
  servoWriteAndWait(servoArm,146,1000);
}

void action5()
{
  printEmote(5);
  servoWriteAndWait(servoRoof,20,700);
  servoWriteAndWait(servoArm,146,500);
  servoWriteAndWait(servoArm,130,750);
  servoWriteAndWait(servoArm,146,750);
  printEmote(5);
  for(int angle=146 ; angle > 75 ; angle = angle - 1)
  {
    if(analogRead(interruptInput) >= 1000)
    {
      printEmote(9);
      servoWriteAndWait(servoArm,146,200);
      angle = 146;
      printEmote(8);
    }
    servoWriteAndWait(servoArm,angle,50);
  }
}

void action6()
{
  printEmote(12);
  for (int i=1 ; i <= 4 ; i++)
  {
    if(i>1)
    {
      servoRoof.write(0);
      printEmote(13);
      delay(80);
    }
    printEmote(12);
    controledMove(servoRoof,0,25,2,50);
  }
  servoWriteAndWait(servoArm,146,320);
  servoWriteAndWait(servoArm,0,320);
  servoRoof.write(0);
  printEmote(13);
  delay(80);
  printEmote(12);
}

void action7()
{
  printEmote(4);
  servoWriteAndWait(servoRoof,20,700);
  servoWriteAndWait(servoArm,135,500);
  delay(2000);
  servoWriteAndWait(servoArm,70,1000);
  servoRoof.write(6);
  printEmote(0);
  delay(2000);
  printEmote(5);
  servoWriteAndWait(servoRoof,20,700);
  servoWriteAndWait(servoArm,146,500);
}

void action8()
{
  int angle = 0;
  int thisNote = 0;
  for(angle ; angle <= 36 ; angle = angle + 2)
  {
    printEmote(15);
    servoWriteAndWait(servoRoof,angle,0);
    int noteDuration = (500/imperial_Tone_Notes_Duration[thisNote])*3;
    tone(3, imperial_Tone_Melody[thisNote],noteDuration);

    int pauseBetweenNotes = noteDuration * 1.005;
    delay(pauseBetweenNotes);
    noTone(3);
    printEmote(14);
    thisNote++;
  }
  controledMove(servoArm,0,150,5,33);
}

void action9()
{
  printEmote(16);
  int counterDown = 0,
      counterCrazy = random(5,20),
      counterActivation = 0;
  servoWriteAndWait(servoRoof,20,0);
  while(counterDown < 75)
  {
    if(analogRead(interruptInput) >= 1000)
    {
      counterPush(130);
      counterDown = 0;
      counterActivation++;
      if(counterActivation == counterCrazy)
      {
        servoWriteAndWait(servoRoof,25,50);
        multiplePush(random(3,8));
        servoWriteAndWait(servoArm,130,50);
        servoWriteAndWait(servoRoof,23,50);
        tiredRoof(25,20,1,50,random(2,4));
        inversedControledMove(servoRoof,23,15,1,50);
        controledMove(servoRoof,15,20,1,50);
        printEmote(16);
      }
    }
    else
    {
      delay(50);
      counterDown++;
    }
  }
}

void action10()
{
  LCDPrintString("Prenez les baguettes.",500,false,0);
  LCDPrintString("        3",1000,false,0);
  LCDPrintString("        2",1000,false,0);
  LCDPrintString("        1",1000,false,0);
  printEmote(4);
  servoWriteAndWait(servoRoof,40,3500);
  wheelsMove(-255);
  delay(500);
  wheelStop();
  printEmote(0);
  LCDPrintString("    Ha ha ha !",1000,false,0);
  clearRow(2);
  printEmote(0);
  delay(1000);
  tone(3,1000,100);
  timer(5);
  servoWriteAndWait(servoArm,70,0);
  wheelsMove(-255);
  delay(300);
  wheelStop();
  servoWriteAndWait(servoArm,146,250);
}

void action11()
{
  servoWriteAndWait(servoRoof,20,200);
  printEmote(4);
  delay(1000);
  printEmote(17);
  delay(100);
  printEmote(4);
  delay(400);
  servoWriteAndWait(servoRoof,0,1500);
  printEmote(9);
  LCDPrintString("Laissez moi!",0,true,0);
  wheelsMove(255);
  delay(700);
  wheelStop();
  servoWriteAndWait(servoArm,146,500);
  servoWriteAndWait(servoArm,130,300);
  servoWriteAndWait(servoRoof,10,200);
  servoWriteAndWait(servoRoof,25,200);
  multiplePush(random(3,7));
  servoWriteAndWait(servoRoof,20,200);
  servoWriteAndWait(servoArm,146,200);
  counterPush(130);
  counterPush(130);
  servoWriteAndWait(servoRoof,40,300);
  clearRow(2);
  tiredRoof(40,20,3,503);
}

void action12()
{
  printEmote(9);
  int counterActivation = 0,
      counterDown = 0,
      i = 0;
  servoWriteAndWait(servoRoof,30,600);
  servoWriteAndWait(servoArm,140,200);
  tone(3,500,100);
  servoWriteAndWait(servoArm,110,100);
  servoWriteAndWait(servoArm,140,100);
  tone(3,500,100);
  servoWriteAndWait(servoArm,110,100);
  servoWriteAndWait(servoArm,140,100);
  tone(3,500,100);
  servoWriteAndWait(servoArm,110,100);
  printEmote(8);
  while(counterDown < 100)
  {
    if(analogRead(interruptInput) >= 1000)
    {
      delay(200);
      wheelsMove(-255);
      delay(500);
      wheelStop();
      delay(250);
      counterPush(100+(2*i));
      counterDown = 0;
      counterActivation++;
      if(counterActivation==2)
      {
        printEmote(4);
        for(i ; i<14 ; i++)
        {
          servoWriteAndWait(servoRoof,30-i,150);
          servoWriteAndWait(servoArm,100+(2*i),0);
        }
      }
      if(counterActivation==4)
      {
        servoWriteAndWait(servoArm,146,0);
        wheelsMove(255);
        delay(2000);
        wheelStop();
        break;
      }
    }
    else
    {
      delay(50);
      counterDown++;
    }
  }
  if(counterActivation<4)
  {
    delay(300);
    servoWriteAndWait(servoArm,146,0);
    wheelsMove(255);
    delay(counterActivation*500);
    wheelStop();
  }
}

void action13()
{
  int i=0;
  servoWriteAndWait(servoRoof,20,500);
  servoWriteAndWait(servoArm,90,1000);
  controledMove(servoArm,90,125,1,50);
  servoWriteAndWait(servoArm,90,1000);
  controledMove(servoArm,90,135,1,50);
  servoWriteAndWait(servoArm,90,1000);
  controledMove(servoArm,90,142,1,50);
  servoWriteAndWait(servoArm,90,2000);
  servoWriteAndWait(servoArm,146,500);

}

/**
________________________________________________________________________________________
________________________________________________________________________________________

                               RETOUR ETAT INITIAL
________________________________________________________________________________________
________________________________________________________________________________________
**/

void reset()    // réinitialise les servos moteurs et le LCD
{
  servoWriteAndWait(servoArm,0,350);
  servoWriteAndWait(servoRoof,0,200);
  clearRow(2);
  printEmote(0);
}
