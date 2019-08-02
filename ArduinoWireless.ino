/*  RADIOCONTROLLI SRL ARDUINO WIRELESS
 *  AUTHOR : CASTALDO MARIO 
 *  https://www.radiocontrolli.com/products/arduino-wireless-868mhz.html 
 *   
 *  Project based with Arduino and RCQ3-868Mhz Shield. The system 
 *  allows remote communication and configuration of other actuators 
 *  within the same network. The main menu consists of:
 *  
 *  1) Configure Remote Actuator
 *  2) Remote Control
 *  3) Search New Actuator 
 *  4) Choose Actuator
 *  
 * Connect all Hardare to Arduino and start playing !
*/
#include <SoftwareSerial.h>

#include <LiquidCrystal.h>
#include <Keypad.h>
#include <EEPROM.h>

#define rx 10
#define tx 9
#define T2 3

void aggiornaMenu();
void cambiamenu();
void memory();
void search();
void choose();

SoftwareSerial seriale (rx, tx);
String m;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const byte ROWS = 4;
const byte COLS = 4;
byte chose[14][13]={
  {0x23, 0x5E, 0x43, 0x7E, 0x7E, 0x7E, 0x7E, 0x01, 0x01, 0x01, 0x01, 0x50, 0x3A},
  {0x23, 0x5E, 0x43, 0x7E, 0x7E, 0x7E, 0x7E, 0x01, 0x01, 0x01, 0x02, 0x50, 0x3A},
  {0x23, 0x5E, 0x43, 0x7E, 0x7E, 0x7E, 0x7E, 0x01, 0x01, 0x01, 0x03, 0x50, 0x3A},
  {0x23, 0x5E, 0x43, 0x7E, 0x7E, 0x7E, 0x7E, 0x01, 0x01, 0x01, 0x04, 0x50, 0x3A},
  {0x23, 0x5E, 0x43, 0x7E, 0x7E, 0x7E, 0x7E, 0x01, 0x01, 0x01, 0x05, 0x50, 0x3A},
  {0x23, 0x5E, 0x43, 0x7E, 0x7E, 0x7E, 0x7E, 0x01, 0x01, 0x01, 0x06, 0x50, 0x3A},
  {0x23, 0x5E, 0x43, 0x7E, 0x7E, 0x7E, 0x7E, 0x01, 0x01, 0x01, 0x07, 0x50, 0x3A},
  {0x23, 0x5E, 0x43, 0x7E, 0x7E, 0x7E, 0x7E, 0x01, 0x01, 0x01, 0x08, 0x50, 0x3A},
  {0x23, 0x5E, 0x43, 0x7E, 0x7E, 0x7E, 0x7E, 0x01, 0x01, 0x01, 0x09, 0x50, 0x3A},
  {0x23, 0x5E, 0x43, 0x7E, 0x7E, 0x7E, 0x7E, 0x01, 0x01, 0x01, 0x0A, 0x50, 0x3A},
  {0x23, 0x5E, 0x43, 0x7E, 0x7E, 0x7E, 0x7E, 0x01, 0x01, 0x01, 0x0B, 0x50, 0x3A},
  {0x23, 0x5E, 0x43, 0x7E, 0x7E, 0x7E, 0x7E, 0x01, 0x01, 0x01, 0x0C, 0x50, 0x3A},
  {0x23, 0x5E, 0x43, 0x7E, 0x7E, 0x7E, 0x7E, 0x01, 0x01, 0x01, 0x0D, 0x50, 0x3A},
  {0x23, 0x5E, 0x43, 0x7E, 0x7E, 0x7E, 0x7E, 0x01, 0x01, 0x01, 0x3E, 0x50, 0x3A}
};
byte A[13]={0x23, 0x5E, 0x43, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x50, 0x3A};

char keys[ROWS][COLS] =
{
{'1','2','3','A'},
{'4','5','6','B'},
{'7','8','9','C'},
{'*','0','#','D'}
};

const char  messaggi[3][17] = {
  {
    //  '1','2','3','4','5','6','7','8','9','0','1','2','3','4','5','6'
    'A', 'R', 'D', 'U', 'I', 'N', 'O', '-', 'W', 'I', 'R', 'E', 'L', 'E', 'S', 'S', 0
  }
  ,
  {
    //  '1','2','3','4','5','6','7','8','9','0','1','2','3','4','5','6'
    'b', 'y', 'R', 'A', 'D', 'I', 'O', 'C', 'O', 'N', 'T', 'R', 'O', 'L', 'L', 'I', 0
  }
  ,
  {
    // '1','2','3','4','5','6','7','8','9','0','1','2','3','4','5','6'
    ' ', ' ', ' ', ' ', 'W', 'E', 'L', 'C', 'O', 'M', 'E', ' ', ' ', ' ', ' ', ' ', 0
  }
};

byte rowPins[] = { A0, A1, A2, A3 };
byte colPins[] = { A4, A5, 7, 6};

int menu=1 ;
int pippo;
int check;
unsigned char stato;
String dato;
String verifica;
char value[17];
int lock;

Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup() {   //inizializzazione menu principale
  Serial.begin(115200);
  seriale.begin(115200);
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print(messaggi[0]);
  delay(3000);
  lcd.setCursor(0,1);
  lcd.print(messaggi[1]);
  delay(3000);
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print(messaggi[2]);
  delay(2000);
  aggiornaMenu();
}

void loop() { //loop programma
  char keys = kpd.getKey();
  char value[17];
  
 if(keys){
  if(keys == 0)exit;
  switch(keys){
    case 'A' :
           menu--;
           if(menu == 0)menu = 1;
           aggiornaMenu();
           delay(100);
           break;
    case 'B' :
           menu++;
           if(menu > 4 && check == 0)menu = 4;
           aggiornaMenu();
           delay(100);
           break;
    case 'C' :
           esegui();
           delay(100);
           break;
    case '*' :
          menu = 1;
          pippo = 0;
          check = 0;
          aggiornaMenu();
          delay(100);
          break;
    case'#':
           lcd.clear();
           for (int i = 0 ; i < EEPROM.length() ; i++) {
           EEPROM.write(i,0x7E);
           }
           lcd.print("  EEPROM RESET  ");
           delay(1500);
           aggiornaMenu();
           break;
  }
  }
  delay(200);
  if(check==1){
    configure();
  }else if(check==2){
    if(menu==4) check=0;
    comandi();
 } else if(check==3){
    search();
 }   else if(check==4){
    switch(keys){
    case 'A' :
           pippo--;
           if(pippo > 16) pippo=0;
           choose();
           delay(100);
           break;
    case 'B' :
           pippo++;
           if(pippo > 15) pippo=15;
           choose();
           delay(100);
           break;
    case 'C' :
          esegui2();
          delay(100);
          break;
    case '#':
          check=0;
          break;
    case '*':
          aggiornaMenu();
          delay(500);
          break;       
  }
   }
}

void esegui2() { //Tasto C selezionare menu
switch (pippo) {
    case 1:
      azione5();
      break;
    case 2:
      azione6();
      break;
    case 3:
      azione7();
      break;
    case 4:
      azione8();
      break;
    case 5:
      azione9();
      break;
    case 6:
      azione10();
      break;
    case 7:
      azione11();
      break;
    case 8:
      azione12();
      break;
    case 9:
      azione13();
      break;
    case 10:
      azione14();
      break;
    case 11:
      azione15();
      break;
    case 12:
      azione16();
      break;
    case 13:
      azione17();
      break;
    case 14:
      azione18();
      break;
    case 15:
      azione19();
      break;
  }
}

void esegui() { //Tasto C selezionare menu
switch (menu) {
    case 1:
      azione1();
      break;
    case 2:
      azione2();
      break;
    case 3:
      azione3();
      break;
    case 4:
      azione4();
      break;
  }
}

void azione1() {
   lcd.clear();
   lcd.print("  Digit 1 to D  ");
   lcd.setCursor(1,1);
   lcd.print(" to Configure ");
   check=1;
}

void azione2() {
  lcd.clear();
  lcd.print("->Insert Code<-");
   check=2;
}

