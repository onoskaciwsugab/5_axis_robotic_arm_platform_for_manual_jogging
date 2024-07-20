#include <Encoder.h>
Encoder my_encoder1(2, 7);//C1;C2 port on Encoder1
float output1, error1, set_point, Prop1, Kp1, Integ1, Ki1, last_error1, pwm1;
float int_error1, PID1, Deriv1, Kd1, der_error1;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Encoder my_encoder2(3, 4);//C1;C2 port on Encoder2
float output2, error2, set_point2, Prop2, Kp2, Integ2, Ki2, last_error2, pwm2;
float int_error2, PID2, Deriv2, Kd2, der_error2, position1, position2;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

boolean flag = false;
int iflag = 0;
int interval = 400;

String setPoint1;
String setPoint2; 
  
unsigned long previousMillis = 0;  
const long intervalmilis = 200; 

void setup() {
  // put your setup code here, to run once:
  pinMode(9, OUTPUT);//Motor PWM Pin
  pinMode(10, OUTPUT);//Motor PWM Pin

  //  pinMode(7, INPUT);//Motor PWM Pin
  //  pinMode(3, INPUT);//Motor PWM Pin
  //  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //
  //  pinMode(2, INPUT);//Motor PWM Pin
  //  pinMode(4, INPUT);//Motor PWM Pin

  pinMode(5, OUTPUT);//Motor PWM Pin
  pinMode(6, OUTPUT);//Motor PWM Pin
  Serial.begin(115200);
}

void loop() {
  if (Serial.available() > 0) {
    // read the incoming byte
    String incomingValue = Serial.readStringUntil('\n'); // Read data until '\n

    setPoint1 = incomingValue.substring(0,incomingValue.indexOf(';'));
    set_point = setPoint1.toInt();
    Serial.println(set_point);
    
    incomingValue = incomingValue.substring(incomingValue.indexOf(';')+1);
    setPoint2 = incomingValue.substring(0,incomingValue.indexOf('>'));
    set_point2 = setPoint2.toInt();
    Serial.println(set_point2); 
      
   // flag = true;
  }
  
 // while(flag){
    
    //set_point = setPoint1.toInt();//original:5000
    Kp1 = 0.65;// 5.73
    Ki1 = 0.0001;//0.015
    Kd1 = 0.001;//0.33
  
    long position1 = my_encoder1.read();
    output1 = position1;
    error1 = set_point - output1;
    int_error1 = int_error1 + error1;
    der_error1 = error1 - last_error1;
  
    if (int_error1 > (-1*set_point))
      int_error1 = -1*set_point;//original: (int_error > 5000)int_error = 5000;
    if (int_error1 < set_point)
      int_error1 = set_point;//original: (int_error < -5000)int_error = -5000;
  
    Prop1 = Kp1 * error1;
    Integ1 = Ki1 * int_error1;
    Deriv1 = Kd1 * der_error1;
    PID1 = Prop1 + Integ1 + Deriv1;
  
    if (error1 < 1 and error1 > -1)
    {
      PID1 = 0;
    }
  
    pwm1 = abs(PID1);
    if (pwm1 > 255) {
      pwm1 = 255;
    }
  
    if (pwm1 < 0) {
      pwm1 = 0;
    }
  
    if (PID1 > 0)//moving CCW if PID > 0
    {
      analogWrite(10, 0);//original: analogWrite(9, 0)
      analogWrite(9, pwm1);//original: analogWrite(10, pwm)
    }
  
    if (PID1 < 0)//moving CCW if PID < 0
    {
      analogWrite(9, 0);//original: analogWrite(10, 0)
      analogWrite(10, pwm1);//original: analogWrite(9, pwm)
    }
    last_error1 = error1;
  
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  
    //set_point2 = float(setPoint2.toInt());//original:5000
    //Serial.println("");
    //set_point2 = setPoint2.toInt();
    Kp2 = 0.65;
    Ki2 = 0.0001;
    Kd2 = 0.001;
  
  //  Kp2 = 1.85;
  //  Ki2 = 0.06;
  //  Kd2 = 0.65;
  
    long position2 = my_encoder2.read();
  
    output2 = position2;
    error2 = set_point2 - output2;
    int_error2 = int_error2 + error2;
    der_error2 = error2 - last_error2;
  
    if (int_error2 > set_point2)
      int_error2 = set_point2;//original: (int_error > 5000)int_error = 5000;
      
    if (int_error2 < (-1*set_point2))
      int_error2 = -1*set_point2;//original: (int_error < -5000)int_error = -5000;
  
    Prop2 = Kp2 * error2;
    Integ2 = Ki2 * int_error2;
    Deriv2 = Kd2 * der_error2;
    PID2 = Prop2 + Integ2 + Deriv2;
  
    if (error2 < 1 and error2 > -1)
    {
      PID2 = 0;
    }
  
    pwm2 = abs(PID2);
    if (pwm2 > 255) {
      pwm2 = 255;
    }
  
    if (pwm2 < 0) {
      pwm2 = 0;
    }
  
    if (PID2 > 0)//moving CCW if PID > 0
    {
      analogWrite(5, 0);//original: analogWrite(9, 0)
      analogWrite(6, pwm2);//original: analogWrite(10, pwm)
    }
  
    if (PID2 < 0)//moving CCW if PID < 0
    {
      analogWrite(6, 0);//original: analogWrite(10, 0)
      analogWrite(5, pwm2);//original: analogWrite(9, pwm)
    }
  
    last_error2 = error2;
    
//    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= intervalmilis) {
      previousMillis = currentMillis;
      Serial.print(set_point);
      Serial.print(",");
      Serial.print(output1);
      Serial.print(",");
      Serial.print(set_point2);
      Serial.print(",");
      Serial.println(output2);
    }

    
    //Serial.print(",");
    //Serial.print( pwm1);
    //  Serial.print(",");
    //  Serial.print( pwm2);
    //Serial.print(",");
    //  Serial.print( error1);
    //  Serial.print(",");
    //Serial.println( error2);
    
    delay(10);

//    iflag++;
//    if(iflag == interval) {
//      flag = false;
//      iflag = 0;
//    }
//  }
}