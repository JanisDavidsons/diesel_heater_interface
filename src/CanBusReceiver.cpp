#include "CanBusReceiver.h"

CanBusReceiver::CanBusReceiver(MCP2515 &mcp2515)
    : mcp2515(mcp2515),
      trendExhaust(TREND_EXHAUST_NOT_CHANGING),
      voltageChanged(true)
{
}

template <typename T>
void CanBusReceiver::copyCanMsgToStruct(const struct can_frame &canMsg, T &frame)
{
  frame.can_id = canMsg.can_id;
  frame.can_dlc = canMsg.can_dlc;

  for (int i = 0; i < frame.can_dlc; ++i)
  {
    frame.data[i] = canMsg.data[i];
  }
}

void CanBusReceiver::processFrameVoltageSensor(frameVoltageSensor &voltageData)
{
  uint16_t reconstructedValue =
      (static_cast<uint8_t>(voltageData.data[0])) |
      (static_cast<uint8_t>(voltageData.data[1]) << 8);

  double newVoltage = static_cast<double>(reconstructedValue) / 10;

  if (newVoltage != voltageData.voltage)
  {
    voltageData.voltage = newVoltage;
    voltageData.changed = true;
    voltageData.isOutdated = false;
    voltageData.lastUpdated = millis();
  }
}

void CanBusReceiver::processFrameFlameSensor(frameFlameSensor &flameData)
{
  flameData.value = flameData.data[0];
  flameData.isIncreasing = static_cast<bool>(flameData.data[1]);
  flameData.isDecreasing = static_cast<bool>(flameData.data[2]);

  setTrend(flameData);
  flameData.lastUpdated = seconds;
  flameData.isOutdated = false;
}

void CanBusReceiver::processFrameHeaterState(frameHeaterState &stateData)
{
  stateData.prevState = stateData.state;
  stateData.prevMode = stateData.mode;

  stateData.state = stateData.data[0];
  stateData.mode = stateData.data[1];
  stateData.lastUpdated = seconds;
}

void CanBusReceiver::processFrameHeaterTemperature(frameTemperature &tempData)
{
  tempData.prevCoolant = tempData.coolant;
  tempData.prevSurface = tempData.surface;

  int16_t reconstructedCoolantTmp =
      (tempData.data[0]) |
      (tempData.data[1] << 8);

  int16_t reconstructedSurfaceTmp =
      (tempData.data[2]) |
      (tempData.data[3] << 8);

  tempData.coolant = static_cast<double>(reconstructedCoolantTmp) / 10;
  tempData.surface = static_cast<double>(reconstructedSurfaceTmp) / 10;
  tempData.lastUpdated = seconds;
  tempData.isOutdated = false;
}


void CanBusReceiver::processFrameInjectionPump(frameInjectionPump &injectionPumpData)
{

  injectionPumpData.prevFrequency = injectionPumpData.frequency;

  int16_t reconstructedData =
      (injectionPumpData.data[0]) |
      (injectionPumpData.data[1] << 8);

  injectionPumpData.frequency = static_cast<double>(reconstructedData) / 10;
  injectionPumpData.lastUpdated = seconds;
  injectionPumpData.isOutdated = false;
}

