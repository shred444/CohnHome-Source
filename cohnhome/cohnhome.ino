#include <Bridge.h>
#include <Parse.h>

//Global Variables
int runCounter = 0;

void setup() {
	Serial.begin(9600);
	delay(4000);
	while(!Serial);
	Serial.println("Application Starting");

	Serial.print("Initializing the bridge... ");
	Bridge.begin(250000);
	Serial.println("Complete");
	//Console.begin();
	//while (!Console);
	//Serial.println("You're connected to the Console!!!!");

	// Initialize Parse
	Serial.print("Initializing Parse... ");
	Parse.begin("RZHVMzw0mIcePiDXxkn5D1RXVqQwcoAQBSyqLZuO", "CjNQPQP75JgCu5OGEkZvNyw2fZnv3F4fJ9m31Jeu");
	Serial.println("Complete");

	//createParseObject();
	
	// Start push service
	//Parse.startPushService();
	//Serial.print("Push Installation ID:");
	//Serial.println(Parse.getInstallationId());

	/*
	String lastMsgID = "Hello World";
	EEPROM.put(0, lastMsgID);
	EEPROM.get(0, lastMsgID);
	Serial.println("Last direct message id: " + lastMsgID);
	*/
}

void loop() {
	// put your main code here, to run repeatedly:
	Serial.println("Running" + String(runCounter));
	delay(1000);

	runCounter++;
}
