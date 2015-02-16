#include <LiquidCrystal.h>
#include <OneWire.h>
#include <DallasTemperature.h>

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

int tempPin = 3; 
int lightPin = 2;
int targetTemp = 24; // C
float celsius; // Float to hold sensor value

// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 3

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature tempSensors(&oneWire);

void setup()
{
	Serial.begin(9600);

	// Setup LCD
	lcd.begin(16, 2);

	// Set light/heat to output
	pinMode(lightPin, OUTPUT);

	// Turn light/Heatsource on to start
	digitalWrite(lightPin, HIGH);

	displayTargetMessage();

	// Start up the library
	tempSensors.begin(); // IC Default 9 bit. If you have troubles consider upping it 12. Ups the delay giving the IC more time to process the temperature measurement

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
		digitalWrite(lightPin, LOW);
	}
	else
	{
		digitalWrite(lightPin, HIGH);
	}
}

void reportTemp(float temp)
{
	Serial.print(temp);
	Serial.println(" C");



	Serial.print(convertToF(temp));
	Serial.println(" F");

	lcd.setCursor(0, 1);
	String message = "Current: ";
	message += convertToF(temp);
	message += " F";
	lcd.print(message);
}

float readTempSensor()
{
	// Using the factor is more effcient but not as clear what is going on
	// celsius = analogRead(tempPin) * 0.48875855;

	celsius = 0;

	// I am not sure if we need this since we are using digital...
	//// get 8 samples then report the average
	//for (int i = 0; i <= 7; i++)
	//{
	//	tempSensors.requestTemperatures();
	//	celsius += tempSensors.getTempCByIndex(0); //TODO: This may be slow, look for faster way
	//}

	//// Get the avg
	//celsius = celsius / 8;

	tempSensors.requestTemperatures();

	celsius = tempSensors.getTempCByIndex(0);

	return celsius;
}

float convertToF(float celsius)
{
	return celsius * 1.8000 + 32.00;
}

void displayTargetMessage()
{
	lcd.setCursor(0, 0);
	String message = "Target: ";
	message += convertToF(targetTemp);
	message += " F";
	lcd.print(message);
}
