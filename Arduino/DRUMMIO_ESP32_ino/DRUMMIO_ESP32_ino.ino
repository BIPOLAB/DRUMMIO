#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Encoder.h>
#include <BLEMidi.h>
#include <Adafruit_NeoPixel.h>

// Definición de constantes
#define BL_PIN_LED 33

//ENCODER PINS
#define ENC_PIN_A 17
#define ENC_PIN_B 16
#define ENC_SWITCH_PIN 19

//LCD DIR & PIN
#define I2C_ADDR 0x27
#define LCD_COLUMNS 16
#define LCD_LINES 2

//DEFINE NEOPIXELS QTY
#define NUMPIXELS 8 //
//DEFINE MUX PINS
#define MUXS0 12   // MUX S0
#define MUXS1 14   // MUX S1
#define MUXS2 27   // MUX S2
#define MUXS3 26   // MUX S3
#define MUXSIG 33  // MUX S3 / ESP PIN ADC1_0
#define NP_PIN 13  // NEOPIXEL PIN
//DEFINE DEBOUNCE FOR SWITCH
#define DEBOUNCE_DELAY 20  // Delay for debouncing the switch (milliseconds)


//Objetos
LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLUMNS, LCD_LINES);
ESP32Encoder encoder;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, NP_PIN, NEO_GRB + NEO_KHZ800);


//Definicion de Variables Locales
int midiChannel = 0;
unsigned long lastEncoderActivityTime = 0;
int bleConnected;
int bleDisconnected = 1;
const char *menuOptions[] = {
  "PAD1",
  "PAD2",
  "PAD3",
  "PAD4",
  "InPut 1",
  "InPut 2",
  "InPut 3",
  "InPut 4"
};

const char *subMenuOptions[][8] = {
  {"Sensitivity", "Threshold1", "Scantime", "Masktime", "Curvetype", "Note", "Mode", "Back"},
  {"Sensitivity", "Threshold1", "Scantime", "Masktime", "Curvetype", "Note", "Mode", "Back"},
  {"Sensitivity", "Threshold1", "Scantime", "Masktime", "Curvetype", "Note", "Mode", "Back"},
  {"Sensitivity", "Threshold1", "Scantime", "Masktime", "Curvetype", "Note", "Mode", "Back"},
  {"Sensitivity", "Threshold1", "Scantime", "Masktime", "Curvetype", "Note", "Mode", "Back"},
  {"Sensitivity", "Threshold1", "Scantime", "Masktime", "Curvetype", "Note", "Mode", "Back"},
  {"Sensitivity", "Threshold1", "Scantime", "Masktime", "Curvetype", "Note", "Mode", "Back"},
  {"Sensitivity", "Threshold1", "Scantime", "Masktime", "Curvetype", "Note", "Mode", "Back"},
};




// Definición de enumeraciones para los distintos estados del menu
enum State {
  STATE_HOME_SCREEN,
  STATE_MAIN_MENU,
  STATE_SUB_MENU
};

//CURRENT STATE FOR DISPLAY
State currentState = STATE_HOME_SCREEN;

int currentMenuOption = 0;
int currentSubMenuOption = 0;
bool switchPressed = false;
unsigned long lastSwitchTime = 0;

void setup() {
  
  BLEMidiServer.begin("DrummIO MIDI BT");
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  pinMode(ENC_PIN_A, INPUT_PULLUP);
  pinMode(ENC_PIN_B, INPUT_PULLUP);
  pinMode(ENC_SWITCH_PIN, INPUT_PULLUP);
  pinMode(BL_PIN_LED,OUTPUT);

  
  encoder.attachHalfQuad(ENC_PIN_A, ENC_PIN_B);
  displayHomeScreen();
}

void loop() {
  // Obtención de la hora actual
  unsigned long currentTime = millis();

  bleStatus();
  switch (currentState) {
    case STATE_HOME_SCREEN:
      if (digitalRead(ENC_SWITCH_PIN) == LOW && !switchPressed && millis() - lastSwitchTime > DEBOUNCE_DELAY) {
        switchPressed = true;
        currentState = STATE_MAIN_MENU;
        displayMainMenu();
      }
      break;

    case STATE_MAIN_MENU:
      handleMainMenuNavigation();
      break;

    case STATE_SUB_MENU:
      handleSubMenuNavigation();
      break;
  }

  // Reset switchPressed and update lastSwitchTime when switch is released
  if (digitalRead(ENC_SWITCH_PIN) == HIGH) {
    switchPressed = false;
    lastSwitchTime = millis();
  }
}

