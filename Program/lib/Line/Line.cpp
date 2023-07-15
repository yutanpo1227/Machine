#include "Line.h"
#include <Arduino.h>
#include <MCP_ADC.h>

Line::Line()
{
    MCP3008 _mcp;
    bool is_onLine[8];
}

void Line::setup()
{
    _mcp.begin(10);
}

int Line::read(int ch)
{
    return _mcp.analogRead(ch);
}

void Line::onLine()
{
    for(int i = 0; i < 8; i++)
    {
        is_onLine[i] = false;
        if(read(i) > 200)
        {
            is_onLine[i] = true;
        }
    }
}