// I2C protocol

/*
  Author: Martin Eden
  Last mod.: 2026-04-13
*/

#pragma once

#include <me_BaseTypes.h>
#include <me_BaseInterfaces.h>

namespace me_I2C
{
  // 7-bit device address
  typedef TUint_1 TDeviceAddress;

  /*
    I2C hardware implementation for Master

    Hardware in ATmega328 wants to know confirmation status
    before even showing byte received. Spoiling nice protocol.
  */
  class TI2C_Master_Bare
  {
    public:
      void EnableModule();
      void DisableModule();

      void Start();
      void Stop();

      TBool Send_Address_Read(TDeviceAddress);
      TBool Send_Address_Write(TDeviceAddress);
      TBool Send_Data(TUint_1);

      TUint_1 Get_Byte(TBool WantMore);
  };

  // I2C protocol implementation for Master
  class TI2C_Master
  {
    public:
      void Init();
      void Done();

      TBool Send(TDeviceAddress, IInputStream *);
      TBool Receive(TDeviceAddress, TUint_2, IOutputStream *);

    private:
      TI2C_Master_Bare Bare;
  };
}

/*
  2026-04-07
  2026-04-13
*/
