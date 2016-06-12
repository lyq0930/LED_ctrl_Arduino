/* Example program for from IRLib – an Arduino library for infrared encoding and decoding
 * Version 1.3   January 2014
 * Copyright 2014 by Chris Young http://cyborg5.com
 * Updates by Gabriel Staples -- http://www.ElectricRCAircraftGuy.com -- to make work with library V1.6.0,
 * and to add information on Mark_Excess, etc. 
 * Based on original example sketch for IRremote library 
 * Version 0.11 September, 2009
 * Copyright 2009 Ken Shirriff
 * http://www.righto.com/
 */
/* IRanalyze receives repeated values from a remote and averages the results. Should help in
 * analyzing unknown protocols. You have to press the same key repeatedly. If you press a 
 * different key the totals reset and it computes new averages.
 */
 
/*
Notes on Mark_Excess:
~By Gabriel Staples, 30 Jan. 2016 
Try different values here for Mark_Excess. 50us is a good starting guess. Ken Shirriff originally used 100us. 
It is assumed that your IR receiver filters the modulated signal such that Marks (LOW periods
from the IR receiver) are several dozen microseconds too long and Spaces (HIGH periods from the
IR receiver) are the same amount too short. This is the case for most IR receivers. Therefore, 
IRLib automatically *subtracts* Mark_Excess from Marks and *adds* Mark_Excess to Spaces 
after receiving the raw IR data, and before decoding it. 
If using the dirt-cheap (<$1 for 10) 1838 IR receivers from Ebay, however, 
I recommend setting Mark_Excess to -31us for IRrecv, and -37us for IRrecvPCI. If using the 
higher quality TSOP4838 ones, I recommend setting Mark_Excess to +45us for IRrecv, and +55us
for IRrecvPCI. If Mark_Excess is off by too much, your IR receiver will 
appear not to work correctly at all, and will not properly decode IR signals. To evaluate
your receiver, run the "IRanalyze.ino" sketch and carefully compare your outputs to what you 
should be getting for your particular protocol. You can find the timing values for each 
protocol in the decode functions of this library, ex: "IRdecodeNEC::decode" lists the timing
values used when decoding the very popular NEC protocol. 
*/
 
#include <IRLib.h>
#include <IRLibRData.h>

int RECV_PIN = 2;

unsigned int Samples,i, LowSpace, LowMark, HighMark, HighSpace, interval,
            balm,aalm,bahm,aahm,bals,aals,bahs,aahs;
unsigned char bacm,aacm,bacs,aacs, Mark_Count,Space_Count;
unsigned int Accumulated_Time[RAWBUF];
unsigned long Mark_Avg, Space_Avg,baam,aaam,baas,aaas;

//TRY THIS PROGRAM WITH VARIOUS RECEIVERS:
// IRrecv     My_Receiver(RECV_PIN); //50us timer-based interrupt receiver object 
// IRrecvLoop My_Receiver(RECV_PIN); //timer-free, interrupt-free loop polling-based receiver object 
IRrecvPCI  My_Receiver(0); //External pin change interrupt receiver object
                           //Use interrupt=0. This is pin 2 on Arduino Uno and Mega, pin 3 on Leonardo.
                           //See here for which pin it is on other Arduinos: https://www.arduino.cc/en/Reference/AttachInterrupt

