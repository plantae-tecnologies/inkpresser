#ifndef PRINTER_POSIX_H
#define PRINTER_POSIX_H

#include "PrinterInterface.h"
#include "JobStatus.h"
#include <cups/cups.h>
#include <vector>
#include <string>

class PrinterPosix : public PrinterInterface
{
public:
    std::vector<PrinterInfo> getPrinters() override;
    int printRaw(const std::vector<uint8_t> &data, const std::string &documentName, const std::string &printer = "") override;
    std::string getDefaultPrinterName() override;
    std::vector<JobInfo> getJobs(const std::string &printer = "") override;
    std::optional<JobInfo> getJob(int jobId, const std::string &printer = "") override;
    bool cancelJob(int jobId, const std::string &printer = "") override;
    
private:
    PrinterInfo parsePrinter(const cups_dest_t &dest);
    JobInfo parseJob(const cups_job_t &job);
    JobStatus parseJobStatus(ipp_jstate_t status);
};

#endif
