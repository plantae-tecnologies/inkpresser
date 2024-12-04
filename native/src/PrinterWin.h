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
    std::vector<std::string> getPrinters() override;
    int printRaw(const std::vector<uint8_t> &data, const std::string &printer = "") override;
    std::string getDefaultPrinterName() override;
    std::vector<JobInfo> getJobs(const std::string &printer = "") override;
    JobInfo getJob(int jobId, const std::string &printer = "") override;
    bool cancelJob(int jobId, const std::string &printer = "") override;

private:
    JobInfo parseJob(const JOB_INFO_1A &jobInfo, const std::string &printer);
    JobStatus parseJobStatus(DWORD status);
};

#endif
