int mode = 0;
int turn;
int turncount=0;
int m=-1;
int grip=-1;
int rocks_collect;
int tot_rocks=5;
int US_check=-1;
int IR_obst_check=-1;
int IR_rock_check=-1;
int right=0;
int directionnow=1;
String receive_data() {
  String message = "";
  while (Serial.available() > 0) {
    message = Serial.readString();
  }
  return message;
}

char algo(int data[3]){
  US_check = data[0];
  IR_obst_check = data[1];
  IR_rock_check = data[2];
  
  if(mode==0){
    if (US_check==0){
      
      if ((IR_obst_check==0) && (IR_rock_check==0)){
        //initial direction
        Serial.print(directionnow);
        return 'f';
    
      } else if (IR_rock_check==1){
        mode=1;
        Serial.print(directionnow);
        return 'f'; 
        
      } else if (IR_obst_check==1){
        if (right==0){
          right=1;
          if (directionnow!=-2){
            directionnow+=1;
          }else if (directionnow=-2){
            directionnow=1;
            }
          Serial.print(directionnow);
          return 'r';
          }
        else{
          right=0;
          directionnow=directionnow*-1;
          Serial.print(directionnow);;
          return 'b';
        }
        
      }
      
    } else {
      if (right==0){
          right=1;
          if (directionnow!=-2){
            directionnow+=1;
          }else if(directionnow==-2){
            directionnow=1;
          }
          Serial.print(directionnow);
          return 'r';
          }
      else{
        right=0;
        directionnow=directionnow*-1;
        Serial.print(directionnow);
        return 'b';
        }
    }
  }
}

void setup() {
  Serial.begin(9600);
}

void loop() {
  int sensor[3] = {-1,-1,-1};
  String mess = receive_data();
  for(int i = 0; i<3 && mess!="" ; i++){
    if(mess[i] == '1'){
      sensor[i] = 1;
    }else{
      sensor[i] = 0;
    }
  }
  if(sensor[0] != -1 && sensor[1] != -1 && sensor[2] != -1){
    Serial.print("SENSOR DATA: \t");
    for(int i = 0; i<3 ; i++){
      Serial.print(sensor[i]);
      if(i == 2){
        Serial.print("\n");
      }
    }
    char out = algo(sensor);
    Serial.print("OUTPUT ALGORITHM:\t");
    Serial.print(out);
    Serial.print("\n");
  }

}
