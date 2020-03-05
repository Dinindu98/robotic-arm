#include <Servo.h>

Servo myservo[3]; 
Servo myservopick;

#define baseArm A2
#define subArm A3
#define turningPoint A4
#define pick 5
#define pickServo 12

bool button = false; 
bool pickStatus = false; 
bool blockloop = false;
bool runState = false;
bool modeButton =false;
bool modeState = false;


int receivedValue = 60;

int state[3] = {120,120,60}; // define servo state in oreder to A B C

int destination[3] = {170,170,150}; // define servo destination in order to A B C

int progress[3] = {0,0,0};

int progress_state = 0;

int read_position[3]={0,0,0};

void setup() {
  myservo[0].attach(9); 
  myservo[1].attach(10);
  myservo[2].attach(11);
  myservopick.attach(pickServo);
  pinMode(pick,OUTPUT);

}

void loop() {
  if(Serial.available()>1){
      receivedValue = map(Serial.parseInt(),0,550,80,140);
    String data = Serial.readString();// data  input format => xx,xx,xx in order to x y z positions
    read_position[0] = data.substring(0,2).toInt(); //x
    read_position[1] = data.substring(3,5).toInt(); //y
    read_position[2] = data.substring(6,8).toInt(); //z

    destination[0] = read_position[0]*10;
    destination[1] = read_position[1]*10;
    destination[2] = read_position[2]*10;
    progress_state=1;
    automated();
    Serial.println(read_position[2]+1);
  }

}

void automated(){
  while(progress_state!=0){
    progress_state=0;
    for(int x=0;x<3;x++){
      progress[x] = servoTurn(x);
      myservo[x].write(state[x]);
      progress_state+=progress[x];
      delay(10);
      if(Serial.available()>1){
        receivedValue = map(Serial.parseInt(),0,550,80,140);
      }
      picker(digitalRead(pick));
    }
  }
}

bool servoTurn(int x){
   //myservo.write(pos);
   if(state[x]!=destination[x]){
    if(state[x]>destination[x]){
      state[x]--;
    }else{
      state[x]++;
    }
    Serial.println(state[x]);
    return true;
   }
   else return false;
}

void picker(bool x){

  delay(10);
  if(x && !blockloop){
    pickStatus = !pickStatus;
    blockloop = true;
  }else if(!x && blockloop ){
    blockloop = false;
  }

  if(pickStatus){
    myservopick.write(50);
  }else{
    myservopick.write(120);
  }
}
