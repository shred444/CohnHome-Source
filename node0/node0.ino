#include <config.h>
#include <message_types.h>
#include <Bridge.h>
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

// nRF24L01(+) radio attached using Getting Started board 
RF24 radio(9,10);

// Network uses that radio
RF24Network network(radio);

// Address of our node
const uint16_t this_node = 0;

// Address of the other node
const uint16_t other_node = 1;



String createDashing(String widget, String value){

	return "{\"auth_token\":\"jc\", " + value + "}";
}

void setup(void)
{
  Serial.begin(9600);
  while(!Serial);

  Bridge.begin();
  Serial.println("Bridge Started");
 
  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 90, /*node address*/ this_node);
}

void loop(void)
{
	// Pump the network regularly
	network.update();
	
	// Is there anything ready for us?
	while ( network.available() )
	{
		// If so, grab it and print it out
		RF24NetworkHeader header;
		network.peek(header);
		//printf_P(PSTR("%lu: APP Received #%u type %c from 0%o\n\r"),millis(),header.id,header.type,header.from_node);
		
		if (header.type == 'S')
		{
		    S_message payload;
		    network.read(header,&payload,sizeof(payload));
		    Serial.print("Rx packet #" + String(header.id) + " from Node" + String(header.from_node));
		    Serial.println("Temp: " + String(payload.temp_reading) + "   Humidity: " + String(payload.humidity_reading));
			
			Process p;            
			p.begin("curl");  
			p.addParameter("-d");
			p.addParameter(createDashing("temperature", "\"current\":" + String(payload.temp_reading)));
			p.addParameter(String(server_ip) + "/widgets/temperature"); 	  
			p.run();
			
			p.begin("curl");  
			p.addParameter("-d");
			p.addParameter(createDashing("temperature", "\"value\":" + String(payload.humidity_reading)));
			p.addParameter(String(server_ip) + "/widgets/humidity"); 	  
			p.run();

		}
	}
}
// vim:ai:cin:sts=2 sw=2 ft=cpp
