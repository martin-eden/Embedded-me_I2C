// I2C hardware support for ATmega328

/*
  Author: Martin Eden
  Last mod.: 2026-04-10
*/

/*
  For Master Mode

  For Slave mode there will be more Statuses and device address
  in byte 186.
*/

#pragma once

#include <me_I2C.h>

#include <me_BaseTypes.h>

namespace me_I2C
{
  namespace Bare_ATmega328
  {
    TUint_1 * BitRate = (TUint_1 *) 184;

    struct TPrescale
    {
      TUint_1 Prescaler : 2;
      TUint_1 : 6;
    };

    TPrescale * Prescale = (TPrescale *) 185;

    enum struct Prescales
    {
      FullSpeed = 0,
      Prescale_2Pow2 = 1,
      Prescale_2Pow4 = 2,
      Prescale_2Pow6 = 3,
    };

    struct TStatus
    {
      TUint_1 : 3;
      TUint_1 Status : 5;
    };

    volatile TStatus * Status = (TStatus *) 185;

    enum struct Statuses
    {
      ProtocolViolation = 0,
      Sent_Start = 1,
      Sent_RepeatedStart = 2,
      Sent_AddrWrite_Got_Ack = 3,
      Sent_AddrWrite_Got_Nack = 4,
      Sent_Data_Got_Ack = 5,
      Sent_Data_Got_Nack = 6,
      ArbitrationLost = 7,
      Sent_AddrRead_Got_Ack = 8,
      Sent_AddrRead_Got_Nack = 9,
      Got_Data_Sent_Ack = 10,
      Got_Data_Sent_Nack = 11,
      Idle = 31,
    };

    volatile TUint_1 * Data = (TUint_1 *) 187;

    struct TControl
    {
      TBool EnableInterrupt : 1;
      TUint_1 : 1;
      TBool EnableModule : 1;
      TBool IsWriteCollision : 1;
      TBool Emit_Stop : 1;
      TBool Emit_Start : 1;
      TBool Confirm : 1;
      TBool Ready : 1;
    };

    volatile TControl * Control = (TControl *) 188;
  };
}

/*
  2026-04-07
  2026-04-10
*/
