#define PAN_ID "4321"
#define CHANNEL_ID "F"
#define led_red 4
#define led_green 5
#define pin_master 6

//variables for communication
String str_message;
boolean xbee_connected = false;
boolean xbee_master = false;

//Function for receiving string message from serial
boolean receive_data(int t_stop) {  // function input is t_stop denotes the time in milliseconds that the microcontroller will be looking waiting for the message to arrive
  String message;                   // function outputs a boolean to notify if a message has been recieved or not
  float t_start = millis();
  
  while ((millis()-t_start) < t_stop){
    
    while (Serial.available() > 0) {
      message = Serial.readString();
      str_message = message;
      return true;
    }
  }
  return false;
}

// Function of initializing communication between master and slave
boolean xbee_init(int nr_sec) {
  Serial.begin(9600);
  boolean first_check = false;
  boolean second_check = false;
  
  for(int i = 0; i<nr_sec && second_check == false && first_check == false ; i++){
    if(xbee_master){
      Serial.print("x");
    }
    
    if(receive_data(1000)){
      for(int i = 0 ; i<str_message.length() ; i++){
        if(str_message[i] == '&' && xbee_master){
          first_check = true;
        }else{
          if(str_message[i] == 'x'){
            first_check = true;
          }
        }
      }
      
      if(first_check == true && xbee_master){
        Serial.print("&&&\n");
        second_check = true;
      }
    }
  }
  
  for(int i = 0; i<nr_sec && second_check == false && first_check == true && xbee_master == false; i++){
    Serial.print("&");
    if(receive_data(1000)){
      for(int i = 0 ; i<str_message.length() ; i++){
        if(str_message[i] == '&'){
          second_check = true;
        }
      }
      if(second_check == true){
        Serial.print("\n");
      }
    }
  }
  
  
  if(first_check == false || second_check == false){
    digitalWrite(led_red,HIGH);
    delay(5000);
    digitalWrite(led_red,LOW);
    return false;
  }else{
    digitalWrite(led_green,HIGH);
    delay(5000);
    digitalWrite(led_green,LOW);
    return true;
  }
}

void communication_setup() {
  pinMode(pin_master,INPUT);
  pinMode(7,OUTPUT);
  digitalWrite(7,HIGH);
  if(digitalRead(pin_master)==HIGH){ // if you have sorted the two master pins then its the master
    xbee_master = true;
  }
  digitalWrite(7,LOW);
  
  xbee_connected = xbee_init(30); // initializes communication between xbees
}

void send_data() {
  Serial.print("r:1-3");
}

void setup() {
  pinMode(led_red,OUTPUT);
  pinMode(led_green,OUTPUT);
  
  digitalWrite(led_red,LOW);
  digitalWrite(led_green,LOW);
  
  communication_setup();
}
