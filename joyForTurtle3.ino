#include <micro_ros_arduino.h>
#include <stdio.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <geometry_msgs/msg/twist.h>

#if !defined(ESP32)
#error This example is only available for ESP32
#endif

// --- 1. กำหนดพินจอยสติก (ตามที่คุณ Book เทส) ---
#define PIN_W 19
#define PIN_A 18
#define PIN_S 23
#define PIN_D 4
#define PIN_X 15  // ใช้ปุ่ม Left เป็นปุ่มหยุด (X)

// พินที่เหลือ (สำรองไว้ใช้งานในอนาคต)
#define PIN_MID    13
#define PIN_RIGHT  12
#define PIN_UP     26
#define PIN_L_RED  25  // ปุ่มฝั่งขวา-ซ้าย
#define PIN_R_RED  14  // ปุ่มฝั่งขวา-ขวา
#define PIN_DOWN   27

#define LED_PIN 16 // ESPino32 Onboard LED

// --- ROS2 Objects ---
rcl_publisher_t publisher;
geometry_msgs__msg__Twist msg;
rclc_support_t support;
rcl_allocator_t allocator;
rcl_node_t node;

#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){error_loop();}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){}}

void error_loop(){
  while(1){
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    delay(100);
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("begin Serial");
  // --- 2. ตั้งค่า Wi-Fi และ Agent IP (เช็ก IP คอมให้ตรงนะครับ) ---
  
  // บังคับให้เป็น (char *) เพื่อให้ Warning หายไป
  set_microros_wifi_transports((char *)"SSID_WIFI", (char *)"PASSWORD", (char *)"192.168.1.1", 8888);
  

  // เพิ่มบรรทัดนี้เพื่อความนิ่ง 1,000 ล้านเปอร์เซ็นต์
  WiFi.setSleep(false);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  // Setup ปุ่มกดแบบ Pull-up ทั้งหมด
  int allPins[] = {PIN_W, PIN_A, PIN_S, PIN_D, PIN_X, PIN_MID, PIN_RIGHT, PIN_UP, PIN_L_RED, PIN_R_RED, PIN_DOWN};
  for(int p : allPins) {
    pinMode(p, INPUT_PULLUP);
  }

  delay(2000);

  allocator = rcl_get_default_allocator();

  // --- 3. ตั้งค่า Domain ID = 30 ---
  rcl_init_options_t init_options = rcl_get_zero_initialized_init_options();
  RCCHECK(rcl_init_options_init(&init_options, allocator));
  RCCHECK(rcl_init_options_set_domain_id(&init_options, 30));

  RCCHECK(rclc_support_init_with_options(&support, 0, NULL, &init_options, &allocator));

  // สร้าง Node ชื่อ "espino32_controller"
  RCCHECK(rclc_node_init_default(&node, "espino32_controller", "", &support));

  // สร้าง Publisher ส่งไปที่ "/cmd_vel"
  RCCHECK(rclc_publisher_init_default(
    &publisher,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(geometry_msgs, msg, Twist),
    "cmd_vel"));

  // ล้างค่าเริ่มต้น
  msg.linear.x = 0.0; msg.linear.y = 0.0; msg.linear.z = 0.0;
  msg.angular.x = 0.0; msg.angular.y = 0.0; msg.angular.z = 0.0;
  
  Serial.println("micro-ROS Connected & Ready!");
}

void loop() {
  // 1. เช็กความเสถียรของ WiFi ถ้าหลุดให้ Restart บอร์ดทันที (แก้ปัญหาค้างถาวร)
  if (WiFi.status() != WL_CONNECTED) {
    ESP.restart();
  }

  // 2. ใช้ Timer คุมความถี่ 10Hz
  static unsigned long last_run = 0;
  if (millis() - last_run >= 100) {
    last_run = millis();

    // รีเซ็ตค่าความเร็ว
    msg.linear.x = 0.0;
    msg.angular.z = 0.0;

    // Logic ปุ่มกด (แบบเดิม)
    if (digitalRead(PIN_W) == LOW) msg.linear.x = 0.15; // เพิ่มความเร็วหน่อยจะได้เห็นชัดๆ
    else if (digitalRead(PIN_S) == LOW) msg.linear.x = -0.15;

    if (digitalRead(PIN_A) == LOW) msg.angular.z = 0.6;
    else if (digitalRead(PIN_D) == LOW) msg.angular.z = -0.6;

    if (digitalRead(PIN_X) == LOW) {
      msg.linear.x = 0.0;
      msg.angular.z = 0.0;
    }

    // 3. ส่งข้อมูลแบบ "ไม่แคร์โลก" (ใช้ RCSOFTCHECK)
    // ถ้าส่งไม่ได้ก็ช่างมัน ไม่ต้องค้าง ไม่ต้องเข้า error_loop
    RCSOFTCHECK(rcl_publish(&publisher, &msg, NULL));

    // ไฟสถานะ
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
  }

  // 4. หัวใจสำคัญ: ปล่อยให้ ESP32 เคลียร์งาน Background
  // ถ้าไม่มีบรรทัดนี้ WiFi Stack จะพังง่ายมาก
  delay(1); 
}
