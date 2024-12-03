#ifndef PRINTER_INTERFACE_H
#define PRINTER_INTERFACE_H

#include <string>
#include <vector>
#include "JobInfo.h"

class PrinterInterface
{
public:
    virtual ~PrinterInterface() = default;
    virtual std::vector<std::string> getPrinters() = 0;
    virtual int printRaw(const std::vector<uint8_t> &data, const std::string &printer = "") = 0;
    virtual std::string getDefaultPrinterName() = 0;
    virtual JobInfo getJob(int jobId, const std::string &printer = "") = 0;
};

#endif
