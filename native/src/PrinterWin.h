#ifndef PRINTER_WIN_H
#define PRINTER_WIN_H

#include "PrinterInterface.h"
#include <vector>
#include <string>

class PrinterWin : public PrinterInterface
{
public:
    std::vector<std::string> getPrinters() override;
    bool printRaw(const std::vector<uint8_t> &data, const std::string &printer = "") override;
    std::string getDefaultPrinterName() override;
};

#endif
