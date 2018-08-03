// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 13;
//String inputString = "";         // a string to hold incoming data
//boolean stringComplete = false;  // whether the string is complete

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);   
  Serial.begin(115200);               //initial the Serial  
  // reserve 200 bytes for the inputString:
  //inputString.reserve(200);
}

// the loop routine runs over and over again forever:
void loop() {
  //serialEvent(); //call the function
  // print the string when a newline arrives:
  //if (stringComplete) {
  //  Serial.println(inputString);
  //  // clear the string:
  //  inputString = "";
  //  stringComplete = false;
  //}
  
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);               // wait for a second
  if(Serial.available())
    {
        Serial.write(Serial.read());    //send what has been received
    }  
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
//void serialEvent() {
//  while (Serial.available()) {
//    // get the new byte:
//    char inChar = (char)Serial.read();
//    // add it to the inputString:
//    inputString += inChar;
//    // if the incoming character is a newline, set a flag
//    // so the main loop can do something about it:
//    if (inChar == '\n') {
//      stringComplete = true;
//    }
//  }
//}
