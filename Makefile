current_dir = $(shell pwd)

BOARD_TAG			=	nano
BOARD_SUB			=	atmega328old
ARDUINO_LIBS		=	GxEPD2 SPI Adafruit_GFX Wire RTClib U8g2_Adafruit_GFX
MONITOR_PORT		=	/dev/ttyUSB0
USER_LIB_PATH		= $(current_dir)/lib

include /usr/share/arduino/Arduino.mk
