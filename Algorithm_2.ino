#define grid_size 10

int mode = 2;
boolean double_turn =0;
int turncount=0;
int rocks_collect;
int tot_rocks=5;
int directionnow=-1; // N = 1; S = -1; E = 2; W = 2
int x = 0;
int y = 0;
int x_max = grid_size;
int y_max = grid_size;
String str;
char venusmap[grid_size][grid_size] = {"0000s00000",
                                       "0000p00000",
                                       "0000p00000",
                                       "0000p00000",
                                       "0000ppppo0",
                                       "0000o00p00",
                                       "0000000p00",
                                       "0000000p00",
                                       "0000000x00",
                                       "0000000000"}; 

// SIMULATION OF SENSOR DATA & MOVEMENT FUNCTION
int sensor[3] = {0,0,0};
boolean init_2 = true;
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
  Serial.print("POSITION:\t");
  Serial.print("x = ");
  Serial.print(x);
  Serial.print("\t Y = ");
  Serial.print(y);
  Serial.print("\n");
}

void set_start(){
  for(int i = 0; i<grid_size ; i++){
    for(int j = 0; j<grid_size ; j++){
      if(venusmap[i][j] == 'x'){
        y = i;
        x = j;
      }
    }
  }
  movement('0');
  print_map();
}

void print_map(){         // PRINT MAP
  Serial.print("\nMAP VISUALIZATION:\n\n");
  
  Serial.print(" -");
  for(int a = 0; a<grid_size ; a++){
    Serial.print("----");
  }
  Serial.print("\n");
  
  for(int i = 0; i<grid_size ; i++){
    Serial.print(" | ");
    for(int j = 0; j<grid_size ; j++){
      Serial.print(venusmap[i][j]);
      Serial.print(" | ");
    }
    Serial.print("\n");
    
    Serial.print(" -");
    for(int a = 0; a<grid_size ; a++){
      Serial.print("----");
    }
    Serial.print("\n");
  }
}

// Map Functions

void update_map(char direc, char a, int increment){
  if(direc=='0'){
    direc = dirchar(directionnow);
  }
  switch(direc){
    default:
      break;
    case 'N':
      venusmap[y-increment][x] = a;
      break;
    case 'S':
      venusmap[y+increment][x] = a;
      break;
    case 'E':
      venusmap[y][x+increment] = a;
      break;
    case 'W':
      venusmap[y][x-increment] = a;
      break;
  }
}

void update_pos(char direc, int increment){
  if(direc=='0'){
    direc = dirchar(directionnow);
  }
  switch(direc){
    default:
      break;
    case 'N':
      y = y-increment;
      break;
    case 'S':
      y = y+increment;
      break;
    case 'E':
      x = x+increment;
      break;
    case 'W':
      x = x-increment;
      break;
  }
}

bool check_map(char direc, char letter, int increment){ // This checks if the robot already travelled to a certain square
  if(direc=='0'){
    direc = dirchar(directionnow);
  }  
  switch(direc){
    default:
      return false;
      break;
    case 'N':
      if(venusmap[y-increment][x] == letter) {
        return true;
      }else{
        return false;
      }      
      break;
    case 'S':
      if(venusmap[y+increment][x] == letter) {
        return true;
      }else{
        return false;
      }
      break;
    case 'E':
      if(venusmap[y][x+increment] == letter) {
        return true;
      }else{
        return false;
      }
      break;
    case 'W':
      if(venusmap[y][x+increment] == letter) {
        return true;
      }else{
        return false;
      }
      break;
  }
}

// ALGORITHM Functions

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
      return 'l';//note: r is also rock
      }
    
    
  }else{
      double_turn=false;
      directionnow=directionnow*-1;
      return 'b';
  }
}

char check_forward(int US, int cliff, int rock) {
  //obstacle checkv
  if(check_map('0','p',1) || US == 1 || cliff == 1 ){
    if(check_map('0','p',1) == false){
      update_map('0','o',1);
    }
    return 'o'; //Does this mean that this redefines the 'p' values in the array to 'o' values?
  }
  //rock check
  if(rock == 1){
    return 'r';
  }else{
    return '0';
  }
  
}

char algo_0(){
  
  int US_check = sensor[0];
  int IR_obst_check = sensor[1];
  int IR_rock_check = sensor[2];
  
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
    if(check_map('0','s',0) == false){
      update_map('0','p',0);
    }
    update_map('0','x',1);
    update_pos('0',1);
    return 'f';
  }
}

char algo_2(){
  if(init_2 == true){
    init_2 = false;
    directionnow=directionnow*-1;
    return 'b';
  }
  if(check_map('0','s',1)){    
    double_turn=false;
    update_map('0','p',0);
    update_pos('0',1);
    
    mode = 0;
    
    return 'f';
  }
  if(check_map('0','p',1) == false && check_map('0','s',1) == false){ // change direction
    char check = algodirection();
    double_turn=true;
    if (check!='b'){
      turncount++;
    }
    
    return check;
  }
  if(check_map('0','p',1)){ // go forward
    double_turn=false;
    update_map('0','p',0);
    update_map('0','x',1);
    update_pos('0',1);
    
    return 'f';
  }
}

void setup() {
  Serial.begin(9600);
  set_start();
}

void loop() {
  // MODES:
  // 0 = FIND ROCK
  // 1 = GET ROCK
  // 2 = BACKTRACK

  delay(1000);
  if(mode == 2){

    movement(algo_2());
    print_map();
  }
}
