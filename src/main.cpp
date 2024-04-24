#pragma region INCLUDE SECTION

  #include <Arduino.h>
  #include <WiFi.h>
  #include <WebServer.h>

#pragma endregion

#pragma region DEFINE SECTION
  
  #define SOIL_MOISTURE_PIN 25
  #define RELAY_PIN         26
  #define BUTTON_PIN        27

#pragma endregion

#pragma region DEFINE VARIABLE

  volatile long pressedCount = 0;
  volatile long rawPressedCount =0;
  volatile long lastButtonPressMS =0;

#pragma endregion

#pragma region DEFINE FUNCTION

  float moistureConverter(uint16_t sensorRead);

  void toggleRelay(void* arg);

  void ICACHE_RAM_ATTR ButtonISR();
#pragma endregion

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLDOWN);
  attachInterrupt(BUTTON_PIN , ButtonISR, FALLING);
  // xTaskCreatePinnedToCore(toggleRelay, "ToggleRelay", 2048, NULL , 2, NULL,0);
}

void loop() {
// 
  Serial.printf("Debounce Presses: %d\n", pressedCount);
  Serial.printf("Raw Presses: %d\n", rawPressedCount);
  delay(5000);
}

#pragma region FUNCTION IMPLEMENTATION

  float moistureConverter(uint16_t sensorRead){
    return 100 - ((sensorRead/4095.00)*100);
  }

  void toggleRelay(void* arg){
    
    // if(BUTTON_PIN == HIGH){
    //   digitalWrite(RELAY_PIN, HIGH);
    // }

  }

  void ICACHE_RAM_ATTR ButtonISR(){
    rawPressedCount ++;
    if((millis() - lastButtonPressMS) >200) {
      lastButtonPressMS = millis();
      pressedCount++;
    }
  }

#pragma endregion