#include <BleGamepad.h>

// ชื่อที่ปรากฏเวลา Scan Bluetooth
BleGamepad bleGamepad("ESPino32 Gamepad", "MakerThai", 100);

// กำหนด Pin ตามที่คุณไล่มา
const int L_UP    = 19;
const int L_LEFT  = 18;
const int L_RIGHT = 4;
const int L_DOWN  = 23;

const int R_UP    = 26; // จะแมพเป็นปุ่ม Y
const int R_LEFT  = 25; // จะแมพเป็นปุ่ม X
const int R_RIGHT = 5;  // จะแมพเป็นปุ่ม B
const int R_DOWN  = 27; // จะแมพเป็นปุ่ม A (ปุ่มกระโดดหลักใน Roblox)

const int SELECT_1 = 15;
const int SELECT_2 = 13;

const int testPins[] = {19, 18, 4, 23, 26, 25, 5, 27, 15, 13};
const int numPins = sizeof(testPins) / sizeof(testPins[0]);
const int LED_PIN = 16;

void setup() {

  Serial.begin(115200);

  // เริ่มต้น BLE Gamepad
  bleGamepad.begin();

  pinMode(LED_PIN, OUTPUT);

  for (int i = 0; i < numPins; i++) {
    pinMode(testPins[i], INPUT_PULLUP);
  }
  Serial.println("Waiting for BLE connection...");

}

void loop() {
  if (bleGamepad.isConnected()) {
    digitalWrite(LED_PIN, HIGH);

    // --- แก้ไขจอยฝั่งซ้าย: ใช้การเช็คแบบแยกอิสระเพื่อให้กดเฉียงได้ ---
    signed char hatValue = HAT_CENTERED;

    bool up    = (digitalRead(L_UP) == LOW);
    bool down  = (digitalRead(L_DOWN) == LOW);
    bool left  = (digitalRead(L_LEFT) == LOW);
    bool right = (digitalRead(L_RIGHT) == LOW);

    if (up && right)      hatValue = HAT_UP_RIGHT;
    else if (up && left)  hatValue = HAT_UP_LEFT;
    else if (down && right) hatValue = HAT_DOWN_RIGHT;
    else if (down && left)  hatValue = HAT_DOWN_LEFT;
    else if (up)    hatValue = HAT_UP;
    else if (down)  hatValue = HAT_DOWN;
    else if (left)  hatValue = HAT_LEFT;
    else if (right) hatValue = HAT_RIGHT;
    else            hatValue = HAT_CENTERED;

    bleGamepad.setHat(hatValue);

    // --- จอยฝั่งขวา (ใช้ if แยกกันอยู่แล้ว กดพร้อมกันได้เลย) ---
    if (digitalRead(R_DOWN) == LOW)  bleGamepad.press(BUTTON_1); else bleGamepad.release(BUTTON_1);
    if (digitalRead(R_RIGHT) == LOW) bleGamepad.press(BUTTON_2); else bleGamepad.release(BUTTON_2);
    if (digitalRead(R_LEFT) == LOW)  bleGamepad.press(BUTTON_3); else bleGamepad.release(BUTTON_3);
    if (digitalRead(R_UP) == LOW)    bleGamepad.press(BUTTON_4); else bleGamepad.release(BUTTON_4);

    // --- ปุ่มเสริม ---
    if (digitalRead(SELECT_1) == LOW) bleGamepad.press(BUTTON_7); else bleGamepad.release(BUTTON_7); 
    if (digitalRead(SELECT_2) == LOW) bleGamepad.press(BUTTON_8); else bleGamepad.release(BUTTON_8);

    bleGamepad.sendReport();
  } else {
    digitalWrite(LED_PIN, (millis() / 500) % 2);
  }

  delay(5); // ลด delay ลงเหลือ 5ms เพื่อให้ตอบสนองไวขึ้น (ลดอาการหน่วง)
}