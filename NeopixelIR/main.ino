void stripComplete();
void pickAPattern(byte i);

NeoPatterns strip(Num_pixels, PIN, NEO_GRBW + NEO_KHZ800, &stripComplete);

void setup(void){
  Serial.begin(9600);
  Serial.println("Progam start");
  strip.setBrightness(BRIGHTNESS);
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
  
  if(currentMillis - previousMillis > 10000 && strip.ActivePattern == THEATER_CHASE){
    strip.RainbowCycle(20);
    previousMillis = currentMillis;
  }
  if(currentMillis - previousMillis > 10000 && strip.ActivePattern == RAINBOW_CYCLE){
    strip.Scanner(strip.Wheel(random(255)), 10);
    previousMillis = currentMillis;
  }
  if(currentMillis - previousMillis > 10000 && strip.ActivePattern == SCANNER){
    strip.Fade(strip.Color1, strip.Color2, 100, 200);
    previousMillis = currentMillis;
  }
  if(currentMillis - previousMillis > 10000 && strip.ActivePattern == FADE){
    DEBUG_PRINTLN("WhiteOverRainbow...");
    strip.whiteOverRainbow();
    previousMillis = currentMillis;
  }
  if(currentMillis - previousMillis > 10000 && strip.ActivePattern == WhiteOverRainbow){
    strip.TheaterChase(strip.Wheel(random(255)), strip.Wheel(random(255)), 80);
    previousMillis = currentMillis;
  }
 
  if(currentMillis - previousMillis6 > 1000){
    DEBUG_PRINT(++timeInSecond);     DEBUG_PRINT("s, "); 
    previousMillis6 = currentMillis;
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
    if(currentMillis - previousMillis3 > 40000){
      strip.Fade(strip.Color1, strip.Color2, 100, 200);
    }
    if(currentMillis - previousMillis4 > 30000){
      strip.TheaterChase(strip.Wheel(random(255)), strip.Wheel(random(255)), 80);
    }
    irrecv.resume();
    Serial.print("new value: ");
    Serial.println(results.value, HEX);  
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
