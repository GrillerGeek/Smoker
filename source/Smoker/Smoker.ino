#include <LiquidCrystal.h>
#include <OneWire.h>
#include <DallasTemperature.h>

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

int tempPin = 3;
int lightPin = 2;
int upPin = 4;
int downPin = 5;

int upState = HIGH;      // the current state of the output pin
int upReading;           // the current reading from the input pin
int upPrevious = LOW;    // the previous reading from the input pin

int downState = HIGH;      // the current state of the output pin
int downReading;           // the current reading from the input pin
int downPrevious = LOW;    // the previous reading from the input pin

long upTime = 0;         // the last time the output pin was toggled
long downTime = 0;         // the last time the output pin was toggled
long debounce = 50;  // the debounce time, increase if the output flickers

float targetTemp = 200.00; // F taget temp
float threshold = 3.00;
float increment = 5.00;
float highThreshold;
float lowThreshold;

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
	pinMode(upPin, INPUT_PULLUP);
	pinMode(downPin, INPUT_PULLUP);

	// Turn on Heatsource on to start
	digitalWrite(lightPin, LOW);

	// Set thresholds
	setThresholds();

	displayTargetMessage();

	// Start up the library
	tempSensors.begin(); // IC Default 9 bit. If you have troubles consider upping it 12. Ups the delay giving the IC more time to process the temperature measurement

}

void loop()
{
	readButton(upPin, "UP");

	readButton(downPin, "DOWN");

	float reading = readTempSensor();

	reportTemp(reading);

	toggleHeat(reading);
}

void readButton(int buttonPin, String button)
{
	int reading = digitalRead(buttonPin);

	// if the input just went from LOW and HIGH and we've waited long enough
	// to ignore any noise on the circuit, toggle the output pin and remember
	// the time
	if (button == "UP")
	{
		if (reading == LOW && upPrevious == HIGH && millis() - upTime > debounce) {
			if (upState == LOW)
				upState = HIGH;
			else
				upState = LOW;

			upTime = millis();
			Serial.println("Up button pushed");
			targetTemp += increment;
			setThresholds();
			displayTargetMessage();
		}

		upPrevious = reading;
	}
	
	if (button == "DOWN")
	{
		if (reading == LOW && downPrevious == HIGH && millis() - downTime > debounce) {
			if (downState == LOW)
				downState = HIGH;
			else
				downState = LOW;

			downTime = millis();
			Serial.println("Down button pushed");
			targetTemp -= increment;
			setThresholds();
			displayTargetMessage();
		}

		downPrevious = reading;
	}
}


void setThresholds()
{
	highThreshold = targetTemp + threshold;
	lowThreshold = targetTemp - threshold;
}

void toggleHeat(float temp)
{
	if (temp >= highThreshold)
	{
		// turn off
		digitalWrite(lightPin, HIGH);
	}
	else if (temp <= lowThreshold)
	{
		// turn on
		digitalWrite(lightPin, LOW);
	}
}

void reportTemp(float temp)
{
	Serial.print(temp);
	Serial.println(" F");

	lcd.setCursor(0, 1);
	String message = "Current: ";
	message += temp;
	message += " F";
	lcd.print(message);
}

float readTempSensor()
{
	float temp = 0;
	tempSensors.requestTemperatures();
	temp = tempSensors.getTempFByIndex(0);

	return temp;
}

void displayTargetMessage()
{
	lcd.setCursor(0, 0);
	String message = "Target: ";
	message += targetTemp;
	message += " F";
	lcd.print(message);
}
