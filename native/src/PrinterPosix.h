#ifndef PRINTER_POSIX_H
#define PRINTER_POSIX_H

#include "PrinterInterface.h"
#include "JobStatus.h"
#include <cups/cups.h>
#include <vector>
#include <string>
#include <cstdint>

class PrinterPosix : public PrinterInterface
{
public:
    std::vector<PrinterInfo> getPrinters() override;
    std::optional<std::string> getDefaultPrinterName() override;
    int printRaw(const std::vector<uint8_t> &data, const std::string &documentName, const std::optional<std::string> &printer = std::nullopt) override;
    std::vector<JobInfo> getJobs(const std::optional<std::string> &printer = std::nullopt) override;
    std::optional<JobInfo> getJob(int jobId, const std::optional<std::string> &printer = std::nullopt) override;
    bool cancelJob(int jobId, const std::optional<std::string> &printer = std::nullopt) override;
    
private:
    PrinterInfo parsePrinter(const cups_dest_t &dest);
    JobInfo parseJob(const cups_job_t &job);
    JobStatus parseJobStatus(ipp_jstate_t status);
};

#endif
