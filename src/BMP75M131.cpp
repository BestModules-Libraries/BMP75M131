/*************************************************
File:               BMP75M131.cpp
Author:             BESTMODULES
Description:        I2C communication with the BMP75M131 and obtain the corresponding value
History：            
  V1.00.1   -- initial version；2023-04-03；Arduino IDE :  ≥v1.8.13
**************************************************/
#include "BMP75M131.h"

/*************************************************
Description:        Constructed function
Input:              *theWire：Wire Port
Output:             None
Return:             None
Others:             
*************************************************/
BMP75M131::BMP75M131(TwoWire *theWire)
{
   _wire = theWire;
}

/*************************************************
Description:        initialize
Input:              None  
Output:             None
Return:             None
Others:             
*************************************************/
void BMP75M131::begin(uint8_t address)
{
  _i2cAddr = address;
  _wire->begin();
  dataTxBbuf[0] = MODULE_TPYE;  
}

/*************************************************
Description:        Get the number of modules
Input:              moduleNumberVal: Variables for storing number of cascaded modules 
Output:             moduleNumberVal: Number of cascaded modules
Return:             Communication status: COMMUNICATION_OK(0x00) or COMMUNICATION_ERROR(0xFF)
Others:             
*************************************************/
uint8_t BMP75M131::getNumber()
{
  uint8_t dataIndex;
  uint8_t moduleNumberVal=0;
  dataTxBbuf[1] = 0x00;
  dataTxBbuf[2] = 0x02;
  dataTxBbuf[3] = CMD_GET_MODULE_NUMBER;
  dataTxBbuf[4] = 0;
  for (dataIndex = 0; dataIndex < 4; dataIndex++)
  {
    dataTxBbuf[4] += dataTxBbuf[dataIndex];
  }

  writeBytes(dataTxBbuf,5);
  delay(40); //Response delay time
  if (readBytes(dataRxBbuf,6) == CHECK_OK)
  {
    moduleNumberVal = dataRxBbuf[4];
  }
  _moduleNumber = dataRxBbuf[4];
    
  return moduleNumberVal;
}

/*************************************************
Description:        Get a relay switch status
Input:              id：relay module id; 
                    relayStatus: Variables for storing the relay switch status 
Output:             relayStatus: Relay switch status
Return:             Communication status: COMMUNICATION_OK(0x00) or COMMUNICATION_ERROR(0xFF)
Others:               
*************************************************/
uint8_t BMP75M131::getRelayStatus(uint8_t id)
{
  uint8_t dataIndex;
  uint8_t relayStatus=0;
  dataTxBbuf[1] = id;
  dataTxBbuf[2] = 0x02;
  dataTxBbuf[3] = CMD_GET_RELAY_STATUS;
  dataTxBbuf[4] = 0;
  for (dataIndex = 0; dataIndex < 4; dataIndex++)
  {
    dataTxBbuf[4] += dataTxBbuf[dataIndex];
  }
  
  writeBytes(dataTxBbuf,5);
  delay(40); //Response delay time
  if (readBytes(dataRxBbuf,6) == CHECK_OK)
  {
    relayStatus = dataRxBbuf[4];
  }
  return relayStatus;
}

/*************************************************
Description:        Get all relay status
Input:              allRelayStatus: The array that stores all relay switch states                    
Output:             allRelayStatus: All relay switch states
Return:             Communication status: COMMUNICATION_OK(0x00) or COMMUNICATION_ERROR(0xFF)
Others:             
*************************************************/
void BMP75M131::getAllRelayStatus(uint8_t allRelayStatus[])
{
  uint8_t dataIndex;
  uint8_t retVal = 1;
  uint8_t datalen = 6 + getNumber();
  dataTxBbuf[1] = 0x00;
  dataTxBbuf[2] = 0x02;
  dataTxBbuf[3] = CMD_GET_ALL_RELAY_STATUS;
  dataTxBbuf[4] = 0;
  for (dataIndex = 0; dataIndex < 4; dataIndex++)
  {
    dataTxBbuf[4] += dataTxBbuf[dataIndex];
  }
  writeBytes(dataTxBbuf,5);
  delay(40); //Response delay time
  if (readBytes(dataRxBbuf,datalen) == CHECK_OK)
  {
    retVal = dataRxBbuf[1];
  }
  if(retVal == 0)
  {
    for (dataIndex = 1; dataIndex <= dataRxBbuf[4]; dataIndex++)
    {
      allRelayStatus[dataIndex] = dataRxBbuf[4 + dataIndex];
    }
  }

     for (uint8_t i = 0; i <= 20; i++)
    {
     Serial.println(dataRxBbuf[i]); 
    }
}

