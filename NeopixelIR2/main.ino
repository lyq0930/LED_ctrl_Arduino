void stripComplete();
void pickAPattern(byte i);

NeoPatterns strip(Num_pixels, PIN, NEO_GRBW + NEO_KHZ800, &stripComplete);

void setup(void){
  Serial.begin(9600);
  Serial.println("Progam start");
  strip.begin();
  My_Receiver.Mark_Excess=50; //us; mark/space correction factor
  My_Receiver.enableIRIn(); // Start the receiver
  i_p = 1;
  strip.ActivePattern = THEATER_CHASE;
  strip.TotalSteps = 255;
  strip.Interval = 200;
  strip.Color1 = strip.Wheel(random(255));
  strip.Color2 = strip.Wheel(random(255));
}

void loop(void){
  unsigned long currentMillis = millis();
  strip.Update();
  
  if (My_Receiver.getResults(&My_Decoder)){ //if IR data is ready to be decoded 
    My_Decoder.decode();
    Serial.print("Received one signal: ");  
    Serial.println(My_Decoder.value, HEX);
    if (My_Decoder.value == 0xFF906F) {
      Serial.println("Press RED");
      strip.RainbowCycle(20);
    }
    if (My_Decoder.value == 0xFF10EF) {
      Serial.println("Press Green");
      strip.Scanner(strip.Wheel(random(255)), 10);
    }
    //My_Decoder.dumpResults();
    My_Receiver.resume(); 
    Serial.print("new value: ");
    Serial.println(My_Decoder.value, HEX);  
  }
}

void stripComplete(){
//  DEBUG_PRINTLN("Enterring stripComplete()");
//  if(i_p < 5){
//    i_p++;
//  }else{
//    i_p = 1;
//  }
//  strip.ActivePattern = pattern(i_p);
//  DEBUG_PRINTLN(patterns[i_p]);
//  strip.Update();
}

void pickAPattern(byte i){
  DEBUG_PRINT("Picking a pattern, you picked: ");
  DEBUG_PRINTLN(patterns[i]);
  strip.ActivePattern = pattern(i);
  strip.Update();
}