void azione3() {
  lcd.clear();
  lcd.print("Digit C to Search");
  check=3;
}

void azione4(){
  lcd.clear();
  choose();
  check=4;
}

void azione5(){
  if(pippo==1){
  byte addr2[6]={0x5E, 0x43,EEPROM.read(0), EEPROM.read(1), EEPROM.read(2), EEPROM.read(3)}; 
  lcd.clear();
  sprintf(value, "%02x %02x %02x %02x", EEPROM.read(0), EEPROM.read(1), EEPROM.read(2), EEPROM.read(3));
  lcd.print(value);
  delay(3000);
  lcd.clear();
  seriale.write(addr2,6);
  lcd.setCursor(0,0);
  lcd.print("    Device    ");
  lcd.setCursor(1,2);
  lcd.print("   Selected   ");
  delay(1000);
  //lcd.clear();
  //delay(500);
  choose();
  }
}

void azione6(){
  if(pippo==2){
  byte addr[6]={0x5E, 0x43,EEPROM.read(4), EEPROM.read(5), EEPROM.read(6), EEPROM.read(7)}; 
  lcd.clear();
  sprintf(value, "%02x %02x %02x %02x", EEPROM.read(4), EEPROM.read(5), EEPROM.read(6), EEPROM.read(7));
  lcd.print(value);
  delay(3000);
  lcd.clear();
  seriale.write(addr,6);
  lcd.setCursor(0,0);
  lcd.print("    Device    ");
  lcd.setCursor(1,2);
  lcd.print("   Selected   ");
  delay(1000);
  //lcd.clear();
  //delay(500);
    choose();

  }
}

void azione7(){
  if(pippo==3){
  byte addr3[6]={0x5E, 0x43,EEPROM.read(8), EEPROM.read(9), EEPROM.read(10), EEPROM.read(11)}; 
  lcd.clear();
  sprintf(value, "%02x %02x %02x %02x", EEPROM.read(8), EEPROM.read(9), EEPROM.read(10), EEPROM.read(11));
  lcd.print(value);
  delay(3000);
  lcd.clear();
  seriale.write(addr3,6);
  lcd.setCursor(0,0);
  lcd.print("    Device    ");
  lcd.setCursor(1,2);
  lcd.print("   Selected   ");
  delay(1000);
 // lcd.clear();
 // delay(500);
    choose();

  }
}

void azione8(){
  if(pippo==4){
  byte addr4[6]={0x5E, 0x43,EEPROM.read(12), EEPROM.read(13), EEPROM.read(14), EEPROM.read(15)}; 
  lcd.clear();
  sprintf(value, "%02x %02x %02x %02x", EEPROM.read(12), EEPROM.read(13), EEPROM.read(14), EEPROM.read(15));
  lcd.print(value);
  delay(3000);
  lcd.clear();
  seriale.write(addr4,6);
  lcd.setCursor(0,0);
  lcd.print("    Device    ");
  lcd.setCursor(1,2);
  lcd.print("   Selected   ");
  delay(1000);
  lcd.clear();
  delay(500);
    choose();

  }
}
void azione9(){
  if(pippo==5){
    byte addr5[6]={0x5E, 0x43,EEPROM.read(16), EEPROM.read(17), EEPROM.read(18), EEPROM.read(19)}; 
     lcd.clear();
  sprintf(value, "%02x %02x %02x %02x", EEPROM.read(16), EEPROM.read(17), EEPROM.read(18), EEPROM.read(19));
  lcd.print(value);
  delay(3000);
  lcd.clear();
  seriale.write(addr5,6);
  lcd.setCursor(0,0);
  lcd.print("    Device    ");
  lcd.setCursor(1,2);
  lcd.print("   Selected   ");
  delay(1000);
  lcd.clear();
  delay(500);
    choose();
  }
}

void azione10(){
  if(pippo==6){
    byte addr6[6]={0x5E, 0x43,EEPROM.read(20), EEPROM.read(21), EEPROM.read(22), EEPROM.read(23)}; 
     lcd.clear();
  sprintf(value, "%02x %02x %02x %02x", EEPROM.read(20), EEPROM.read(21), EEPROM.read(22), EEPROM.read(23));
  lcd.print(value);
  delay(3000);
  lcd.clear();
  seriale.write(addr6,6);
  lcd.setCursor(0,0);
  lcd.print("    Device    ");
  lcd.setCursor(1,2);
  lcd.print("   Selected   ");
  delay(1000);
  lcd.clear();
  delay(500);
    choose();
  }
}

void azione11(){
  if(pippo==7){
    byte addr7[6]={0x5E, 0x43,EEPROM.read(24), EEPROM.read(25), EEPROM.read(26), EEPROM.read(27)}; 
     lcd.clear();
  sprintf(value, "%02x %02x %02x %02x", EEPROM.read(24), EEPROM.read(25), EEPROM.read(26), EEPROM.read(27));
  lcd.print(value);
  delay(3000);
  lcd.clear();
  seriale.write(addr7,6);
  lcd.setCursor(0,0);
  lcd.print("    Device    ");
  lcd.setCursor(1,2);
  lcd.print("   Selected   ");
  delay(1000);
  lcd.clear();
  delay(500);
    choose();
  }
}

void azione12(){
  if(pippo==8){
    byte addr8[6]={0x5E, 0x43,EEPROM.read(28), EEPROM.read(29), EEPROM.read(30), EEPROM.read(31)}; 
     lcd.clear();
  sprintf(value, "%02x %02x %02x %02x", EEPROM.read(28), EEPROM.read(29), EEPROM.read(30), EEPROM.read(31));
  lcd.print(value);
  delay(3000);
  lcd.clear();
  seriale.write(addr8,6);
  lcd.setCursor(0,0);
  lcd.print("    Device    ");
  lcd.setCursor(1,2);
  lcd.print("   Selected   ");
  delay(1000);
  lcd.clear();
  delay(500);
    choose();
  }
}

void azione13(){
  if(pippo==9){
    byte addr9[6]={0x5E, 0x43,EEPROM.read(32), EEPROM.read(33), EEPROM.read(34), EEPROM.read(35)}; 
     lcd.clear();
  sprintf(value, "%02x %02x %02x %02x", EEPROM.read(32), EEPROM.read(33), EEPROM.read(34), EEPROM.read(35));
  lcd.print(value);
  delay(3000);
  lcd.clear();
  seriale.write(addr9,6);
  lcd.setCursor(0,0);
  lcd.print("    Device    ");
  lcd.setCursor(1,2);
  lcd.print("   Selected   ");
  delay(1000);
  lcd.clear();
  delay(500);
    choose();
  }
}

void azione14(){
  if(pippo==10){
    byte addr10[6]={0x5E, 0x43,EEPROM.read(36), EEPROM.read(37), EEPROM.read(38), EEPROM.read(39)}; 
     lcd.clear();
  sprintf(value, "%02x %02x %02x %02x", EEPROM.read(36), EEPROM.read(37), EEPROM.read(38), EEPROM.read(39));
  lcd.print(value);
  delay(3000);
  lcd.clear();
  seriale.write(addr10,6);
  lcd.setCursor(0,0);
  lcd.print("    Device    ");
  lcd.setCursor(1,2);
  lcd.print("   Selected   ");
  delay(1000);
  lcd.clear();
  delay(500);
    choose();
  }
}

void azione15(){
 if(pippo==11){
    byte addr11[6]={0x5E, 0x43,EEPROM.read(40), EEPROM.read(41), EEPROM.read(42), EEPROM.read(43)}; 
     lcd.clear();
  sprintf(value, "%02x %02x %02x %02x", EEPROM.read(40), EEPROM.read(41), EEPROM.read(42), EEPROM.read(43));
  lcd.print(value);
  delay(3000);
  lcd.clear();
  seriale.write(addr11,6);
  lcd.setCursor(0,0);
  lcd.print("    Device    ");
  lcd.setCursor(1,2);
  lcd.print("   Selected   ");
  delay(1000);
  lcd.clear();
  delay(500);
    choose();
  } 
}

