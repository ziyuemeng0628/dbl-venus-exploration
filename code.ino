#define grid_size 10

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
int x = 0;
int y = 0;
int x_max = grid_size;
int y_max = grid_size;
char venusmap[grid_size][grid_size] = {"s000000000",
                                       "0000000000",
                                       "0000000000",
                                       "0000000000",
                                       "0000000000",
                                       "0000000000",
                                       "0000000000",
                                       "0000000000",
                                       "0000000000",
                                       "0000000000"}; 

String receive_data() {
  String message = "";
  while (Serial.available() > 0) {
    message = Serial.readString();
  }
  return message;
}



bool check_map(char letter){
  if (directionnow == 1) {
    y++;
    if(venusmap[x][y] == letter) {
      y--;
      return true;
    }
  } else if (directionnow == -1) {
    y--;
    if(venusmap[x][y] == letter) {
      y++;
      turn = 1;
      return true;
    }
  } else if (directionnow == 2) {
    x++;
    if(venusmap[x][y] == letter) {
      x--;
      turn = 1;
      return true;
    }
  } else if (directionnow == -2) {
    x--;
    if(venusmap[x][y] == letter) {
      x++;
      turn = 1; 
      return true;
    }
  }
  
  return false;
}

char check_forward (int US, int cliff, int rock) {
  //obstacle check
  if(check_map('p') || US == 1 || cliff == 1 || x>9){

    return 'o';
  }
  //rock check
  if(check_map('r')){
    return 'r';
  }else{
    return '1';
  }
  
}

void update_map(char letter){
  venusmap[][] = check_forward();
}

char algo_0(int data[3]){
  // INITIALIZE THE MAP BY FILLING IT WITH 0S 
  // AS THE POSITIONS OF OBSTACLES AND ROCKS 
  // ARE STILL UNKNOWN

  US_check = data[0];
  IR_obst_check = data[1];
  IR_rock_check = data[2];
  
  // MODES:
  // 0 = FIND ROCK
  // 1 = GET ROCK
  // 2 = BACKTRACK
  if(check_forward(US_check,IR_obst_check,IR_rock_check) == 'o'){ // change direction
    
  }
  if(check_forward(US_check,IR_obst_check,IR_rock_check) == 'r'){
    //change mode
  }
  if(check_forward(US_check,IR_obst_check,IR_rock_check) == '1'){ // go for ward
    
  }
    // US_check = 1 -> MOUNTAIN DETECTED
    // IR_obst_check = 1 -> BLACK TAPE DETECTED
    // IR_rock_check = 1 -> PATH DETECTED
    if (US_check==0){
      if ((IR_obst_check==0) && (IR_rock_check==0)){
        right = 0;
        Serial.print(directionnow);
        Serial.print("\n");
        check_forward();
        update_position();
        return 'f';
    
      } else if (IR_rock_check==1){
        mode=1;
        right = 0;
        Serial.print(directionnow);
        Serial.print("\n");
        updatePos();
        return 'f'; 
        
      } else if (IR_obst_check==1){
        if (right==0){
          right=1;
          if (directionnow==2){
            directionnow = -1;
          }else if (directionnow=-2){
            directionnow=1;
          } else {
            directionnow++;
          }
          Serial.print(directionnow);
          Serial.print("\n");
          return 'r';
          }
        else{
          right=0;
          directionnow=directionnow*-1;
          Serial.print(directionnow);
          Serial.print("\n");
          return 'b';
        }
        
      }
      
    } else {
      if (right==0){
          right=1;
          if (directionnow==2){
            directionnow = -1;
          }else if (directionnow=-2){
            directionnow=1;
          } else {
            directionnow++;
          }
          Serial.print(directionnow);
          Serial.print("\n");
          return 'r';
          }
      else{
        right=0;
        directionnow=directionnow*-1;
        Serial.print(directionnow);
        Serial.print("\n");
        return 'b';
        }
    }
  
}

void setup() {
  Serial.begin(9600);
}

// MANUAL INPUT OF SENSOR DATA
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
