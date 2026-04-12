// Hardware I2C implementation for Master mode for ATmega326

/*
  Author: Martin Eden
  Last mod.: 2026-04-12
*/

#include <me_I2C.h>

#include "I2C.Bare.ATmega328.h"

#include <me_BaseTypes.h>
#include <me_Bits.h>
#include <me_Delays.h>

using namespace me_I2C;

const me_Bits::TBitValue
  Mode_Read = 1,
  Mode_Write = 0;

/*
  [Internal] Pack 7-bit value and one bit in byte
*/
static TUint_1 PackAddrMode(
  TDeviceAddress Addr,
  me_Bits::TBitValue Mode
)
{
  return (Addr << 1) | Mode;
}

/*
  [Internal] Clone hardware register for safe-to-work copy
*/
static Bare_ATmega328::TControl GetState()
{
  TAddress Addr;

  Addr = (TAddress) Bare_ATmega328::Control;

  return *(Bare_ATmega328::TControl *) Addr;
}

/*
  [Internal] Get I2C status code
*/
static TUint_1 GetStatus()
{
  return Bare_ATmega328::Status->Status;
}

/*
  [Internal] Wait till I2C hardware is ready
*/
static void WaitForReady()
{
  // 10- 100+ 50- 80+
  const TUint_4 Delay_Us = 120;

  if (Bare_ATmega328::Control->Ready) return;
  me_Delays::Delay_Us(Delay_Us);
}

/*
  [Internal] Set hardware state from memory copy
*/
static void SetState(
  Bare_ATmega328::TControl State
)
{
  /*
    Majestic language, C++!

    This shit compiles to one instruction: "sts 0x00BC, r24"
  */

  TAddress SrcAddr;
  TAddress DestAddr;

  SrcAddr = (TAddress) &State;
  DestAddr = (TAddress) Bare_ATmega328::Control;

  *(TUint_1 *) DestAddr = *(TUint_1 *) SrcAddr;
}

/*
  [Internal] Apply memory copy to hardware. Force Ready flag
*/
static void ApplyState(
  Bare_ATmega328::TControl State
)
{
  State.Ready = true;

  SetState(State);
}

/*
  [Internal] Set transmission speed to 100 kHz
*/
static void SetSpeed_100_kHz()
{
  Bare_ATmega328::Prescale->Prescaler =
    (TUint_1) Bare_ATmega328::Prescales::FullSpeed;
  *Bare_ATmega328::BitRate = 72;
}

/*
  Enable I2C hardware
*/
void TI2C_Master_Bare::EnableModule()
{
  SetSpeed_100_kHz();

  Bare_ATmega328::Control->EnableModule = true;
}

/*
  Disable I2C hardware
*/
void TI2C_Master_Bare::DisableModule()
{
  Bare_ATmega328::Control->EnableModule = false;
}

/*
  Emit "start" state
*/
void TI2C_Master_Bare::Start()
{
  /*
    Just set Start bit and hardware will do magic

    We should check status tho because we can still lose bus arbitration.
    But we're not doing it yet.
  */

  Bare_ATmega328::TControl State;

  State = GetState();

  State.Emit_Start = true;
  State.Emit_Stop = false;

  ApplyState(State);

  WaitForReady();
}

/*
  Emit "stop" state
*/
void TI2C_Master_Bare::Stop()
{
  /*
    Just set Stop bit

    But what will happen from calling this before Start?
    Status will be set to Protocol Violation.
  */

  Bare_ATmega328::TControl State;
  TUint_1 Status;

  Status = GetStatus();

  // Protocol does not allow sending Stop after we acknowledged
  if (Status == (TUint_1) Bare_ATmega328::Statuses::Got_Data_Sent_Ack)
    return;

  // Idle status typically means no SCL connection
  if (Status == (TUint_1) Bare_ATmega328::Statuses::Idle)
    return;

  // Protocol Violation usually means no SDA connection
  if (Status == (TUint_1) Bare_ATmega328::Statuses::ProtocolViolation)
    return;

  State = GetState();

  State.Emit_Start = false;
  State.Emit_Stop = true;

  ApplyState(State);

  /*
    Applying STOP condition does not re-raise Ready flag.
    So we will just watch for STOP flag to go off.
  */
  while (Bare_ATmega328::Control->Emit_Stop);
}

/*
  [Internal] Send byte

  I2C Slave always confirms byte receiving
*/
static void SendByte(
  TUint_1 Byte
)
{
  /*
    Setting Data register must be done before touching Control register.
    Or else you'll get Write Collision status.
  */

  Bare_ATmega328::TControl State;

  *Bare_ATmega328::Data = Byte;

  State = GetState();

  State.Emit_Start = false;
  State.Emit_Stop = false;

  ApplyState(State);

  WaitForReady();
}

/*
  Send device address, set read mode
*/
TBool TI2C_Master_Bare::Send_Address_Read(
  TDeviceAddress DeviceAddress
)
{
  TUint_1 Status;

  SendByte(PackAddrMode(DeviceAddress, Mode_Read));

  Status = GetStatus();

  return
    (Status == (TUint_1) Bare_ATmega328::Statuses::Sent_AddrRead_Got_Ack);
}

/*
  Send device address, set write mode
*/
TBool TI2C_Master_Bare::Send_Address_Write(
  TDeviceAddress DeviceAddress
)
{
  TUint_1 Status;

  SendByte(PackAddrMode(DeviceAddress, Mode_Write));

  Status = GetStatus();

  return
    (Status == (TUint_1) Bare_ATmega328::Statuses::Sent_AddrWrite_Got_Ack);
}

/*
  Send data byte
*/
TBool TI2C_Master_Bare::Send_Data(
  TUint_1 Byte
)
{
  TUint_1 Status;

  SendByte(Byte);

  Status = GetStatus();

  return
    (Status == (TUint_1) Bare_ATmega328::Statuses::Sent_Data_Got_Ack);
}


/*
  Get byte

  Hardware implementation in ATmega328 requires setting reply
  before actually seeing data.
*/
TUint_1 TI2C_Master_Bare::Get_Byte(
  TBool WantMore
)
{
  TUint_1 Byte;
  Bare_ATmega328::TControl State;

  State = GetState();

  State.Confirm = WantMore;

  ApplyState(State);

  WaitForReady();

  Byte = *Bare_ATmega328::Data;

  return Byte;
}

/*
  2026-04-07
  2026-04-08
  2026-04-10
  2026-04-11
  2024-04-12
*/
