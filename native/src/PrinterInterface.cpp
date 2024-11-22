#ifndef PRINTER_INTERFACE_H
#define PRINTER_INTERFACE_H

#include <string>
#include <vector>

class PrinterInterface {
public:
    virtual ~PrinterInterface() = default;
    virtual std::vector<std::string> ListPrinters() = 0;
};

#endif
