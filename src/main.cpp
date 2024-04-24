#pragma region INCLUDE SECTION

  #include <Arduino.h>
  // #include <WiFi.h>
  // #include <WebServer.h>

#pragma endregion

#pragma region DEFINE SECTION
  
  #define SOIL_MOISTURE_PIN 25
  #define RELAY_PIN         26
  #define BUTTON_PIN        27

#pragma endregion

#pragma region DEFINE VARIABLE

  volatile bool buttonPressed = false;
  volatile u_int8_t rawPressedCount =0;
  volatile u_int8_t lastButtonPressMS =0;

#pragma endregion

#pragma region FUNCTION PROTOTYPE SECTION

  void toggleRelay(void* parameter);

  float moistureConverter(uint16_t sensorRead);
  void ICACHE_RAM_ATTR ButtonISR();
#pragma endregion

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLDOWN);
  attachInterrupt(BUTTON_PIN , ButtonISR, FALLING);
  xTaskCreatePinnedToCore(toggleRelay, "toggleRelay", 1024, NULL , 2, NULL,0);
}

void loop() {
// 
  // Serial.printf("Debounce Presses: %d\n", buttonPressed);
  // Serial.printf("Raw Presses: %d\n", rawPressedCount);
  // delay(5000);
}

#pragma region FUNCTION IMPLEMENTATION

  float moistureConverter(uint16_t sensorRead){
    return 100 - ((sensorRead/4095.00)*100);
  }

  void ICACHE_RAM_ATTR ButtonISR(){
    rawPressedCount ++;
    if((millis() - lastButtonPressMS) >200) {
      lastButtonPressMS = millis();
      buttonPressed = !buttonPressed;
    }
  }

  void toggleRelay( void* parameter){
    while (true)
    {
      if(buttonPressed){
        digitalWrite(RELAY_PIN, HIGH);
      }else{
        digitalWrite(RELAY_PIN, LOW);
      }

      vTaskDelay(500/portTICK_PERIOD_MS);
    }
    
  }

#pragma endregion