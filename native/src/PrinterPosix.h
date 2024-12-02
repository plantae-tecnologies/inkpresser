#ifndef PRINTER_POSIX_H
#define PRINTER_POSIX_H

#include "PrinterInterface.h"
#include <cups/cups.h>
#include <vector>
#include <string>

class PrinterPosix : public PrinterInterface
{
public:
    std::vector<std::string> getPrinters() override;
    bool printRaw(const std::vector<uint8_t> &data, const std::string &printer = "") override;
    std::string getDefaultPrinterName() override;
    JobInfo getJob(int jobId, const std::string &printer = "") override;
    
private:
    JobInfo parseJob(const cups_job_t &job, const std::string &printer);

};

#endif
