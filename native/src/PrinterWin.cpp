#include "PrinterWin.h"
#include <windows.h>
#include <vector>
#include <string>
#include <stdexcept>

std::vector<PrinterInfo> PrinterWin::getPrinters()
{
    DWORD needed = 0, returned = 0;

    // Query the required size of the buffer
    EnumPrinters(PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS, nullptr, 2, nullptr, 0, &needed, &returned);

    // No printers found
    if (needed == 0)
    {
        return {};
    }

    // Allocate buffer and retrieve printer information
    std::vector<BYTE> buffer(needed);
    auto printerInfo = reinterpret_cast<PRINTER_INFO_2 *>(buffer.data());
    if (!EnumPrinters(PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS, nullptr, 2, buffer.data(), needed, &needed, &returned))
    {
        throw std::runtime_error("Failed to enumerate printers.");
    }

    // Encapsulate printers
    std::vector<PrinterInfo> printers;
    for (DWORD i = 0; i < returned; ++i)
    {
        printers.push_back(parsePrinter(printerInfo[i]));
    }

    return printers;
}

std::string PrinterWin::getDefaultPrinterName()
{
    // Determine the required buffer size for the default printer name
    DWORD bufferSize = 0;
    GetDefaultPrinterA(nullptr, &bufferSize);

    // No default printer found, return an empty string
    if (bufferSize == 0)
    {
        return "";
    }

    // Allocate a buffer to hold the default printer name
    std::vector<char> buffer(bufferSize);

    // Retrieve the default printer name
    if (!GetDefaultPrinterA(buffer.data(), &bufferSize))
    {
        // Throw an exception if retrieving the default printer fails
        throw std::runtime_error("Failed to get default printer: " + std::to_string(GetLastError()));
    }

    // Convert the retrieved printer name to a std::string and return
    return std::string(buffer.data());
}

int PrinterWin::printRaw(const std::vector<uint8_t> &data, const std::string &documentName, const std::string &printer)
{
    // Checks if a printer was specified; otherwise, uses the default one
    std::string targetPrinter = printer.empty() ? getDefaultPrinterName() : printer;

    if (targetPrinter.empty())
    {
        throw std::runtime_error("No printer specified and no default printer is set.");
    }

    // Open the printer
    HANDLE hPrinter = nullptr;
    if (!OpenPrinterA(const_cast<char *>(targetPrinter.c_str()), &hPrinter, nullptr))
    {
        throw std::runtime_error("Failed to open printer: " + targetPrinter);
    }

    // Configure the print job
    DOC_INFO_1A docInfo = {};
    docInfo.pDocName = const_cast<char *>(documentName.c_str());
    docInfo.pOutputFile = nullptr;
    docInfo.pDatatype = const_cast<char *>("RAW");

    // Start the print job
    DWORD jobId = StartDocPrinterA(hPrinter, 1, reinterpret_cast<BYTE *>(&docInfo));
    if (jobId == 0)
    {
        ClosePrinter(hPrinter);
        throw std::runtime_error("Failed to start print job on printer: " + targetPrinter);
    }

    // Start the print page
    if (!StartPagePrinter(hPrinter))
    {
        EndDocPrinter(hPrinter);
        ClosePrinter(hPrinter);
        throw std::runtime_error("Failed to start printer page on printer: " + targetPrinter);
    }

    // Write data to the printer
    DWORD bytesWritten = 0;
    if (!WritePrinter(hPrinter, const_cast<uint8_t *>(data.data()), static_cast<DWORD>(data.size()), &bytesWritten))
    {
        EndPagePrinter(hPrinter);
        EndDocPrinter(hPrinter);
        ClosePrinter(hPrinter);
        throw std::runtime_error("Failed to write data to printer: " + targetPrinter);
    }

    // Finish resources
    EndPagePrinter(hPrinter);
    EndDocPrinter(hPrinter);
    ClosePrinter(hPrinter);

    return jobId;
}

PrinterInfo PrinterWin::parsePrinter(const PRINTER_INFO_2 &printerInfo)
{
    PrinterInfo info;
    info.name = printerInfo.pPrinterName ? printerInfo.pPrinterName : "";
    return info;
}

JobStatus PrinterWin::parseJobStatus(DWORD status)
{
    if (status & JOB_STATUS_PAUSED)
        return JobStatus::PAUSED;
    if (status & JOB_STATUS_SPOOLING)
        return JobStatus::PENDING;
    if (status & JOB_STATUS_PRINTING)
        return JobStatus::PROCESSING;
    if (status & JOB_STATUS_PRINTED)
        return JobStatus::COMPLETED;
    if (status & JOB_STATUS_DELETED)
        return JobStatus::CANCELED;
    if (status & JOB_STATUS_ERROR || status & JOB_STATUS_BLOCKED_DEVQ || status & JOB_STATUS_USER_INTERVENTION)
        return JobStatus::JOB_ERROR;
    if (status & JOB_STATUS_OFFLINE || status & JOB_STATUS_PAPEROUT)
        return JobStatus::WAITING_FOR_DEVICE;

    return JobStatus::UNKNOWN;
}

