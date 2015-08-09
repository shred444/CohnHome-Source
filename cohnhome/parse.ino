
void createParseObject() {

	Serial.print("Creating Parse object... ");
	
	// Create Object
	ParseObjectCreate create;
	create.setClassName("TestObject");
	create.add("foo", "bar");
	ParseResponse response = create.send();
	 
	//Serial.println("\nResponse for saving a TestObject:");
	//Serial.print(response.getJSONBody());
	if (!response.getErrorCode()) {
		String objectId = response.getString("objectId");
		//Serial.print("Test object id:");
		//Serial.println(objectId);
		Serial.println("Complete");
	} else {
		Serial.println("Failed to save the object");
	}
	response.close(); // Do not forget to free the resource
}



void startPushService(){
	// Start push service
	Serial.print("Starting Push Service... ");
	Parse.startPushService();
	//Serial.print("Push Installation ID:");
	//Serial.println(Parse.getInstallationId());
	Serial.println("Complete");
}

void checkPushes() {
		
	// Check if there is a new push
	// A push with message {"alert":"A test push from Parse!"}
	// will turn on LED for 3 seconds

	if (Parse.pushAvailable()) {
		ParsePush push = Parse.nextPush();
		
		String message = push.getJSONBody();
		Serial.print("New push message size: ");
		Serial.println(message.length());
		Serial.print("New push message content: ");
		Serial.println(message);
		
		String command = push.getString("alert");
		if (command == "A test push from Parse!") {
			digitalWrite(ledPin, HIGH);
			delay(3000);  // wait 3 seconds
			digitalWrite(ledPin, LOW);
		}
		// NOTE: ensure to close current push message
		// otherwise next push won't be available
		push.close();
	}else{
		//Serial.println("No push alerts available");
		
	}
}
