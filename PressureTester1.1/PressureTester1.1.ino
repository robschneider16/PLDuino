



int NumSamples = 10;
double normal1 = 0;
double normal2 = 0;
double Test1Chamber1[10];
double Test1Chamber2[10];
double Test2Chamber1[10];
double Test2Chamber2[10];

  int op = 1;



double S1C1 = -0.04;
double S1C2 = -0.000;
double S2C1 = -0.02;
double S2C2 = -0.06;
double P1 = 2.38;
double P2 = 0.53;
double P3 = 2.06;
double P4 = 2.13;

double slopebuffer =  0.01;
double pressurebuffer = 0.22;

int goo = LOW;




void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(2, OUTPUT); //Door
  pinMode(4, OUTPUT); //gripper
  pinMode(6, OUTPUT); //pistons
  pinMode(7, OUTPUT); //Drop
  pinMode(9, OUTPUT); //chamber 2
  pinMode(12, OUTPUT); //chamber 1
  
  pinMode(3, INPUT_PULLUP);
  pinMode(11, OUTPUT);//seat and bonnet
  
}

double FindMax(double cnums[]){
  double mp = cnums[1];
  for (int x = 2; x < NumSamples; x++){
    if (cnums[x] >= mp){
      mp = cnums[x];
    }
  }
  return mp;
}

double FindSlope(double cnums[]){
  double slope = ((cnums[NumSamples-1] - cnums[0])/NumSamples);
  return slope;
}


void loop() {
  delay(200);
  //Serial.print(o);
  if(digitalRead(3) == LOW){
    goo = HIGH;
  }

  
if(goo == HIGH){
  Serial.println("test start");
  int pass = 1;
  double x = 0.00000;

  
  digitalWrite(4, HIGH);//close gripper
  delay(300);
  digitalWrite(6, HIGH);//push in side pistons
  digitalWrite(9, HIGH);
  delay(500);
  digitalWrite(9, LOW);
  digitalWrite(12, HIGH);
  delay(500);
  digitalWrite(12, LOW);

  digitalWrite(6, LOW);
  delay(100);
  digitalWrite(11, HIGH);
  delay(100);
  digitalWrite(6, HIGH);//push in side pistons
  delay(100);

  //get normal pressure
  x = analogRead(A3) *(5.0000 / 1023.0000);
  normal1 = x;
  x = analogRead(A1) *(5.0000 / 1023.0000);
  normal2 = x;
  P2 = normal1;

  //pressurize chamber 1
  digitalWrite(12, HIGH);
  delay(300);
  digitalWrite(12,LOW);

  //get pressure readings
  for (int i = 0; i<NumSamples; i++){
    delay(60);
    x = analogRead(A3) *(5.00000 / 1023.00000);
    Test1Chamber1[i] = x;
    delay(60);
    x = analogRead(A1)*(5.00000 / 1023.00000);
    Test1Chamber2[i] = x;
    delay(60);
  }

  //pressurize chamber 2
  digitalWrite(9, HIGH);
  delay(300);
  digitalWrite(9,LOW);

  //get pressure readings
  for (int j = 0; j<NumSamples; j++){
    delay(60);
    x = analogRead(A3) *(5.0000 / 1023.000000);
    Test2Chamber1[j] = x;
    delay(60);
    x = analogRead(A1)*(5.0000 / 1023.000000);
    Test2Chamber2[j] = x;
    delay(60);
  }

  //retract pistons and gripper
  digitalWrite(6, LOW);
  digitalWrite(4, LOW);
  digitalWrite(11, LOW);
  
  //Calculate Slopes
  double slope1C1 = FindSlope(Test1Chamber1);
  double slope1C2 = FindSlope(Test1Chamber2);
  double slope2C1 = FindSlope(Test2Chamber1);
  double slope2C2 = FindSlope(Test2Chamber2);


  Serial.println("Chamber Values");
  Serial.println(Test1Chamber1[9]);
  Serial.println(Test1Chamber2[9]);
  Serial.println(Test2Chamber1[9]);
  Serial.println(Test2Chamber2[9]);

  Serial.println("Slope Values");
  Serial.println(slope1C1);
  Serial.println(slope1C2);
  Serial.println(slope2C1);
  Serial.println(slope2C2);


  //check is pass or fail
  if(abs(slope1C1) >= (abs(S1C1) + slopebuffer)){
    pass = 0;//Slope is too steep
    Serial.println("Hole in Chamber 1 s");
  }
  if((Test1Chamber1[9] - normal1) < (P1 - P2 - pressurebuffer)){
    pass = 0;
    Serial.println("Hole in chamber 1 p");
  }
  if(FindMax(Test1Chamber2) > (normal2 + 0.015)){
    pass = 0;
    Serial.println("Hole in wall/seat");
    Serial.println(normal2);
    Serial.println(FindMax(Test1Chamber2));
  }
  if(abs(slope2C1) > (abs(S2C1) + slopebuffer)){
    pass = 0;
    Serial.println("Hole in Chamber 1 s   2nd test");
  }
  if((Test2Chamber1[9] ) < (P3 - pressurebuffer)){
    pass = 0;
    Serial.println("Hole in Chamber 1 p   2nd test");
  }
  if(abs(slope2C2) > (abs(S2C2) + slopebuffer)){
    pass = 0;
    Serial.println("Hole in Chamber 2 s");
  }
  if((Test2Chamber2[9]) < (P4 - pressurebuffer)){
    pass = 0;
    Serial.println("Hole in Chamber 2 p");
  }
  
  if(pass == 1){
    digitalWrite(7, HIGH);
  }else{
    digitalWrite(2, HIGH);
    digitalWrite(7, HIGH);
  }
  delay(500);
  digitalWrite(7,LOW);
  delay(700);
  digitalWrite(2,LOW);
  delay(200);
  goo = LOW;
}
}
