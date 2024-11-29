#ifndef PRINTER_INTERFACE_H
#define PRINTER_INTERFACE_H

#include <string>
#include <vector>

class PrinterInterface
{
public:
    virtual ~PrinterInterface() = default;
    virtual std::vector<std::string> getPrinters() = 0;
    virtual bool printRaw(const std::vector<uint8_t> &data, const std::string &printer = "") = 0;
    virtual std::string getDefaultPrinterName() = 0;
};

#endif
