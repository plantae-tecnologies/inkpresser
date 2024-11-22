#include "PrinterWin.h"
#include <stdexcept>

std::vector<std::string> PrinterWin::ListPrinters() {
    throw std::runtime_error("ListPrinters is not implemented for Windows yet.");
}

bool PrinterWin::PrintRaw(const std::string &printer, const std::string &data) {
    throw std::runtime_error("PrintRaw is not implemented for Windows yet.");
}