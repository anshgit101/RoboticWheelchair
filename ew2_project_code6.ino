const double pi = 3.1428;
double x = 0, y = 0, theta = 0, tprev = 0, tnow = 0, L = 0.56, R = 0.05;//0.155;



// L is the distance between the two wheels = 56cm
// R is the effective radius of the wheels = 15.5cm



//------ variables for measuring RPM ------

const int interruptPin = D2;
double t1 = 0, t2 = 0, pps = 0, rpm = 0, timeOverlapped = 0;


//------ variables for measuring RPM ------

const int interruptPinR = D7;
double t1R = 0, t2R = 0, ppsR = 0, rpmR = 0, timeOverlappedR = 0;

//-------- 
//y[n] = (1 - alpha) * x[n] + alpha * y[n - 1]

double alpha = 0.7;

double t_passed;
double threshold = 200000; // threshold for detecting stoppage

int cnt_display = 0;



void setup() {
  
  Serial.begin(9600);
  
  pinMode(interruptPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin), IntCallback, RISING);

  pinMode(interruptPinR, INPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPinR), IntCallbackR, RISING);
}

void loop() {

  tnow = micros();
  
  double wL = rpm * pi;
  double wR = rpmR * pi;

  

  if(micros() - t2 >= threshold)
    wL = 0;
  if(micros() - t2R >= threshold)
    wR = 0;

  updateValues(wL, wR);
  
  cnt_display ++;
  if(cnt_display > 10000){
    Serial.print("x = ");
    Serial.print(x);
    Serial.print("\t y = ");
    Serial.print(y);
    Serial.print("\t theta = ");
    Serial.print(theta);
    Serial.print("\n");
    cnt_display = 0;
  }
  //Serial.print("wL = ");
  //Serial.print(wL);
  //Serial.print("\t wR = ");
  //Serial.print(wR);
  
  //Serial.print("\n");

  //Serial.print("\t t_1 = ");
  //Serial.print(t1, 8);

  //Serial.print("\t t_2 = ");
  //Serial.print(t2, 8);


  //Serial.print("\t timeOverlapped = ");
  //Serial.print(timeOverlapped, 8);

  //Serial.print("\t pps = ");
  //Serial.print(pps, 8);

  
  //Serial.print("\t t_1r = ");
  //Serial.print(t1R, 8);

  //Serial.print("\t t_2r = ");
  //Serial.print(t2R, 8);
  //Serial.print("\n");

  //Serial.print("\t timeOverlappedR = ");
  //Serial.print(timeOverlappedR, 8);
  
  //Serial.print("\t ppsR = ");
  //Serial.print(ppsR, 8);
  

  //Serial.print("\n");

  tprev = tnow;
  

  
}


void updateValues(double wL, double wR){

  t_passed = (tnow - tprev) / 1000000;
  
  

  double x_del = R * (wL + wR) * cos(theta) / 2;
  double y_del = R * (wL + wR) * sin(theta) / 2;
  double theta_del = R * (wR - wL) / L;
  
  x = x + x_del * t_passed;
  y = y + y_del * t_passed;
  theta = theta + theta_del * t_passed;

  while(theta > 2 * pi)
    theta = theta - 2 * pi;

  while(theta < 0)
    theta = theta + 2 * pi;

  
}


ICACHE_RAM_ATTR void IntCallback(){
  
  t2 = micros();
  timeOverlapped = (t2 - t1)/1000000;
  pps = 1/timeOverlapped;

  // Calculations for rpm:
  // ppr is set to 512
  // rpm = ppm / ppr = pps * 60 / ppr
  // rpm_bigWheel = pps * 60 / (32 * ppr) = 0.00036621 * pps
  
  double rpm_current = 0.00036621 * pps;
  rpm = (1 - alpha) * rpm_current + alpha * rpm;
  

  t1 = t2;
  
}

ICACHE_RAM_ATTR void IntCallbackR(){
  
  t2R = micros();
  timeOverlappedR = (t2R - t1R)/1000000;
  ppsR = 1/timeOverlappedR;

  // Calculations for rpm:
  // ppr is set to 512
  // rpm = ppm / ppr = pps * 60 / ppr
  // rpm_bigWheel = pps * 60 / (32 * ppr) = 0.00036621 * pps
  
  double rpmR_current = 0.00036621 * ppsR;
  rpmR = (1 - alpha) * rpmR_current + alpha * rpmR;

  t1R = t2R;
}
