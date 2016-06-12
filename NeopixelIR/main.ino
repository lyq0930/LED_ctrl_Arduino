void stripComplete();
void pickAPattern(byte i);

NeoPatterns strip(Num_pixels, PIN, NEO_GRBW + NEO_KHZ800, &stripComplete);

void setup(void){
  Serial.begin(9600);
  Serial.println("Progam start");
  strip.begin();
  irrecv.enableIRIn();
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
  if(currentMillis - previousMillis > 70000){
    strip.RainbowCycle(20);
    previousMillis = currentMillis;
  }
  if(currentMillis - previousMillis2 > 50000){
    strip.Scanner(strip.Wheel(random(255)), 10);
    previousMillis2 = currentMillis;
  }
  if(currentMillis - previousMillis3 > 40000){
    strip.Fade(strip.Color1, strip.Color2, 100, 200);
    previousMillis3 = currentMillis;
  }
  if(currentMillis - previousMillis4 > 30000){
    strip.TheaterChase(strip.Wheel(random(255)), strip.Wheel(random(255)), 80);
    previousMillis4 = currentMillis;
  }

  if (irrecv.decode(&results)) {
    Serial.print("Received one signal: ");  
    Serial.println(results.value, HEX);
    if (results.value == 0xFF906F) {
      Serial.println("Press RED");
      strip.RainbowCycle(20);
    }
    if (results.value == 0xFF10EF) {
      Serial.println("Press Green");
      strip.Scanner(strip.Wheel(random(255)), 10);
    }
//    if(currentMillis - previousMillis3 > 40000){
//      strip.Fade(strip.Color1, strip.Color2, 100, 200);
//    }
//    if(currentMillis - previousMillis4 > 30000){
//      strip.TheaterChase(strip.Wheel(random(255)), strip.Wheel(random(255)), 80);
//    }
    irrecv.resume();
    Serial.print("new value: ");
    Serial.println(results.value, HEX);  
  }

//  if(currentMillis - previousMillis > 1000){
//    if(flag == 1){
//      irrecv.resume();
//      flag = 0;
//    }
//  }


//  if(currentMillis - previousMillis5 > 16000){
//    strip.pulseWhite();
//    previousMillis5 = currentMillis;
//  }
    
//  if (irrecv.decode(&results)) {
//    Serial.println("Received one signal");  
//    Serial.println(results.value, HEX);
//    
//    if (results.value == 0xFF906F) {
//      Serial.println("Press RED");
//      pickAPattern(1);
//
//    }
//
//    if (results.value == 0xFF10EF) {
//      Serial.println("Button2");
//      strip.ActivePattern = COLOR_WIPE;
//      strip.Interval = 60;
//      strip.Index = 0;
//    }
//    irrecv.resume();  
//  }
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