JobInfo PrinterWin::parseJob(const JOB_INFO_1A &jobInfo)
{
    JobInfo job;
    job.id = jobInfo.JobId;
    job.printer = jobInfo.pPrinterName ? jobInfo.pPrinterName : "";
    job.document = jobInfo.pDocument ? jobInfo.pDocument : "";
    job.status = to_string(parseJobStatus(jobInfo.Status));
    job.user = jobInfo.pUserName ? jobInfo.pUserName : "";
    return job;
}

std::vector<JobInfo> PrinterWin::getJobs(const std::string &printer)
{
    // Usa a impressora padrão se nenhuma for especificada
    std::string targetPrinter = printer.empty() ? getDefaultPrinterName() : printer;

    if (targetPrinter.empty())
    {
        throw std::runtime_error("No printer specified and no default printer is set.");
    }

    HANDLE hPrinter = nullptr;
    if (!OpenPrinterA(const_cast<char *>(targetPrinter.c_str()), &hPrinter, nullptr))
    {
        throw std::runtime_error("Failed to open printer: " + targetPrinter);
    }

    DWORD needed = 0, returned = 0;
    EnumJobsA(hPrinter, 0, 0xFFFFFFFF, 1, nullptr, 0, &needed, &returned);

    if (needed == 0)
    {
        ClosePrinter(hPrinter);
        return {};
    }

    std::vector<BYTE> buffer(needed);
    if (!EnumJobsA(hPrinter, 0, 0xFFFFFFFF, 1, buffer.data(), needed, &needed, &returned))
    {
        ClosePrinter(hPrinter);
        throw std::runtime_error("Failed to enumerate jobs: " + std::to_string(GetLastError()));
    }

    auto jobInfo = reinterpret_cast<JOB_INFO_1A *>(buffer.data());
    std::vector<JobInfo> jobList;
    for (DWORD i = 0; i < returned; ++i)
    {
        jobList.push_back(parseJob(jobInfo[i]));
    }

    ClosePrinter(hPrinter);
    return jobList;
}

JobInfo PrinterWin::getJob(int jobId, const std::string &printer)
{
    // Checks if a printer was specified; otherwise, uses the default one
    std::string targetPrinter = printer.empty() ? getDefaultPrinterName() : printer;

    if (targetPrinter.empty())
    {
        throw std::runtime_error("No printer specified and no default printer is set.");
    }

    // Open the printer
    HANDLE hPrinter = nullptr;
    if (!OpenPrinterA(const_cast<char *>(targetPrinter.c_str()), &hPrinter, nullptr))
    {
        throw std::runtime_error("Failed to open printer: " + targetPrinter);
    }

    // Determine the required buffer size for job information
    DWORD needed = 0;
    GetJobA(hPrinter, jobId, 1, nullptr, 0, &needed);

    if (needed == 0)
    {
        ClosePrinter(hPrinter);
        throw std::runtime_error("Job not found or failed to retrieve job details.");
    }

    // Allocate buffer and retrieve job information
    std::vector<BYTE> buffer(needed);
    if (!GetJobA(hPrinter, jobId, 1, buffer.data(), needed, &needed))
    {
        ClosePrinter(hPrinter);
        throw std::runtime_error("Failed to retrieve job details: " + std::to_string(GetLastError()));
    }

    // Parse the job information
    auto jobInfo = reinterpret_cast<JOB_INFO_1A *>(buffer.data());
    JobInfo job = parseJob(*jobInfo);

    // Close the printer
    ClosePrinter(hPrinter);

    return job;
}

bool PrinterWin::cancelJob(int jobId, const std::string &printer)
{
    // Checks if a printer was specified; otherwise, uses the default one
    std::string targetPrinter = printer.empty() ? getDefaultPrinterName() : printer;

    if (targetPrinter.empty())
    {
        throw std::runtime_error("No printer specified and no default printer is set.");
    }

    // Open the printer
    HANDLE hPrinter = nullptr;
    if (!OpenPrinterA(const_cast<char *>(targetPrinter.c_str()), &hPrinter, nullptr))
    {
        throw std::runtime_error("Failed to open printer: " + targetPrinter);
    }

    // Try to cancel job
    if (!SetJobA(hPrinter, jobId, 0, nullptr, JOB_CONTROL_CANCEL))
    {
        ClosePrinter(hPrinter);
        throw std::runtime_error("Failed to cancel job: " + std::to_string(GetLastError()));
    }

    // Close the printer
    ClosePrinter(hPrinter);
    return true;
}
