#include "CanBusSender.h"

CanBusSender::CanBusSender(MCP2515 &mcp2515)
  : mcp2515(mcp2515)
{
}

void CanBusSender::initialize()
{
}

void CanBusSender::sendHeaterCommand(uint8_t command)
{
  toggleHeater.can_id = 0x200;
  toggleHeater.can_dlc = 1;
  toggleHeater.data[0] = command;
  mcp2515.sendMessage(&toggleHeater);
}