IRdecode My_Decoder;
IR_types_t Old_Type;
unsigned long Old_Value;
void setup()
{
  Serial.begin(9600);
  delay(1000);while(!Serial);
  Serial.println(F("begin"));  
  //Try different values here for Mark_Excess. 50us is a good starting guess. See detailed notes above for more info.
  My_Receiver.Mark_Excess=45; //us; mark/space correction factor
  My_Receiver.enableIRIn();
  Samples=0;Old_Value=0; Old_Type=UNKNOWN;
  Serial.println(F("Send a signal repeatedly. We will report averages and statistics."));
}
void Tab(void) {Serial.print("\t");};
void loop() {
  if (My_Receiver.getResults(&My_Decoder)) {
    My_Decoder.decode();
    if( (My_Decoder.decode_type != Old_Type) || (My_Decoder.value != Old_Value)) {
      Serial.println(F("Resetting counters"));
      for(i=0;i<RAWBUF;i++) {
        Accumulated_Time[i]=0;
      };
      Samples=0;Old_Value= My_Decoder.value; Old_Type=My_Decoder.decode_type;
    };
    Samples++;
    Serial.print(F("\nSamples=")); Serial.println(Samples,DEC);
    LowSpace = LowMark=65535;
    HighSpace = HighMark=0;
    Mark_Avg= Space_Avg= Mark_Count= Space_Count=0;
    for(i=0;i<My_Decoder.rawlen;i++){
      Accumulated_Time[i]+=My_Decoder.rawbuf[i];
      My_Decoder.rawbuf[i]= Accumulated_Time[i]/Samples;//Put back average so dumpResults can report
    }
    My_Decoder.dumpResults();
    //Perform additional analysis
    for(i=3;i<My_Decoder.rawlen;i++){ //Compute low, high and average mark and space
      interval=My_Decoder.rawbuf[i];
      if(i % 2) {
        Mark_Avg += interval; LowMark=min(LowMark, interval);  HighMark=max(HighMark, interval);Mark_Count++;
      } else {
        Space_Avg += interval; LowSpace=min(LowSpace, interval);  HighSpace=max (HighSpace, interval);Space_Count++;
      }
      My_Decoder.rawbuf[i]= Accumulated_Time[i]/Samples;//Put back average so dumpResults can report
    }
    Mark_Avg /= Mark_Count; Space_Avg /= Space_Count;
    //Now compute below average highs and lows and above average highs and lows
    balm=bals=aalm=aals=32766;
    bahm=bahs=aahm=aahs=baam=baas=aaam=aaas=0;
    bacm=bacs=aacm=aacs=0;
    for(i=3;i<My_Decoder.rawlen;i++){
      interval=My_Decoder.rawbuf[i];
      if(i % 2) {
        if (interval>Mark_Avg) {
          aalm=min(aalm,interval); aahm=max(aahm,interval);aacm++;aaam+=interval;
        } else {
          balm=min(balm,interval); bahm=max(bahm,interval);bacm++;baam+=interval;
        }
      } else {
        if (interval>Space_Avg) {
          aals=min(aals,interval); aahs=max(aahs,interval);aacs++;aaas+=interval;
        } else {
          bals=min(bals,interval); bahs=max(bahs,interval);bacs++;baas+=interval;
        }
      }
    }
    baam/=bacm;baas/=bacs;aaam/=aacm;aaas/=aacs;
    Serial.println(F("\t #\tLow\tHigh\tAvg.")); 
    Serial.print(F("Mark\t"));Serial.print(Mark_Count,DEC);Tab();Serial.print(LowMark,DEC);Tab();Serial.print(HighMark,DEC);Tab();Serial.println(Mark_Avg,DEC);
    Serial.print(F("Space\t"));Serial.print(Space_Count,DEC);Tab();Serial.print(LowSpace,DEC);Tab();Serial.print(HighSpace,DEC);Tab();Serial.println(Space_Avg,DEC);

    Serial.println(F("\nMarks\t\t #\tLow\tHigh\tAvg.")); 
    Serial.print(F("Above Avg\t"));Serial.print(aacm,DEC);Tab();Serial.print(aalm,DEC);Tab();Serial.print(aahm,DEC);Tab();Serial.println(aaam,DEC);
    Serial.print(F("Belows Avg.\t"));Serial.print(bacm,DEC);Tab();Serial.print(balm,DEC);Tab();Serial.print(bahm,DEC);Tab();Serial.println(baam,DEC);
    
    Serial.println(F("\nSpaces\t\t #\tLow\tHigh\tAvg.")); 
    Serial.print(F("Above Avg.\t"));Serial.print(aacs,DEC);Tab();Serial.print(aals,DEC);Tab();Serial.print(aahs,DEC);Tab();Serial.println(aaas,DEC);
    Serial.print(F("Below Avg.\t"));Serial.print(bacs,DEC);Tab();Serial.print(bals,DEC);Tab();Serial.print(bahs,DEC);Tab();Serial.println(baas,DEC);

#define RATIO(n,d) Tab(); Serial.print(float(n)/float(d));
    Serial.print(F("\nRatios\t\tBelow\tAbove\n"));
    Serial.print(F("Head Mark"));RATIO(My_Decoder.rawbuf[1],baam);RATIO(My_Decoder.rawbuf[1],aaam);
    Serial.print(F("\nHead Space"));RATIO(My_Decoder.rawbuf[2],baas);RATIO(My_Decoder.rawbuf[2],aaas);
    Serial.print(F("\n\nMark Above/Below="));RATIO(aaam,baam);
    Serial.print(F("\nSpace Above/Below="));RATIO(aaas,baas);
    Serial.println();
    My_Receiver.resume();
  };
}

