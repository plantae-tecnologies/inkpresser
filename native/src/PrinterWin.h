#ifndef PRINTER_WIN_H
#define PRINTER_WIN_H

#include "PrinterInterface.h"
#include <vector>
#include <string>

class PrinterWin : public PrinterInterface {
public:
    std::vector<std::string> ListPrinters() override;
    bool PrintRaw(const std::string &printer, const std::string &data) override;
};

#endif
