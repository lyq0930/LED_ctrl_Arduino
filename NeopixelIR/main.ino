void stripComplete();
NeoPatterns strip(Num_pixels, PIN, NEO_GRBW + NEO_KHZ800, &stripComplete);

void setup(void){
  Serial.begin(9600);
  Serial.println("Progam start");
  strip.begin();
  irrecv.enableIRIn();
  i_p = 1;
  strip.ActivePattern = THEATER_CHASE;
  strip.TotalSteps = 255;
  strip.Interval = 10;
  strip.Color1 = strip.Wheel(random(255));
  strip.Color2 = strip.Wheel(random(255));
}

void loop(void){
  unsigned long currentMillis = millis();
  strip.Update();
//  if (irrecv.decode(&results)) {
//    Serial.println("Received one signal");  
//    Serial.println(results.value, HEX);
//    irrecv.resume();  
//    
////    if (results.value == 0xFF906F) {
////      Serial.println("Press RED");
////      strip.ColorWipe(strip.Color(255, 0, 0), 0);
////    }
////
////    if (results.value == 0xFF10EF) {
////      Serial.println("Button2");
////      strip.ColorWipe(strip.Color(0, 255, 0), 0);
////    }
////    
////    if (results.value == 0xFF50AF) {
////      Serial.println("Button3");
////      strip.ColorWipe(strip.Color(0, 0, 255), 0);
////    }
////    
////    if (results.value == 0xFFD02F) {
////      Serial.println("Button4");
////      strip.ColorWipe(strip.Color(255, 255, 255), 0);
////    }
////    
//    if (results.value == 0xFFC837) {
//      Serial.println("Button5");
//      strip.ActivePattern = THEATER_CHASE;
//      strip.Interval = 100;
//      strip.ActivePattern = RAINBOW_CYCLE;
//      strip.TotalSteps = 255;
//      strip.Interval = 10;
//    }  
////    if(currentMillis - previousMillis > 1000) {
////      previousMillis = currentMillis; 
////      irrecv.resume();
////      Serial.println("ir resetted");
////    }  
//    irrecv.resume();  
//  }
}

void stripComplete(){
    DEBUG_PRINTLN("Enterring stripComplete()");
  if(i_p < 7){
    i_p++;
    strip.ActivePattern = pattern(i_p);
  }else{
    i_p = 0;
  }
  strip.Update();
}

