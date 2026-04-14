// I2C protocol

/*
  Author: Martin Eden
  Last mod.: 2026-04-14
*/

#include <me_I2C.h>

#include <me_BaseTypes.h>

using namespace me_I2C;

/*
  Init

  Set-up pullup resistors for SCL and SDA pins, enable hardware I2C
*/
void TI2C_Master::Init()
{
  Bare.EnableModule();
}

/*
  Done

  Disable hardware I2C. Frees pins from I2C obsession.
*/
void TI2C_Master::Done()
{
  Bare.DisableModule();
}

/*
  Send data
*/
TBool TI2C_Master::Send(
  TDeviceAddress DeviceAddress,
  TUint_2 PacketSize,
  IInputStream * InStream
)
{
  /*
    We're sending <PacketSize> bytes from input stream

    If input stream fails or device didn't acknowledge
    we will emit Stop condition and return false.
  */

  TUnit Byte;
  TBool Result;

  Result = false;

  Bare.Start();

  if (!Bare.Send_Address_Write(DeviceAddress)) goto Finish;

  while (PacketSize != 0)
  {
    if (!InStream->Read(&Byte)) goto Finish;
    if (!Bare.Send_Data(Byte)) goto Finish;
    --PacketSize;
  }

  Result = true;

  Finish:

  Bare.Stop();

  return Result;
}

/*
  Receive data
*/
TBool TI2C_Master::Receive(
  TDeviceAddress DeviceAddress,
  TUint_2 PacketSize,
  IOutputStream * OutStream
)
{
  /*
    Similar as for Send() we will emit stop condition and return false
    if output stream fails.

    After some thought we decided not to check that output stream
    is failed. We will read requested number of bytes from bus.
    Makes code simpler and useful for speed testing.
  */

  TUint_1 Byte;
  TBool Result;
  TBool WantMore;

  // Receiving zero bytes from device is not supported by hardware
  if (PacketSize == 0) return false;

  Result = false;

  Bare.Start();

  if (!Bare.Send_Address_Read(DeviceAddress)) goto Finish;

  while (PacketSize != 0)
  {
    WantMore = (PacketSize != 1);
    Byte = Bare.Get_Byte(WantMore);
    OutStream->Write(Byte);
    --PacketSize;
  };

  Result = true;

  Finish:

  Bare.Stop();

  return Result;
}

/*
  2026-04-07
  2026-04-10
  2026-04-11
  2026-04-14
*/
