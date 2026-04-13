// [me_I2C] test

/*
  Author: Martin Eden
  Last mod.: 2026-04-13
*/

/*
  For test we're using module RTC RTC3231

  We will read 16 bytes from it and print them.
  Before reading we must set read start address which is zero.
*/

#include <me_I2C.h>

#include <me_BaseTypes.h>
#include <me_StreamsCollection.h>
#include <me_StreamTools.h>

#include <me_Console.h>

void RunTest()
{
  me_I2C::TI2C_Master Twi;
  me_StreamsCollection::TWorkmemInputStream RequestStream;
  me_StreamsCollection::TWorkmemOutputStream RespWriteStream;
  me_StreamsCollection::TWorkmemInputStream RespReadStream;
  TUint_1 Byte;

  const TUint_2 RequestSize = 1;
  TUint_1 Request[RequestSize] = { 0 };
  TAddressSegment RequestSeg = M_AsAddrSeg(Request);

  const TUint_2 ResponseSize = 16;
  TUint_1 Buff[ResponseSize];
  TAddressSegment ResponseSeg = M_AsAddrSeg(Buff);

  const TUint_1 DeviceId = 104;

  RequestStream.Init(RequestSeg);
  RespWriteStream.Init(ResponseSeg);

  Twi.Init();
  Twi.Send(DeviceId, &RequestStream);
  Twi.Receive(DeviceId, ResponseSize, &RespWriteStream);
  Twi.Done();

  RespReadStream.Init(RespWriteStream.GetProcessedSegment());
  while (RespReadStream.Read(&Byte))
    Console.Print(Byte);
  Console.EndLine();
}

void setup()
{
  Console.Init();

  Console.Print("[me_I2C] test");

  RunTest();

  Console.Print("Done");
}

void loop()
{
}

/*
  2026-04-07
  2026-04-10
*/