void handleMainMenuNavigation() {
  long encoderValueMainMenu = encoder.getCount() / 2;  // Leer el encoder solo para el menú principal
  unsigned long currentTime = millis();  // Obtener la hora actual

  // Verificar la inactividad y volver a la pantalla de inicio si no hay interacción en el encoder
  if (encoderValueMainMenu == 0) {
    if (currentTime - lastEncoderActivityTime > 5000) {
      checkForIdle();
      lastEncoderActivityTime = currentTime;
    }
  } else {
    lastEncoderActivityTime = currentTime;
  }

  if (encoderValueMainMenu != 0) {
    currentMenuOption = (currentMenuOption + encoderValueMainMenu) % 8;
    if (currentMenuOption < 0)
      currentMenuOption += 8;
    displayMainMenu();
    encoder.clearCount();
  }

  if (digitalRead(ENC_SWITCH_PIN) == LOW && !switchPressed && millis() - lastSwitchTime > DEBOUNCE_DELAY) {
    switchPressed = true;
    currentState = STATE_SUB_MENU;
    currentSubMenuOption = 0;  // Establecer la primera opción del submenú
    displaySubMenu(menuOptions[currentMenuOption], subMenuOptions[currentMenuOption][currentSubMenuOption]);
  }
}


void handleSubMenuNavigation() {
  long encoderValueSubMenu = encoder.getCount() / 2;  // Leer el encoder solo para el submenú
  unsigned long currentTime = millis();  // Obtener la hora actual

  // Verificar la inactividad y volver a la pantalla de inicio si no hay interacción en el encoder
  if (encoderValueSubMenu == 0) {
    if (currentTime - lastEncoderActivityTime > 5000) {
      checkForIdle();
      lastEncoderActivityTime = currentTime;
    }
  } else {
    lastEncoderActivityTime = currentTime;
  }

  if (encoderValueSubMenu != 0) {
    // Si no es la opción "Back", permitir la navegación en el submenú
    if (strcmp(subMenuOptions[currentMenuOption][currentSubMenuOption], "Back") != 0) {
      currentSubMenuOption = (currentSubMenuOption + encoderValueSubMenu) % 8;
      if (currentSubMenuOption < 0)
      
        currentSubMenuOption += 8;
        
      displaySubMenu(menuOptions[currentMenuOption], subMenuOptions[currentMenuOption][currentSubMenuOption]);
      encoder.clearCount();
    }
  }

  // Control de ingreso al menú principal solo si la posición es "Back"
  if (digitalRead(ENC_SWITCH_PIN) == LOW && !switchPressed && millis() - lastSwitchTime > DEBOUNCE_DELAY) {
    if (strcmp(subMenuOptions[currentMenuOption][currentSubMenuOption], "Back") == 0) {
      // Solo si la posición actual es "Back", regresar al menú principal
      switchPressed = true;
      currentState = STATE_MAIN_MENU;
      displayMainMenu();
    }
  }
}

void displayHomeScreen() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("DrummIO");
}

void displayMainMenu() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Menu principal");
  lcd.setCursor(0, 1);
  lcd.print(menuOptions[currentMenuOption]);
  checkForIdle();
}

void displaySubMenu(const char *mainMenuOption, const char *subMenuOption) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(mainMenuOption);
  lcd.setCursor(0, 1);
  lcd.print(subMenuOptions[currentMenuOption][currentSubMenuOption]);
  checkForIdle();
}

void checkForIdle() {
  // Obtención de la hora actual
  unsigned long currentTime = millis();

  // Comprobación de si ha pasado el tiempo de inactividad
  if (currentTime - lastEncoderActivityTime > 15000) {
    // Reinicio del temporizador de inactividad
    lastEncoderActivityTime = currentTime;

    // Reinicio del estado a WELCOME
    currentState = STATE_HOME_SCREEN;
    displayHomeScreen();
  }
}

void bleStatus() {
  // Comprobación de la conexión Bluetooth
  if (BLEMidiServer.isConnected()) {

    if (bleConnected != bleDisconnected){
      digitalWrite(BL_PIN_LED, HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("   Bluetooth    ");
    lcd.setCursor(0, 1);
    lcd.print("   Connected    ");
    delay(2000);
    displayHomeScreen();
    Serial.println("Bluetooth ON");

    }
  bleDisconnected = bleConnected;
    
  } else {
    digitalWrite(BL_PIN_LED, LOW);
    bleDisconnected =1;
    Serial.println("Bluetooth OFF");
  }
}
