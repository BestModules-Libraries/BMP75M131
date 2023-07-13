/*****************************************************************
File:             BMP75M131.h
Author:           BESTMODULES
Description:      Define classes and required variables
History：         
V1.0.1-- initial version；2022-04-03；Arduino IDE : ≥v1.8.13
******************************************************************/
#ifndef _BMP75M131_
#define _BMP75M131_

#include <Arduino.h>
#include <Wire.h>

#define MODULE_TPYE               (0x15)
#define BMP75M131_ADDR               (0x15)

#define CMD_RELAY_CONTROL         (0x01)        
#define CMD_GET_RELAY_STATUS      (0x02)  
#define CMD_GET_MODULE_NUMBER     (0x11)
#define CMD_ALL_RELAY_CONTROL     (0x14)
#define CMD_GET_ALL_RELAY_STATUS  (0x15)
#define CMD_IS_MODULE_READY       (0x16)

#define CONTROL_A_MODULE_TIMEOUT  (2)          //Read the communication timeout of a module（ms）
#define READ_ALL_MODULE_TIMEOUT   (6)          //Read the communication timeout of all modules（ms）

#define CHECK_OK        0
#define CHECK_ERROR     1
#define TIMEOUT_ERROR   2

class BMP75M131
{
  public:
    BMP75M131(TwoWire *theWire = &Wire); 
    void begin(uint8_t i2c_addr = BMP75M131_ADDR);                                                 //Initialize
    //void begin(unsigned long frequency);                          //Initialize and set the communication rate                                     //
    uint8_t getNumber();            //
    uint8_t getRelayStatus(uint8_t id);     //
    void getAllRelayStatus(uint8_t allRelayStatus[]); 
    void setRelaySwitch(uint8_t id, uint8_t controlVal);   //
    void setAllRelay(uint8_t controlVal);                  //
             //
    
  private:
    TwoWire *_wire = NULL;
    uint8_t _i2cAddr;
    uint8_t _moduleNumber = 0;
    uint8_t moisVal = 0;
    uint8_t dataTxBbuf[16] = {0};                                 //Array of data frames send
    uint8_t dataRxBbuf[32] = {0};                                 //Array of data frames receive 
    void writeBytes(uint8_t wbuf[], uint8_t wlen);
    uint8_t readBytes(uint8_t rbuf[], uint8_t rlen);

};

#endif