void azione16(){
  if(pippo==12){
    byte addr12[6]={0x5E, 0x43,EEPROM.read(44), EEPROM.read(45), EEPROM.read(46), EEPROM.read(47)}; 
     lcd.clear();
  sprintf(value, "%02x %02x %02x %02x", EEPROM.read(44), EEPROM.read(45), EEPROM.read(46), EEPROM.read(47));
  lcd.print(value);
  delay(3000);
  lcd.clear();
  seriale.write(addr12,6);
  lcd.setCursor(0,0);
  lcd.print("    Device    ");
  lcd.setCursor(1,2);
  lcd.print("   Selected   ");
  delay(1000);
  lcd.clear();
  delay(500);
    choose();
  }
}

void azione17(){
  if(pippo==13){
    byte addr13[6]={0x5E, 0x43,EEPROM.read(48), EEPROM.read(49), EEPROM.read(50), EEPROM.read(51)}; 
     lcd.clear();
  sprintf(value, "%02x %02x %02x %02x", EEPROM.read(48), EEPROM.read(49), EEPROM.read(50), EEPROM.read(51));
  lcd.print(value);
  delay(3000);
  lcd.clear();
  seriale.write(addr13,6);
  lcd.setCursor(0,0);
  lcd.print("    Device    ");
  lcd.setCursor(1,2);
  lcd.print("   Selected   ");
  delay(1000);
  lcd.clear();
  delay(500);
    choose();
  }
}

void azione18(){
  if(pippo==14){
    byte addr14[6]={0x5E, 0x43,EEPROM.read(52), EEPROM.read(53), EEPROM.read(54), EEPROM.read(55)}; 
     lcd.clear();
  sprintf(value, "%02x %02x %02x %02x", EEPROM.read(52), EEPROM.read(53), EEPROM.read(54), EEPROM.read(55));
  lcd.print(value);
  delay(3000);
  lcd.clear();
  seriale.write(addr14,6);
  lcd.setCursor(0,0);
  lcd.print("    Device    ");
  lcd.setCursor(1,2);
  lcd.print("   Selected   ");
  delay(1000);
  lcd.clear();
  delay(500);
    choose();
  }
}

void azione19(){
  byte B[6]={0x5E, 0x43, 0x7E, 0x7E, 0x7E, 0x7E}; 
  lcd.clear();
  sprintf(value, "%02x %02x %02x %02x", B[2], B[3], B[4], B[5]);
  lcd.print(value);
  delay(3000);
  lcd.clear();
  seriale.write(B,6);
  lcd.setCursor(0,0);
  lcd.print("    Device    ");
  lcd.setCursor(1,2);
  lcd.print("   Selected   ");
  delay(2000);
  lcd.clear();
  delay(500);
    choose();

}

//controlla se è presente un device con address 7E 7E 7E 7E
void search(){
  char keys = kpd.getKey();
  switch(keys){
    case 'C' :
    lcd.clear();
    seriale.write("#?V"); //controllo device
    seriale.flush();
    verifica = seriale.readString();
    const char *matrice;
    matrice = verifica.c_str();
    int length = strlen(matrice);
    if(length >=T2){
    lcd.print("Device Available");
    } else {
    lcd.setCursor(0,0);
    lcd.print("    WARNING!    ");
    lcd.setCursor(0,2);
    lcd.print("Connect Actuator");
  }
  delay(2000);
  aggiornaMenu();
  check=0;
  break;          
  }
}

  //scelta device da pilotare
