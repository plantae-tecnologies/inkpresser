#include "PrinterPosix.h"
#include <stdexcept>
#include <string>
#include <vector>
#include <cups/cups.h>

using namespace std;

std::vector<PrinterInfo> PrinterPosix::getPrinters()
{
    cups_dest_t *dests = nullptr;
    int numDests = cupsGetDests(&dests);

    if (numDests <= 0)
    {
        return {};
    }

    std::vector<PrinterInfo> printers;
    for (int i = 0; i < numDests; ++i)
    {
        printers.push_back(parsePrinter(dests[i]));
    }

    cupsFreeDests(numDests, dests);
    return printers;
}

string PrinterPosix::getDefaultPrinterName()
{
    const char *defaultPrinter = cupsGetDefault();
    return defaultPrinter ? std::string(defaultPrinter) : "";
}

int PrinterPosix::printRaw(const std::vector<uint8_t> &data, const std::string &documentName, const std::string &printer)
{
    // Checks if a printer was specified; otherwise, uses the default one
    std::string targetPrinter = printer.empty() ? getDefaultPrinterName() : printer;

    if (targetPrinter.empty())
    {
        throw std::runtime_error("No printer specified and no default printer is set.");
    }

    // Create a new print job
    int job_id = cupsCreateJob(CUPS_HTTP_DEFAULT, targetPrinter.c_str(), documentName.c_str(), 0, nullptr);
    if (job_id == 0)
    {
        throw std::runtime_error("Failed to create print job: " + std::string(cupsLastErrorString()));
    }

    // Start the document for the print job
    if (HTTP_CONTINUE != cupsStartDocument(CUPS_HTTP_DEFAULT, targetPrinter.c_str(), job_id, documentName.c_str(), CUPS_FORMAT_RAW, 1))
    {
        throw std::runtime_error("Failed to start document: " + std::string(cupsLastErrorString()));
    }

    // Send the raw data to the printer
    if (HTTP_CONTINUE != cupsWriteRequestData(CUPS_HTTP_DEFAULT, reinterpret_cast<const char *>(data.data()), data.size()))
    {
        cupsFinishDocument(CUPS_HTTP_DEFAULT, targetPrinter.c_str());
        throw std::runtime_error("Failed to send print data: " + std::string(cupsLastErrorString()));
    }

    // Finalize the document to complete the print job
    if (IPP_STATUS_OK != cupsFinishDocument(CUPS_HTTP_DEFAULT, targetPrinter.c_str()))
    {
        throw std::runtime_error("Failed to finish document: " + std::string(cupsLastErrorString()));
    }

    return job_id;
}

PrinterInfo PrinterPosix::parsePrinter(const cups_dest_t &dest)
{
    PrinterInfo info;
    info.name = dest.name;
    return info;
}

JobStatus PrinterPosix::parseJobStatus(ipp_jstate_t state)
{
    if (state == IPP_JOB_PENDING)
        return JobStatus::PENDING;

    if (state == IPP_JOB_HELD)
        return JobStatus::PAUSED;

    if (state == IPP_JOB_PROCESSING)
        return JobStatus::PROCESSING;

    if (state == IPP_JOB_COMPLETED)
        return JobStatus::COMPLETED;

    if (state == IPP_JOB_CANCELED)
        return JobStatus::CANCELED;

    if (state == IPP_JOB_ABORTED)
        return JobStatus::JOB_ERROR;

    if (state == IPP_JOB_STOPPED)
        return JobStatus::WAITING_FOR_DEVICE;

    return JobStatus::UNKNOWN;
}

JobInfo PrinterPosix::parseJob(const cups_job_t &job)
{
    JobInfo jobInfo;
    jobInfo.id = job.id;
    jobInfo.printer = job.dest ? job.dest : "";
    jobInfo.document = job.title ? job.title : "";
    jobInfo.status = to_string(parseJobStatus(job.state));
    jobInfo.user = job.user ? job.user : "";
    return jobInfo;
}

std::vector<JobInfo> PrinterPosix::getJobs(const std::string &printer)
{
    // Checks if a printer was specified; otherwise, uses the default one
    std::string targetPrinter = printer.empty() ? getDefaultPrinterName() : printer;

    if (targetPrinter.empty())
    {
        throw std::runtime_error("No printer specified and no default printer is set.");
    }

    cups_job_t *jobs = nullptr;
    int numJobs = cupsGetJobs(&jobs, targetPrinter.c_str(), 0, CUPS_WHICHJOBS_ACTIVE);

    std::vector<JobInfo> jobList;
    for (int i = 0; i < numJobs; ++i)
    {
        jobList.push_back(parseJob(jobs[i]));
    }

    cupsFreeJobs(numJobs, jobs);
    return jobList;
}

std::optional<JobInfo> PrinterPosix::getJob(int jobId, const std::string &printer)
{
    auto allJobs = getJobs(printer);

    // Look for the specific jobId in the fetched jobs
    for (const auto &job : allJobs)
    {
        if (job.id == jobId)
        {
            return job;
        }
    }

    return std::nullopt;
}

bool PrinterPosix::cancelJob(int jobId, const std::string &printer)
{
    // Checks if a printer was specified; otherwise, uses the default one
    std::string targetPrinter = printer.empty() ? getDefaultPrinterName() : printer;

    if (targetPrinter.empty())
    {
        throw std::runtime_error("No printer specified and no default printer is set.");
    }

    // Try to cancel job
    if (!cupsCancelJob(targetPrinter.c_str(), jobId))
    {
        throw std::runtime_error("Failed to cancel job: " + std::string());
    }

    return true;
}