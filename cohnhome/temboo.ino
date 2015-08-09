
void sendDirectMessage(String user, String message) {
	Serial.println("Running SendDirectMessage - Run #" + String(runCounter));
    
    TembooChoreo SendDirectMessageChoreo;

    // Invoke the Temboo client
    SendDirectMessageChoreo.begin();

    // Set Temboo account credentials
    SendDirectMessageChoreo.setAccountName(TEMBOO_ACCOUNT);
    SendDirectMessageChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
    SendDirectMessageChoreo.setAppKey(TEMBOO_APP_KEY);

    // Set profile to use for execution
    SendDirectMessageChoreo.setProfile("CohnHomeTwitter2");
  
    // Set Choreo inputs
    SendDirectMessageChoreo.addInput("Text", message);
    SendDirectMessageChoreo.addInput("ScreenName", user);
    
    // Identify the Choreo to run
    SendDirectMessageChoreo.setChoreo("/Library/Twitter/DirectMessages/SendDirectMessage");
    
    // Run the Choreo; when results are available, print them to serial
    SendDirectMessageChoreo.run();
    
	while(SendDirectMessageChoreo.available()) {
		char c = SendDirectMessageChoreo.read();
		Serial.print(c);
		String name = SendDirectMessageChoreo.readStringUntil('\x1F');
		name.trim();

	}
    
    SendDirectMessageChoreo.close();

	Serial.println("Waiting...");
	delay(30000); // wait 30 seconds between SendDirectMessage calls
}



void runGetLatestDirectMessage() {
	TembooChoreo GetLatestDirectMessageChoreo;
	
	// Invoke the Temboo client
	GetLatestDirectMessageChoreo.begin();
	
	// Set Temboo account credentials
	GetLatestDirectMessageChoreo.setAccountName(TEMBOO_ACCOUNT);
	GetLatestDirectMessageChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
	GetLatestDirectMessageChoreo.setAppKey(TEMBOO_APP_KEY);
	
	// Set profile to use for execution
	GetLatestDirectMessageChoreo.setProfile("CohnHomeTwitter2");
	// Identify the Choreo to run
	GetLatestDirectMessageChoreo.setChoreo("/Library/Twitter/DirectMessages/GetLatestDirectMessage");
	
	// Run the Choreo
	unsigned int returnCode = GetLatestDirectMessageChoreo.run();
	
	// A return code of zero means everything worked
	if (returnCode == 0) {
		while (GetLatestDirectMessageChoreo.available()) {
			String name = GetLatestDirectMessageChoreo.readStringUntil('\x1F');
			name.trim();
			
			String data = GetLatestDirectMessageChoreo.readStringUntil('\x1E');
			data.trim();
			
			if (name == "Text") {
				if (data == "Ping") {
					Serial.println("Ping Received. Turning on the LED");
					digitalWrite(ledPin, HIGH);
				}else{
					Serial.println("Unknown command received: " + data);
				}
			}else if(name == "ID") {
				String msgID = data;
				//EEPROM.put(0, msgID);
			}
		}
	}
	
	GetLatestDirectMessageChoreo.close();
}
