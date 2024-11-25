#ifndef PRINTER_INTERFACE_H
#define PRINTER_INTERFACE_H

#include <string>
#include <vector>

class PrinterInterface {
public:
    virtual ~PrinterInterface() = default;
    virtual std::vector<std::string> ListPrinters() = 0;
    virtual bool PrintRaw(const std::string &printer, const std::vector<uint8_t> &data) = 0;
};

#endif
