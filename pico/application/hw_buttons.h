#pragma once

#include "mcp23017.h"

void buttons_init(void);

void init_mcp(const Mcp23017 &mcp);

uint32_t keyscan(Mcp23017 &mcp, int columns, int rows, int interval_ms);
