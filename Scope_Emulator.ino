// Arduino Uno Scope Emulator   
// D_inputs 8 to 11 = ST-4 inputs
// A_inputs 0 to 3 = LX10 Hand controller (needs pulldown (10k) on A0 input) OR Focus control INPUTS

 // set invert_op to 1 if you require the W,S,N and E outputs to be inverted for the interface card,
 // for example if using the relay board as shown in 'ARDUINO_and_4relay_board-2.jpg' in Photos > GT-pics 
     const int invert_op = 0;
     
     
 // set Uno_R3 to 1 if you are using an Arduino Uno R3 board. 
 // This should help if PHD etc can't connect. If not connecting try = 0
     const int Uno_R3 = 1;

     
     const int CW = 0;
     const int CCW = 1;
     
     const int West = 2;         //pin of the Arduino WEST
     const int South = 3;        //pin of the Arduino SOUTH
     const int North = 4;        //pin of the Arduino NORTH
     const int East = 5;         //pin of the Arduino EAST
     const int MotorStep = 6;    //pin of the Arduino connects to the STEP pin of the EasyDriver board or PWM interface
     const int MotorDir = 7;     //pin of the Arduino connects to the DIR pin of the EasyDriver boardor PWM interface
     
 
  // Scale will provide a scaling factor for each direction, 100 will mean a #:Mgn1000# will produce a 1 second output, 150 a 1.5 second pulse
  // minimum 10, and set in steps of 10 eg 10,20,30.....MAX 300
     const int ScaleNorth=100;
     const int ScaleEast=100;
     const int ScaleSouth=100;
     const int ScaleWest=100;     

  // Sens(itivity) will set the minumum pulse length acknowledged in mSecs
     const int SensNorth=10;
     const int SensEast=10;
     const int SensSouth=10;
     const int SensWest=10;
  
  // Back(lash) when changing direction, adds (xxx)mS to new direction
     const int BackNorth=0;
     const int BackEast=0;
     const int BackSouth=0;
     const int BackWest=0;
     
  // LX10 hand controller set to 1 to use it (and fit the 10k resistor from A0 to GND)
  // YOU CANNOT USE FOCUS CONTROLLER AND THE LX10 HAND CONTROLLER !!
  // IF YOU SET BOTH THEN FOCUS CONTROL WITH OVERIDE HAND CONTROLLER
     const int hcontrol = 0;
     
  // SET FCONTROL1 to 1 to use LX10 HAND CONTROLLER INPUT for focusser input.  
  // YOU CANNOT USE FCONTROL1 AND THE LX10 HAND CONTROLLER !!
  // SERIAL FOCUS CONTROLS WILL STILL WORK IF NEITHER FCONTROL IS SET TO 1.
     const int fcontrol1 = 0;
     
  // SET FCONTROL2 to 1 to use ST-4 INPUT for FOCUS.
     const int fcontrol2 = 0;
     
  // set FOCUS pulses per button press
     const int fastmotor = 800;
     const int medmmotor = 200;
     const int slowmotor = 20;

  // set FOCUS motor speeds
     const int fast = 200;
     const int medm = 800;
     const int slow = 1000;

  // Set pwm to 1 for pulse width modulation for the focusser eg Meade #1209 
  // Set to 0 if using a Stepper Motor focuser (Easydriver board) 
     const int pwm = 1;
     
  // set pwm FOCUS pulses per button press  
     const int pwmfastmotor = 50;
     const int pwmmedmmotor = 50;
     const int pwmslowmotor = 50;
     
  // set pwm FOCUS motor speeds
     const int pwmfast1 = 30;
     const int pwmmedm1 = 16;
     const int pwmslow1 = 8;
     const int pwmfast2 = 2;
     const int pwmmedm2 = 16;
     const int pwmslow2 = 24;
     
  // set drift to 1 to enable, will apply correction as set on pots
     const int drift = 0;
     
  // set timer for drift timeout (900 = 9 seconds)
     const int timer1 = 900;
  // set timer for time between drift correction (400 = 4 seconds)
     const int timer2 = 400;
    

