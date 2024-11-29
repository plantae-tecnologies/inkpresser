#ifndef PRINTER_WIN_H
#define PRINTER_WIN_H

#include "PrinterInterface.h"
#include <vector>
#include <string>

class PrinterWin : public PrinterInterface {
public:
    std::vector<std::string> getPrinters() override;
    bool printRaw(const std::string &printer, const std::vector<uint8_t> &data) override;
    std::string getDefaultPrinterName() override;
};

#endif
