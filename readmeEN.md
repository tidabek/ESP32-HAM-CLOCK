# ESP32 HAM CLOCK / DX Cluster (EN)

ESP32 HAM CLOCK is a clock and DX Cluster terminal for the ESP32-2432S028 (CYD) module with a 2.4" touch TFT. It connects to DX Cluster (telnet), POTA (API), APRS-IS, pulls weather and solar/propagation data, and exposes both a TFT UI and a web UI (TFT emulation). Touch is used for navigation, filters, brightness menu, and calibration. Settings are stored in NVS; LittleFS holds fonts and web assets; `User_Setup.h` is copied into TFT_eSPI before build.

## Features

- DX Cluster via telnet (filters, login, keepalive)
- POTA spots from public API (HTTP)
- APRS-IS: frame reception + optional TX beacon (TCP), `r/lat/lon/km` radius filter, symbol and comment
- Weather + forecast + PM2.5/PM10 from OpenWeather
- Propagation data (hamqsl solarxml)
- TFT screens: Clock (UTC/local), DX, POTA, APRS, HF bands, propagation, weather, matrix clock
- XPT2046 touch: navigation, filter menus, long-press brightness menu; touch/TFT rotation and calibration
- HTTP config portal (AP fallback when no WiFi); preferences saved in NVS
- LittleFS for fonts and HTML/JS/CSS; web UI mirrors the TFT
- Pre-build extra script copies `User_Setup.h` into TFT_eSPI

## Hardware

- Board: ESP32-2432S028 (ESP32 WROOM, 240x320 ILI9341 TFT, XPT2046 touch, onboard backlight)
- Alternative: other ESP32 without TFT (use web UI), or external ILI9341

### Pins (ESP32-2432S028)

ILI9341 display:
- TFT_SCLK: GPIO14
- TFT_MOSI: GPIO13
- TFT_MISO: GPIO12
- TFT_CS: GPIO15
- TFT_DC: GPIO2
- TFT_RST: GPIO4
- TFT_BL: GPIO21 (PWM, default 5 kHz, 8-bit)

XPT2046 touch:
- TOUCH_CS: GPIO33
- TOUCH_IRQ: GPIO36
- TOUCH_MOSI: GPIO32
- TOUCH_MISO: GPIO39
- TOUCH_CLK: GPIO25

Other:
- Backlight PWM channel 0
- No onboard buttons; navigation by touch (bottom corners = screen change)

## PlatformIO Environments

- `esp32-2432s028`: ESP32 Dev + TFT_eSPI + XPT2046; FS: LittleFS; `ENABLE_TFT_DISPLAY`

Key libs (see `platformio.ini`): ArduinoJson 7, TFT_eSPI, XPT2046_Touchscreen.

## Build (PlatformIO)

1. `platformio run -e esp32-2432s028` (default env) or choose another in `platformio.ini`
2. Pre-build `pre:copy_user_setup.py` copies `User_Setup.h` into TFT_eSPI
3. Flash firmware; optionally `pio run -t uploadfs` for LittleFS (`data/`)

## First-Time Setup

1. If no saved WiFi, AP starts: SSID `ESP32-HAM-CLOCK`, password `1234567890`
2. Open `http://192.168.4.1` → Config tab
3. Set WiFi (primary/secondary), DX Cluster host/port, callsign, locator, OpenWeather key, optional QRZ, TFT settings (brightness, language, rotation, calibration)
4. APRS beacon: first beacon is sent about 1 minute after startup, next beacons use the configured interval; user comment is used on each beacon, while the first and every fifth beacon uses the ESP32-HAM-CLOCK project link
5. Save; the board reboots and joins WiFi STA

### APRS-IS (details)

- Web config fields: host, port, callsign, SSID (0-15), passcode, filter radius (1-50 km), beacon enable, symbol (2 chars), comment (ASCII max 43), beacon interval (1-180 min)
- After login, APRS-IS filter is sent as `#filter r/lat/lon/radius` using coordinates from “Your station”
- If user coordinates are not set, filter and distance calculations use fallback `52.40, 16.92`
- TX beacon requires valid coordinates and a valid APRS callsign (not `NOCALL`)
- APRS station buffer limits: 20 entries for web UI and 10 entries on TFT screen

## TFT Screens (short)

- Clock (UTC/local, date, IP, brightness, language)
- DX Cluster (list, mode/band filters)
- APRS-IS (stations, sorting)
- Band Info (HF day/night)
- Sun Spots / propagation
- Weather (current + details, PM2.5/PM10)
- POTA (filters, time-ordered)
- Matrix Clock

## Change Logs

- Polish, 1.22 vs 1.2: `ZMIANY_1.22_vs_1.2.txt`
- Polish, 1.22 vs 1.21: `ZMIANY_1.22_vs_1.21.txt`
- English, 1.22 vs 1.2: `CHANGES_1.22_vs_1.2.txt`
- English, 1.22 vs 1.21: `CHANGES_1.22_vs_1.21.txt`

## Network Cadence

- WiFi: auto-reconnect to last SSID (STA); AP only when no config or at cold start without WiFi
- DX Cluster reconnect: min every 20 s; keepalive CRLF every 30 s
- Weather: every 10 min; on error every 2 min; each cycle makes 3 HTTPS calls (current/forecast/air)
- Propagation: every 60 min; on error retry after 5 min
- POTA API: every 180 s
- QRZ lookup: every 3 s when DX/POTA screen active, otherwise every 10 s (with retry limits)

## License

Open-source for the ham radio community.
