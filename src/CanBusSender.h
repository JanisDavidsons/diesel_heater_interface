#ifndef CAN_BUS_SENDER
#define CAN_BUS_SENDER

#include "mcp2515.h"

class CanBusSender
{
public:
  CanBusSender(MCP2515 &mcp2515);
  void initialize();
  void sendHeaterCommand(uint8_t command);

private:
  MCP2515 &mcp2515;
  struct can_frame toggleHeater;
};

#endif