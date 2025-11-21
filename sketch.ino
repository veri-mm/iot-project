#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <PubSubClient.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
const int gasSensorPin = 34;
const int ledPin = 4;

char ssid[] = "Wokwi-GUEST";
char pass[] = "";

// Configuração MQTT para o broker público HiveMQ
const char* mqtt_broker = "broker.hivemq.com";
const int   mqtt_port   = 1883;  // non-TLS TCP port
const char* mqtt_topic  = "wokwi-airquality";

WiFiClient espClient;
PubSubClient mqttClient(espClient);

void mqttReconnect() {
  // Loop até que a conexão seja restabelecida
  while (!mqttClient.connected()) {
    String clientId = "esp32-airquality-";
    clientId += String((uint32_t)random(0xffff), HEX);

    if (mqttClient.connect(clientId.c_str())) {
      // Conectado ao HiveMQ
      // Aqui você pode assinar tópicos, se necessário
    } else {
      delay(1000);
    }
  }
}

void displayMessage(String line1, String line2, int delayTime = 2000) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(line1);
  lcd.setCursor(0, 1);
  lcd.print(line2);
  delay(delayTime);
}

void setup() {
  // Inicializa o LCD
  lcd.init();
  lcd.backlight();

  // Inicializa a comunicação serial para depuração e semente aleatória
  Serial.begin(115200);
  randomSeed(micros());

  // Inicializa o pino do LED
  pinMode(ledPin, OUTPUT);

  // Conecta ao Wi-Fi
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
  }

  // Configura o cliente MQTT (broker HiveMQ)
  mqttClient.setServer(mqtt_broker, mqtt_port);
  mqttReconnect();
}

void loop() {
  if (!mqttClient.connected()) {
    mqttReconnect();
  }
  mqttClient.loop();

  // Lê o valor do gás do sensor de gás (saída analógica)
  int gasValue = analogRead(gasSensorPin);

  // Determina o nível de gás com base nos critérios
  String gasLevel;

  if (gasValue >= 0 && gasValue <= 1364) {
    gasLevel = "Good";
  } else if (gasValue >= 1365 && gasValue <= 2730) {
    gasLevel = "Normal";
  } else {
    gasLevel = "Bad";
  }

  // Determina a qualidade do ar apenas com base no nível de gás
  String airQuality;

  if (gasLevel == "Good" || gasLevel == "Normal") {
    airQuality = "Good Air Quality";
  } else {
    airQuality = "Bad Air Quality";
  }

  // Publica as leituras no broker MQTT HiveMQ em formato JSON (semelhante ao sketch.py)
  String payload = "{";
  payload += "\"gasValue\":" + String(gasValue) + ",";
  payload += "\"gasLevel\":\"" + gasLevel + "\",";
  payload += "\"airQuality\":\"" + airQuality + "\"";
  payload += "}";
  mqttClient.publish(mqtt_topic, payload.c_str());

  // Exibe o nível de gás e o valor do gás no LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Gas Level: " + gasLevel);
  lcd.setCursor(0, 1);
  lcd.print("Gas Value: " + String(gasValue));
  delay(2000); // Exibe o nível e o valor do gás por 2 segundos

  // Exibe a qualidade do ar no LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Air Quality: ");
  lcd.setCursor(0, 1);
  lcd.print(airQuality);
  delay(2000); // Exibe a qualidade do ar por 2 segundos   

  // Controla o LED com base na qualidade do ar
  if (airQuality == "Bad Air Quality") {
    digitalWrite(ledPin, HIGH); // Liga o LED
  } else {
    digitalWrite(ledPin, LOW); // Desliga o LED
  }
}
