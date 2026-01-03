# ESP32 Web Server LED Control

Simple ESP32 project that demonstrates Wi-Fi connectivity and web-based GPIO control for beginners.

## 📋 Description

This project creates a web server on ESP32 that allows you to control an external LED through a web interface. Perfect for learning IoT basics, Wi-Fi connectivity, and GPIO control.

## 🔧 Hardware Requirements

- **ESP32 Dev Module**
- **LED** (any color)
- **Resistor** (220-330 Ω)
- **Breadboard and jumper wires**

## 🔌 Wiring Diagram

```
ESP32                    LED
GPIO 2 ----[Resistor]----Anode(+)
GND ---------------------Cathode(-)
```

## ⚙️ Software Requirements

- [PlatformIO IDE](https://platformio.org/) extension for VS Code
- Or [Arduino IDE](https://www.arduino.cc/en/software) 2.0+

## 🚀 Getting Started

### 1. Clone the repository
```bash
git clone https://github.com/YOUR_USERNAME/esp32-playground.git
cd esp32-playground
```

### 2. Configure Wi-Fi Credentials

Edit `src/main.cpp` and update lines 19-20:
```cpp
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
```

### 3. Upload to ESP32

**Using PlatformIO (VS Code):**
1. Open the project folder in VS Code
2. Connect ESP32 via USB
3. Click **Upload** button (→) in the bottom status bar
4. Open **Serial Monitor** (🔌) to view the IP address

**Using Arduino IDE:**
1. Open `ESP32_WebServer_LED.ino`
2. Select **Tools → Board → ESP32 Dev Module**
3. Select your COM port
4. Click **Upload**

### 4. Control the LED

1. Open Serial Monitor (115200 baud)
2. Find the IP address (e.g., `192.168.1.123`)
3. Open a web browser and navigate to that IP
4. Click **ON** / **OFF** buttons to control the LED

## 📸 Web Interface

The web server provides a clean, responsive interface with:
- Current LED status display
- ON/OFF control buttons
- Device information (IP address, GPIO pin)

## 🎓 Learning Objectives

This project teaches:
- ✅ ESP32 Wi-Fi connectivity
- ✅ Creating HTTP web servers
- ✅ Handling HTTP requests
- ✅ GPIO control (digital output)
- ✅ HTML/CSS for embedded systems
- ✅ Serial communication for debugging

## 📁 Project Structure

```
esp32-playground/
├── src/
│   └── main.cpp              # Main source code
├── include/                  # Header files (if needed)
├── lib/                      # Custom libraries
├── platformio.ini            # PlatformIO configuration
├── ESP32_WebServer_LED.ino   # Arduino IDE version
└── README.md                 # This file
```

## 🛠️ Troubleshooting

**ESP32 not connecting to Wi-Fi:**
- Double-check SSID and password
- Ensure your Wi-Fi is 2.4 GHz (ESP32 doesn't support 5 GHz)
- Check router settings (some routers block IoT devices)

**Upload fails:**
- Install USB drivers (CP210x or CH340)
- Try pressing and holding the BOOT button during upload
- Check the correct COM port is selected

**Can't access web page:**
- Ensure your computer is on the same Wi-Fi network
- Check the IP address in Serial Monitor
- Try pinging the ESP32: `ping 192.168.x.xxx`

## 📝 License

This project is open source and available under the MIT License.

## 🤝 Contributing

Feel free to fork this project and submit pull requests with improvements!

## 👨‍💻 Author

Created for beginners learning ESP32 and IoT development.

---

**Happy coding! 🚀**
