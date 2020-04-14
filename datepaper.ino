#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>

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
#define HEAD_FONT		u8g2_font_osb21_tr
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

const DateTime theDay (2018, 6, 23, 0, 0, 0);
// 10800 * 8s = 1d
int limit = 10800;

void show();
void showDef(char days[]);
void showXmas(char days[]);
void showNewYear(char days[]);
void showBday(char days[]);
void myWatchdogEnable(const byte interval);
int getOffsetToCenter(char days[]);

ISR(WDT_vect)
{
	wdt_disable();
}

void setup(void)
{
	DateTime now = rtc.now();
	int limit_reducer = 0;

	// reduce limit of first loop once
	// to make sure interrupt comes around 00.00
	limit_reducer = now.second();
	limit_reducer = limit_reducer + (now.minute() * 60);
	limit_reducer = limit_reducer + (now.hour() * 60 * 60);
	limit_reducer = (int) limit_reducer / 8;
	limit = (limit - limit_reducer) + 1;

	display.init(57600);
	display.setRotation(1);
	display.fillScreen(GxEPD_WHITE);

	u8g2Fonts.begin(display);
}

void loop()
{
	show();

	display.powerOff();
	display.hibernate();

	for (int i = 0; i < limit; i++)
		myWatchdogEnable (0b100001);  // 8 seconds

	// reset limit to keep waking up around 00.00
	// 10800 * 8s = 1d
	limit = 10800;

	sleep_disable();
	power_all_enable();
}

void draw(const char text[], const uint8_t *font, uint16_t color,
		 uint8_t x, uint8_t y)
{
	u8g2Fonts.setCursor(x, y);
	u8g2Fonts.setForegroundColor(color);
	u8g2Fonts.setFont(font);
	u8g2Fonts.print(text);
}

void show()
{
	DateTime now = rtc.now();
	TimeSpan ts = now - theDay;
	char days[5];

	sprintf(days, "%d", ts.days());

	if (now.month() == 12 && now.day() == 24)
		showXmas(days);
	else if (now.month() == 1 && now.day() == 1)
		showNewYear(days);
	else if (now.month() == 6 && now.day() == 18)
		showBday(days);
	else
		showDef(days);
}

void u8g2init()
{
	u8g2Fonts.setFontMode(1);
	u8g2Fonts.setFontDirection(0);
	u8g2Fonts.setForegroundColor(GxEPD_BLACK);
	u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
}

void showDef(char days[])
{
	u8g2init();

	display.firstPage();

	do {
		display.fillScreen(GxEPD_WHITE);
		draw("BBBB", SYMBOL_FONT, GxEPD_RED, 5, 48);
		draw(days, DEF_FONT, GxEPD_BLACK, getOffsetToCenter(days), 115);
		draw("BBBB", SYMBOL_FONT, GxEPD_RED, 5, 192);
	} while (display.nextPage());
}

void showXmas(char days[])
{
	u8g2init();

	display.firstPage();

	do {
		display.fillScreen(GxEPD_WHITE);
		draw("B", SYMBOL_FONT, GxEPD_RED, 5, 48);
		draw("happy", HEAD_FONT, GxEPD_BLACK, 63, 34);
		draw("B", SYMBOL_FONT, GxEPD_RED, 147, 48);
		draw(days, DEF_FONT, GxEPD_BLACK, getOffsetToCenter(days), 115);
		draw("B", SYMBOL_FONT, GxEPD_RED, 5, 192);
		draw("Xmas", HEAD_FONT, GxEPD_BLACK, 67, 178);
		draw("B", SYMBOL_FONT, GxEPD_RED, 147, 192);
	} while (display.nextPage());
}

void showNewYear(char days[])
{
	u8g2init();

	display.firstPage();

	do {
		display.fillScreen(GxEPD_WHITE);
		draw("B", SYMBOL_FONT, GxEPD_RED, 5, 48);
		draw("happy", HEAD_FONT, GxEPD_BLACK, 63, 34);
		draw("B", SYMBOL_FONT, GxEPD_RED, 147, 48);
		draw(days, DEF_FONT, GxEPD_BLACK, getOffsetToCenter(days), 115);
		draw("B", SYMBOL_FONT, GxEPD_RED, 5, 192);
		draw("new", HEAD_FONT, GxEPD_BLACK, 75, 158);
		draw("year", HEAD_FONT, GxEPD_BLACK, 73, 188);
		draw("B", SYMBOL_FONT, GxEPD_RED, 147, 192);
	} while (display.nextPage());
}

void showBday(char days[])
{
	u8g2init();

	display.firstPage();

	do {
		display.fillScreen(GxEPD_WHITE);
		draw("B", SYMBOL_FONT, GxEPD_RED, 5, 48);
		draw("happy", HEAD_FONT, GxEPD_BLACK, 63, 34);
		draw("B", SYMBOL_FONT, GxEPD_RED, 147, 48);
		draw(days, DEF_FONT, GxEPD_BLACK, getOffsetToCenter(days), 115);
		draw("B", SYMBOL_FONT, GxEPD_RED, 5, 192);
		draw("bday", HEAD_FONT, GxEPD_BLACK, 73, 178);
		draw("B", SYMBOL_FONT, GxEPD_RED, 147, 192);
	} while (display.nextPage());
}

void myWatchdogEnable(const byte interval)
{ 
	MCUSR = 0;                          // reset various flags
	WDTCSR |= 0b00011000;               // see docs, set WDCE, WDE
	WDTCSR =  0b01000000 | interval;    // set WDIE, and appropriate delay

	wdt_reset();
	power_adc_disable();
	power_spi_disable();
	power_timer0_disable();
	power_timer2_disable();
	set_sleep_mode(SLEEP_MODE_PWR_SAVE); 
	sleep_mode();
} 

int getOffsetToCenter(char days[])
{
	if (strlen(days) > 4)
		return 17;
	else if (strlen(days) > 3)
		return 34;
	else
		return 51;
}
