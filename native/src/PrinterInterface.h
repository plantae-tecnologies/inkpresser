#ifndef PRINTER_INTERFACE_H
#define PRINTER_INTERFACE_H

#include <string>
#include <vector>
#include <optional>
#include "JobInfo.h"
#include "PrinterInfo.h"

class PrinterInterface
{
public:
virtual ~PrinterInterface() = default;
    virtual std::vector<PrinterInfo> getPrinters() = 0;
    virtual std::optional<std::string> getDefaultPrinterName() = 0;
    virtual int printRaw(const std::vector<uint8_t> &data, const std::string &documentName, const std::optional<std::string> &printer = std::nullopt) = 0;
    virtual int print(const std::vector<uint8_t> &data, const std::string &documentName, const std::optional<std::string> &printer = std::nullopt) = 0;
    virtual std::vector<JobInfo> getJobs(const std::optional<std::string> &printer = std::nullopt) = 0;
    virtual std::optional<JobInfo> getJob(int jobId, const std::optional<std::string> &printer = std::nullopt) = 0;
    virtual bool cancelJob(int jobId, const std::optional<std::string> &printer = std::nullopt) = 0;
};

#endif