void setup() {
   // initialize serial communication:
      Serial.begin(9600); 
      
   // initialize the OUTPUT pins:
      pinMode(MotorStep, OUTPUT);
      pinMode(MotorDir, OUTPUT);
      pinMode(West, OUTPUT);
      pinMode(South, OUTPUT);
      pinMode(North, OUTPUT);
      pinMode(East, OUTPUT);

   // initialize the INPUT pins: 
      for (int Pin = 8; Pin < 13; Pin++) {pinMode(Pin, INPUT);digitalWrite(Pin, HIGH);}
      
   // initialize the FOCUS pins: 
      digitalWrite(MotorStep, LOW);
      digitalWrite(MotorDir, CW);
   
   // YOU CANNOT USE FOCUS CONTROLLER AND THE LX10 HAND CONTROLLER !!
    
      
     int val1 = HIGH;
     int val2 = LOW;
     if (invert_op == 0) {val1 = HIGH; val2 = LOW;}
     if (invert_op > 0) {val1 = LOW; val2 = HIGH;}
     if (invert_op > 0) {digitalWrite(West, val2);digitalWrite(South, val2);digitalWrite(North, val2);digitalWrite(East, val2);}

       
   // initial response for ASCOM 'Meade Classic and Autostar 1' driver ( or ST-4 or hand / focus controller button pressed)
      int analog0 = 0; int analog1 = 0; int analog2 = 0; int analog3 = 0; int analog4 = 0;int analog5 = 0;
      if (fcontrol1 > 0) {digitalWrite(A0, HIGH);}
      digitalWrite(A1, HIGH); digitalWrite(A2, HIGH); digitalWrite(A3, HIGH); digitalWrite(A4, HIGH); digitalWrite(A5, HIGH); 
      analog0 = analogRead(0); analog1 = analogRead(1); analog2 = analogRead(2); analog3 = analogRead(3); analog4 = analogRead(4);analog5 = analogRead(5);
      int hNorth=0; int hEast=0; int hSouth=0; int hWest=0;
      int fdirn=0; int fslow=0; int fmedm=0; int ffast=0; 
      int inByteA = 'a';
      
      while (Uno_R3 < 1 && Serial.available() < 1  &&  digitalRead(8) == HIGH && digitalRead(9)== HIGH && digitalRead(10)== HIGH && digitalRead(11)== HIGH
      && digitalRead(12)== HIGH && hWest <1 && hSouth <1 && hNorth <1 && hEast <1 && fslow <1 && fmedm <1 && ffast <1)
      {
      inByteA = Serial.read();
      analog0 = analogRead(0); analog1 = analogRead(1); analog2 = analogRead(2); analog3 = analogRead(3); analog4 = analogRead(4);analog5 = analogRead(5);
      if (hcontrol > 0 && analog0 > 1000) {hWest = 1;}
      if (fcontrol1 > 0 && analog0 < 50) {fdirn = 1;}
      if (hcontrol > 0 && analog1 < 50) {hEast = 1;}
      if (fcontrol1 > 0 && analog1 < 50) {fslow = 1;}
      if (hcontrol > 0 && analog2 < 50) {hSouth = 1;}
      if (fcontrol1 > 0 && analog2 < 50) {fmedm = 1;}
      if (hcontrol > 0 && analog3 < 50) {hNorth = 1;}
      if (fcontrol1 > 0 && analog3 < 50) {ffast = 1;}
      
      if (fcontrol2 > 0 && digitalRead(8) == LOW ) {fdirn = 1;}
      if (fcontrol2 > 0 && digitalRead(9) == LOW ) {fslow = 1;}
      if (fcontrol2 > 0 && digitalRead(10) == LOW ) {fmedm = 1;}
      if (fcontrol2 > 0 && digitalRead(11) == LOW ) {ffast = 1;}
      if (fcontrol2 < 1 && fcontrol1 < 1) {fdirn =0;fslow = 0;fmedm = 0;ffast = 0;}
      if (fslow >0) {fmedm == 0; ffast == 0;}
      if (fmedm >0) {ffast == 0;}
      } 
      delay (1);
      Serial.print (char(48));Serial.print (char(48));Serial.print (char(48));Serial.print (char(42));
      Serial.print (char(48));Serial.print (char(48));Serial.print (char(35));Serial.print (char(10));     
      }

