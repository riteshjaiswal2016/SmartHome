#include <SPI.h>
#include <Ethernet.h>

EthernetServer server(7412);// Server port


const byte BedroomLightPin1 = 3;// Select pin to control BedroomLight Relay1
const byte BedroomLightPin2 = 4;// Select pin to control BedroomLight Relay2
const byte KitchenLightPin1 = 5;// Select pin to control KitchenLight Relay1
const byte KitchenLightPin2 = 6;// Select pin to control KitchenLight Relay2
const byte HallFanPin1 = 7;// Select pin to control HallFan Relay1
const byte HallFanPin2 = 8;// Select pin to control HallFan Relay2

byte mac[] = { 0x00, 0x08, 0xDC, 0xAB, 0xCD, 0xEF };// Physical MAC address
byte ip[] = { 200, 10, 10, 3 };// Fixed IP address
byte gateway[] = { 200, 10,10 , 1 };// Router Gateway Internet access
byte subnet[] = { 255, 255, 255, 0 };// Subnet mask

String readString;
boolean logout=true;

void setup()
{
delay(300);// Delay for Ethernet shield initialization (Arduino has 65mS Power Up delay and W5100 reset logic has 280mS)
pinMode(BedroomLightPin1, OUTPUT);// Define pin for Bedroom Light Relay1 as Output
pinMode(BedroomLightPin2, OUTPUT);// Define pin for Bedroom Light Relay2 as Output
pinMode(KitchenLightPin1, OUTPUT);// Define pin for Kitchen Light Relay1 as Output
pinMode(KitchenLightPin2, OUTPUT);// Define pin for Kitchen Light Relay2 as Output
pinMode(HallFanPin1, OUTPUT);// Define pin for HallFan Relay1 as Output
pinMode(HallFanPin2, OUTPUT);// Define pin for HallFan Relay2 as Output


Serial.begin(9600);// Initialize serial communications at 9600 bps
Serial.println("Arduino Based Home Automation Project");	// Display Arduino title
Ethernet.begin(mac, ip, gateway, subnet);	// Start Ethernet

server.begin();
Serial.print("Ethernet Shield initialized. Local IP address is:");
Serial.println(Ethernet.localIP());			// Print Server IP address
}