void choose(){
       byte ad[13]={0x23, 0x5E, 0x43, 0x7E, 0x7E, 0x7E, 0x7E,EEPROM.read(0), EEPROM.read(1), EEPROM.read(2), EEPROM.read(3), 0x50, 0x3A};
        byte ad2[13]={0x23, 0x5E, 0x43, 0x7E, 0x7E, 0x7E, 0x7E, EEPROM.read(4), EEPROM.read(5), EEPROM.read(6), EEPROM.read(7), 0x50, 0x3A};
        byte ad3[13]={0x23, 0x5E, 0x43, 0x7E, 0x7E, 0x7E, 0x7E, EEPROM.read(8), EEPROM.read(9), EEPROM.read(10), EEPROM.read(11), 0x50, 0x3A};
        byte ad4[13]={0x23, 0x5E, 0x43, 0x7E, 0x7E, 0x7E, 0x7E, EEPROM.read(12), EEPROM.read(13), EEPROM.read(14), EEPROM.read(15), 0x50, 0x3A};
        byte ad5[13]={0x23, 0x5E, 0x43, 0x7E, 0x7E, 0x7E, 0x7E, EEPROM.read(16), EEPROM.read(17), EEPROM.read(18), EEPROM.read(19), 0x50, 0x3A};
        byte ad6[13]={0x23, 0x5E, 0x43, 0x7E, 0x7E, 0x7E, 0x7E, EEPROM.read(20), EEPROM.read(21), EEPROM.read(22), EEPROM.read(23), 0x50, 0x3A};
        byte ad7[13]={0x23, 0x5E, 0x43, 0x7E, 0x7E, 0x7E, 0x7E, EEPROM.read(24), EEPROM.read(25), EEPROM.read(26), EEPROM.read(27), 0x50, 0x3A};
        byte ad8[13]={0x23, 0x5E, 0x43, 0x7E, 0x7E, 0x7E, 0x7E, EEPROM.read(28), EEPROM.read(29), EEPROM.read(30), EEPROM.read(31), 0x50, 0x3A};
        byte ad9[13]={0x23, 0x5E, 0x43, 0x7E, 0x7E, 0x7E, 0x7E, EEPROM.read(32), EEPROM.read(33), EEPROM.read(34), EEPROM.read(35), 0x50, 0x3A};
        byte ad10[13]={0x23, 0x5E, 0x43, 0x7E, 0x7E, 0x7E, 0x7E, EEPROM.read(36), EEPROM.read(37), EEPROM.read(38), EEPROM.read(39), 0x50, 0x3A};
        byte ad11[13]={0x23, 0x5E, 0x43, 0x7E, 0x7E, 0x7E, 0x7E, EEPROM.read(40), EEPROM.read(41), EEPROM.read(42), EEPROM.read(43), 0x50, 0x3A};
        byte ad12[13]={0x23, 0x5E, 0x43, 0x7E, 0x7E, 0x7E, 0x7E, EEPROM.read(44), EEPROM.read(45), EEPROM.read(46), EEPROM.read(47), 0x50, 0x3A};
        byte ad13[13]={0x23, 0x5E, 0x43, 0x7E, 0x7E, 0x7E, 0x7E, EEPROM.read(48), EEPROM.read(49), EEPROM.read(50), EEPROM.read(51), 0x50, 0x3A};
        byte ad14[13]={0x23, 0x5E, 0x43, 0x7E, 0x7E, 0x7E, 0x7E, EEPROM.read(52), EEPROM.read(53), EEPROM.read(54), EEPROM.read(55), 0x50, 0x3A};
        
        for(int i=0;i<13;i++){
          Serial.print(ad[i],HEX);
        }
        
        switch(pippo){
            case 1:
                if(strcmp(A,ad)!=0 & strcmp(A,ad2)!=0){
                lcd.clear();
                sprintf(value,">  %02x %02x %02x %02x", ad[7], ad[8], ad[9], ad[10]);
                lcd.print(value);
                lcd.setCursor(0,1);
                sprintf(value,"  %02x %02x %02x %02x", ad2[7], ad2[8], ad2[9], ad2[10]);
                lcd.print(value);
                }else if(strcmp(A,ad)!=0){
                  lcd.clear();
                sprintf(value,">  %02x %02x %02x %02x", ad[7], ad[8], ad[9], ad[10]);
                lcd.print(value);  
                   }else if(strcmp(A,ad2)!=0){
                      lcd.clear();
                      sprintf(value,"  %02x %02x %02x %02x", ad2[7], ad2[8], ad2[9], ad2[10]);
                      lcd.print(value);  
                   }else if(strcmp(A,ad3)!=0 && strcmp(A,ad4)!=0){
                lcd.clear();
                sprintf(value,">  %02x %02x %02x %02x", ad3[7], ad3[8], ad3[9], ad3[10]);
                lcd.print(value);
                lcd.setCursor(0,1);
                sprintf(value,"  %02x %02x %02x %02x", ad4[7], ad4[8], ad4[9], ad4[10]);
                lcd.print(value);
                }else if(strcmp(A,ad[2])!=0){
                  lcd.clear();
                sprintf(value,">  %02x %02x %02x %02x", ad3[7], ad3[8], ad3[9], ad3[10]);
                lcd.print(value);  
                   }else if(strcmp(A,ad[3])!=0){
                      lcd.clear();
                      sprintf(value,"  %02x %02x %02x %02x", ad4[7], ad4[8], ad4[9], ad4[10]);
                      lcd.print(value);  
                   }else{
                    lcd.clear();
                    lcd.print("Config Actuator");
                    aggiornaMenu();
                   }
                break;
           case 2:
              if(strcmp(A,ad)!=0 & strcmp(A,ad2)!=0){
                lcd.clear();
                sprintf(value,"  %02x %02x %02x %02x", ad[7], ad[8], ad[9], ad[10]);
                lcd.print(value);
                lcd.setCursor(0,1);
                sprintf(value,">  %02x %02x %02x %02x", ad2[7], ad2[8], ad2[9], ad2[10]);
                lcd.print(value);
                }else if(strcmp(A,ad)!=0){
                  lcd.clear();
                sprintf(value,"  %02x %02x %02x %02x", ad[7], ad[8], ad[9], ad[10]);
                lcd.print(value);  
                   }else if(strcmp(A,ad2)!=0){
                      lcd.clear();
                      sprintf(value,">  %02x %02x %02x %02x", ad2[7], ad2[8], ad2[9], ad2[10]);
                      lcd.print(value);  
                   }else if(strcmp(A,ad3)!=0 && strcmp(A,ad4)!=0){
                lcd.clear();
                sprintf(value,"  %02x %02x %02x %02x", ad3[7], ad3[8], ad3[9], ad3[10]);
                lcd.print(value);
                lcd.setCursor(0,1);
                sprintf(value,">  %02x %02x %02x %02x", ad4[7], ad4[8], ad4[9], ad4[10]);
                lcd.print(value);
                }else if(strcmp(A,ad[2])!=0){
                  lcd.clear();
                sprintf(value,"  %02x %02x %02x %02x", ad3[7], ad3[8], ad3[9], ad3[10]);
                lcd.print(value);  
                   }else if(strcmp(A,ad[3])!=0){
                      lcd.clear();
                      sprintf(value,">  %02x %02x %02x %02x", ad4[7], ad4[8], ad4[9], ad4[10]);
                      lcd.print(value);  
                   }else{
                    lcd.clear();
                    lcd.print("Config Actuator");
                    aggiornaMenu();
                   }
                break;
           case 3:
               if(strcmp(A,ad3)!=0 & strcmp(A,ad4)!=0){
                lcd.clear();
                sprintf(value,">  %02x %02x %02x %02x", ad3[7], ad3[8], ad3[9], ad3[10]);
                lcd.print(value);
                lcd.setCursor(0,1);
                sprintf(value,"  %02x %02x %02x %02x", ad4[7], ad4[8], ad4[9], ad4[10]);
                lcd.print(value);
                }else if(strcmp(A,ad3)!=0){
                  lcd.clear();
                sprintf(value,">  %02x %02x %02x %02x", ad3[7], ad3[8], ad3[9], ad3[10]);
                lcd.print(value);  
                   }else if(strcmp(A,ad4)!=0){
                      lcd.clear();
                      sprintf(value,"  %02x %02x %02x %02x", ad4[7], ad4[8], ad4[9], ad4[10]);
                      lcd.print(value);  
                   }else if(strcmp(A,ad)!=0 && strcmp(A,ad2)!=0){
                lcd.clear();
                sprintf(value,">  %02x %02x %02x %02x", ad[7], ad[8], ad[9], ad[10]);
                lcd.print(value);
                lcd.setCursor(0,1);
                sprintf(value,"  %02x %02x %02x %02x", ad2[7], ad2[8], ad2[9], ad2[10]);
                lcd.print(value);
                }else if(strcmp(A,ad[2])!=0){
                  lcd.clear();
                sprintf(value,">  %02x %02x %02x %02x", ad[7], ad[8], ad[9], ad[10]);
                lcd.print(value);  
                   }else if(strcmp(A,ad[3])!=0){
                      lcd.clear();
                      sprintf(value,"  %02x %02x %02x %02x", ad2[7], ad2[8], ad2[9], ad2[10]);
                      lcd.print(value);  
                   }else{
                    lcd.clear();
                    lcd.print("Config Actuator");
                    aggiornaMenu();
                   }
               break;
          case 4:
            if(strcmp(A,ad3)!=0 & strcmp(A,ad4)!=0){
                lcd.clear();
                sprintf(value,"  %02x %02x %02x %02x", ad3[7], ad3[8], ad3[9], ad3[10]);
                lcd.print(value);
                lcd.setCursor(0,1);
                sprintf(value,">  %02x %02x %02x %02x", ad4[7], ad4[8], ad4[9], ad4[10]);
                lcd.print(value);
                }else if(strcmp(A,ad3)!=0){
                  lcd.clear();
                sprintf(value,"  %02x %02x %02x %02x", ad3[7], ad3[8], ad3[9], ad3[10]);
                lcd.print(value);  
                   }else if(strcmp(A,ad4)!=0){
                      lcd.clear();
                      sprintf(value,">  %02x %02x %02x %02x", ad4[7], ad4[8], ad4[9], ad4[10]);
                      lcd.print(value);  
                   }else if(strcmp(A,ad)!=0 && strcmp(A,ad2)!=0){
                lcd.clear();
                sprintf(value,"  %02x %02x %02x %02x", ad[7], ad[8], ad[9], ad[10]);
                lcd.print(value);
                lcd.setCursor(0,1);
                sprintf(value,">  %02x %02x %02x %02x", ad2[7], ad2[8], ad2[9], ad2[10]);
                lcd.print(value);
                }else if(strcmp(A,ad[2])!=0){
                  lcd.clear();
                sprintf(value,"  %02x %02x %02x %02x", ad3[7], ad3[8], ad3[9], ad3[10]);
                lcd.print(value);  
                   }else if(strcmp(A,ad[3])!=0){
                      lcd.clear();
                      sprintf(value,">  %02x %02x %02x %02x", ad4[7], ad4[8], ad4[9], ad4[10]);
                      lcd.print(value);  
                   }else{
                    lcd.clear();
                    lcd.print("Config Actuator");
                    aggiornaMenu();
                   }
                break;
          case 5:
                if(strcmp(A,ad5)!=0 & strcmp(A,ad6)!=0){
                lcd.clear();
                sprintf(value,">  %02x %02x %02x %02x", ad5[7], ad5[8], ad5[9], ad5[10]);
                lcd.print(value);
                lcd.setCursor(0,1);
                sprintf(value,"  %02x %02x %02x %02x", ad6[7], ad6[8], ad6[9], ad6[10]);
                lcd.print(value);
                }else if(strcmp(A,ad5)!=0){
                  lcd.clear();
                sprintf(value,">  %02x %02x %02x %02x", ad5[7], ad5[8], ad5[9], ad5[10]);
                lcd.print(value);  
                   }else if(strcmp(A,ad6)!=0){
                      lcd.clear();
                      sprintf(value,"  %02x %02x %02x %02x", ad6[7], ad6[8], ad6[9], ad6[10]);
                      lcd.print(value);  
                   }else if(strcmp(A,ad3)!=0 && strcmp(A,ad4)!=0){
                lcd.clear();
                sprintf(value,">  %02x %02x %02x %02x", ad3[7], ad3[8], ad3[9], ad3[10]);
                lcd.print(value);
                lcd.setCursor(0,1);
                sprintf(value,"  %02x %02x %02x %02x", ad4[7], ad4[8], ad4[9], ad4[10]);
                lcd.print(value);
                }else if(strcmp(A,ad3[2])!=0){
                  lcd.clear();
                sprintf(value,">  %02x %02x %02x %02x", ad3[7], ad3[8], ad3[9], ad3[10]);
                lcd.print(value);  
                   }else if(strcmp(A,ad3[3])!=0){
                      lcd.clear();
                      sprintf(value,"  %02x %02x %02x %02x", ad4[7], ad4[8], ad4[9], ad4[10]);
                      lcd.print(value);  
                   }else{
                    lcd.clear();
                    lcd.print("Config Actuator");
                    aggiornaMenu();
                   }
               break;   
          case 6:
               if(strcmp(A,ad5)!=0 & strcmp(A,ad6)!=0){
                lcd.clear();
                sprintf(value,"  %02x %02x %02x %02x", ad5[7], ad5[8], ad5[9], ad5[10]);
                lcd.print(value);
                lcd.setCursor(0,1);
                sprintf(value,">  %02x %02x %02x %02x", ad6[7], ad6[8], ad6[9], ad6[10]);
                lcd.print(value);
                }else if(strcmp(A,ad5)!=0){
                  lcd.clear();
                sprintf(value,"  %02x %02x %02x %02x", ad5[7], ad5[8], ad5[9], ad5[10]);
                lcd.print(value);  
                   }else if(strcmp(A,ad6)!=0){
                      lcd.clear();
                      sprintf(value,">  %02x %02x %02x %02x", ad6[7], ad6[8], ad6[9], ad6[10]);
                      lcd.print(value);  
                   }else if(strcmp(A,ad3)!=0 && strcmp(A,ad4)!=0){
                lcd.clear();
                sprintf(value,"  %02x %02x %02x %02x", ad3[7], ad3[8], ad3[9], ad3[10]);
                lcd.print(value);
                lcd.setCursor(0,1);
                sprintf(value,">  %02x %02x %02x %02x", ad4[7], ad4[8], ad4[9], ad4[10]);
                lcd.print(value);
                }else if(strcmp(A,ad[2])!=0){
                  lcd.clear();
                sprintf(value,"  %02x %02x %02x %02x", ad3[7], ad3[8], ad3[9], ad3[10]);
                lcd.print(value);  
                   }else if(strcmp(A,ad[3])!=0){
                      lcd.clear();
                      sprintf(value,">  %02x %02x %02x %02x", ad4[7], ad4[8], ad4[9], ad4[10]);
                      lcd.print(value);  
                   }else{
                    lcd.clear();
                    lcd.print("Config Actuator");
                    aggiornaMenu();
                   }
                break;
          case 7:
                if(strcmp(A,ad7)!=0 & strcmp(A,ad8)!=0){
                lcd.clear();
                sprintf(value,">  %02x %02x %02x %02x", ad7[7], ad7[8], ad7[9], ad7[10]);
                lcd.print(value);
                lcd.setCursor(0,1);
                sprintf(value,"  %02x %02x %02x %02x", ad8[7], ad8[8], ad8[9], ad8[10]);
                lcd.print(value);
                }else if(strcmp(A,ad7)!=0){
                  lcd.clear();
                sprintf(value,">  %02x %02x %02x %02x", ad7[7], ad7[8], ad7[9], ad7[10]);
                lcd.print(value);  
                   }else if(strcmp(A,ad8)!=0){
                      lcd.clear();
                      sprintf(value,"  %02x %02x %02x %02x", ad8[7], ad8[8], ad8[9], ad8[10]);
                      lcd.print(value);  
                   }else if(strcmp(A,ad5)!=0 && strcmp(A,ad6)!=0){
                lcd.clear();
                sprintf(value,">  %02x %02x %02x %02x", ad5[7], ad5[8], ad5[9], ad5[10]);
                lcd.print(value);
                lcd.setCursor(0,1);
                sprintf(value,"  %02x %02x %02x %02x", ad6[7], ad6[8], ad6[9], ad6[10]);
                lcd.print(value);
                }else if(strcmp(A,ad5[2])!=0){
                  lcd.clear();
                sprintf(value,">  %02x %02x %02x %02x", ad5[7], ad5[8], ad5[9], ad5[10]);
                lcd.print(value);  
                   }else if(strcmp(A,ad3[3])!=0){
                      lcd.clear();
                      sprintf(value,"  %02x %02x %02x %02x", ad6[7], ad6[8], ad6[9], ad6[10]);
                      lcd.print(value);  
                   }else{
                    lcd.clear();
                    lcd.print("Config Actuator");
                    aggiornaMenu();
                   }
                break;
          case 8:
                if(strcmp(A,ad7)!=0 & strcmp(A,ad8)!=0){
                lcd.clear();
                sprintf(value,"  %02x %02x %02x %02x", ad7[7], ad7[8], ad7[9], ad7[10]);
                lcd.print(value);
                lcd.setCursor(0,1);
                sprintf(value,">  %02x %02x %02x %02x", ad8[7], ad8[8], ad8[9], ad8[10]);
                lcd.print(value);
                }else if(strcmp(A,ad7)!=0){
                  lcd.clear();
                sprintf(value,"  %02x %02x %02x %02x", ad7[7], ad7[8], ad7[9], ad7[10]);
                lcd.print(value);  
                   }else if(strcmp(A,ad8)!=0){
                      lcd.clear();
                      sprintf(value,">  %02x %02x %02x %02x", ad8[7], ad8[8], ad8[9], ad8[10]);
                      lcd.print(value);  
                   }else if(strcmp(A,ad5)!=0 && strcmp(A,ad6)!=0){
                lcd.clear();
                sprintf(value,"  %02x %02x %02x %02x", ad5[7], ad5[8], ad5[9], ad5[10]);
                lcd.print(value);
                lcd.setCursor(0,1);
                sprintf(value,">  %02x %02x %02x %02x", ad6[7], ad6[8], ad6[9], ad6[10]);
                lcd.print(value);
                }else if(strcmp(A,ad[2])!=0){
                  lcd.clear();
                sprintf(value,"  %02x %02x %02x %02x", ad5[7], ad5[8], ad5[9], ad5[10]);
                lcd.print(value);  
                   }else if(strcmp(A,ad[3])!=0){
                      lcd.clear();
                      sprintf(value,">  %02x %02x %02x %02x", ad6[7], ad6[8], ad6[9], ad6[10]);
                      lcd.print(value);  
                   }else{
                    lcd.clear();
                    lcd.print("Config Actuator");
                    aggiornaMenu();
                   }
                break;
          case 9:
                if(strcmp(A,ad9)!=0 & strcmp(A,ad10)!=0){
                lcd.clear();
                sprintf(value,">  %02x %02x %02x %02x", ad9[7], ad9[8], ad9[9], ad9[10]);
                lcd.print(value);
                lcd.setCursor(0,1);
                sprintf(value,"  %02x %02x %02x %02x", ad10[7], ad10[8], ad10[9], ad10[10]);
                lcd.print(value);
                }else if(strcmp(A,ad9)!=0){
                  lcd.clear();
                sprintf(value,">  %02x %02x %02x %02x", ad9[7], ad9[8], ad9[9], ad9[10]);
                lcd.print(value);  
                   }else if(strcmp(A,ad10)!=0){
                      lcd.clear();
                      sprintf(value,"  %02x %02x %02x %02x", ad10[7], ad10[8], ad10[9], ad10[10]);
                      lcd.print(value);  
                   }else if(strcmp(A,ad7)!=0 && strcmp(A,ad8)!=0){
                lcd.clear();
                sprintf(value,">  %02x %02x %02x %02x", ad7[7], ad7[8], ad7[9], ad7[10]);
                lcd.print(value);
                lcd.setCursor(0,1);
                sprintf(value,"  %02x %02x %02x %02x", ad8[7], ad8[8], ad8[9], ad8[10]);
                lcd.print(value);
                }else if(strcmp(A,ad5[2])!=0){
                  lcd.clear();
                sprintf(value,">  %02x %02x %02x %02x", ad7[7], ad7[8], ad7[9], ad7[10]);
                lcd.print(value);  
                   }else if(strcmp(A,ad3[3])!=0){
                      lcd.clear();
                      sprintf(value,"  %02x %02x %02x %02x", ad8[7], ad8[8], ad8[9], ad8[10]);
                      lcd.print(value);  
                   }else{
                    lcd.clear();
                    lcd.print("Config Actuator");
                    aggiornaMenu();
                   }
                break;
          case 10:
                if(strcmp(A,ad9)!=0 & strcmp(A,ad10)!=0){
                lcd.clear();
                sprintf(value,"  %02x %02x %02x %02x", ad9[7], ad9[8], ad9[9], ad9[10]);
                lcd.print(value);
                lcd.setCursor(0,1);
                sprintf(value,">  %02x %02x %02x %02x", ad10[7], ad10[8], ad10[9], ad10[10]);
                lcd.print(value);
                }else if(strcmp(A,ad9)!=0){
                  lcd.clear();
                sprintf(value,"  %02x %02x %02x %02x", ad9[7], ad9[8], ad9[9], ad9[10]);
                lcd.print(value);  
                   }else if(strcmp(A,ad10)!=0){
                      lcd.clear();
                      sprintf(value,">  %02x %02x %02x %02x", ad10[7], ad10[8], ad10[9], ad10[10]);
                      lcd.print(value);  
                   }else if(strcmp(A,ad7)!=0 && strcmp(A,ad8)!=0){
                lcd.clear();
                sprintf(value,"  %02x %02x %02x %02x", ad7[7], ad7[8], ad7[9], ad7[10]);
                lcd.print(value);
                lcd.setCursor(0,1);
                sprintf(value,">  %02x %02x %02x %02x", ad8[7], ad8[8], ad8[9], ad8[10]);
                lcd.print(value);
                }else if(strcmp(A,ad[2])!=0){
                  lcd.clear();
                sprintf(value,"  %02x %02x %02x %02x", ad7[7], ad7[8], ad7[9], ad7[10]);
                lcd.print(value);  
                   }else if(strcmp(A,ad[3])!=0){
                      lcd.clear();
                      sprintf(value,">  %02x %02x %02x %02x", ad8[7], ad8[8], ad8[9], ad8[10]);
                      lcd.print(value);  
                   }else{
                    lcd.clear();
                    lcd.print("Config Actuator");
                    aggiornaMenu();
                   }
                break;
          case 11:
                if(strcmp(A,ad11)!=0 & strcmp(A,ad12)!=0){
                lcd.clear();
                sprintf(value,">  %02x %02x %02x %02x", ad11[7], ad11[8], ad11[9], ad11[10]);
                lcd.print(value);
                lcd.setCursor(0,1);
                sprintf(value,"  %02x %02x %02x %02x", ad12[7], ad12[8], ad12[9], ad12[10]);
                lcd.print(value);
                }else if(strcmp(A,ad11)!=0){
                  lcd.clear();
                sprintf(value,">  %02x %02x %02x %02x", ad11[7], ad11[8], ad11[9], ad11[10]);
                lcd.print(value);  
                   }else if(strcmp(A,ad12)!=0){
                      lcd.clear();
                      sprintf(value,"  %02x %02x %02x %02x", ad12[7], ad12[8], ad12[9], ad12[10]);
                      lcd.print(value);  
                   }else if(strcmp(A,ad9)!=0 && strcmp(A,ad10)!=0){
                lcd.clear();
                sprintf(value,">  %02x %02x %02x %02x", ad9[7], ad9[8], ad9[9], ad9[10]);
                lcd.print(value);
                lcd.setCursor(0,1);
                sprintf(value,"  %02x %02x %02x %02x", ad10[7], ad10[8], ad10[9], ad10[10]);
                lcd.print(value);
                }else if(strcmp(A,ad5[2])!=0){
                  lcd.clear();
                sprintf(value,">  %02x %02x %02x %02x", ad9[7], ad9[8], ad9[9], ad9[10]);
                lcd.print(value);  
                   }else if(strcmp(A,ad3[3])!=0){
                      lcd.clear();
                      sprintf(value,"  %02x %02x %02x %02x", ad10[7], ad10[8], ad10[9], ad10[10]);
                      lcd.print(value);  
                   }else{
                    lcd.clear();
                    lcd.print("Config Actuator");
                    aggiornaMenu();
                   }
                break; 
          case 12:
                if(strcmp(A,ad11)!=0 & strcmp(A,ad12)!=0){
                lcd.clear();
                sprintf(value,"  %02x %02x %02x %02x", ad11[7], ad11[8], ad11[9], ad11[10]);
                lcd.print(value);
                lcd.setCursor(0,1);
                sprintf(value,">  %02x %02x %02x %02x", ad12[7], ad12[8], ad12[9], ad12[10]);
                lcd.print(value);
                }else if(strcmp(A,ad11)!=0){
                  lcd.clear();
                sprintf(value,"  %02x %02x %02x %02x", ad11[7], ad11[8], ad11[9], ad11[10]);
                lcd.print(value);  
                   }else if(strcmp(A,ad12)!=0){
                      lcd.clear();
                      sprintf(value,">  %02x %02x %02x %02x", ad12[7], ad12[8], ad12[9], ad12[10]);
                      lcd.print(value);  
                   }else if(strcmp(A,ad9)!=0 && strcmp(A,ad10)!=0){
                lcd.clear();
                sprintf(value,"  %02x %02x %02x %02x", ad9[7], ad9[8], ad9[9], ad9[10]);
                lcd.print(value);
                lcd.setCursor(0,1);
                sprintf(value,">  %02x %02x %02x %02x", ad10[7], ad10[8], ad10[9], ad10[10]);
                lcd.print(value);
                }else if(strcmp(A,ad[2])!=0){
                  lcd.clear();
                sprintf(value,"  %02x %02x %02x %02x", ad9[7], ad9[8], ad9[9], ad9[10]);
                lcd.print(value);  
                   }else if(strcmp(A,ad[3])!=0){
                      lcd.clear();
                      sprintf(value,">  %02x %02x %02x %02x", ad10[7], ad10[8], ad10[9], ad10[10]);
                      lcd.print(value);  
                   }else{
                    lcd.clear();
                    lcd.print("Config Actuator");
                    aggiornaMenu();
                   }
                break;   
          case 13:
                if(strcmp(A,ad13)!=0 & strcmp(A,ad14)!=0){
                lcd.clear();
                sprintf(value,">  %02x %02x %02x %02x", ad13[7], ad13[8], ad13[9], ad13[10]);
                lcd.print(value);
                lcd.setCursor(0,1);
                sprintf(value,"  %02x %02x %02x %02x", ad14[7], ad14[8], ad14[9], ad14[10]);
                lcd.print(value);
                }else if(strcmp(A,ad13)!=0){
                  lcd.clear();
                sprintf(value,">  %02x %02x %02x %02x", ad13[7], ad13[8], ad13[9], ad13[10]);
                lcd.print(value);  
                   }else if(strcmp(A,ad14)!=0){
                      lcd.clear();
                      sprintf(value,"  %02x %02x %02x %02x", ad14[7], ad14[8], ad14[9], ad14[10]);
                      lcd.print(value);  
                   }else if(strcmp(A,ad11)!=0 && strcmp(A,ad12)!=0){
                lcd.clear();
                sprintf(value,">  %02x %02x %02x %02x", ad11[7], ad11[8], ad11[9], ad11[10]);
                lcd.print(value);
                lcd.setCursor(0,1);
                sprintf(value,"  %02x %02x %02x %02x", ad12[7], ad12[8], ad12[9], ad12[10]);
                lcd.print(value);
                }else if(strcmp(A,ad5[2])!=0){
                  lcd.clear();
                sprintf(value,">  %02x %02x %02x %02x", ad11[7], ad11[8], ad11[9], ad11[10]);
                lcd.print(value);  
                   }else if(strcmp(A,ad3[3])!=0){
                      lcd.clear();
                      sprintf(value,"  %02x %02x %02x %02x", ad12[7], ad12[8], ad12[9], ad12[10]);
                      lcd.print(value);  
                   }else{
                    lcd.clear();
                    lcd.print("Config Actuator");
                    aggiornaMenu();
                   }
                break;
          case 14:
                if(strcmp(A,ad13)!=0 & strcmp(A,ad14)!=0){
                lcd.clear();
                sprintf(value,"  %02x %02x %02x %02x", ad13[7], ad13[8], ad13[9], ad13[10]);
                lcd.print(value);
                lcd.setCursor(0,1);
                sprintf(value,">  %02x %02x %02x %02x", ad14[7], ad14[8], ad14[9], ad14[10]);
                lcd.print(value);
                }else if(strcmp(A,ad13)!=0){
                  lcd.clear();
                sprintf(value,"  %02x %02x %02x %02x", ad13[7], ad13[8], ad13[9], ad13[10]);
                lcd.print(value);  
                   }else if(strcmp(A,ad14)!=0){
                      lcd.clear();
                      sprintf(value,">  %02x %02x %02x %02x", ad14[7], ad14[8], ad14[9], ad14[10]);
                      lcd.print(value);  
                   }else if(strcmp(A,ad11)!=0 && strcmp(A,ad12)!=0){
                lcd.clear();
                sprintf(value,"  %02x %02x %02x %02x", ad11[7], ad11[8], ad11[9], ad11[10]);
                lcd.print(value);
                lcd.setCursor(0,1);
                sprintf(value,">  %02x %02x %02x %02x", ad12[7], ad12[8], ad12[9], ad12[10]);
                lcd.print(value);
                }else if(strcmp(A,ad5[2])!=0){
                  lcd.clear();
                sprintf(value,"  %02x %02x %02x %02x", ad11[7], ad11[8], ad11[9], ad11[10]);
                lcd.print(value);  
                   }else if(strcmp(A,ad3[3])!=0){
                      lcd.clear();
                      sprintf(value,"> %02x %02x %02x %02x", ad12[7], ad12[8], ad12[9], ad12[10]);
                      lcd.print(value);  
                   }else{
                    lcd.clear();
                    lcd.print("Config Actuator");
                    aggiornaMenu();
                   }
                break;      
          case 15:
              lcd.clear();
              lcd.clear();
              sprintf(value,">  %02x %02x %02x %02x", A[7], A[8], A[9], A[10]);
              lcd.print(value);
              lcd.setCursor(0,1);
              lcd.print(" ");
              break;
       }
       char keys = kpd.getKey();
       switch(keys){
        case '*':
                  aggiornaMenu();
                  break;
       }
       
}

