#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
//Temp sesnor
const int tmpPin = A0;
//Motor Driver
int Enable = 13;
int inp3 = 8;
int inp4 = 9;
int yellowLed = 10; //suppose a heating light

//Ldr sensor
const int bulbPin = 7;
const int ldrPin = A1;
//PIR sensor
int inputPin = 6;               // choose the input pin (for PIR sensor)
int pirState = LOW;             // we start, assuming no motion detected
int val = 0;                 
void setup() {
  //PIR 
  pinMode(inputPin, INPUT);
  lcd.begin(16, 2);
  Serial.begin(9600);
  //temp sensor
  pinMode(tmpPin, INPUT);
  //Motor Driver
  pinMode(Enable, OUTPUT);
  pinMode(inp3, OUTPUT);
  pinMode(inp4, OUTPUT);
  pinMode(yellowLed, OUTPUT);
  //ldrsensor
  pinMode(bulbPin, OUTPUT);
  pinMode(ldrPin, INPUT);
  Serial.begin(9600);
}
 
void loop()
  {
  val = digitalRead(inputPin);  // read input value
  if (val == HIGH)
    {
    digitalWrite(bulbPin, HIGH);  // turn LED ON
    if (pirState == LOW) 
      {
        int ldrStatus = analogRead(ldrPin);
        int Temp = analogRead(tmpPin);
        light(ldrStatus);
        temperature(Temp);
        delay(1000); // 1 seconds delay
        pirState = HIGH;
      }
    } 
    else 
    {
      digitalWrite(bulbPin, LOW); // turn LED OFF
      if (pirState == HIGH)
      {
        digitalWrite(bulbPin, LOW);
        analogWrite(Enable,0);
        pirState = LOW;
      }
    }
  }
//light related code
void light(int ldrStatus){
  if (ldrStatus <= 300) 
  {
    digitalWrite(bulbPin, HIGH);
    Serial.print("Its DARK, Turn on the Bulb \n");
  lcd.setCursor(10, 1);
    lcd.print("L= ON ");
  } 
  else 
  {
    digitalWrite(bulbPin, LOW);
    Serial.print("Its BRIGHT, Turn off the Bulb \n");
    lcd.setCursor(10, 1);
    lcd.print("L= OFF");
  }
}
//temperature related code
void temperature(int Temp){
  float voltage = Temp * 5.0; //converting reading to voltage
  voltage /= 1024.0; 
  float c = (voltage - .5) * 100; //conveting voltage to celsius
  float f = (c * 9 / 5 + 32); //converting calsius to farenheit 
  Serial.println(c);
  lcd.setCursor(0, 0);
  lcd.print("T=");
  lcd.setCursor(3, 0);
  lcd.print(c);
  lcd.setCursor(8, 0);
  lcd.print("C");
  lcd.setCursor(10, 0);
  lcd.print(f);
  lcd.setCursor(15, 0);
  lcd.print("F");
  //temp sensor and fan
  digitalWrite(inp3, LOW);  // To drive the fan in a direction
  digitalWrite(inp4, HIGH);
  lcd.setCursor(0, 1);
  if(c < 15) {                 // If the temperature less than 25
      analogWrite(Enable,0);  //Fann OFF
      lcd.print("Heat  ON");
      digitalWrite(yellowLed, HIGH); //heating light
      delay(10);
      Serial.print("Heating Light is on");
    }
    else if(c>=15 & c<20) {      // If the temperature is between 20 & 30
      analogWrite(Enable, 0);  // 50% of maximum cycle value (255). 
      lcd.print("Fan  OFF");
      digitalWrite(yellowLed, LOW);
      delay(10);
    }
    else if(c>=20 & c<30) {      // If the temperature is between 20 & 30
      analogWrite(Enable, 128);  // 50% of maximum cycle value (255). 
      lcd.print("Fan: 50%");
      digitalWrite(yellowLed, LOW);
      delay(10);
    }
    else if(c>=30 & c<35) {      // If the temperature is between 30 & 35
      analogWrite(Enable, 178);        // 70% of maximum cycle value (255).
      lcd.print("Fan: 70%");
      digitalWrite(yellowLed, LOW);
      delay(10);
    }
    else if(c>=35 & c<45) {      // If the temperature is between 35 & 40
      analogWrite(Enable, 216);        // 80% of maximum cycle value (255).
      lcd.print("Fan: 85%");
      digitalWrite(yellowLed, LOW);
      delay(10);
    }
    else if(c>=45) {                // If the temperature is above 35
      analogWrite(Enable, 255);        // 100%
      lcd.print("Fan:100%");
      digitalWrite(yellowLed, LOW);
        delay(10);
    }
}