void loop() {
  // initialise variables
     int val1 = HIGH;
     int val2 = LOW;
     if (invert_op == 0) {val1 = HIGH; val2 = LOW;}
     if (invert_op > 0) {val1 = LOW; val2 = HIGH;}
     if (invert_op > 0) {digitalWrite(West, val2);digitalWrite(South, val2);digitalWrite(North, val2);digitalWrite(East, val2);}

     
  // initialize direction variables to 0
     int lenNorth=0;
     int lenEast=0;
     int lenSouth=0;
     int lenWest=0;
     int MoveNorth=0;
     int MoveEast=0;
     int MoveSouth=0;
     int MoveWest=0;

  // initialize timer variables to 0
     int Timer=0; int i=0;int fspeed = slow;int lenfocus=0;
     int fspeed1 = pwmslow1;int fspeed2 = pwmslow2;
     int input8=0;int input9=0;int input10=0;int input11=0;int input12=0;
     int analog0 = 0; int analog1 = 0; int analog2 = 0; int analog3 = 0;int analog4 = 0;int analog5 = 0;
     int hNorth=0; int hEast=0; int hSouth=0; int hWest=0;
     int fdirn=0; int fslow=0; int fmedm=0; int ffast=0;
     int tNorth=0; int tEast=0; int tSouth=0; int tWest=0;
     if(invert_op == 1) {digitalWrite(West, val2);digitalWrite(South, val2);digitalWrite(North, val2);digitalWrite(East, val2);}
  
  // YOU CANNOT USE FOCUS CONTROLLER AND THE LX10 HAND CONTROLLER !!
     if(fcontrol1 >0) {hcontrol ==0;}
     int timer3 = 0;
  label:
  // initialize variables:
     int inByte0 = 'a';
     int inByte1 = 'a';
     int inByte2 = 'a';
     int inByte3 = 'a';
     int inByte4 = 'a';
     int inByte5 = 'a';
     int inByte6 = 'a';
     int inByte7 = 'a';
     int inByte8 = 'a';
     
  // wait for ':' character, start of a command
     while (inByte0 != ':') {inByte0 = Serial.read();delay(10);
     
  // read ST-4 input (digital inputs 8 to 11)
     input8 = digitalRead(8); input9 = digitalRead(9); input10 = digitalRead(10); input11 = digitalRead(11);
     analog0 = analogRead(0); analog1 = analogRead(1); analog2 = analogRead(2); analog3 = analogRead(3);analog4 = analogRead(4);analog5 = analogRead(5);
     hNorth=0; hEast=0; hSouth=0; hWest=0;fdirn =0;fslow = 0;fmedm = 0;ffast = 0;
     if (hcontrol > 0 && analog0 > 1000) {hWest = 1;}
     if (fcontrol1 > 0 && analog0 < 50) {fdirn = 1;}
     if (hcontrol > 0 && analog1 < 50) {hEast = 1;}
     if (fcontrol1 > 0 && analog1 < 50) {fslow = 1;}
     if (hcontrol > 0 && analog2 < 50) {hSouth = 1;}
     if (fcontrol1 > 0 && analog2 < 50) {fmedm = 1;}
     if (hcontrol > 0 && analog3 < 50) {hNorth = 1;}
     if (fcontrol1 > 0 && analog3 < 50) {ffast = 1;}
     if (fcontrol2 > 0 && digitalRead(8) == LOW ) {fdirn = 1;}
     if (fcontrol2 > 0 && digitalRead(9) == LOW ) {fslow = 1;}
     if (fcontrol2 > 0 && digitalRead(10) == LOW ) {fmedm = 1;}
     if (fcontrol2 > 0 && digitalRead(11) == LOW ) {ffast = 1;}
     if (fcontrol2 < 1 && fcontrol1 < 1) {fdirn =0;fslow = 0;fmedm = 0;ffast = 0;}
     if (hcontrol < 1) {hWest =0;hSouth = 0;hNorth = 0;hEast = 0;}     
     if (fslow >0) {fmedm == 0; ffast == 0;}
     if (fmedm >0) {ffast == 0;}
      
    
  // act on a ST-4 or focus input
     if (fdirn <1 && fslow >0 && pwm != 1) { RotateMotor(CW, slowmotor, slow, slow, val2);}  //Rotate motor clockwise slowly
     if (fdirn <1 && fmedm >0 && pwm != 1) { RotateMotor(CW, medmmotor, medm, medm, val2);}  //Rotate motor clockwise medium
     if (fdirn <1 && ffast >0 && pwm != 1) { RotateMotor(CW, fastmotor, fast, fast, val2);}  //Rotate motor clockwise fast
     
     if (fdirn >0 && fslow >0 && pwm != 1) { RotateMotor(CCW, slowmotor, slow, slow, val2);}  //Rotate motor counterclockwise slowly
     if (fdirn >0 && fmedm >0 && pwm != 1) { RotateMotor(CCW, medmmotor, medm, medm, val2);}  //Rotate motor counterclockwise medium
     if (fdirn >0 && ffast >0 && pwm != 1) { RotateMotor(CCW, fastmotor, fast, fast, val2);}  //Rotate motor counterclockwise fast
  
     if (fdirn <1 && fslow >0 && pwm != 0) { RotateMotor(CW, pwmslowmotor, pwmslow1, pwmslow2, val2);}  
     if (fdirn <1 && fmedm >0 && pwm != 0) { RotateMotor(CW, pwmmedmmotor, pwmmedm1, pwmmedm2, val2);}  
     if (fdirn <1 && ffast >0 && pwm != 0) { RotateMotor(CW, pwmfastmotor, pwmfast1, pwmfast2, val2);}  
     
     if (fdirn >0 && fslow >0 && pwm != 0) { RotateMotor(CCW, pwmslowmotor, pwmslow2, pwmslow1, val2);}  
     if (fdirn >0 && fmedm >0 && pwm != 0) { RotateMotor(CCW, pwmmedmmotor, pwmmedm2, pwmmedm1, val2);}  
     if (fdirn >0 && ffast >0 && pwm != 0) { RotateMotor(CCW, pwmfastmotor, pwmfast2, pwmfast1, val2);} 
    
     if (digitalRead(12)== LOW){ vRotateMotor(CCW, fastmotor, pwmfast1, pwmfast2, val2);}   
     
     if (hNorth==1 or hEast==1 or hSouth==1 or hWest==1 or 
     input8 == LOW or input9 == LOW or input10 == LOW or input11 == LOW or tWest == 1 or tEast == 1 or tNorth == 1 or tSouth == 1) { 
            
     Timer=0;
     if ((fcontrol2 <1 && input8 == LOW) or hWest ==1)  {lenWest = 21; lenEast = 0; tWest =1;}
     if ((fcontrol2 <1 && input9 == LOW) or hNorth ==1)  {lenNorth = 21; lenSouth = 0; tNorth =1;}
     if ((fcontrol2 <1 && input10 == LOW) or hSouth ==1) {lenSouth = 21; lenNorth = 0; tSouth =1;}
     if ((fcontrol2 <1 && input11 == LOW) or hEast ==1) {lenEast = 21; lenWest = 0; tEast =1;}
    
     if (fcontrol2 <1 && input8 == HIGH && hWest ==0 && tWest == 1 ) {tWest =0;} 
     if (fcontrol2 <1 && input9 == HIGH && hNorth ==0 && tNorth == 1 ) {tNorth =0;}
     if (fcontrol2 <1 && input10 == HIGH && hSouth ==0 && tSouth == 1 ) {tSouth =0;}
     if (fcontrol2 <1 && input11 == HIGH && hEast ==0 && tEast == 1 ) {tEast =0;}   
    }
    
  // Keep moving North after a #:Mgnyyyy# command for the received period   
     if (lenNorth > 0 && lenSouth ==0) {digitalWrite(North, val1);digitalWrite(South, val2);if(MoveNorth != 1) {Serial.print (char(78));}MoveNorth=1;lenNorth=lenNorth-10;
     if (lenNorth <10) {digitalWrite(North, val2);lenNorth=0;MoveNorth=0;Serial.print (char(110));Serial.print (char(10));}}

  // Cancel North if South #:Mgsyyyy# received whilst moving North 
     if (lenNorth > 0 && lenSouth >0 && MoveNorth==1) {digitalWrite(North, val2);lenNorth=0;MoveNorth=0;Serial.print (char(110));lenSouth=lenSouth+BackSouth;}
 
  // Keep moving East after a #:Mgeyyyy# command for the received period
     if (lenEast > 0 && lenWest ==0) {digitalWrite(East, val1);digitalWrite(West, val2);if(MoveEast != 1) {Serial.print (char(69));}MoveEast=1;lenWest=0;lenEast=lenEast-10;
     if (lenEast <10) {digitalWrite(East, val2);lenEast=0;MoveEast=0;Serial.print (char(101));Serial.print (char(10));}}
 
  // Cancel East if West #:Mgwyyyy# received whilst moving East 
     if (lenEast > 0 && lenWest >0 && MoveEast==1) {digitalWrite(East, val2);lenEast=0;MoveEast=0;Serial.print (char(101));lenWest=lenWest+BackWest;}

  // Keep moving South after a #:Mgsyyyy# command for the received period
     if (lenSouth > 0 && lenNorth ==0) {digitalWrite(South, val1);digitalWrite(North, val2);if(MoveSouth != 1) {Serial.print (char(83));}MoveSouth=1;lenNorth=0;lenSouth=lenSouth-10;
     if (lenSouth <10) {digitalWrite(South, val2);lenSouth=0;MoveSouth=0;Serial.print (char(115));Serial.print (char(10));}}
  
  // Cancel South if North #:Mgnyyyy# received whilst moving South 
     if (lenSouth > 0 && lenNorth >0 && MoveSouth==1) {digitalWrite(South, val2);lenSouth=0;MoveSouth=0;Serial.print (char(115));lenNorth=lenNorth+BackNorth;}
 
  // Keep moving West after a #:Mgwyyyy# command for the received period
     if (lenWest > 0 && lenEast ==0) {digitalWrite(West, val1);digitalWrite(East, val2);if(MoveWest != 1) {Serial.print (char(87));}MoveWest=1;lenEast=0;lenWest=lenWest-10;
     if (lenWest <10) {digitalWrite(West, val2);lenWest=0;MoveWest=0;Serial.print (char(119));Serial.print (char(10));}}

  // Cancel West if East #:Mgeyyyy# received whilst moving West 
     if (lenWest > 0 && lenEast >0 && MoveWest==1) {digitalWrite(West, val2);lenWest=0;MoveWest=0;Serial.print (char(119));lenEast=lenEast+BackEast;} 
     
  // increment time since last command
     Timer = Timer + 1;
     timer3 = timer3 + 1; 
         
  // ensure Timer always a positive number, after rollover at 32,768.
     if (Timer > 30000) {Timer = 0;}
     if (timer3 > timer2) {timer3 = 0;}
     
     if (Timer > timer1) {
     
     analog4 = analogRead(4);analog5 = analogRead(5);
     if (timer3 == 0 and drift == 1) 
     {
     if (analog4 < 451) {lenWest = 500 - analog4;}
     if (analog4 > 549) {lenEast = analog4 -500;}
     if (analog5 < 451) {lenSouth = 500 - analog5;}
     if (analog5 > 549) {lenNorth = analog5 -500;}
     }
     }     
    
 } 
     
   // read the first 2 characters of a command
      delay (1);
      inByte1 = Serial.read(); 
      delay (1);
      inByte2 = Serial.read(); 
          
   // calculate value for first 2 characters of a command
      int valByte = (256 * inByte1) + inByte2;
            
      switch (valByte) {
      case 19822: 
   // #:Mn# Move North   
      digitalWrite(South, val2);Serial.print (char(115));lenSouth=0;MoveSouth=0;digitalWrite(North, val1);lenNorth=0;MoveNorth=0;Serial.print (char(78));Timer = 0;
      break;
      case 19827:
   // #:Ms# Move South    
      digitalWrite(North, val2);Serial.print (char(110));lenNorth=0;MoveNorth=0;digitalWrite(South, val1);lenSouth=0;MoveSouth=0;Serial.print (char(83));Timer = 0;
      break;
      case 19831: 
   // #:Mw# Move West   
      digitalWrite(East, val2);Serial.print (char(101));lenEast=0;MoveEast=0;digitalWrite(West, val1);lenWest=0;MoveWest=0;Serial.print (char(87));Timer = 0;
      break;
      case 19813: 
   // #:Me# Move East   
      digitalWrite(West, val2);Serial.print (char(119));lenWest=0;MoveWest=0;digitalWrite(East, val1);lenEast=0;MoveEast=0;Serial.print (char(69));Timer = 0;
      break;
      case 20846:  
   // #:Qn# Quit North  
      digitalWrite(North, val2);lenNorth = 0; MoveNorth = 0; Serial.print (char(110)); Timer = 0;
      break;
      case 20851:
   // #:Qs# Quit South
      digitalWrite(South, val2); lenSouth = 0; MoveSouth = 0; Serial.print (char(115)); Timer = 0;
      break;
      case 20855:
   // #:Qw# Quit West    
      digitalWrite(West, val2); lenWest = 0; MoveWest = 0; Serial.print (char(119)); Timer = 0;
      break;
      case 20837: 
   // #:Qe# Quit East   
      digitalWrite(East, val2); lenEast = 0; MoveEast = 0; Serial.print (char(101)); Timer = 0;
      break;
      case 20771:
   // #:Q# Quit all moves     
      digitalWrite(West, val2);digitalWrite(South, val2);digitalWrite(North, val2);digitalWrite(East, val2);
      lenNorth = 0; MoveNorth = 0; lenSouth = 0; MoveSouth = 0; lenWest = 0; MoveWest = 0; lenEast = 0; MoveEast = 0;
      Serial.print (char(110)); Serial.print (char(115)); Serial.print (char(119)); Serial.print (char(101)); Timer = 0;
      break;
      case 17963: 
   // #:F+# Focus outward   
      if (pwm < 1) {RotateMotor(CCW, slowmotor, fspeed, fspeed, val2);}
      if (pwm > 0) {RotateMotor(CCW, pwmslowmotor, fspeed2, fspeed1, val2);}
      break;
      case 17965:
   // #:F-# Focus inward   
      if (pwm < 1) {RotateMotor(CW, slowmotor, fspeed, fspeed, val2);}
      if (pwm > 0) {RotateMotor(CW, pwmslowmotor, fspeed1, fspeed2, val2);}
      break;
      case 17990:
   // #:FF# Focus Fast   
      fspeed1 = fast; fspeed2 = fast;
      if (pwm > 0) {fspeed1 = pwmfast1; fspeed2 = pwmfast2;}
      break;
      case 17997:
   // #:FM# Focus Medium  
      fspeed1 = medm; fspeed2 = medm;
      if (pwm > 0) {fspeed1 = pwmmedm1; fspeed2 = pwmmedm2;}
      break;
      case 18003:
   // #:FS# Focus Slow   
      fspeed1 = slow; fspeed2 = slow;
      if (pwm > 0) {fspeed1 = pwmslow1; fspeed2 = pwmslow2;}
      break;
      case 18258:
   // #:GR# 00:00.0# 
      Serial.print (char(48));Serial.print (char(48));Serial.print (char(58));Serial.print (char(48));
      Serial.print (char(48));Serial.print (char(46));Serial.print (char(48));Serial.print (char(35));
      break;
      case 18259:
   // #:GS# 00:00:00#  
      Serial.print (char(48));Serial.print (char(48));Serial.print (char(58));Serial.print (char(48));
      Serial.print (char(48));Serial.print (char(58));Serial.print (char(48));Serial.print (char(48));
      Serial.print (char(35));
      break;
      case 18263:
   // #:GW# AT0
      Serial.print (char(65));Serial.print (char(84));Serial.print (char(48));
      break;
      case 18266:
   // #:GZ# 000*00'00# 
      Serial.print (char(48));Serial.print (char(48));Serial.print (char(48));Serial.print (char(42));Serial.print (char(48));
      Serial.print (char(48));Serial.print (char(39));Serial.print (char(48));Serial.print (char(48));Serial.print (char(35));
      break;
      case 18244:
   // #:GD# s00*00'00#
      Serial.print (char(115));Serial.print (char(48));Serial.print (char(48));Serial.print (char(42));Serial.print (char(48));
      Serial.print (char(48));Serial.print (char(39));Serial.print (char(48));Serial.print (char(48));Serial.print (char(35));
      break;
      case 18241:
   // #:GA# s00*00'00#
      Serial.print (char(115));Serial.print (char(48));Serial.print (char(48));Serial.print (char(42));Serial.print (char(48));
      Serial.print (char(48));Serial.print (char(39));Serial.print (char(48));Serial.print (char(48));Serial.print (char(35));
      break;
      case 18292:
   // #:Gt# s00*00#
      Serial.print (char(115));Serial.print (char(48));Serial.print (char(48));Serial.print (char(42));Serial.print (char(48));
      Serial.print (char(48));Serial.print (char(35));
      break;
      case 16171: 
   // #:?+# 00:00:00#
      Serial.print (char(48));Serial.print (char(48));Serial.print (char(58));Serial.print (char(48));Serial.print (char(48));
      Serial.print (char(58));Serial.print (char(48));Serial.print (char(48));Serial.print (char(35));
      break;
      case 19540:
   // #:LT# LED TEST 
      digitalWrite(West, val1); delay(500);digitalWrite(West,val2); 
      digitalWrite(South, val1); delay(500);digitalWrite(South,val2); 
      digitalWrite(North, val1); delay(500);digitalWrite(North,val2); 
      digitalWrite(East, val1); delay(500);digitalWrite(East,val2); 
      break;
      case 18000: 
   // #:FPsDDDDD# Move focus with pulse length eg :FP+010000# 
   // read direction from  command #:FPsDDDDD#
      delay (1);
      inByte3 = Serial.read(); 
              
   // act on 3rd character = direction
      switch (inByte3) 
      {
      case 43: 
   // #:FP+DDDDD# Move Focus 
      delay (1);
      inByte4 = Serial.read();
      delay (1);
      inByte5 = Serial.read();
      delay (1);
      inByte6 = Serial.read();
      delay (1);
      inByte7 = Serial.read();
      delay (1);
      inByte8 = Serial.read();
   // calculate pulse length from #:FP+DDDDD# command
      lenfocus = ((10000 * (inByte4-48)) +(1000 * (inByte5-48)) +(100 * (inByte6-48))+(10 * (inByte7-48))+(inByte8-48));
      if (pwm < 1) {RotateMotor(CW, lenfocus, slow, slow, val2);}
      if (pwm > 0) {RotateMotor(CW, lenfocus, pwmslow2, pwmslow1, val2);}
      break;
      
      case 45: 
   // #:FP-DDDDD# Move Focus 
      delay (1);
      inByte4 = Serial.read();
      delay (1);
      inByte5 = Serial.read();
      delay (1);
      inByte6 = Serial.read();
      delay (1);
      inByte7 = Serial.read();
      delay (1);
      inByte8 = Serial.read();
   // calculate pulse length from #:FP-DDDDD# command
      lenfocus = ((10000 * (inByte4-48)) +(1000 * (inByte5-48)) +(100 * (inByte6-48))+(10 * (inByte7-48))+(inByte8-48));
      if (pwm < 1) {RotateMotor(CCW, lenfocus, slow, slow, val2);}
      if (pwm > 0) {RotateMotor(CCW, lenfocus, pwmslow2, pwmslow1, val2);}
      break;
      }
      
      case 19815: 
   // #:Mgx Move with pulse length eg :Mgx0750# 
   // read direction from :Mgxyyyy# command 
      delay (1);
      inByte3 = Serial.read(); 
      Timer = 0;
        
   // act on 3rd character = direction
      switch (inByte3) {
      case 110: 
   // #:Mgnyyyy# Move North for yyyy mS 
      delay (1);Timer = 0;
      inByte4 = Serial.read();
      delay (1);
      inByte5 = Serial.read();
      delay (1);
      inByte6 = Serial.read();
      delay (1);
      inByte7 = Serial.read();
      
   // calculate pulse length from #:Mgnyyyy# command
      if (ScaleNorth == 100) {lenNorth = ((1000 * (inByte4-48)) +(100 * (inByte5-48))+(10 * (inByte6-48))+(inByte7-48));}
      if (ScaleNorth != 100) {lenNorth = ((ScaleNorth/10) *(100 * (inByte4-48)) +(10 * (inByte5-48))+(1 * (inByte6-48)));}
      if (lenNorth < SensNorth) {lenNorth = 0;digitalWrite(South, val2);digitalWrite(North, val2);}
      // Serial.print (lenNorth); Serial.print (char(10)); 
      break;
      case 101: 
   // #:Mgeyyyy# Move East for yyyy mS 
      delay (1);Timer = 0;
      inByte4 = Serial.read();
      delay (1);
      inByte5 = Serial.read();
      delay (1);
      inByte6 = Serial.read();
      delay (1);
      inByte7 = Serial.read();
   // calculate pulse length from #:Mgeyyyy# command
      if (ScaleEast == 100) {lenEast = ((1000 * (inByte4-48)) +(100 * (inByte5-48))+(10 * (inByte6-48))+(inByte7-48));}
      if (ScaleEast != 100) {lenEast = ((ScaleNorth/10) *(100 * (inByte4-48)) +(10 * (inByte5-48))+(1 * (inByte6-48)));}
      if (lenEast < SensEast) {lenEast = 0;digitalWrite(West, val2);digitalWrite(East, val2);}
      
      break; 
      case 115: 
   // #:Mgsyyyy#    Move South for yyyy mS 
      delay (1);Timer = 0;
      inByte4 = Serial.read();
      delay (1);
      inByte5 = Serial.read();
      delay (1);
      inByte6 = Serial.read();
      delay (1);
      inByte7 = Serial.read();
   // calculate pulse length from #:Mgsyyyy# command
      if (ScaleSouth == 100) {lenSouth = ((1000 * (inByte4-48)) +(100 * (inByte5-48))+(10 * (inByte6-48))+(inByte7-48));}
      if (ScaleSouth != 100) {lenSouth = ((ScaleNorth/10) *(100 * (inByte4-48)) +(10 * (inByte5-48))+(1 * (inByte6-48)));}
      if (lenSouth < SensSouth) {lenSouth = 0;digitalWrite(South, val2);digitalWrite(North, val2);}
      break;
      case 119: 
   // #:Mgwyyyy#    Move West for yyyy mS 
      delay (1);Timer = 0;
      inByte4 = Serial.read();
      delay (1);
      inByte5 = Serial.read();
      delay (1);
      inByte6 = Serial.read();
      delay (1);
      inByte7 = Serial.read();
   // calculate pulse length from #:Mgwyyyy# command
      if (ScaleWest == 100) {lenWest = ((1000 * (inByte4-48)) +(100 * (inByte5-48))+(10 * (inByte6-48))+(inByte7-48));}
      if (ScaleWest != 100) {lenWest = ((ScaleNorth/10) *(100 * (inByte4-48)) +(10 * (inByte5-48))+(1 * (inByte6-48)));}
      if (lenWest < SensWest) {lenWest = 0;digitalWrite(West, val2);digitalWrite(East, val2);}
      break; 
      // default:   Serial.print (char(35)); 
          }
          
      case 18262: 
   // #:GV*# get command with 3 characters, eg :GVT# 
   // read 3rd character from #:GVx# command 
      delay (1);
      inByte3 = Serial.read(); 
   // act on 3rd character 
      switch (inByte3) {
      case 84: 
   // #:GVT# Get Time 00:00:00#
      Serial.print (char(48));Serial.print (char(48));Serial.print (char(58));Serial.print (char(48));Serial.print (char(48));
      Serial.print (char(58));Serial.print (char(48));Serial.print (char(48));Serial.print (char(35));
      break;
      case 80: 
   // #:GVP# Get Product Name ETX Autostar#  
      Serial.print (char(69));Serial.print (char(84));Serial.print (char(88));Serial.print (char(32));Serial.print (char(65));
      Serial.print (char(117));Serial.print (char(116));Serial.print (char(111));Serial.print (char(115));Serial.print (char(116));
      Serial.print (char(97));Serial.print (char(114));Serial.print (char(35));
      break; 
      case 70: 
   // #:GVF# ETX Autostar#
      Serial.print (char(69));Serial.print (char(84));Serial.print (char(88));Serial.print (char(32));Serial.print (char(65));
      Serial.print (char(117));Serial.print (char(116));Serial.print (char(111));Serial.print (char(115));Serial.print (char(116));
      Serial.print (char(97));Serial.print (char(114));Serial.print (char(35));
      break; 
      case 78: 
   // #:GVN# Get Emulator Software Version 12.C#
      Serial.print (char(49));Serial.print (char(50));Serial.print (char(46));Serial.print (char(99));Serial.print (char(53));
      Serial.print (char(32));Serial.print (char(110));Serial.print (char(115));Serial.print (char(35));
      break;
      case 68: 
   // #:GVD# Get Emulator Software Version Date  011 10 2013#
      Serial.print (char(48));Serial.print (char(49));Serial.print (char(49));Serial.print (char(32));Serial.print (char(49));
      Serial.print (char(48));Serial.print (char(32));Serial.print (char(50));Serial.print (char(48));Serial.print (char(49));
      Serial.print (char(51));Serial.print (char(35));
      break;
 
   // default:  Serial.print (char(35)); 
          }
   // default:  Serial.print (char(35));
      
    } 
      goto label;
  }
   
     void vRotateMotor(int Direction, int Steps, int Speed1, int Speed2, int val2)
    
     {
     int analog4 = 0; analog4 = analogRead(4);
     int analog5 = 0; analog5 = analogRead(5); 
     // Serial.print (analog4);Serial.print (char(32));Serial.print (analog5);Serial.print (char(10));
     while (digitalRead(12)== LOW && (analog4 < 401 or analog4 > 599))
     {
     int i; analog4 = analogRead(4);
     Speed1 = 1; Speed2 = 1;
     if (pwm > 0 && analog4 < 401 ) {Speed1 = (((400-analog4)/17.54)+8); Speed2 = 32-Speed1; Direction = CW; Steps = 1;}
     if (pwm > 0 && analog4 > 599 ) {Speed1 = 24-((analog4-600)/19.23); Speed2 = 32-Speed1; Direction = CCW; Steps = 1;}
     // Serial.print (analog4);Serial.print (char(32));Serial.print (Speed1);Serial.print (char(32));Serial.print (Speed2);Serial.print (char(10));
     
     if (pwm < 1 && analog4 < 399) {Speed1 = slow; Speed2 = slow; Direction = CW; Steps = 1;}
     if (pwm < 1 && analog4 < 300) {Speed1 = medm; Speed2 = medm; Direction = CW; Steps = 1;}
     if (pwm < 1 && analog4 < 200) {Speed1 = fast; Speed2 = fast; Direction = CW; Steps = 1;}
     
     if (pwm < 1 && analog4 > 601) {Speed1 = slow; Speed2 = slow; Direction = CCW; Steps = 1;}
     if (pwm < 1 && analog4 > 700) {Speed1 = medm; Speed2 = medm; Direction = CCW; Steps = 1;}
     if (pwm < 1 && analog4 > 800) {Speed1 = fast; Speed2 = fast; Direction = CCW; Steps = 1;}
     
     digitalWrite(West, val2);digitalWrite(South, val2);digitalWrite(North, val2);digitalWrite(East, val2);
     digitalWrite(MotorDir, Direction);    
     for(i = 0; i < Steps; i++)
     {
     digitalWrite(MotorStep, HIGH);
     if (pwm > 0) {delay (Speed1);}
     if (pwm < 1) {delayMicroseconds(Speed1);}
     digitalWrite(MotorStep, LOW);
     if (pwm > 0) {delay(Speed2);}
     if (pwm < 1) {delayMicroseconds(Speed2);}
     } 
     }
     digitalWrite(MotorDir, LOW);
     }
     
     void RotateMotor(int Direction, int Steps, int Speed1, int Speed2, int val2) 
     {
     int i;
     digitalWrite(West, val2);digitalWrite(South, val2);digitalWrite(North, val2);digitalWrite(East, val2);
     digitalWrite(MotorDir, Direction);    
     for(i = 0; i < Steps; i++)
     {
     digitalWrite(MotorStep, HIGH);
     if (pwm > 0) {delay (Speed1);}
     if (pwm < 1) {delayMicroseconds(Speed1);}
     digitalWrite(MotorStep, LOW);
     if (pwm > 0) {delay(Speed2);}
     if (pwm < 1) {delayMicroseconds(Speed2);}
     } 
     digitalWrite(MotorDir, LOW);
     } 

    


