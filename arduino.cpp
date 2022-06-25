#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SFE_BMP180.h>
int smokeA0 = 1;
int buzzer = 10;
LiquidCrystal_I2C lcd(0x27,16,2);
SFE_BMP180 pressure;
 
void setup()
{
  lcd.begin();
  pinMode(smokeA0, INPUT);
  pinMode(buzzer, OUTPUT);
  Serial.begin(9600);
  if (pressure.begin())
    Serial.println("BMP180 init success");
  else
  {
    Serial.println("BMP180 init fail");
    while (1);
  }
  delay(1500);
}
 
void loop()
{
  char status;
  double T, P;
  int gas;
  lcd.setBacklight(HIGH);
  status = pressure.startTemperature();
  gas = analogRead(smokeA0);
  Serial.println(gas);

  if(gas > 300){
    lcd.clear();
    tone(buzzer, 1000, 200); // inicia o buzzer
    lcd.setCursor(0,0);
    lcd.print("GAS DETECTADO");
    lcd.setCursor(0,1);
    lcd.print("CUIDADO!");
    delay(500);
  }
  else if(status == 0){
    noTone(buzzer);
    lcd.clear();
    lcd.print("SENSOR ");
    lcd.setCursor(0,1);
    lcd.print("NAO ENCONTRADO");
    delay(500);
  }
  else{
    noTone(buzzer);
    lcd.clear();
    delay(status);

    status = pressure.getTemperature(T);
    lcd.setCursor(0,0);
    lcd.print("T: ");
    lcd.print(T);
    lcd.print(" C");

    status = pressure.startPressure(3);
    delay(status);
    status = pressure.getPressure(P,T);
    lcd.setCursor(0,1);
    lcd.print("P: ");
    lcd.print(P);
    lcd.print(" hPa");
    delay(1000);
  }
  delay(1000);
}