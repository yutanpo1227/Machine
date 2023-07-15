#ifndef Line_h
#define Line_h

#include <Arduino.h>
#include <MCP_ADC.h>

class Line {
    public:
        Line();
        void setup();
        int read(int ch);
        void onLine();
        bool is_onLine[8];
    private:
        MCP3008 _mcp;
};

#endif