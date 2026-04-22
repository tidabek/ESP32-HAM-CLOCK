// User_Setup.h dla ESP32-2432S028 (CYD - Cheap Yellow Display)
// Konfiguracja bazowana na działającym projekcie referencyjnym:
// Setup801_ESP32_CYD_ILI9341_240x320.h

#define USER_SETUP_LOADED
#define USER_SETUP_ID 801

// ESP32 z SPI
#define ESP32

// Driver - używamy alternatywnego drivera ILI9341_2 (działa lepiej na CYD)
//#define ILI9341_DRIVER       // Generic driver for common displays
#define ILI9341_2_DRIVER     // Alternative ILI9341 driver
//#define ILI9342_DRIVER // alternatywny driver dla ILI9342 (niektóre wyświetlacze CYD mogą używać tego drivera, ale ILI9341_2 działa dobrze)
//#define ST7789_DRIVER        // Driver dla ST7789V (240x320)
// Display resolution
#define TFT_WIDTH  240
#define TFT_HEIGHT 320

// Niektóre moduły ST7789V wymagają odwróconego porządku kolorów (BGR zamiast RGB)
//#define TFT_RGB_ORDER TFT_BGR

// SPI pins dla ESP32-2432S028 (zgodne z projektem referencyjnym)
#define TFT_MISO 12
#define TFT_MOSI 13
#define TFT_SCLK 14
#define TFT_CS   15  // Chip select control pin
#define TFT_DC    2  // Data Command control pin
#define TFT_RST  -1  // Set TFT_RST to -1 if display RESET is connected to ESP32 board RST
// jeśli używamy zewnętrznego TFT ILI9341 z własnym sterowaniem resetem to RST od TFT podpiąć na stałę do +3,3V 

// Backlight pin
#define TFT_BL   21            // LED back-light control pin
#define TFT_BACKLIGHT_ON HIGH  // Level to turn ON back-light (HIGH or LOW)

// The ESP32 has 2 free SPI ports i.e. VSPI and HSPI, the VSPI is the default.
// If the VSPI port is in use and pins are not accessible (e.g. TTGO T-Beam)
// then uncomment the following line:
#define USE_HSPI_PORT

// SPI frequency (z projektu referencyjnego - 55 MHz działa dobrze)
//#define SPI_FREQUENCY  27000000
#define SPI_FREQUENCY  55000000

// Optional reduced SPI frequency for reading TFT
#define SPI_READ_FREQUENCY  20000000

// The XPT2046 requires a lower SPI clock rate of 2.5MHz so we define that here:
#define SPI_TOUCH_FREQUENCY  2500000

// Touch pins (match CYD wiring; guard with ifndef for alternate boards)
#ifndef TOUCH_CS
#define TOUCH_CS 33
#endif
#ifndef TOUCH_IRQ
#define TOUCH_IRQ 36 //domyślnie 36 a dla zewnętrznego TFT ILI9341 użyć 35
#endif
#ifndef TOUCH_MOSI
#define TOUCH_MOSI 32
#endif
#ifndef TOUCH_MISO
#define TOUCH_MISO 39 //domyślnie 39 a dla zewnętrznego TFT ILI9341 użyć 27
#endif
#ifndef TOUCH_CLK
#define TOUCH_CLK 25
#endif

// Inwersja kolorów - w razie odwróconych kolorów na ST7789V odkomentuj poniższą linię
#define TFT_INVERSION_ON

// Fonts
#define LOAD_GLCD   // Font 1. Original Adafruit 8 pixel font
#define LOAD_FONT2  // Font 2. Small 16 pixel high font
#define LOAD_FONT4  // Font 4. Medium 26 pixel high font
//#define LOAD_FONT6  // Font 6. Large 48 pixel font (nieużywany)
//#define LOAD_FONT7  // Font 7. 7 segment 48 pixel font (nieużywany)
//#define LOAD_FONT8  // Font 8. Large 75 pixel font (nieużywany)
//#define LOAD_GFXFF  // FreeFonts (nieużywane w projekcie)

// Comment out the #define below to stop the SPIFFS filing system and smooth font code being loaded
// this will save ~20kbytes of FLASH
#define SMOOTH_FONT

// Włącz dekodowanie UTF-8 (potrzebne dla polskich znaków w czcionkach VLW)
#define UTF8_SUPPORT
