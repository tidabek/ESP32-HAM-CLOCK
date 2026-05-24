# ESP32-HAM-CLOCK

ESP32 HAM CLOCK to zegar i terminal DX/APRS dla ESP32 (głównie CYD ESP32-2432S028), z wyświetlaczem TFT 2.4" i równoległym panelem WWW.

PROJEKT OBUDOWY do wydruku 3d dla tego projektu:

https://makerworld.com/pl/models/2402822-datadisplay-v1-updated#profileId-2633814

Projekt łączy:
- DX Cluster (telnet),
- POTA (API),
- HAMALERT (telnet),
- APRS-IS (RX + opcjonalny TX beacon),
- pogodę i prognozę (OpenWeather),
- dane propagacyjne/solar.

Ustawienia są zapisywane w NVS, a zasoby WWW/fonty są trzymane w LittleFS.

## Najważniejsze funkcje

- DX Cluster: filtry, logowanie, keepalive.
- POTA: spoty z publicznego API.
- HAMALERT: pobieranie spotów przez telnet.
- APRS-IS:
  - odbiór ramek,
  - filtr `#filter r/lat/lon/radius`,
  - opcjonalny TX beacon,
  - APRS ALERT (lista znaków, nearby/WX, LED).
- OpenWeather:
  - pogoda bieżąca,
  - prognoza,
  - PM2.5/PM10.
- TFT + dotyk XPT2046:
  - menu filtrów,
  - jasność,
  - rotacje,
  - kalibracja,
  - auto-switch ekranów.
- Panel WWW:
  - zakładki `DXSPOT`, `POTA`, `HAMALERT`, `APRS`, `ALERT`, `Ekran TFT`, `Ustawienia`, `Instrukcja`.
- Build helper:
  - `pre:copy_user_setup.py` automatycznie kopiuje `User_Setup.h` do biblioteki TFT_eSPI.

## Sprzęt

- Główny target: `ESP32-2432S028` (CYD), TFT ILI9341 240x320, dotyk XPT2046.
- Alternatywnie: inne ESP32 (także bez TFT, wtedy obsługa przez WWW).

### Piny (CYD ESP32-2432S028)

Wyświetlacz ILI9341:
- `TFT_SCLK` GPIO14
- `TFT_MOSI` GPIO13
- `TFT_MISO` GPIO12
- `TFT_CS` GPIO15
- `TFT_DC` GPIO2
- `TFT_RST` GPIO4
- `TFT_BL` GPIO21 (PWM, domyślnie 5 kHz, 8 bit)

Dotyk XPT2046:
- `TOUCH_CS` GPIO33
- `TOUCH_IRQ` GPIO36
- `TOUCH_MOSI` GPIO32
- `TOUCH_MISO` GPIO39
- `TOUCH_CLK` GPIO25

## Środowiska PlatformIO

- `esp32-2432s028` (domyślne)
- `esp32-ili9341-38pin`

Kluczowe biblioteki: ArduinoJson 7, TFT_eSPI, XPT2046_Touchscreen.

## Build i upload

1. Build firmware:
   `platformio run -e esp32-2432s028`
2. Upload firmware:
   `platformio run -e esp32-2432s028 -t upload`
3. Upload LittleFS (`data/`):
   `platformio run -e esp32-2432s028 -t uploadfs`

## Pierwsze uruchomienie

1. Jeśli brak zapisanej konfiguracji WiFi, urządzenie uruchomi AP:
   SSID `ESP32-HAM-CLOCK`, hasło `1234567890`.
2. Wejdź na `http://192.168.4.1` (tryb AP) lub na IP pokazane na TFT.
3. W zakładce `Ustawienia` skonfiguruj:
   WiFi,
   DX,
   POTA,
   HAMALERT,
   APRS,
   OpenWeather,
   QRZ,
   parametry TFT/dotyku.
4. Zapis ustawień powoduje restart modułu.

## Ekrany TFT

Projekt obsługuje 11 ekranów (kolejność konfigurowalna, każdy slot może być `OFF`):

1. HAM CLOCK
2. DX CLUSTER
3. APRS-IS
4. APRS RADAR
5. BAND INFO
6. SUN SPOTS
7. WEATHER
8. WEATHER FORECAST
9. POTA CLUSTER
10. HAMALERT
11. MATRIX

## Dokumentacja użytkownika

- PL: `INSTRUKCJA.txt` oraz `data/instrukcja.txt`
- EN: `data/manual.txt` oraz `readmeEN.md`

## Pliki zmian

- Zmiany 1.22 vs 1.2: `ZMIANY_1.22_vs_1.2.txt`
- Zmiany 1.22 vs 1.21: `ZMIANY_1.22_vs_1.21.txt`
- Changes 1.22 vs 1.2: `CHANGES_1.22_vs_1.2.txt`
- Changes 1.22 vs 1.21: `CHANGES_1.22_vs_1.21.txt`

## Szybkie troubleshooting

- WWW nie otwiera się:
  sprawdź, czy jesteś w tej samej sieci, albo połącz się z AP i użyj `192.168.4.1`.
- Brak stylów/pusta strona:
  wgraj ponownie LittleFS (`uploadfs`), sprawdź obecność plików w `data/`.
- Brak DX/POTA/HAMALERT:
  sprawdź internet oraz host/port/login/hasła.
- Brak pogody:
  sprawdź `weather_key` i współrzędne.
- Dotyk przesunięty/odwrócony:
  popraw `Touch Rotation` i `touch_swap_mode`, ewentualnie reset kalibracji.

## Licencja

MIT License. Szczegóły: `LICENSE`.
