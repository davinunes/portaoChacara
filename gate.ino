const int pinRelay = 0;
 
const char* ssid = "azak0"; 
const char* password = "obrigado"; 

/* INICIALIZA O Wifi
   e o cliente http
   Todas as variáveis abaixo são referentes ao wifi e ao delay entre os acessos a internet
*/

// #include <WiFi.h>
#include <ESP8266WiFi.h>
#include "SinricPro.h"
#include "SinricProSwitch.h"

WiFiClient client;

#define APP_KEY ""
#define APP_SECRET ""
#define SWITCH_ID  ""

/*
 * Setup
 */

void setup() {
  Serial.begin(115200);
  pinMode(pinRelay, OUTPUT);
  //inicia o rele desligado
  digitalWrite(pinRelay, HIGH);

    WiFi.begin(ssid, password);
    
    int i = 0;
    while (WiFi.status() != WL_CONNECTED) {
      
      delay(500);
      Serial.println("Conectando na rede WiFi!");
    }
    Serial.println("Conectado na rede WiFi!");
  setupSinricPro();
}

//========================================================================

void loop() {
  
  if (Serial.available() > 0) {
    // Leitura do dado recebido
    char command = Serial.read();
    
    // Verifique o comando recebido
    if (command == '1') {
      // Ative o relé
      digitalWrite(pinRelay, LOW);
      Serial.println("Relé ativado!");
    } else if (command == '0') {
      // Desative o relé
      digitalWrite(pinRelay, HIGH);
      Serial.println("Relé desativado!");
    }
  }

  SinricPro.handle();

}

/*
 * funções
 */
void setupSinricPro()
{
  SinricProSwitch& mySwitch = SinricPro[SWITCH_ID];
  mySwitch.onPowerState(onPowerState);
  SinricPro.begin(APP_KEY, APP_SECRET);
}

bool onPowerState(const String &deviceId, bool &state) {
  //Serial.println("Dispositivo acionado: " + state ? "on":"off");
  Serial.printf("Interruptor %s agora está %s \r\n", deviceId.c_str(), state?"on":"off");
  digitalWrite(pinRelay, LOW);
  delay(1000);
  digitalWrite(pinRelay, HIGH);
  // get Switch device back
  SinricProSwitch& mySwitch = SinricPro[SWITCH_ID];
  mySwitch.sendPowerStateEvent(0); // send the new powerState to SinricPro server**
  return true; // indicate that callback handled correctly
  
}
