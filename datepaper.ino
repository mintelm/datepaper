#include <Wire.h>
#include <RTClib.h>

#include <GxEPD2_3C.h>
#include <U8g2_for_Adafruit_GFX.h>

#define MAX_DISPLAY_BUFFER_SIZE 800
#define MAX_HEIGHT_3C(EPD)	(EPD::HEIGHT <= (MAX_DISPLAY_BUFFER_SIZE / 2)  \
				/ (EPD::WIDTH / 8) ? EPD::HEIGHT :	       \
				(MAX_DISPLAY_BUFFER_SIZE / 2) /		       \
				(EPD::WIDTH / 8))

#define DEF_FONT		u8g2_font_osb41_tr
#define SYMBOL_FONT		u8g2_font_open_iconic_human_6x_t

GxEPD2_3C<GxEPD2_154c, MAX_HEIGHT_3C(GxEPD2_154c)> display(
					GxEPD2_154c(
						/*CS=10*/	SS, 
						/*DC=*/		9, 
						/*RST=*/	8, 
						/*BUSY=*/	7
					));

RTC_DS3231 rtc;
U8G2_FOR_ADAFRUIT_GFX u8g2Fonts;

void show();
void draw(const char text[], const uint8_t *font, uint16_t color,
		 uint8_t x, uint8_t y);

void setup(void)
{
	Serial.begin(57600);
	Serial.println();
	Serial.println("setup");
	delay(100);

	display.init(57600);
	display.setRotation(1);
	display.fillScreen(GxEPD_WHITE);

	u8g2Fonts.begin(display);
}

void loop()
{
	show();
	delay(1000);
	display.powerOff();
	display.hibernate();
}

void show()
{
	/* DateTime now = rtc.now(); */
	/* char Time[10]; */

	/* sprintf(Time, "%d:%d:%d", now.hour(), now.minute(), now.second()); */

	u8g2Fonts.setFontMode(1);
	u8g2Fonts.setFontDirection(0);
	u8g2Fonts.setForegroundColor(GxEPD_BLACK);
	u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
	u8g2Fonts.setFont(DEF_FONT);

	display.firstPage();

	do {
		display.fillScreen(GxEPD_WHITE);
		draw("BBBB", SYMBOL_FONT, GxEPD_RED, 5, 48);
		draw("12345", DEF_FONT, GxEPD_RED, 20, 115);
		draw("BBBB", SYMBOL_FONT, GxEPD_RED, 5, 190);
	} while (display.nextPage());

}

void draw(const char text[], const uint8_t *font, uint16_t color,
		 uint8_t x, uint8_t y)
{
	u8g2Fonts.setCursor(x, y);
	u8g2Fonts.setForegroundColor(color);
	u8g2Fonts.setFont(font);
	u8g2Fonts.print(text);
}
