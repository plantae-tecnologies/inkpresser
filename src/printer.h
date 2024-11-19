#ifndef PRINTER_H
#define PRINTER_H

#include <string>
#include <vector>

std::vector<std::string> listPrinters();
bool printRaw(const std::string &printerName, const std::string &data);

#endif
