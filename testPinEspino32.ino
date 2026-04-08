// กำหนดพินที่ต้องการเทส
const int testPins[] = {2, 4, 5, 12, 13, 14, 15, 18, 19, 21, 22, 23, 25, 26, 27};
const int numPins = sizeof(testPins) / sizeof(testPins[0]);
const int LED_BUILTIN_PIN = 16; // GPIO 16 สำหรับ ESPino32

void setup() {
  Serial.begin(115200);
  Serial.println("--- Starting Pin Test with LED Feedback ---");
  
  // Setup LED
  pinMode(LED_BUILTIN_PIN, OUTPUT);
  digitalWrite(LED_BUILTIN_PIN, LOW); // เริ่มต้นให้ดับก่อน

  // Setup Buttons
  for (int i = 0; i < numPins; i++) {
    pinMode(testPins[i], INPUT_PULLUP);
  }
}

void loop() {
  String output = "";
  bool anyButtonPressed = false;
  
  for (int i = 0; i < numPins; i++) {
    int state = digitalRead(testPins[i]);
    output += "Pin " + String(testPins[i]) + ": " + String(state) + " | ";
    
    // ถ้ามีพินไหนโดนจิ้มลง GND (state == 0)
    if (state == LOW) {
      anyButtonPressed = true;
    }
  }
  
  // สั่งเปิด-ปิด LED ตามสถานะการกด
  if (anyButtonPressed) {
    digitalWrite(LED_BUILTIN_PIN, LOW); // ดับเมื่อปล่อย
  } else {
    digitalWrite(LED_BUILTIN_PIN, HIGH);  // ติดเมื่อกด
  }
  
  Serial.println(output);
  delay(100);
}