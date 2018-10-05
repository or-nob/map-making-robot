/*calibration parameters*/
int forwardBeforeLeftRot = 330;
int forwardAfterLeftRot = 200;

int rotatingDelay = 445;

int ABSA = 182; //right motor speed
int ABSB = 182;//left motor speed
/* end calibration parameters*/


int pinState0 = 11;
int pinState1 = 12;

int Echo1 = A4; //LEFT_SENSOR ECHO
int Trig1 = A5; //LEFT_SENSOR TRIG
int Echo2 = A2; //MID_SENSOR ECHO
int Trig2 = A3; //MID_SENSOR TRIG
int Echo3 = A0; //RIGHT_SENSOR ECHO
int Trig3 = A1; //RIGHT_SENSOR TRIG

int in1 = 2;
int in2 = 3;
int in3 = 4;
int in4 = 5;
int ENA = 9;
int ENB = 10;

void _mBackward()
{
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  digitalWrite(in3,LOW);
  digitalWrite(in4,HIGH);
  analogWrite(ENA,ABSA);
  analogWrite(ENB,ABSB);
  Serial.println("ROBOT_MOVING_FORWARD");
}

void _mForward()
{
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  digitalWrite(in3,HIGH);
  digitalWrite(in4,LOW);
  analogWrite(ENA,ABSA);
  analogWrite(ENB,ABSB);
  Serial.println("ROBOT_MOVING_BACKWARD");
}
void _mright()
{
  
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  digitalWrite(in3,HIGH);
  digitalWrite(in4,LOW);
  analogWrite(ENA,ABSA);
  analogWrite(ENB,ABSB);
  Serial.println("ROBOT_MOVING_LEFT");
}

void _mleft()
{
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  digitalWrite(in3,LOW);
  digitalWrite(in4,HIGH);
  analogWrite(ENA,ABSA);
  analogWrite(ENB,ABSB);
  Serial.println("ROBOT_MOVING_RIGHT");
} 


void _mStop()
{
  digitalWrite(in1,LOW);
  digitalWrite(in2,LOW);
  digitalWrite(in3,LOW);
  digitalWrite(in4,LOW);
  digitalWrite(ENA,LOW);
  digitalWrite(ENB,LOW);
  Serial.println("ROBOT_STOP");
} 

void move90DegreeRight(){
  _mright();
  delay(rotatingDelay);
  _mStop();
  //_mForward();
}




void move90DegreeLeft(){
  _mleft();
  delay(rotatingDelay);
  _mStop();
}

/*Ultrasonic distance measurement Sub function*/
int Left_Distance_test() 
{
digitalWrite(Trig1, LOW); 
delayMicroseconds(2);
digitalWrite(Trig1, HIGH); 
delayMicroseconds(20);
digitalWrite(Trig1, LOW); 
float Fdistance = pulseIn(Echo1, HIGH); 
delay(10);

Fdistance= Fdistance/ 29 / 2; 
return (int)Fdistance;
} 
int Front_Distance_test() 
{
digitalWrite(Trig2, LOW); 
delayMicroseconds(2);
digitalWrite(Trig2, HIGH); 
delayMicroseconds(20);
digitalWrite(Trig2, LOW); 
float Fdistance = pulseIn(Echo2, HIGH);
delay(10);
Fdistance= Fdistance/ 29 / 2; 
return (int)Fdistance;
} 

int initLeftDistance = 0;

void setup() 
{ 
  Serial.begin(9600); 
  pinMode(Echo1, INPUT); 
  pinMode(Trig1, OUTPUT); 
  pinMode(Echo2, INPUT); 
  pinMode(Trig2, OUTPUT); 
  pinMode(Echo3, INPUT); 
  pinMode(Trig3, OUTPUT); 
  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
  pinMode(in3,OUTPUT);
  pinMode(in4,OUTPUT);
  pinMode(ENA,OUTPUT);
  pinMode(ENB,OUTPUT);
  
  pinMode(pinState0, INPUT);
  pinMode(pinState1, INPUT);
  pinMode(13, OUTPUT);
  
  

  //initLeftDistance = Left_Distance_test() + 10;
  initLeftDistance = Left_Distance_test() + 10; 

  _mStop();
} 

void runBot(){
  if(Front_Distance_test() < 10) {
    move90DegreeRight();
    Serial.println("1st condition");
    initLeftDistance = Left_Distance_test() + 10;
   }
  else if (Left_Distance_test() > initLeftDistance  ) {
    _mForward();
    delay(forwardBeforeLeftRot);
    move90DegreeLeft();
    _mForward();
    delay(forwardBeforeLeftRot);
    initLeftDistance = Left_Distance_test() + 10;
  }
  else _mForward(); 
  //_mForward();
}

int distanceArray[30];
char rotationArray[30]; 
int currCnt = 0, i = 0;
void runbotwithdistance(){
  if(Front_Distance_test() < 10) {
    move90DegreeRight();
    Serial.println("1st condition");
    initLeftDistance = Left_Distance_test() + 10;
    
    distanceArray[i] = currCnt;
    currCnt = 0;
    rotationArray[i] = 'r';
    i++;
   }
  else if (Left_Distance_test() > initLeftDistance  ) {
    _mForward();
    delay(forwardBeforeLeftRot);
    move90DegreeLeft();
    _mForward();
    delay(forwardAfterLeftRot);
    initLeftDistance = Left_Distance_test() + 10;


    distanceArray[i] = currCnt;
    currCnt = 0;
    rotationArray[i] = 'l';
    i++;
  }
  else{
    _mForward();
    delay(200);
    /*_mStop();
    int t = Left_Distance_test() + 10;
    if( (initLeftDistance - 10 < t) && (initLeftDistance + 10 > t) ){
      initLeftDistance = t;  
    }*/
    
    currCnt++;
  }
}

void drawMap(){
  for(int j = 0; j < i; j++){
    for(int k = 0; k < distanceArray[j]; k++){
      _mForward();
      delay(200);
    }
    if(rotationArray[j] == 'l'){
      move90DegreeLeft();
    }
    
    if(rotationArray[j] == 'r'){
      move90DegreeRight();
    }
  }
  
}

void loop() 
{ 
  if( digitalRead(pinState0) == 1){
    //digitalWrite(13, LOW);
    runbotwithdistance();
  }
  else{
    if(digitalRead(pinState1) == 1){
      //digitalWrite(13, HIGH);
      /*distanceArray[0] = 3;
      distanceArray[1] = 2;
      distanceArray[2] = 1;
      rotationArray[0] = 'l';
      rotationArray[1] = 'r';
      rotationArray[2] = 'l';
      i = 3;*/
     drawMap();
    }
    else _mStop();
  }
  /*move90DegreeLeft();
  delay(500);*/
}

