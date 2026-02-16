# SAP Pillbox

An ESP32‑based smart pillbox system that helps patients stay on track with their medication schedules.  
It integrates with a backend API to fetch pill schedules, send notifications, and log medication events (`medlogs`) with statuses like **taken** or **missed**.  
The device uses an OLED display, buzzer alarm, and capacitive touch sensors for user interaction.

---

## ✨ Features

- **Schedule Fetching**: Pulls pill schedules from backend API (`/schedules/device/{chip_id}`).
- **Notifications**: Posts reminders to backend (`/notifications/{chip_id}`) and triggers buzzer alarms.
- **Touch Sensor Control**: User can deactivate alarms by touching a sensor mapped to each pill slot.
- **Medlogs**: Sends medication logs (`/medlogs/{chip_id}`) with status:
  - `"taken"` when user touches sensor within alarm window.
  - `"missed"` if no touch detected within timeout (default 3 hours).
- **OLED Display**: Shows current time, status, and pill reminders.
- **Debounce Logic**: Ensures only one notification/medlog per pill slot per day.

---

## 📂 Project Structure
```code
sap-pillbox/
├── sketch_SAP.ino       # Main Arduino sketch
├── alarms.h             # Alarm + buzzer + medlog logic
├── schedules.h          # Schedule fetching + parsing
├── notifications.h      # Encapsulated notification POST
├── touch.h              # Touch sensor configuration + functions
├── touch.cpp            # Touch sensor implementation
└── README.md            # Project documentation
```

---

## ⚙️ Setup

1. **Hardware**
   - ESP32 development board
   - OLED display (SSD1306)
   - Buzzer
   - Capacitive touch sensors (pins: 12, 14, 27, 13, 15, 2, 4)

2. **Software**
   - Arduino IDE with ESP32 board support
   - Libraries:
     - `Adafruit_GFX`
     - `Adafruit_SSD1306`
     - `ArduinoJson`
     - `HTTPClient`

3. **Configuration**
   - Set your device API key and chip ID in `sketch_SAP.ino`:
     ```cpp
     String apiKey = "YOUR_DEVICE_API_KEY";
     String chipIdStr = "YOUR_DEVICE_CHIP_ID";
     ```
   - The device will fetch its `user_id` dynamically from `/devices/{chip_id}`.

---

## 🚀 Usage

- On startup:
  - ESP32 connects to WiFi.
  - Fetches `user_id` and schedules.
  - Calibrates touch sensors.
- At scheduled times:
  - Buzzer alarm activates.
  - Notification is posted to backend.
- User interaction:
  - Touch sensor → alarm off + medlog `"taken"`.
  - No touch within timeout → medlog `"missed"`.

---

## 🛠️ Development Notes

- **Testing**: You can shorten the alarm timeout (e.g., 30s) by adjusting:
  ```cpp
  const unsigned long alarmTimeoutMs = 30UL * 1000UL; // 30 seconds
- **Extensibility**: Touch sensors can be mapped to specific pill slots for direct logging.
- **Backend**: Built with FastAPI, endpoints validate device API keys and store schedules/medlogs.

