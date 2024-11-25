#ifndef PRINTER_POSIX_H
#define PRINTER_POSIX_H

#include "PrinterInterface.h"
#include <vector>
#include <string>

class PrinterPosix : public PrinterInterface {
public:
    std::vector<std::string> ListPrinters() override;
    bool PrintRaw(const std::string &printer, const std::vector<uint8_t> &data) override;
};

#endif
