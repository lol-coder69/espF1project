#include <Arduino.h>

#include <Adafruit_Fingerprint.h>

HardwareSerial serialPort(2); 
Adafruit_Fingerprint fng = Adafruit_Fingerprint(&serialPort);
int getFingerprintIDez();
void printHex(int num, int precision);
uint8_t downloadFngTemplate(uint16_t id);
void setup()
{
  while (!Serial)
    ;
  Serial.begin(9600);
  Serial.println("Fingerprint template extractor");
  fng.begin(57600);
  if (fng.verifyPassword())
  {
    Serial.println("Found fingerprint sensor!");
  }
  else
  {
    Serial.println("Did not find fingerprint sensor :(");
    while (1)
      ;
  }
  for (int fng = 1; fng < 10; fng++)
  {
    downloadFngTemplate(fng);
  }
}

uint8_t downloadFngTemplate(uint16_t id)
{
  Serial.println("------------------------------------");
  Serial.print("Attempting to load #");
  Serial.println(id);
  uint8_t p = fng.loadModel(id);
  switch (p)
  {
  case FINGERPRINT_OK:
    Serial.print("Template ");
    Serial.print(id);
    Serial.println(" loaded");
    break;
  case FINGERPRINT_PACKETRECIEVEERR:
    Serial.println("Communication error");
    return p;
  default:
    Serial.print("Unknown error ");
    Serial.println(p);
    return p;
  }
  Serial.print("Attempting to get #");
  Serial.println(id);
  p = fng.getModel();
  switch (p)
  {
  case FINGERPRINT_OK:
    Serial.print("Template ");
    Serial.print(id);
    Serial.println(" transferring:");
    break;
  default:
    Serial.print("Unknown error ");
    Serial.println(p);
    return p;
  }
  uint8_t bytesReceived[534]; 
  memset(bytesReceived, 0xff, 534);
  uint32_t starttime = millis();
  int i = 0;
  while (i < 534 && (millis() - starttime) < 20000)
  {
    if (serialPort.available())
    {
      bytesReceived[i++] = serialPort.read();
    }
  }
  Serial.print(i);
  Serial.println(" bytes read.");
  Serial.println("Decoding packet...");
  uint8_t fingerTemplate[512]; 
  memset(fingerTemplate, 0xff, 512);
  int uindx = 9, index = 0;
  memcpy(fingerTemplate + index, bytesReceived + uindx, 256); // first 256 bytes
  uindx += 256;                                               // skip data
  uindx += 2;                                                 // skip checksum
  uindx += 9;                                                 // skip next header
  index += 256;                                               // advance pointer
  memcpy(fingerTemplate + index, bytesReceived + uindx, 256); // second 256 bytes
  for (int i = 0; i < 512; ++i)
  {
    printHex(fingerTemplate[i], 2);
  }
  Serial.println("\ndone.");
  return p;
  /*
    uint8_t templateBuffer[256];
    memset(templateBuffer, 0xff, 256);  //zero out template buffer
    int index=0;
    uint32_t starttime = millis();
    while ((index < 256) && ((millis() - starttime) < 1000))
    {
    if (mySerial.available())
    {
      templateBuffer[index] = mySerial.read();
      index++;
    }
    }

    Serial.print(index); Serial.println(" bytes read");

    //dump entire templateBuffer.  This prints out 16 lines of 16 bytes
    for (int count= 0; count < 16; count++)
    {
    for (int i = 0; i < 16; i++)
    {
      Serial.print("0x");
      Serial.print(templateBuffer[count*16+i], HEX);
      Serial.print(", ");
    }
    Serial.println();
    }*/
}
void printHex(int num, int precision)
{
  char tmp[16];
  char format[128];

  sprintf(format, "%%.%dX", precision);

  sprintf(tmp, format, num);
  Serial.print(tmp);
}
void loop()
{
}
