# ESPINO32-BLE-Robotics-Joystick

โปรเจกต์สร้างจอยสติ๊กไร้สายด้วยบอร์ด ESPino32 (ESP32) ที่ออกแบบมาเพื่อการใช้งานแบบ Hybrid สามารถสลับโหมดเป็น Bluetooth Gamepad สำหรับเล่นเกมบน PC หรือใช้ส่งคำสั่งควบคุมหุ่นยนต์ TurtleBot3 (ROS2) ผ่านเครือข่ายไร้สายได้โดยตรง

<div align="center">
  <img src="https://raw.githubusercontent.com/Buntungjai/ESPINO32-BLE-Robotics-Joystick/main/images/joystick.JPG" alt="demo" width="600">
</div>

**วิดีโอสาธิตการใช้งาน:**
* [จอยสำหรับ Xbox (YouTube)](https://youtu.be/adv-6_9NLgY)
* [ควบคุมหุ่นยนต์ Turtlebot3 (YouTube)](https://youtu.be/TNNunkKxHq8?si=Pb2Aw65VEmP1qJ-r)

### คุณสมบัติหลัก
- **Dual-Mode Functionality:** รองรับทั้งโหมด BLE Gamepad และโหมดควบคุมหุ่นยนต์ผ่าน Wi-Fi (ROS2)
- **Custom Hardware Design:** บัดกรีวงจรด้วยมือบนแผ่นไข่ปลา (Perfboard) ขนาด 9x15 cm พร้อมเคสอะคริลิคใส
- **Power Management:** มีระบบจ่ายไฟในตัวด้วยแบตเตอรี่ 18650 พร้อมโมดูลชาร์จ ทำให้ใช้งานแบบไร้สายได้ 100%
- **Ergonomic Layout:** ออกแบบตำแหน่งปุ่มกด WASD และปุ่มฟังก์ชันให้ถนัดมือเหมือนจอยเกมมาตรฐาน

### การสร้าง Hardware
ทำ perfboard สำหรับให้เสียบ บอร์ด ESPINO32 ลงไปได้
<p align="center">
  <img src="https://raw.githubusercontent.com/Buntungjai/ESPINO32-BLE-Robotics-Joystick/main/images/espino32.JPG" width="45%" />
  <img src="https://raw.githubusercontent.com/Buntungjai/ESPINO32-BLE-Robotics-Joystick/main/images/espino32Plug.JPG" width="45%" />
</p>

ออกแบบลายวงจรด้วยโปรแกรม Fritzing
<div align="center">
  <img src="https://raw.githubusercontent.com/Buntungjai/ESPINO32-BLE-Robotics-Joystick/main/images/backPCB_bb.png" alt="circuit layout" width="600">
</div>

ติดแผ่นอะคลิลิคอีกชั้นเพื่อให้จับถนัดมือมากขึ้น
<div align="center">
  <img src="https://raw.githubusercontent.com/Buntungjai/ESPINO32-BLE-Robotics-Joystick/main/images/perfboard.JPG" alt="assembly" width="600">
</div>
