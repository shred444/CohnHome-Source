#include <Bridge.h>
#include <Parse.h>
#include <Console.h>
#include <Temboo.h>
#include "TembooAccount.h" // contains Temboo account information
#include <EEPROM.h>
#include <SimpleTimer.h>

//Global Variables
int runCounter = 0;
const int ledPin = 13; //red led on Yun
SimpleTimer timer;
int tweetCounter = 0;
int maxTweets = 2;

const char sender[] PROGMEM = "https://spreadsheets.google.com/feeds/cells/1XJpDbeshpUnjyYekKR5MkiXk62Qi8-vhrGZboEjht40/omtx7hz/private/full/R3C2?access_token=ya29.ywHuvz4MqrLY1mzl40zFeGeM_KBdEN2Ov99RbBcxaIIqrQCkylD1y1VFP39rFoFQBV2O";
const char csv[] PROGMEM = "https://docs.google.com/spreadsheets/d/1XJpDbeshpUnjyYekKR5MkiXk62Qi8-vhrGZboEjht40/pub?output=csv&id=1XJpDbeshpUnjyYekKR5MkiXk62Qi8-vhrGZboEjht40";


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
	
	/*
	char lastMsgID[10];
	EEPROM.put(0, "hello");
	EEPROM.get(0, lastMsgID);
	Serial.println("Last direct message id: " + String(lastMsgID));
	*/

	String quote = String("\"");
	Process p;
	//p.runShellCommand("echo hello > /tmp/jonathan.txt");
	//p.runShellCommand("curl  https://spreadsheets.google.com/feeds/cells/1XJpDbeshpUnjyYekKR5MkiXk62Qi8-vhrGZboEjht40/omtx7hz/private/full/R3C2?access_token=ya29.ywHuvz4MqrLY1mzl40zFeGeM_KBdEN2Ov99RbBcxaIIqrQCkylD1y1VFP39rFoFQBV2O -k > /tmp/jonathan.txt");  
	//p.runShellCommand("sh /tmp/google.txt > /tmp/jonathan.txt");
	//p.runShellCommand(sender);
	//p.runShellCommand("echo mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm1234567890123 >> /tmp/goo.txt");


	
	//p.begin("curl");
	//p.addParameter("google.com");
	//p.addParameter("https://spreadsheets.google.com/feeds/cells/1XJpDbeshpUnjyYekKR5MkiXk62Qi8-vhrGZboEjht40/omtx7hz/private/full/R3C2?access_token=ya29.ywHx2KhG_IZyV46cuplazQQQIqSI9n24fV2PH3m_3wiRw0LnUofg6F291F_cSch69tnz");
	//p.addParameter("-H");
	//p.addParameter("\"Authorization: Bearer ya29.ywHx2KhG_IZyV46cuplazQQQIqSI9n24fV2PH3m_3wiRw0LnUofg6F291F_cSch69tnz\"");
	//p.addParameter("-k");

	//p.addParameter("https://spreadsheets.google.com/feeds/cells/1XJpDbeshpUnjyYekKR5MkiXk62Qi8-vhrGZboEjht40/omtx7hz/private/full/R3C2 -H \"Authorization: Bearer ya29.ywHx2KhG_IZyV46cuplazQQQIqSI9n24fV2PH3m_3wiRw0LnUofg6F291F_cSch69tnz\" -k");
	/*p.run();
	delay(1000);
	while (p.available()>0) {
		char c = p.read();
		Serial.print(c);
	}
	*/
	Serial.println("Bearer");
	timer.setInterval(5000,checkHome);
}


void checkHome(){
	static bool lastCheck = getHomeStatus("anyoneHome");
	if(getHomeStatus("anyoneHome")) {
		Serial.println("Someone's Home!");
		digitalWrite(ledPin, HIGH);
	}else{
		Serial.println("House is Empty!");
		digitalWrite(ledPin, LOW);
	}	

	if (getHomeStatus("anyoneHome") != lastCheck){
		//send a tweet
		if(tweetCounter < maxTweets){
			sendDirectMessage("shred444", ("Home Alone! Run="));
			tweetCounter++;
		}else{
			Serial.println("Too Many Tweets");
		}
	}

	lastCheck = getHomeStatus("anyoneHome");

	runCounter++;

}
String runCurl(){

	String buffer = "";
	Process p;
	p.begin("curl");
	Serial.println(String(csv));
	p.addParameter("https://docs.google.com/spreadsheets/d/1XJpDbeshpUnjyYekKR5MkiXk62Qi8-vhrGZboEjht40/pub?output=csv&id=1XJpDbeshpUnjyYekKR5MkiXk62Qi8-vhrGZboEjht40");
	p.addParameter("-k");
	p.run();
	//Serial.print("Running cURL... ");
	while (p.available()>0) {
		char c = p.read();
		buffer+=c;
		//Serial.print(c);
	}

	
	Serial.flush();
	//Serial.println("Complete");
	Serial.println(buffer);
	return buffer;
}

bool getHomeStatus(String key) {
	String homeStatus = runCurl();
	String result;
	//Serial.println("Looking for " + key + " in " + homeStatus);
	int keypos;
	
	if (keypos = homeStatus.indexOf(key)) {
    	int firstcomma = homeStatus.indexOf(',',keypos);
    	int endOfLine = homeStatus.indexOf('\r',firstcomma+1);
    	//Serial.println("Found key at " + String(keypos) + " " + String(firstcomma) + " " + String(endOfLine));
    	result = homeStatus.substring(firstcomma+1,endOfLine);
    	result.trim();
    	Serial.println("Value=" + result + ".");
    	if (result == "TRUE")
    		return true;
    	else
    		return false;
    	
	}else{
		Serial.println("Missing Key");
	}

	
	
}



void loop() {
	// put your main code here, to run repeatedly:
	//Serial.println("Running " + String(runCounter));
	timer.run();
	//checkPushes();


}