void loop(){
EthernetClient client = server.available();// Create a client connection

if (client == true){
while (client.connected()){
if (client.available()){
char c = client.read();					  // Read char by char HTTP request

if (readString.length() < 100){
readString = readString + c;			// Store characters to string
}

if (c == '\n'){                   //NewLine in http request means complete request read in readString variable 
Serial.println(readString);

if(readString.indexOf("loggedout") > 0){
client.println("http/1.1 200 ok");            // Send  
client.println("content-type: text/html");    //standard http
client.println();                             //headers

client.println("<!doctype html><html>");
client.println("<body bgcolor='blue'>");
client.println("<center><head><title>Home Web Control</title></head>");         
client.println("<h2>Login Portal</h2>");                                        //LoginPage 
client.println("<form action="" method='get'>");
client.println("User name: <input type='text' name='userName'><br><br>");
client.println("Password : <input type='password' name='password'><br><br>");
client.println("<input type='submit' value='Submit'>");
client.println("</form>");
client.println("<center/></body></html>");


delay(1);                         // Page loading delay
client.stop();                    // Stopping client
readString = "";                  // Clearing string for next read    
logout=true;
}

else if((readString.indexOf("ritesh") > 0 && readString.indexOf("12345") >0) || logout==false) {  //By this I set my Arduino's Username and password
logout=false;

client.println("http/1.1 200 ok");                                                                // Send standard http headers
client.println("content-type: text/html");
client.println();
client.println("<!doctype html><html>");
client.println("<body bgcolor='blue'>");
client.println("<center><head><title>Home Web Control</title></head>");
client.println("<h2>Control It Remotely</h2>");
client.println("<h4>Now if you forget to turn off your home appliance before leaving your home then no worry using this product control your home appliances through Internet in a secured manner.</h4>");


client.println("<form action="" method='post'>");							
client.println("<input name='Logout' type='submit' value='loggedout'><br>");		//Logout information sent to arduino using http request by post 
client.println("</form><br><br><br><br><br>");												          //request on the click of this logout button


//This form is filled by user by checking the radio buttons then
//when sumbit clicked then this form info will sent to arduino as http post request
client.println("<form action="" method='post'>");									
client.println("BEDROOM LIGHT :  <input type='radio' name='BedroomLight' value='BedroomLightOn'>");
client.println("   <input type='radio' name='BedroomLight' value='BedroomLightOff' checked><br>");

client.println("KITCHEN LIGHT :  <input type='radio' name='KitchenLight' value='KitchenLightOn'>");
client.println("   <input type='radio' name='KitchenLight' value='KitchenLightOff' checked><br>");

client.println("HALL FAN :  <input type='radio' name='KitchenLight' value='HallFanOn'>");
client.println("   <input type='radio' name='KitchenLight' value='HallFanOff' checked><br><br>");

client.println("<input type='submit' value='Submit'><br><br>");
client.println("</form><br><br>");

//When this manualControlling button clicked user then switch 
//over to manualControl Mode.
client.println("<form action="" method='post'>");									
client.println("<input name='manualControlling' type='submit' value='manualControlOn'><br><br><br><br>");
client.println("</form><br><br><br><br>");


if(readString.indexOf("BedroomLightOn") > 0) {
digitalWrite(BedroomLightPin1, LOW);			          //Trun on 
digitalWrite(BedroomLightPin2, LOW);			          //BedroomLight
client.println("BEDROOM LIGHT : ON<br>");}

if(readString.indexOf("BedroomLightOff") > 0) {
digitalWrite(BedroomLightPin2, HIGH);			          // Turn off BedroomLight
client.println("BEDROOM LIGHT : OFF<br>");}

if(readString.indexOf("KitchenLightOn") > 0){ 
digitalWrite(KitchenLightPin1, LOW);			          // Turn 
digitalWrite(KitchenLightPin2, LOW);				          //on KitchenLight
client.println("KITCHEN LIGHT : ON<br>");}

if(readString.indexOf("KitchenLightOff") > 0){ 
digitalWrite(KitchenLightPin2, HIGH);			          // Turn off KitchenLight
client.println("KITCHEN LIGHT : OFF<br>");}

if(readString.indexOf("HallFanOn") > 0) {
digitalWrite(HallFanPin1, LOW);					            // Turn on
digitalWrite(HallFanPin2, LOW);					            // HallFan
client.println("HALL FAN : ON<br>");}

if(readString.indexOf("HallFanOff") > 0) {
digitalWrite(HallFanPin2, HIGH);				            //Turn off HallFan 
client.println("HALL FAN : OFF<br>");}		

if(readString.indexOf("manualControlOn") > 0){
client.println(F("Manual Control is ON Now!<br>"));  //When we want to control all Home appliances by Board switches 
                                                     //then we switch to this manualControl Mode
digitalWrite(BedroomLightPin1, HIGH);       
digitalWrite(BedroomLightPin2, LOW);         
digitalWrite(KitchenLightPin1, HIGH);                //In this mode we made all the appliance's output,Board switch dependent.
digitalWrite(KitchenLightPin2, LOW);
digitalWrite(HallFanPin1, HIGH);
digitalWrite(HallFanPin2, LOW);							      
}

client.println("</center></body></html>");
delay(1);										                                                             // Page loading delay
client.stop();									                                                         // Stopping client
readString = "";								                                                         // Clearing string for next read
}

else if(logout==true){                                                                    //After Logout, Open the Login Page
client.println("http/1.1 200 ok");                                                        // Send  
client.println("content-type: text/html");                                                //standard http
client.println();                                                                         //headers

client.println("<!doctype html><html>");
client.println("<body bgcolor='blue'>");
client.println("<center><head><title>Home Web Control</title></head>");         
client.println("<h2>Login Portal</h2>");                                                  //LoginPage 
client.println("<form action="" method='post'>");
client.println("User name: <input type='text' name='userName'><br><br>");
client.println("Password : <input type='password' name='password'><br><br>");
client.println("<input type='submit' value='Submit'>");
client.println("</form>");
client.println("<center/></body></html>");


delay(1);                         // Page loading delay
client.stop();                    // Stopping client
readString = "";                  // Clearing string for next read								      
}
}
}
}
}
}




