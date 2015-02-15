#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

int tempPin = 0; // Analog pin for sensor
int lightPen = 2;
int targetTemp = 24; // C
float celsius; // Float to hold sensor value

void setup()
{
	// Setup LCD
	lcd.begin(16, 2);

	Serial.begin(9600);
	pinMode(2, OUTPUT);

	// Turn light on to start
	digitalWrite(lightPen, HIGH);

	lcd.setCursor(0, 0);
	String message = "Target: ";
	message += targetTemp * 1.8000 + 32.00;
	message += " F";
	lcd.print(message);
}

void loop()
{
	float reading = readTempSensor();

	reportTemp(reading);

	toggleLight(reading);

	delay(5000);

}

void toggleLight(float temp)
{
	// If temp is above 24c Turn light off
	if (temp >= targetTemp)
	{
		digitalWrite(lightPen, LOW);
	}
	else
	{
		digitalWrite(lightPen, HIGH);
	}
}

void reportTemp(float temp)
{
	Serial.print(temp);
	Serial.println(" C");

	

	Serial.print(temp * 1.8000 + 32.00);
	Serial.println(" F");

	lcd.setCursor(0, 1);
	String message = "Current: ";
	message += temp * 1.8000 + 32.00;
	message += " F";
	lcd.print(message);
}

float readTempSensor()
{
	// Using the factor is more effcient but not as clear what is going on
	// celsius = analogRead(tempPin) * 0.48875855;

	celsius = 0;

	// get 8 samples then report the average
	for (int i = 0; i <= 7; i++)
	{
		celsius += (5 * analogRead(tempPin) * 100) / 1024;
	}

	// Get the avg
	celsius = celsius / 8;

	return celsius;
}