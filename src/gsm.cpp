#include "gsm.h"
#include "utilities.h"
#include "relay.h"

void gsm::init(){
	SerialMon.begin(115200);
	SerialAT.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);

	pinMode(MODEM_RST, OUTPUT);
	pinMode(MODEM_PWRKEY, OUTPUT);
	pinMode(MODEM_POWER_ON, OUTPUT);
	
	// Keep reset high
	digitalWrite(MODEM_RST, HIGH);

	// Turn on the Modem power first
	digitalWrite(MODEM_POWER_ON, HIGH);

	// Pull down PWRKEY for more than 1 second according to manual requirements
	digitalWrite(MODEM_PWRKEY, HIGH);
	delay(100);
	digitalWrite(MODEM_PWRKEY, LOW);
	delay(1000);
	digitalWrite(MODEM_PWRKEY, HIGH);


	// intialize sim800L
	sendCommand("AT");
	delay(3000);
	sendCommand("+CNETLIGHT=0"); // turn off LED
	sendCommand("AT+CMGF=1"); // Set the SMS mode to text mode
	sendCommand("AT+CNMI=2,2,0,0,0"); // Decides how newly arrived SMS messages should be handled
}

void gsm::sendCommand(String command){
	SerialAT.println(command);
	SerialAT.flush();

	String line = SerialAT.readStringUntil('\n');
	SerialMon.print("> ");
	SerialMon.println(command);
	SerialMon.print("< ");
	SerialMon.println(line);
}

void gsm::sendSMS(String number, String message){
	sendCommand("AT+CMGF=1");
	sendCommand("AT+CMGS=\"" + number + "\"");
	sendCommand(message);
	sendCommand("\x1A");
}

void gsm::update(){
	// forward debug serial to AT module
	while (SerialMon.available()){
		int c = SerialMon.read();
		SerialAT.write(c); // Forward debug input to AT module
		SerialMon.write(c); // Forward debug input to AT module
	}
	// forward AT output to serial monitor and parse outputs
	while(SerialAT.available()){
		String line = SerialAT.readStringUntil('\n');
		SerialMon.println(line);

		// parse outputs
		if(line.startsWith("+CMT")){ // new SMS received
			// get the phone number
			String number = line.substring(line.indexOf("\"") + 1);
			number = number.substring(0, number.indexOf("\""));

			SerialMon.print("=== Received SMS:  ===\n");
			while (SerialAT.available()){
				line = SerialAT.readStringUntil('\n');
				int id = line.substring(1,2).toInt();
				relay::set(id-1, line.substring(3,4).toInt());
				SerialMon.println(line);
			}
			String msg = "Relay state: \n R1=" + String(relay::state[0]) + " R2=" + String(relay::state[1]) + " R3=" + String(relay::state[2]) + " R4=" + String(relay::state[3]);
			sendSMS(number, msg);
			
			SerialMon.print("=== SMS END === \n");
		}
	}

}

