#include <IRremote.hpp>

const int RECV_PIN = 4;  // Пин приемника
const int IR_SEND_PIN = 5; // Пин передатчика
const int LED_PIN = 2;   // Встроенный синий светодиод (обычно 2 пин на ESP32-S3)

unsigned long lastShotTime = 0;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Инициализация приемника и передатчика
  IrReceiver.begin(RECV_PIN, DISABLE_LED_FEEDBACK);
  IrSender.begin(IR_SEND_PIN);
  
  Serial.println("Танк готов к бою!");
}

void loop() {
  // 1. СТРЕЛЬБА: Каждые 3 секунды отправляем сигнал
  if (millis() - lastShotTime > 3000) {
    Serial.println("Выстрел!");
    IrSender.sendNEC(0x1234, 32); // Отправляем сигнал
    lastShotTime = millis();
  }

  // 2. ПРИЕМ: Если поймали сигнал
  if (IrReceiver.decode()) {
    Serial.print("Попадание! Код: 0x");
    Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);
    
    // Мигаем встроенным светодиодом при попадании
    digitalWrite(LED_PIN, HIGH);
    delay(500); // Светодиод горит 0.5 секунды
    digitalWrite(LED_PIN, LOW);
    
    IrReceiver.resume(); // Ждем следующий сигнал
  }
}
