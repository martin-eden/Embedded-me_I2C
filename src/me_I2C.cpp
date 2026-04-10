// I2C protocol

/*
  Author: Martin Eden
  Last mod.: 2026-04-10
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
  TUint_2 NumBytesSent;

  Result = false;

  Bare.Start();

  if (!Bare.Send_Address_Write(DeviceAddress)) goto Finish;

  NumBytesSent = 0;
  while (NumBytesSent < PacketSize)
  {
    if (!InStream->Read(&Byte)) goto Finish;

    if (!Bare.Send_Data(Byte)) goto Finish;

    ++NumBytesSent;
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
  */

  TUnit Byte;
  TBool Result;
  TUint_2 ByteNumber;
  TBool WantMore;

  // No support in receiving zero bytes from device
  if (PacketSize == 0) return false;

  Result = false;

  Bare.Start();

  if (!Bare.Send_Address_Read(DeviceAddress)) goto Finish;

  ByteNumber = 0;
  do {
    ++ByteNumber;

    WantMore = (ByteNumber < PacketSize);

    Byte = Bare.Get_Byte(WantMore);

    if (!OutStream->Write(Byte)) goto Finish;
  }
  while (WantMore);

  Result = true;

  Finish:

  Bare.Stop();

  return Result;
}

/*
  2026-04-07
  2026-04-10
*/
