#include <config.h>
#include <message_types.h>
#include "DHT.h"
#include <SimpleTimer.h>

#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

DHT dht(DHTPIN, DHTTYPE);

SimpleTimer doorTimer;
SimpleTimer tempTimer;


// nRF24L01(+) radio attached using Getting Started board 
RF24 radio(9,10);

// Network uses that radio
RF24Network network(radio);

// Address of our node
const uint16_t this_node = 1;

// Address of the other node
const uint16_t other_node = 0;

// When did we last send?
unsigned long last_sent;

// How many have we sent already
unsigned long packets_sent;


void setup() {
	Serial.begin(9600);
	while(!Serial);
	
	Serial.println("Application Starting");

	SPI.begin();
	radio.begin();
	network.begin(/*channel*/ 90, /*node address*/ this_node);
	Serial.println("Radio Setup");

	pinMode(pin, OUTPUT);
	dht.begin();
  	//attachInterrupt(0, garageTrigger, CHANGE);
	doorTimer.setInterval(1000, checkGarage); 
	tempTimer.setInterval(5000, checkTemp); 
	Serial.println("Timer Setup");
  	
}

void loop() {
	// put your main code here, to run repeatedly:
	//sensorValue = analogRead(garageInput);
	network.update();
	doorTimer.run();
	tempTimer.run();
}

void checkGarage(){
	bool currentState = digitalRead(garageInput);
	if(currentState != garageState)
	{
		garageState = currentState;
   		//Serial.println("Change " + String(garageState));	
		
		String doorStatus;
		if (currentState)
			doorStatus = "Closed"; //Serial.println("Closed");
		else
			doorStatus = "Opened"; //Serial.println("Open");
		
		Serial.print("Garage Door " + doorStatus);
	}

}

String createDashing(String widget, String value){

	return "{\"auth_token\":\"jc\", " + value + "}";
}

void checkTemp(){
	// Reading temperature or humidity takes about 250 milliseconds!
	// Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
	float h = dht.readHumidity();
	// Read temperature as Celsius (the default)
	float t = dht.readTemperature();
	// Read temperature as Fahrenheit (isFahrenheit = true)
	float f = dht.readTemperature(true);
	
	// Check if any reads failed and exit early (to try again).
	if (isnan(h) || isnan(t) || isnan(f)) {
		Serial.println("Failed to read from DHT sensor!");
		return;
	}
	Serial.println("Temp: " + String(f) + "   Humidity: " + String(h));

	//send the message
	S_message payload = {
		f,h
	};
    RF24NetworkHeader header(/*to node*/ other_node,'S');
    bool ok = network.write(header,&payload,sizeof(payload));
    if (ok)
      Serial.println("ok.");
    else
      Serial.println("failed.");
}

void garageTrigger()
{
	
	static unsigned long last_interrupt_time = 0;
	unsigned long interrupt_time = millis();
	// If interrupts come faster than 200ms, assume it's a bounce and ignore
	if (interrupt_time - last_interrupt_time > 200) 
	{
		delay(100);
		int temp = digitalRead(garageInput);
	Serial.println("Change " + String(temp));	
	}
	last_interrupt_time = interrupt_time;

}