void comandi(){  //comandi da mandare via keypad 
  char keys = kpd.getKey();
  if(keys == 0)return;
  switch(keys){  
    case '1' :
            lcd.clear();
            seriale.write("#0AB");
            seriale.flush();
            lcd.print("Control Output1");
            break; 
    case '2' :
            lcd.clear();
            seriale.write("#1CD");
            seriale.flush();
            lcd.print("Control Output2");
            break;
    case '3' :
            lcd.clear();
            seriale.write("#2BD");
            seriale.flush();
            lcd.print("Control Output3");
            break;
    case '4' :
            lcd.clear();
            seriale.write("#3CF");
            seriale.flush();
            lcd.print("Control Output4");
            break;
    case '5' :
            lcd.clear();
            seriale.write("#?I"); 
            seriale.flush();
            dato = seriale.readString();
            lcd.print("Input State:");
            lcd.print(dato);
            break;
    case '6' : 
            lcd.clear();
            seriale.write("#?O");
            seriale.flush();
            dato = seriale.readString();
            lcd.print("OutputState:");
            lcd.print(dato);
            break;
    case '7' :
            lcd.clear();
            seriale.write("#?B");
            seriale.flush();
            dato = seriale.readString();
            lcd.print("BatteryL : ");
            lcd.print(dato);
            break;
     case '8' :
           lcd.clear();
           seriale.write("#?T");
           seriale.flush();
           dato = seriale.readString();
           lcd.print("Temperature : ");
           lcd.print(dato);
           break;
     case '9' :
           lcd.clear();
           seriale.write("#011");
           delay(100);
           seriale.write("#021");
           delay(200);
           seriale.write("#031");
           delay(300);
           seriale.write("#041");
           seriale.flush();
           lcd.print("All Output ON");
           break;
      case '0' :
           lcd.clear();
           seriale.write("#010");
           delay(100);
           seriale.write("#020");
           delay(200);
           seriale.write("#030");
           delay(300);
           seriale.write("#040");
           seriale.flush();
           lcd.print("All Output OFF");
           break; 
      case '*':
            check = 0;
            aggiornaMenu();
            break;
  }
}