/*************************************************
Description:        Control a relay 
Input:              id：relay module id
                    controlVal：Relay switch control value
                               0 - Open
                               1 - Close
Output:             None
Return:             Communication status: COMMUNICATION_OK(0x00) or COMMUNICATION_ERROR(0xFF)
Others:             
*************************************************/
void BMP75M131::setRelaySwitch(uint8_t id, uint8_t controlVal)
{
  uint8_t dataIndex;
  moisVal = 0;
  dataTxBbuf[1] = id;
  dataTxBbuf[2] = 0x03;
  dataTxBbuf[3] = CMD_RELAY_CONTROL;
  dataTxBbuf[4] = controlVal;
  dataTxBbuf[5] = 0;
  for (dataIndex = 0; dataIndex < 5; dataIndex++)
  {
    dataTxBbuf[5] += dataTxBbuf[dataIndex];
  }

   writeBytes(dataTxBbuf, 6);
   delay(40); //Response delay time
  if (readBytes(dataRxBbuf,5) == CHECK_OK)
  {
     moisVal = dataRxBbuf[3];
  }
  
}

/*************************************************
Description:        Control all relay 
Input:              controlVal：Relay switch control value
                               0 - Open
                               1 - Close
Output:             None
Return:             Communication status: COMMUNICATION_OK(0x00) or COMMUNICATION_ERROR(0xFF) 
Others:             
*************************************************/
void BMP75M131::setAllRelay(uint8_t controlVal)
{
  uint8_t dataIndex;
  
  dataTxBbuf[1] = 0x00;
  dataTxBbuf[2] = 0x03;
  dataTxBbuf[3] = CMD_ALL_RELAY_CONTROL;
  dataTxBbuf[4] = controlVal;
  dataTxBbuf[5] = 0;
  for (dataIndex = 0; dataIndex < 5; dataIndex++)
  {
    dataTxBbuf[5] += dataTxBbuf[dataIndex];
  }
  writeBytes(dataTxBbuf, 6);
  delay(40); //Response delay time
  if (readBytes(dataRxBbuf,5) == CHECK_OK)
  {
    moisVal = dataRxBbuf[2];
  }
}


void BMP75M131::writeBytes(uint8_t wbuf[], uint8_t wlen)
{
  if (_wire != NULL)
  {
    while (_wire->available() > 0)
    {
      _wire->read();
    }
    _wire->beginTransmission(_i2cAddr);
    _wire->write(wbuf, wlen);
    _wire->endTransmission();
  }
}

uint8_t BMP75M131::readBytes(uint8_t rbuf[], uint8_t rlen)
{
  uint8_t i = 0, checkSum = 0;
  _wire->requestFrom(_i2cAddr, rlen);
  if (_wire->available() == rlen)
  {
    for (i = 0; i < rlen; i++)
    {
      rbuf[i] = _wire->read();
    }
  }
  else
  {
    return TIMEOUT_ERROR;
  }

  /* Check Sum */
  for (i = 0; i < (rlen-1); i++)
  {
    checkSum += rbuf[i];
  }
  if (checkSum == rbuf[rlen - 1])
  {
    return CHECK_OK; // Check correct
  }
  else
  {
    return CHECK_ERROR; // Check error
  }
}
