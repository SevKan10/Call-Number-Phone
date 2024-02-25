#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

byte degree[8] = {
  0B00000,
  0B00000,
  0B00000,
  0B00000,
  0B00000,
  0B00000,
  0B00000,
  0B11111
};

int UP = 15;
int DOWN = 4;
int SW = 5;
int CALL = 18;
int BUZZ = 2;
int phone[10] = { 0, 0, 0, 0, 0, 0, 0, 0 };
int num1 = 0;
int num2 = 1;
int i;
String numberPhone = "";

void setup() {
  Serial.begin(9600);

  Serial2.begin(115200, SERIAL_8N1, 16, 17);
  Serial2.println("AT+CMGF=1"); delay(50);
  Serial2.println("AT+CNMI=2,2,0,0,0"); delay(50);
  Serial2.println("AT+CMGL=\"REC UNREAD\""); delay(50);
  Serial2.println("AT+CMGD=1,4"); delay(50);

  Wire.begin(23, 22);
  lcd.init();
  lcd.backlight();
  lcd.createChar(1, degree);

  pinMode(UP, INPUT_PULLUP);
  pinMode(DOWN, INPUT_PULLUP);
  pinMode(SW, INPUT_PULLUP);
  pinMode(CALL, INPUT_PULLUP);
  pinMode(BUZZ, OUTPUT);

  for(int i = 10; i >-1 ;i-- )
  {  
    lcd.setCursor(7,0);
    lcd.print(i);
    lcd.print(" ");
    lcd.setCursor(3,1);
    lcd.print("WATTING...");
    delay(1000);
  }
  lcd.clear();
}

void loop() {

  lcd.setCursor(11,0);
  lcd.print("Enter");
  lcd.setCursor(10,1);
  lcd.print("number");

  numPlus();
  numMinus();
  changeCursor();
  callNumber();
  
  lcd.setCursor(0, 0);
  lcd.print(phone[0]); lcd.print(phone[1]); lcd.print(phone[2]); lcd.print(phone[3]); lcd.print(phone[4]); 
  lcd.print(phone[5]); lcd.print(phone[6]); lcd.print(phone[7]); lcd.print(phone[8]); lcd.print(phone[9]);
  lcd.print("       ");

  lcd.setCursor(num1, 0);
  lcd.print(phone[num1]);
  lcd.setCursor(num1, 1);
  lcd.write(1);
}

void numPlus() 
{
  static bool upButtonState = 1;
  if (digitalRead(UP) == 0 && upButtonState == 1) 
  {
    Ring();
    phone[num1]++;
    if (phone[num1] > 9) {phone[num1] = 0;}
  }
  upButtonState = digitalRead(UP);
}

void numMinus() 
{
  static bool downButtonState = 1;
  if (digitalRead(DOWN) == 0 && downButtonState == 1)
  {
    Ring();
    phone[num1]--;
    if (phone[num1] < 0) {phone[num1] = 9;}
  }
  downButtonState = digitalRead(DOWN);
}
void changeCursor() 
{
  static bool swButtonState = 1;
  if (digitalRead(SW) == 0 && swButtonState == 1) 
  {
    lcd.clear();
    Ring();
    num1++;
    if (num1 > 9) {num1 = 0;}
    lcd.setCursor(0, 0);
    lcd.print(phone[0]); lcd.print(phone[1]); lcd.print(phone[2]); lcd.print(phone[3]); lcd.print(phone[4]);
    lcd.print(phone[5]); lcd.print(phone[6]); lcd.print(phone[7]); lcd.print(phone[8]); lcd.print(phone[9]);
    lcd.print("       ");
    lcd.setCursor(num1, 1);
    lcd.write(1);
  }
  swButtonState = digitalRead(SW);
}
void callNumber() 
{
  bool currentcallButtonState = digitalRead(CALL);
  if (currentcallButtonState == 0 ) 
  {
    for (int i = 0; i < 10; i++) {numberPhone += String(phone[i]); delay(50); Serial.println(numberPhone);}
    if (numberPhone.length() == 10) 
    {
      lcd.clear();
      lcd.setCursor(3, 0);
      lcd.print("CALLING...");
      lcd.setCursor(3, 1);
      lcd.print(numberPhone);

      Serial2.println("AT+CMGF=1"); delay(500);
      Serial2.println("AT+CMGS=\"" + numberPhone + "\"\r"); delay(500);
      Serial2.print("Help me!");
      Serial2.println((char)26);
      delay(5000);

      Serial2.print(F("ATD"));
      Serial2.print(numberPhone);
      Serial2.print(F(";\r\n"));
      ringCall();
      Serial2.print(F("ATH"));
      Serial2.print(F(";\r\n"));
      delay(500);

      numberPhone = "";
      lcd.clear();
    } 
    else 
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("ERROR:");
      lcd.setCursor(0, 1);
      lcd.print("Invalid phone");
      delay(2000);
      lcd.clear();
    }
  }
}

void Ring(){digitalWrite(BUZZ, 1); delay(50); digitalWrite(BUZZ, 0);}

void ringCall(){for(int i = 0; i<300; i++){digitalWrite(BUZZ,1); delay(50);digitalWrite(BUZZ, 0); delay(50);}}