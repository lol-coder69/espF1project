#include <Arduino.h>
#include <Adafruit_Fingerprint.h>

HardwareSerial serialport(2);// use UART2
Adafruit_Fingerprint fng = Adafruit_Fingerprint(&serialport);

uint8_t id;
uint8_t getfingerId();

void setup()
{
    Serial.begin(9600);
    while(!Serial);
    delay(100);
    Serial.println("\n\n detecting fingerprint");
    fng.begin(57600);// set the data rate for the sensor serial port
    delay(5);
    if (fng.verifyPassword())
    {
        Serial.println("\n\n found the fingerprint sensor");
    }
    else
    {
        Serial.println("\n\n unable to find the fingerprint sensor");
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
}
uint8_t readnum(void)
{
    uint8_t num = 0;
    while(num == 0)
    {
        while(!Serial.available());
        num = Serial.parseInt();
    }
    return num;
}
void loop()// run over and over again
{
    Serial.println("please type the id number (from 1 to 100) you want to save this as");
    id = readnum();
    if(id == 0)// ID #0 not allowed, try again!
    {
        return;
    }
    Serial.println("enrolling id number");
    Serial.println(id);
    while(!getfingerId());
}
uint8_t getfingerId()
{
    int fngId = -1;
    Serial.println(id);
    while(fngId != FINGERPRINT_OK)
    {
        fngId = fng.getImage();
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
    } // OK success!
    fngId = fng.image2Tz(1);
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
    Serial.println("remove the finger");
    delay(2000);
    fngId = 0;
    while(fngId != FINGERPRINT_NOFINGER)
    {
        fngId = fng.getImage();
    }
    Serial.print("id is");
    Serial.println(id);
    fngId  = -1;
    Serial.println("place the same finger again");
    while(fngId != FINGERPRINT_OK)
    {
        fngId = fng.getImage();
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
    } // OK success!
    fngId = fng.image2Tz(2);
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
        }// OK converted!
    Serial.println("creating model for the id: ");
    Serial.print(id);
    fngId = fng.createModel();
    if(fngId == FINGERPRINT_OK)
    {
        Serial.println("prints have matched");
    }
    else if(fngId == FINGERPRINT_PACKETRECIEVEERR)
    {
        Serial.println("communication error has occured");
        return fngId;
    }
    else if(fngId == FINGERPRINT_ENROLLMISMATCH)
    {
        Serial.println("fingerprint didnot match");
        return fngId;
    }
    else
    {
        Serial.println("encountered an unkown error");
        return fngId;
    }
    Serial.println("id is :");
    Serial.println(id);
    fngId = fng.storeModel(id);
    if(fngId == FINGERPRINT_OK)
    {
        Serial.println("prints have matched");
    }
    else if(fngId == FINGERPRINT_PACKETRECIEVEERR)
    {
        Serial.println("communication error has occured");
        return fngId;
    }
    else if(fngId == FINGERPRINT_BADLOCATION)
    {
        Serial.println("communication error has occured");
        return fngId;
    }
    else
    {
        Serial.println("encountered an unkown error");
        return fngId;
    }
    return true;
}
