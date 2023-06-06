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
String str;
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

int *sensor_data(){
  int sensor[3] = {-1,-1,-1};
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
  
  return sensor;
}

void movement(char a){    // PRINT OUTPUT ALGORITHM
  Serial.print("OUTPUT ALGORITHM:\t");
  Serial.print(a);
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

char check_forward (int US, int cliff, int rock) {
  //obstacle check
  if(check_map_forward('p') || US == 1 || cliff == 1 ){

    return 'o'; //Does this mean that this redefines the 'p' values in the array to 'o' values?
  }
  //rock check
  if(check_map_forward('r')){
    return 'r';
  }else{
    return '1';
  }
  
}

void update_map(char letter){
//  venusmap[][] = check_forward();
}

char algo_0(int data[3]){
  // INITIALIZE THE MAP BY FILLING IT WITH 0S
  
  US_check = data[0];
  IR_obst_check = data[1];
  IR_rock_check = data[2];
  
  if(check_forward(US_check,IR_obst_check,IR_rock_check) == 'o'){ // change direction
    //right/left
  }
  if(check_forward(US_check,IR_obst_check,IR_rock_check) == 'r'){
    //change mode=1
  }
  if(check_forward(US_check,IR_obst_check,IR_rock_check) == '1'){ // go forward
    //turncount++
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
    movement(algo_0(sensor_data()));
  }

}
