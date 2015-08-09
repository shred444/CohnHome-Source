#include <Bridge.h>
#include <Parse.h>
#include <Console.h>
#include <Temboo.h>
#include "TembooAccount.h" // contains Temboo account information
#include <EEPROM.h>

//Global Variables
int runCounter = 0;
const int ledPin = 13; //red led on Yun

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
	//startPushService();
	
	
	char lastMsgID[10];
	EEPROM.put(0, "hello");
	EEPROM.get(0, lastMsgID);
	Serial.println("Last direct message id: " + String(lastMsgID));


	//String homeStatus = runCurl();
	//Serial.println(homeStatus);
	getHomeStatus("jenIsHome");
}

String runCurl(){

	String buffer = "";
	Process p;
	p.begin("curl");
	//p.addParameter("http://arduino.cc/asciilogo.txt");
	p.addParameter("https://docs.google.com/spreadsheets/d/1XJpDbeshpUnjyYekKR5MkiXk62Qi8-vhrGZboEjht40/pub?output=csv");
	p.addParameter("-k");
	p.run();
	Serial.print("Running cURL... ");
	while (p.available()>0) {
		char c = p.read();
		buffer+=c;
		//Serial.print(c);
	}

	
	Serial.flush();
	Serial.println("Complete");
	//Serial.println(buffer);
	return buffer;
}

bool getHomeStatus(String key) {
	String homeStatus = runCurl();
	if (homeStatus.substring(0) == key) {
    	Serial.println("Found it"); 
	}else{
		Serial.println("Missing Key");
	}
	
}



void loop() {
	// put your main code here, to run repeatedly:
	Serial.println("Running " + String(runCounter));
	
	//checkPushes();

	
	delay(1000);

	runCounter++;
}
