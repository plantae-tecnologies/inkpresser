#ifndef PRINTER_WIN_H
#define PRINTER_WIN_H

#include "PrinterInterface.h"
#include "JobStatus.h"
#include <vector>
#include <string>
#include <windows.h>

class PrinterWin : public PrinterInterface
{
public:
    std::vector<PrinterInfo> getPrinters() override;
    int printRaw(const std::vector<uint8_t> &data, const std::string &documentName, const std::string &printer = "") override;
    std::string getDefaultPrinterName() override;
    std::vector<JobInfo> getJobs(const std::string &printer = "") override;
    std::optional<JobInfo> getJob(int jobId, const std::string &printer = "") override;
    bool cancelJob(int jobId, const std::string &printer = "") override;

private:
    JobInfo parseJob(const JOB_INFO_1A &jobInfo);
    JobStatus parseJobStatus(DWORD status);
    PrinterInfo parsePrinter(const PRINTER_INFO_2 &printerInfo);
};

#endif
