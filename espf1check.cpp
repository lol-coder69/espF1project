#include <Arduino.h>
#include <Adafruit_Fingerprint.h>

HardwareSerial serialPort(2);
Adafruit_Fingerprint fng = Adafruit_Fingerprint(&serialPort);

uint8_t checkfingerId();

void setup()
{
    Serial.begin(9600);
    while(!Serial);
    delay(100);
    Serial.println("fingerprint detect test");
    fng.begin(57600);
    delay(5);
    if(fng.verifyPassword())
    {
        Serial.println("found the finerprint sensor");
    }
    else
    {
        Serial.println("did not find fingerprint sensor");
        while(1)
        {
            delay(1);
        }
    }
  Serial.println(F("Reading sensor parameters"));
  fng.getParameters();
  Serial.print(F("Status: 0x"));
  Serial.println(fng.status_reg, HEX);
  Serial.print(F("Sys ID: 0x"));
  Serial.println(fng.system_id, HEX);
  Serial.print(F("Capacity: "));
  Serial.println(fng.capacity);
  Serial.print(F("Security level: "));
  Serial.println(fng.security_level);
  Serial.print(F("Device address: "));
  Serial.println(fng.device_addr, HEX);
  Serial.print(F("Packet len: "));
  Serial.println(fng.packet_len);
  Serial.print(F("Baud rate: "));
  Serial.println(fng.baud_rate);

  fng.getTemplateCount();
  if(fng.templateCount == 0)
  {
    Serial.println("sensor does not contain any fingerprint data");
  }
  else
  {
    Serial.println("waiting for valid finger");
    Serial.println("sensor contains : ");
    Serial.print(fng.templateCount);
    Serial.println(" templates");
  }
}
void loop()
{
    checkfingerId();
    delay(50);
}
uint8_t checkfingerId()
{
    uint8_t fngId = fng.getImage();
    switch(fngId)
        {
            case FINGERPRINT_OK:
            Serial.println("fingerprint taken");
            break;

            case FINGERPRINT_NOFINGER:
            Serial.println("no finger has been detected");
            return fngId;

            case FINGERPRINT_PACKETRECIEVEERR:
            Serial.println("communication error");
            return fngId;

            case FINGERPRINT_IMAGEFAIL:
            Serial.println("failed to detect fingerprint");
            return fngId;

            default:
            Serial.println("encountered an unkown error");
            return fngId;
        }
    fngId = fng.image2Tz();
    switch(fngId)
        {
            case FINGERPRINT_OK:
            Serial.println("fingerprint taken");
            break;

            case FINGERPRINT_IMAGEMESS:
            Serial.println("the fingerprint is too messy");
            return fngId;

            case FINGERPRINT_PACKETRECIEVEERR:
            Serial.println("communication error");
            return fngId;

            case FINGERPRINT_FEATUREFAIL:
            Serial.println("failed to detect fingerprint sensor features");
            return fngId;

            case FINGERPRINT_INVALIDIMAGE:
            Serial.println("failed to detect sensor features");
            return fngId;

            default:
            Serial.println("encountered an unkown error");
            return fngId;
        }
    fngId = fng.fingerSearch();
    if(fngId == FINGERPRINT_OK)
    {
        Serial.println("found a print match");
    }
    else if(fngId == FINGERPRINT_PACKETRECIEVEERR)
    {
        Serial.println("Communication Error");
        return fngId;
    }
    else if(fngId == FINGERPRINT_NOTFOUND)
    {
        Serial.println("did not found a fingerprint match");
        return fngId;
    }
    else
    {
        Serial.println("unkown error encountered");
        return fngId;
    }
    Serial.println("printing the id");
    Serial.println(fng.fingerID);
    Serial.println(fng.confidence);

    return fng.fingerID;
}
int getfingerprintDez()
{
    uint8_t fngId = fng.getImage();
    if(fngId == FINGERPRINT_OK)
    {
        return -1;
    }
    fngId = fng.image2Tz();
    if(fngId != FINGERPRINT_OK)
    {
        return -1;
    }
    fngId = fng.fingerFastSearch();
    if(fngId != FINGERPRINT_OK)
    {
        return -1;
    }
    Serial.println("found an id no. ");
    Serial.println(fng.fingerID);
    Serial.println("confidence of ");
    Serial.println(fng.confidence);
    return fng.fingerID;
}


