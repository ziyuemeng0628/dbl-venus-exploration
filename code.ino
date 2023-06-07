#define grid_size 10

int mode = 0;
int turn;
boolean double_turn =0;
int turncount=0;
int m=-1;
int grip=-1;
int rocks_collect;
int tot_rocks=5;
int US_check=-1;
int IR_obst_check=-1;
int IR_rock_check=-1;
bool right;
bool left;
int directionnow=-1;
int x = 4;
int y = 0;
int x_max = grid_size;
int y_max = grid_size;
String str;
int sensor[3];
char venusmap[grid_size][grid_size] = {"0000s00000",
                                       "0000000000",
                                       "0000000000",
                                       "0000000000",
                                       "0000000000",
                                       "0000000000",
                                       "0000000000",
                                       "0000000000",
                                       "0000000000",
                                       "0000000000"}; 

// SIMULATION OF SENSOR DATA & MOVEMENT FUNCTION
boolean receive_data(int t_stop) {
  String message;
  float t_start = millis();
  
  while ((millis()-t_start) < t_stop){
    
    while (Serial.available() > 0) {
      message = Serial.readString();
      str = message;
      return true;
    }
  }
  return false;
}

void sensor_data(){
  sensor[0] = -1;
  sensor[1] = -1;
  sensor[2] = -1;
  for(int i = 0; i<3 ; i++){
    if(str[i] == '1'){
      sensor[i] = 1;
    }else{
      sensor[i] = 0;
    }
  }
  
  Serial.print("SENSOR DATA: \t");
  for(int i = 0; i<3 ; i++){
    Serial.print(sensor[i]);
    if(i == 2){
      Serial.print("\n");
    }
  }
}

void movement(char a){    // PRINT OUTPUT ALGORITHM
  Serial.print("OUTPUT ALGORITHM:\t");
  Serial.print(a);
  Serial.print("\n");
  Serial.print("DIRECTION:\t");
  Serial.print(dirchar(directionnow));
  Serial.print("\n");
  Serial.print("TURNCOUNT:\t");
  Serial.print(turncount);
  Serial.print("\n");
}

void print_map(){         // PRINT MAP
  Serial.print("\nMAP VISUALIZATION:\n\n");
  for(int a = 0; a<grid_size ; a++){
    Serial.print("----");
  }
  Serial.print("\n");
  for(int i = 0; i<grid_size ; i++){
    for(int j = 0; j<grid_size ; j++){
      Serial.print(venusmap[i][j]);
      Serial.print(" | ");
    }
    Serial.print("\n");
    for(int a = 0; a<grid_size ; a++){
      Serial.print("----");
    }
    Serial.print("\n");
  }
}


// ALGORITHM
bool check_map_forward(char letter){ // This checks if the robot already travelled to a certain square
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
      return true;
    }
  } else if (directionnow == -2) {
    x--;
    if(venusmap[x][y] == letter) {
      x++; 
      return true;
    }
  }
  
  return false;
}
//even or odd checking 
int turncheck(int turncount){
  if (turncount%2==0){
    return 0;
  }else{
    //turn: right turn, right increment=1
    return 1;
  }
}


//changing direction and outputting turn left,right,180
char algodirection(){
  if(double_turn == false){
    if (turncheck(turncount)==1){\
  
      if (directionnow==2){
        directionnow=-1;
      }
      if(directionnow==-2){
        directionnow=1;
      }
      if(directionnow==1){
        directionnow = 2;             
      }
      if(directionnow==-1){
        directionnow = -2;             
      }
      right=true;
      return 'r';//note: r is also rock
      
    }else{
    
      if (directionnow==2){
        directionnow=1;
        }
      else if (directionnow==-2){
        directionnow=-1;
        }
      else if(directionnow==1){
        directionnow=-2;             
        }
      else if (directionnow==-1){
        directionnow=2;
      }
      left=true;
      return 'l';//note: r is also rock
      }
    
    
  }else{
      double_turn=false;
      directionnow=directionnow*-1;
      return 'b';
  }
}
char check_forward (int US, int cliff, int rock) {
  //obstacle checkv
  if( US == 1 || cliff == 1 ){
    return 'o'; //Does this mean that this redefines the 'p' values in the array to 'o' values?
  }
  //rock check
  if(rock == 1){
    return 'r';
  }else{
    return '0';
  }
  
}
char dirchar(int dirrectionnow) {
  switch (dirrectionnow) {
    case 1:
      return 'N';
      break;
    case -1:
      return 'S';
      break;
    case 2:
      return 'E';
      break;
    case -2:
      return 'W';
      break;
  }
}


void update_map(char letter){
//  venusmap[][] = check_forward();
}

char algo_0(){
  // INITIALIZE THE MAP BY FILLING IT WITH 0S
  
  US_check = sensor[0];
  IR_obst_check = sensor[1];
  IR_rock_check = sensor[2];
  
  if(check_forward(US_check,IR_obst_check,IR_rock_check) == 'o'){ // change direction
    char check = algodirection();
    double_turn=true;
    if (check!='b'){
      turncount++;
    }
    return check;
    
    
  }
  if(check_forward(US_check,IR_obst_check,IR_rock_check) == 'r'){
    //change mode=1
  }
  if(check_forward(US_check,IR_obst_check,IR_rock_check) == '0'){ // go forward
    double_turn=false;
    return 'f';
    
  }
}

void setup() {
  Serial.begin(9600);
  print_map();
}

void loop() {
  // MODES:
  // 0 = FIND ROCK
  // 1 = GET ROCK
  // 2 = BACKTRACK
  
  if(receive_data(1000)){
    sensor_data();
    movement(algo_0());
  }
}
