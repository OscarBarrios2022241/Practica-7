//Oscar Jose Barrios Cotom - 2022241

#include <Wire.h>//Para comunicarse con I2C
#include <LiquidCrystal_I2C.h>//Para comunicarse con la LCD por medio de I2C
#include <Keypad.h>//Libreria para el teclado
#include <Servo.h>//Libreria para el servo
#include <RTClib.h>//Libreria para el Reloj de tiempo

const int SERVO_PIN = 10;//Pin para la señal del servo
const int CORRECT_PASSWORD = 7777;//Contraseña
int password = 0;
bool doorLocked = true;
lk3
RTC_DS1307 rtc;//Se denomina el nombre rtc para el Reloj de Tiempo
LiquidCrystal_I2C lcd(0x27, 16, 2);//Para definir como es la LCD
Servo servomotor;

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {//Filas y columnas del teclado matricial
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};


byte rowPins[ROWS] = {9, 8, 7, 6};//Filas y columnas del teclado matricial
byte colPins[COLS] = {5, 4, 3, 2};//Filas y columnas del teclado matricial

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);


void setup() {
  Wire.begin();//Se inicia la comunicacion con I2C
  lcd.init();//Se inicia la pantalla LCD
  lcd.backlight();//Se enciende la luz trasera del LCD
  servomotor.attach(SERVO_PIN);//Servo al pin 10
  rtc.begin();//Se inicia el modulo RTC
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));//se confugura Hora y fecha actual
  lcd.setCursor(0, 0);//Fila 0 y columna 0
  lcd.print("Ingresa contra:");//Se imprime en esto en la LCD en 0 y 0
}

void loop() {
  DateTime now = rtc.now();//Hora actual
  lcd.setCursor(0, 1);//Fila 0 y columna 1 del LCD
  lcd.print(now.timestamp(DateTime::TIMESTAMP_TIME));

  if (doorLocked) {//se espera que ingrese la contraseña
    lcd.setCursor(0, 0);
    lcd.print("Ingresa contra:");
    servomotor.write(0);

    char key = keypad.getKey();//Se leen las teclas presionadas
    if (key != NO_KEY) {//Comprueba si se presionaron las teclas
      password = password * 10 + (key - '0');//Cuando se presiona una tecla se va agregando a la palabra password
      if (password == CORRECT_PASSWORD) {//Si la contra es correcta
        lcd.setCursor(0, 1);
        lcd.print("Correcta");
        lcd.setCursor(0, 0);
        lcd.print("Puerta abierta");
        servomotor.write(90);//Gira a 90° si es correcta la contra
        delay(2000);//Tiempo
        password = 0;//Se reinicia la contra
        doorLocked = false;//Desbloquea la puerta
      } else if (password > 9999) {//Si se ingresa una contra incorrecta
        lcd.clear();
        lcd.print("incorrecta");
        delay(1000);
        servomotor.write(0);//No gira nada si la contra esta mala
        password = 0;//Se reinicia la contra
      }
    }
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Puerta abierta ");
    servomotor.write(90);
    delay(1000);
    servomotor.write(0);
    doorLocked = true;
  }
}
