#include <Arduino.h>
#include <DSpotterSDK_MakerHL.h>
#include <LED_Control.h>
#include "CybLicense_1694078235.h"

#define DSPOTTER_LICENSE g_lpdwLicense

#define COMMAND_ROT_BLINKEN    10000
#define COMMAND_GRUEN_BLINKEN  10001
#define COMMAND_BLAU_BLINKEN   10002
#define COMMAND_REGENBOGEN     10003
#define COMMAND_WEISSES_LICHT  10004

#if defined(TARGET_ARDUINO_NANO33BLE) || defined(TARGET_PORTENTA_H7) || defined(TARGET_NICLA_VISION)
#include "Model_1694078235.h"             // The packed level one model file.
#elif defined(TARGET_NANO_RP2040_CONNECT)
#include "Model_1694078235.h"             // The packed level zero model file.
#endif

int R=0;
int G=64;
int B=128;
int RI=1;
int GI=1;
int BI=1;

#define DSPOTTER_MODEL g_lpdwModel

// The VR engine object. Only can exist one, otherwise not worked.
static DSpotterSDKHL g_oDSpotterSDKHL;

// Callback function for VR engine
void VRCallback(int nFlag, int nID, int nScore, int nSG, int nEnergy)
{
  if (nFlag==DSpotterSDKHL::InitSuccess)
  {
      //ToDo
  }
  else if (nFlag==DSpotterSDKHL::GetResult)
  {
     switch(nID)
      {
        case COMMAND_ROT_BLINKEN:
          //LED_RGB_Green();
            Serial.println("rot blinken");
            for(int i=0; i<10; i++) {
              analogWrite(LEDR, 0);
              delay(200);
              analogWrite(LEDR, 255);
              delay(200);
            }
          break;
        case COMMAND_GRUEN_BLINKEN:
            Serial.println("grün blinken");
            for(int i=0; i<10; i++) {
              analogWrite(LEDG, 0);
              delay(200);
              analogWrite(LEDG, 255);
              delay(200);
            }
          break;
        case COMMAND_BLAU_BLINKEN:
            Serial.println("blau blinken");
            for(int i=0; i<10; i++) {
              analogWrite(LEDB, 0);
              delay(200);
              analogWrite(LEDB, 255);
              delay(200);
            }
          break;
        case COMMAND_REGENBOGEN:
            Serial.println("Regenbogen");

            for(int i=0; i<10; i++) { 
              for(int y=0;y<255;y++) {  
                analogWrite(LEDG, R);
                analogWrite(LEDB, G);
                analogWrite(LEDR, B);
                G=G+GI;
                B=B+BI;
                if(R>254) RI=-1;
                if(G>254) GI=-1;
                if(B>254) BI=-1;
                if(R==0) RI=1;
                if(G==0) GI=1;
                if(B==0) BI=1;
                delay(10);
              }
            }
            analogWrite(LEDR, 255);
            analogWrite(LEDG, 255);
            analogWrite(LEDB, 255);
          break;        
        case COMMAND_WEISSES_LICHT:
          Serial.println("weisses Licht");
          analogWrite(LEDB, 0);
          analogWrite(LEDR, 0);
          analogWrite(LEDG, 0);
          delay(2000);
          analogWrite(LEDB, 255);
          analogWrite(LEDR, 255);
          analogWrite(LEDG, 255);
          break;
        default:
          break;
      }
  }
  else if (nFlag==DSpotterSDKHL::ChangeStage)
  {
      switch(nID)
      {
          case DSpotterSDKHL::TriggerStage:
            LED_RGB_Off();
            LED_BUILTIN_Off();
            break;
          case DSpotterSDKHL::CommandStage:
            LED_BUILTIN_On();
            break;
          default:
            break;
      }
  }
  else if (nFlag==DSpotterSDKHL::GetError)
  {
      if (nID == DSpotterSDKHL::LicenseFailed)
      {
          //Serial.print("DSpotter license failed! The serial number of your device is ");
          //Serial.println(DSpotterSDKHL::GetSerialNumber());
      }
      g_oDSpotterSDKHL.Release();
      while(1);//hang loop
  }
  else if (nFlag == DSpotterSDKHL::LostRecordFrame)
  {
      //ToDo
  }
}

void setup()
{
  // Init LED control
  LED_Init_All();
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);
  // Init Serial output for show debug info
  Serial.begin(9600);
  while(!Serial);
  DSpotterSDKHL::ShowDebugInfo(true);

  // Init VR engine & Audio
  if (g_oDSpotterSDKHL.Init(DSPOTTER_LICENSE, sizeof(DSPOTTER_LICENSE), DSPOTTER_MODEL, VRCallback) != DSpotterSDKHL::Success)
    return;
}

void loop()
{
  // Do VR
  g_oDSpotterSDKHL.DoVR();
}

