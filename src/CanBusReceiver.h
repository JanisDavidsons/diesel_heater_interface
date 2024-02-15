#ifndef CAN_BUS_RECEIVER
#define CAN_BUS_RECEIVER

#include <Arduino.h>
#include <mcp2515.h>

class CanBusReceiver
{
public:
  enum Trend
  {
    TREND_EXHAUST_NOT_CHANGING,
    TREND_EXHAUST_RISING,
    TREND_EXHAUST_DECREASING
  };

private:
  MCP2515 &mcp2515;
  uint8_t seconds = 0;

  struct can_frame canMsg;

  Trend trendExhaust;

  struct frameFlameSensor
  {
    uint32_t can_id;
    uint8_t can_dlc;
    uint8_t data[3];
    uint8_t value = 0;
    bool isIncreasing = false;
    bool isDecreasing = false;
    uint8_t lastUpdated = 0;
    bool isOutdated = true;
  } exhaustTemperature;

  struct frameVoltageSensor
  {
    uint32_t can_id;
    uint8_t can_dlc;
    uint8_t data[2];
    double voltage = 9.0;
    bool changed = true;
    uint8_t lastUpdated = 0;
    bool isOutdated = true;
  } voltageSensor;

  struct frameHeaterState
  {
    uint32_t can_id;
    uint8_t can_dlc;
    uint8_t data[2];
    int8_t state = -1;
    int8_t mode = -1;
    uint8_t lastUpdated = 0;
  } heaterState;

  struct frameTemperature
  {
    uint32_t can_id;
    uint8_t can_dlc;
    uint8_t data[4];
    double coolant = -20.0;
    double surface = -10.0;
    uint8_t lastUpdated = 0;
    bool isOutdated = true;
  } heaterTemperature;

  struct frameInjectionPump
  {
    uint32_t can_id;
    uint8_t can_dlc;
    uint8_t data[2];
    double frequency = -1.0;
    uint8_t lastUpdated = 0;
    bool isOutdated = true;
  } injectionPump;

  struct frameCombustionFan
  {
    uint32_t can_id;
    uint8_t can_dlc;
    uint8_t data[1];
    uint8_t speed = 255;
    uint8_t lastUpdated = 0;
    bool isOutdated = true;
  } combustionFan;

  struct frameGlowPlug
  {
    uint32_t can_id;
    uint8_t can_dlc;
    uint8_t data[1];
    uint8_t isOn = 255;
    uint8_t lastUpdated = 0;
    bool isOutdated = true;
  } gowPlug;

  void setTrend(frameFlameSensor &sensor);

public:
  bool voltageChanged;
  bool temperatureChanged;

  CanBusReceiver(MCP2515 &mcp2515);
  const struct can_frame &getCanMsg() const;
  double getExhaustTemp() const;
  uint8_t getHeaterStateIndex();
  uint8_t getHeaterModeIndex();
  void checkMessage();
  bool isExhaustTempIncreasing();
  bool isExhaustTempDecreasing();
  Trend getTExhaustTrend();
  double getCoolantTmp();
  double getSurfaceTmp();

  frameTemperature getHeaterTemp();
  frameFlameSensor getExhaustTemp();
  frameHeaterState getHeateState();
  frameVoltageSensor getVoltage();
  frameInjectionPump getInjectionPump();
  frameCombustionFan getCombustionFan();
  frameGlowPlug getGlowPlug();

  template <typename T>
  void copyCanMsgToStruct(const struct can_frame &canMsg, T &frame);

  void processFrameVoltageSensor(frameVoltageSensor &voltageData);
  void processFrameFlameSensor(frameFlameSensor &flameData);
  void processFrameHeaterState(frameHeaterState &stateData);
  void processFrameHeaterTemperature(frameTemperature &temperatureData);
  void processFrameInjectionPump(frameInjectionPump &injectionPumpData);
  void processFrameCombustionFan(frameCombustionFan &combustionFanData);
  void processFrameGlowPlug(frameGlowPlug &glowPlugData);
  void tick();
};

#endif