void aggiornaMenu(){  //aggiornamento menu con scroll con tasti A e B
  switch (menu) {
    case 1:
      lcd.clear();
      lcd.print(">Config Actuator");
      lcd.setCursor(0,1);
      lcd.print("Remote Control");
      break;
    case 2:
      lcd.clear();
      lcd.print("Config Actuator");
      lcd.setCursor(0,1);
      lcd.print(">Remote Control");
      break;
    case 3:
      lcd.clear();
      lcd.print(">Search Actuator");
      lcd.setCursor(0,1);
      lcd.print("Choose Actuator");
      break;
    case 4:
      lcd.clear();
      lcd.print("Search Actuator");
      lcd.setCursor(0,1);
      lcd.print(">Choose Actuator");
      break;
  }
}

void configure(){ //COMANDI DI CONFIGURAZIONE DA ESEGUIRE QUANDO TI TROVI ALL'INTERNO DI AZIONE1
  char keys = kpd.getKey();
  if(keys){
    switch(keys){
      case '1': //incrementa la cella 3 del vettore
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Remote Address :");
          lcd.setCursor(1,2);
          sprintf(value,"  %02x %02x %02x %02x", chose[0][7], chose[0][8], chose[0][9], chose[0][10]);
          lcd.print(value);
          delay(1500);
          lcd.clear();
          seriale.write(chose[0], 13);
          EEPROM.write(0,chose[0][7]);
          EEPROM.write(1,chose[0][8]);
          EEPROM.write(2,chose[0][9]);
          EEPROM.write(3,chose[0][10]);
          lcd.setCursor(0,0);
          lcd.print("    Device    ");
          lcd.setCursor(1,2);
          lcd.print("  Configured  ");
          delay(2000);
          lcd.clear();
          check = 0;
          aggiornaMenu();          
          break;
      case '2' : //incrementa la cella 4 del vettore
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Remote Address :");
          lcd.setCursor(1,2);
          sprintf(value,"  %02x %02x %02x %02x", chose[1][7], chose[1][8], chose[1][9], chose[1][10]);
          lcd.print(value);
          delay(1500);
          lcd.clear();
          seriale.write(chose[1], 13);
          EEPROM.write(4,chose[1][7]);
          EEPROM.write(5,chose[1][8]);
          EEPROM.write(6,chose[1][9]);
          EEPROM.write(7,chose[1][10]);
          lcd.setCursor(0,0);
          lcd.print("    Device    ");
          lcd.setCursor(1,2);
          lcd.print("  Configured  ");
          delay(2000);
          lcd.clear();
          check = 0;
          aggiornaMenu();
          break;
      case '3' : //incrementa la cella 5 del vettore
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Remote Address :");
          lcd.setCursor(1,2);
          sprintf(value,"  %02x %02x %02x %02x", chose[2][7], chose[2][8], chose[2][9], chose[2][10]);
          lcd.print(value);
          delay(1500);
          lcd.clear();
          seriale.write(chose[2], 13);
          EEPROM.write(8,chose[2][7]);
          EEPROM.write(9,chose[2][8]);
          EEPROM.write(10,chose[2][9]);
          EEPROM.write(11,chose[2][10]);
          lcd.setCursor(0,0);
          lcd.print("    Device    ");
          lcd.setCursor(1,2);
          lcd.print("  Configured  ");
          delay(2000);
          lcd.clear();
          check = 0;
          aggiornaMenu();          
          break;
      case '4' : //incrementa la cella 6 del vettore
          lcd.clear();          
          lcd.setCursor(0,0);
          lcd.print("Remote Address :");
          lcd.setCursor(1,2);
          sprintf(value,"  %02x %02x %02x %02x", chose[3][7], chose[3][8], chose[3][9], chose[3][10]);
          lcd.print(value);
          delay(1500);
          lcd.clear();
          seriale.write(chose[3], 13);
          EEPROM.write(12,chose[3][7]);
          EEPROM.write(13,chose[3][8]);
          EEPROM.write(14,chose[3][9]);
          EEPROM.write(15,chose[3][10]);
          lcd.setCursor(0,0);
          lcd.print("    Device    ");
          lcd.setCursor(1,2);
          lcd.print("  Configured  ");
          delay(2000);
          lcd.clear();
          check = 0;
          aggiornaMenu();
          break;
      case '5' :
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Remote Address :");
          lcd.setCursor(1,2);
          sprintf(value, " %02x %02x %02x %02x", chose[4][7], chose[4][8], chose[4][9], chose[4][10]);
          lcd.print(value);
          delay(1500);
          lcd.clear();
          seriale.write(chose[4], 13);
          EEPROM.write(16,chose[4][7]); 
          EEPROM.write(17,chose[4][8]);  
          EEPROM.write(18,chose[4][9]);  
          EEPROM.write(19,chose[4][10]);
          lcd.setCursor(0,0);
          lcd.print("    Device    ");
          lcd.setCursor(1,2);
          lcd.print("  Configured  ");
          delay(2000);
          lcd.clear();
          check = 0;
          aggiornaMenu();
          break;
      case '6' :
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Remote Address :");
          lcd.setCursor(1,2);
          sprintf(value, " %02x %02x %02x %02x", chose[5][7], chose[5][8], chose[5][9], chose[5][10]);
          lcd.print(value);
          delay(1500);
          lcd.clear();
          seriale.write(chose[5], 13);
          EEPROM.write(20,chose[5][7]); 
          EEPROM.write(21,chose[5][8]);  
          EEPROM.write(22,chose[5][9]);  
          EEPROM.write(23,chose[5][10]);
          lcd.setCursor(0,0);
          lcd.print("    Device    ");
          lcd.setCursor(1,2);
          lcd.print("  Configured  ");
          delay(2000);
          lcd.clear();
          check = 0;
          aggiornaMenu();
          break;
      case '7' :
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Remote Address :");
          lcd.setCursor(1,2);
          sprintf(value, " %02x %02x %02x %02x", chose[6][7], chose[6][8], chose[6][9], chose[6][10]);
          lcd.print(value);
          delay(1500);
          lcd.clear();
          seriale.write(chose[6], 13);
          EEPROM.write(24,chose[6][7]); 
          EEPROM.write(25,chose[6][8]);  
          EEPROM.write(26,chose[6][9]);  
          EEPROM.write(27,chose[6][10]);
          lcd.setCursor(0,0);
          lcd.print("    Device    ");
          lcd.setCursor(1,2);
          lcd.print("  Configured  ");
          delay(2000);
          lcd.clear();
          check = 0;
          aggiornaMenu();
          break;
      case '8' :
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Remote Address :");
          lcd.setCursor(1,2);
          sprintf(value, " %02x %02x %02x %02x", chose[7][7], chose[7][8], chose[7][9], chose[7][10]);
          lcd.print(value);
          delay(1500);
          lcd.clear();
          seriale.write(chose[7], 13);
          EEPROM.write(28,chose[7][7]); 
          EEPROM.write(29,chose[7][8]);  
          EEPROM.write(30,chose[7][9]);  
          EEPROM.write(31,chose[7][10]);
          lcd.setCursor(0,0);
          lcd.print("    Device    ");
          lcd.setCursor(1,2);
          lcd.print("  Configured  ");
          delay(2000);
          lcd.clear();
          check = 0;
          aggiornaMenu();
          break;
      case '9' : 
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Remote Address :");
          lcd.setCursor(1,2);
          sprintf(value, " %02x %02x %02x %02x", chose[8][7], chose[8][8], chose[8][9], chose[8][10]);
          lcd.print(value);
          delay(1500);
          lcd.clear();
          seriale.write(chose[8], 13);
          EEPROM.write(32,chose[8][7]); 
          EEPROM.write(33,chose[8][8]);  
          EEPROM.write(34,chose[8][9]);  
          EEPROM.write(35,chose[8][10]);
          lcd.setCursor(0,0);
          lcd.print("    Device    ");
          lcd.setCursor(1,2);
          lcd.print("  Configured  ");
          delay(2000);
          lcd.clear();
          check = 0;
          aggiornaMenu();
          break;
      case 'A' :
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Remote Address :");
          lcd.setCursor(1,2);
          sprintf(value, " %02x %02x %02x %02x", chose[9][7], chose[9][8], chose[9][9], chose[9][10]);
          lcd.print(value);
          delay(1500);
          lcd.clear();
          seriale.write(chose[9], 13);
          EEPROM.write(36,chose[9][7]); 
          EEPROM.write(37,chose[9][8]);  
          EEPROM.write(38,chose[9][9]);  
          EEPROM.write(39,chose[9][10]);
          lcd.setCursor(0,0);
          lcd.print("    Device    ");
          lcd.setCursor(1,2);
          lcd.print("  Configured  ");
          delay(2000);
          lcd.clear();
          check = 0;
          aggiornaMenu();
          break;
      case 'B' :
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Remote Address :");
          lcd.setCursor(1,2);
          sprintf(value, " %02x %02x %02x %02x", chose[10][7], chose[10][8], chose[10][9], chose[10][10]);
          lcd.print(value);
          delay(1500);
          lcd.clear();
          seriale.write(chose[10], 13);
          EEPROM.write(40,chose[10][7]); 
          EEPROM.write(41,chose[10][8]);  
          EEPROM.write(42,chose[10][9]);  
          EEPROM.write(43,chose[10][10]);
          lcd.setCursor(0,0);
          lcd.print("    Device    ");
          lcd.setCursor(1,2);
          lcd.print("  Configured  ");
          delay(2000);
          lcd.clear();
          check = 0;
          aggiornaMenu();
          break;
      case 'C' :
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Remote Address :");
          lcd.setCursor(1,2);
          sprintf(value, " %02x %02x %02x %02x", chose[11][7], chose[11][8], chose[11][9], chose[11][10]);
          lcd.print(value);
          delay(1500);
          lcd.clear();
          seriale.write(chose[11], 13);
          EEPROM.write(44,chose[11][7]); 
          EEPROM.write(45,chose[11][8]);  
          EEPROM.write(46,chose[11][9]);  
          EEPROM.write(47,chose[11][10]);
          lcd.setCursor(0,0);
          lcd.print("    Device    ");
          lcd.setCursor(1,2);
          lcd.print("  Configured  ");
          delay(2000);
          lcd.clear();
          check = 0;
          aggiornaMenu();
          break;
      case 'D' :
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Remote Address :");
          lcd.setCursor(1,2);
          sprintf(value, " %02x %02x %02x %02x", chose[12][7], chose[12][8], chose[12][9], chose[12][10]);
          lcd.print(value);
          delay(1500);
          lcd.clear();
          seriale.write(chose[12], 13);
          EEPROM.write(48,chose[12][7]); 
          EEPROM.write(49,chose[12][8]);  
          EEPROM.write(50,chose[12][9]);  
          EEPROM.write(51,chose[12][10]);
          lcd.setCursor(0,0);
          lcd.print("    Device    ");
          lcd.setCursor(1,2);
          lcd.print("  Configured  ");
          delay(2000);
          lcd.clear();
          check = 0;
          aggiornaMenu();
          break;
      case '0':
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Remote Address :");
          lcd.setCursor(1,2);
          sprintf(value, " %02x %02x %02x %02x", chose[13][7], chose[13][8], chose[13][9], chose[13][10]);
          lcd.print(value);
          delay(1500);
          lcd.clear();
          seriale.write(chose[13], 13);
          EEPROM.write(52,chose[13][7]); 
          EEPROM.write(53,chose[13][8]);  
          EEPROM.write(54,chose[13][9]);  
          EEPROM.write(55,chose[13][10]);
          lcd.setCursor(0,0);
          lcd.print("    Device    ");
          lcd.setCursor(1,2);
          lcd.print("  Configured  ");
          delay(2000);
          lcd.clear();
          check = 0;
          aggiornaMenu();
          break;    
      case '*':
            check = 0;
            aggiornaMenu();
      break;
    }
  }
}