void CanBusReceiver::checkMessage()
{
  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK)
  {
    if (canMsg.can_id == 0x100)
    {
      if (canMsg.can_dlc == sizeof(exhaustTemperature.data))
      {
        copyCanMsgToStruct(canMsg, exhaustTemperature);
        processFrameFlameSensor(exhaustTemperature);
      }
      else
      {
        Serial.println("Error: Size mismatch in received 0x100 message");
      }
    }
    else if (canMsg.can_id == 0x101)
    {
      if (canMsg.can_dlc == sizeof(voltageSensor.data))
      {
        copyCanMsgToStruct(canMsg, voltageSensor);
        processFrameVoltageSensor(voltageSensor);
        Serial.print("Setting Voltage ");
        Serial.println(seconds);
      }
      else
      {
        Serial.println("Error: Size mismatch in received 0x101 message");
      }
    }
    else if (canMsg.can_id == 0x102)
    {
      if (canMsg.can_dlc == sizeof(heaterState.data))
      {
        copyCanMsgToStruct(canMsg, heaterState);
        processFrameHeaterState(heaterState);
      }
      else
      {
        Serial.println("Error: Size mismatch in received 0x102 message");
      }
    }
    else if (canMsg.can_id == 0x103)
    {
      if (canMsg.can_dlc == sizeof(heaterTemperature.data))
      {
        copyCanMsgToStruct(canMsg, heaterTemperature);
        processFrameHeaterTemperature(heaterTemperature);
      }
      else
      {
        Serial.println("Error: Size mismatch in received 0x103 message");
      }
    }
    else if (canMsg.can_id == 0x104)
    {
      if (canMsg.can_dlc == sizeof(injectionPump.data))
      {
        copyCanMsgToStruct(canMsg, injectionPump);
        processFrameInjectionPump(injectionPump);
      }
      else
      {
        Serial.println("Error: Size mismatch in received 0x104 message");
      }
    }
  }
}

double CanBusReceiver::getExhaustTemp() const
{
  return exhaustTemperature.value;
}

const struct can_frame &CanBusReceiver::getCanMsg() const
{
  return canMsg;
}

bool CanBusReceiver::isExhaustTempIncreasing()
{
  return exhaustTemperature.isIncreasing;
}

bool CanBusReceiver::isExhaustTempDecreasing()
{
  return exhaustTemperature.isDecreasing;
}

void CanBusReceiver::setTrend(frameFlameSensor &sensor)
{
  if (sensor.isIncreasing)
  {
    trendExhaust = TREND_EXHAUST_RISING;
  }
  else if (sensor.isDecreasing)
  {
    trendExhaust = TREND_EXHAUST_DECREASING;
  }
  else
  {
    trendExhaust = TREND_EXHAUST_NOT_CHANGING;
  }
}

CanBusReceiver::Trend CanBusReceiver::getTExhaustTrend()
{
  return trendExhaust;
}

uint8_t CanBusReceiver::getHeaterStateIndex()
{
  return heaterState.state;
}

uint8_t CanBusReceiver::getHeaterModeIndex()
{
  return heaterState.mode;
}

double CanBusReceiver::getCoolantTmp()
{
  return heaterTemperature.coolant;
}

double CanBusReceiver::getSurfaceTmp()
{
  return heaterTemperature.surface;
}

double CanBusReceiver::getPrevCoolantTmp()
{
  return heaterTemperature.prevCoolant;
}

double CanBusReceiver::getPrevSurfaceTmp()
{
  return heaterTemperature.prevSurface;
}

void CanBusReceiver::tick()
{
  seconds++;

  if (seconds - heaterTemperature.lastUpdated > 5)
  {
    heaterTemperature.isOutdated = true;
    heaterTemperature.coolant = -10.0;
    heaterTemperature.surface = -10.0;
  }

  if (seconds - exhaustTemperature.lastUpdated > 5)
  {
    exhaustTemperature.isOutdated = true;
    exhaustTemperature.value = 0.0;
  }

  if (seconds - heaterState.lastUpdated > 5)
  {
    heaterState.state = -1;
    heaterState.mode = -1;
  }

  if (seconds - voltageSensor.lastUpdated > 10)
  {
    Serial.println("Setting voltage outdated");
    voltageSensor.voltage = 9.0;
    voltageSensor.isOutdated = true;
  }

  if (seconds - injectionPump.lastUpdated >5)
  {
    injectionPump.frequency = -1.0;
    injectionPump.isOutdated = true;
  }
  
}

CanBusReceiver::frameTemperature CanBusReceiver::getHeaterTemp()
{
  return heaterTemperature;
}

CanBusReceiver::frameFlameSensor CanBusReceiver::getExhaustTemp()
{
  return exhaustTemperature;
}

CanBusReceiver::frameHeaterState CanBusReceiver::getHeateState()
{
  return heaterState;
}

CanBusReceiver::frameVoltageSensor CanBusReceiver::getVoltage()
{
  return voltageSensor;
}

CanBusReceiver::frameInjectionPump CanBusReceiver::getInjectionPump()
{
  return injectionPump;
}