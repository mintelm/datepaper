BOARD_TAG			=	nano
BOARD_SUB			=	atmega328old
ARDUINO_LIBS	=	GxEPD2-master SPI Adafruit-GFX-Library-master Wire RTClib U8g2_for_Adafruit_GFX-master
MONITOR_PORT	=	/dev/ttyUSB0

include /usr/share/arduino/Arduino.mk
