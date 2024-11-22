#ifndef PRINTER_H
#define PRINTER_H

#include <string>
#include <vector>
#include <map>

std::vector<std::string> listPrinters();
bool printRaw(const std::string &printerName, const std::string &data);
int printFile(const std::string &filename, const std::string &docname, const std::string &printerName, const std::map<std::string, std::string> &options);

#endif
