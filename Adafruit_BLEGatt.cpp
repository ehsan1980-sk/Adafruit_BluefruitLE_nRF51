/**************************************************************************/
/*!
    @file     Adafruit_BLEGatt.cpp
    @author   hathach

    @section LICENSE

    Software License Agreement (BSD License)

    Copyright (c) 2016, Adafruit Industries (adafruit.com)
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holders nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/**************************************************************************/

#include "Adafruit_BLEGatt.h"


/******************************************************************************/
/*!
    @brief Constructor
*/
/******************************************************************************/
Adafruit_BLEGatt::Adafruit_BLEGatt(Adafruit_BLE& ble) :
  _ble(ble)
{

}

/******************************************************************************/
/*!
    @brief Clear all GATT data
*/
/******************************************************************************/
bool Adafruit_BLEGatt::clear(void)
{
  return _ble.atcommand( F("AT+GATTCLEAR") );
}

/******************************************************************************/
/*!
    @brief Add a service with 16-bit UUID
    @return Service ID (starting from 1). If failed 0 is returned
*/
/******************************************************************************/
uint8_t Adafruit_BLEGatt::addService(uint16_t uuid16)
{
  int32_t service_id;
  VERIFY_RETURN_( _ble.atcommandIntReply( F("AT+GATTADDSERVICE=UUID"), &service_id, uuid16), 0 );
  return (uint8_t) service_id;
}

/******************************************************************************/
/*!
    @brief Add a service with 128-bit UUID
    @return Service ID (starting from 1). If failed 0 is returned
*/
/******************************************************************************/
uint8_t Adafruit_BLEGatt::addService(uint8_t uuid128[])
{
  int32_t service_id;
  VERIFY_RETURN_( _ble.atcommandIntReply( F("AT+GATTADDSERVICE=UUID128"), &service_id, uuid128, 16), 0 );
  return (uint8_t) service_id;
}

/******************************************************************************/
/*!
    @brief Add a characteristics with UUID16 to a newly added service
    @param
    @return Chars ID (starting from 1). If failed 0 is returned
*/
/******************************************************************************/
uint8_t Adafruit_BLEGatt::addCharacteristic(uint16_t uuid16, uint8_t properties, uint8_t min_len, uint8_t max_len, GattCharsDataType_t datatype)
{
  bool isOK;
  int32_t chars_id;
  uint8_t current_mode = _ble.getMode();

  // switch mode if necessary to execute command
  if ( current_mode == BLUEFRUIT_MODE_DATA ) _ble.setMode(BLUEFRUIT_MODE_COMMAND);

  _ble.print( F("AT+GATTADDCHAR=UUID=") );
  _ble.print(uuid16);

  _ble.print( F(",PROPERTIES=") );
  _ble.print(properties);

  _ble.print( F(",MIN_LEN=") );
  _ble.print(min_len);

  _ble.print( F(",MAX_LEN=") );
  _ble.print(max_len);

  _ble.print( F(",DATATYPE=") );
  _ble.print((uint8_t) datatype);

  _ble.println(); // execute command

//  if (_verbose) SerialDebug.print( F("\n<- ") );
  chars_id = _ble.readline_parseInt();

  isOK = _ble.waitForOK();

  // switch back if necessary
  if ( current_mode == BLUEFRUIT_MODE_DATA ) _ble.setMode(BLUEFRUIT_MODE_DATA);

  return isOK ? chars_id : 0;
}

/******************************************************************************/
/*!
    @brief Add a characteristics with UUID128 to a newly added service
    @param
    @return Chars ID (starting from 1). If failed 0 is returned
*/
/******************************************************************************/
uint8_t Adafruit_BLEGatt::addCharacteristic(uint8_t uuid128[], uint8_t properties, uint8_t min_len, uint8_t max_len, GattCharsDataType_t datatype)
{
  bool isOK;
  int32_t chars_id;
  uint8_t current_mode = _ble.getMode();

  // switch mode if necessary to execute command
  if ( current_mode == BLUEFRUIT_MODE_DATA ) _ble.setMode(BLUEFRUIT_MODE_COMMAND);

  _ble.print( F("AT+GATTADDCHAR=UUID128=") );
  _ble.printByteArray(uuid128, 16);

  _ble.print( F(",PROPERTIES=") );
  _ble.print(properties);

  _ble.print( F(",MIN_LEN=") );
  _ble.print(min_len);

  _ble.print( F(",MAX_LEN=") );
  _ble.print(max_len);

  _ble.print( F(",DATATYPE=") );
  _ble.print((uint8_t) datatype);

  _ble.println(); // execute command

//  if (_verbose) SerialDebug.print( F("\n<- ") );
  chars_id = _ble.readline_parseInt();

  isOK = _ble.waitForOK();

  // switch back if necessary
  if ( current_mode == BLUEFRUIT_MODE_DATA ) _ble.setMode(BLUEFRUIT_MODE_DATA);

  return isOK ? chars_id : 0;
}

/******************************************************************************/
/*!
    @brief Set Characteristics value with data buffer
    @param
*/
/******************************************************************************/
bool Adafruit_BLEGatt::setChar(uint8_t charID, uint8_t const data[], uint8_t size)
{
  uint16_t argtype[] = { AT_ARGTYPE_UINT8, AT_ARGTYPE_BYTEARRAY+ ((uint16_t)size) };
  const void* args[] = { (void*) ((uint32_t) charID), data };

  return _ble.atcommand_full(F("AT+GATTCHAR"), NULL, 2, argtype, args);
}

/******************************************************************************/
/*!
    @brief Set Characteristics value with data buffer
    @param
*/
/******************************************************************************/
bool Adafruit_BLEGatt::setChar(uint8_t charID, char const* str)
{
  uint16_t argtype[] = { AT_ARGTYPE_UINT8, AT_ARGTYPE_STRING };
  const void* args[] = { (void*) ((uint32_t) charID), str };

  return _ble.atcommand_full(F("AT+GATTCHAR"), NULL, 2, argtype, args);
}

/******************************************************************************/
/*!
    @brief Set Characteristics value with data buffer
    @param charID Characteristics ID
    @param buf buffer to hold data
    @param bufsize size of buffer
    @return number of bytes copied to buffer (up to bufsize).
            0 usually means error.
*/
/******************************************************************************/
uint8_t Adafruit_BLEGatt::getChar(uint8_t charID, uint8_t* buf, uint8_t bufsize)
{
  uint8_t current_mode = _ble.getMode();

  // switch mode if necessary to execute command
  if ( current_mode == BLUEFRUIT_MODE_DATA ) _ble.setMode(BLUEFRUIT_MODE_COMMAND);

  // use RAW command version
  _ble.print( F("AT+GATTCHARRAW=") );
  _ble.println(charID);

  uint16_t len = _ble.readraw(); // readraw swallow OK/ERROR already
  len = min(len, bufsize);

  memcpy(buf, _ble.buffer, len);

  // switch back if necessary
  if ( current_mode == BLUEFRUIT_MODE_DATA ) _ble.setMode(BLUEFRUIT_MODE_DATA);

  return len;
}
