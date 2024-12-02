#include "PrinterPosix.h"
#include <stdexcept>
#include <string>
#include <vector>
#include <cups/cups.h>

using namespace std;

vector<string> PrinterPosix::getPrinters()
{
    cups_dest_t *destinations = nullptr;
    int num_destinations = cupsGetDests(&destinations);

    if (num_destinations <= 0)
    {
        // Clean up and return an empty list if no printers are found
        cupsFreeDests(num_destinations, destinations);
        return {};
    }

    // Convert each CUPS destination to a standard string and store in the list
    vector<string> printers;
    printers.reserve(num_destinations);
    for (int i = 0; i < num_destinations; ++i)
    {
        printers.emplace_back(destinations[i].name);
    }

    // Free resources allocated by CUPS
    cupsFreeDests(num_destinations, destinations);
    return printers;
}

string PrinterPosix::getDefaultPrinterName()
{
    const char* defaultPrinter = cupsGetDefault();
    return defaultPrinter ? std::string(defaultPrinter) : "";
}

bool PrinterPosix::printRaw(const vector<uint8_t> &data, const string &printer)
{
    // Checks if a printer was specified; otherwise, uses the default one
    string targetPrinter = printer.empty() ? getDefaultPrinterName() : printer;

    if (targetPrinter.empty())
    {
        throw runtime_error("No printer specified and no default printer is set.");
    }

    // Create a new print job
    int job_id = cupsCreateJob(CUPS_HTTP_DEFAULT, targetPrinter.c_str(), "Raw Print Job", 0, nullptr);
    if (job_id == 0)
    {
        throw runtime_error("Failed to create print job: " + string(cupsLastErrorString()));
    }

    // Start the document for the print job
    if (HTTP_CONTINUE != cupsStartDocument(CUPS_HTTP_DEFAULT, targetPrinter.c_str(), job_id, "Raw Print Job", CUPS_FORMAT_RAW, 1))
    {
        throw runtime_error("Failed to start document: " + string(cupsLastErrorString()));
    }

    // Send the raw data to the printer
    if (HTTP_CONTINUE != cupsWriteRequestData(CUPS_HTTP_DEFAULT, reinterpret_cast<const char *>(data.data()), data.size()))
    {
        cupsFinishDocument(CUPS_HTTP_DEFAULT, targetPrinter.c_str());
        throw runtime_error("Failed to send print data: " + string(cupsLastErrorString()));
    }

    // Finalize the document to complete the print job
    if (IPP_STATUS_OK != cupsFinishDocument(CUPS_HTTP_DEFAULT, targetPrinter.c_str()))
    {
        throw runtime_error("Failed to finish document: " + string(cupsLastErrorString()));
    }

    return true;
}

JobInfo PrinterPosix::parseJob(const cups_job_t &job, const std::string &printer) {
    const char* stateString = ippEnumString("job-state", job.state);
    if (!stateString) {
        stateString = "unknown";
    }

    JobInfo jobInfo;
    jobInfo.id = job.id;
    jobInfo.printer = printer;
    jobInfo.document = job.title ? job.title : "";
    jobInfo.status = stateString;
    jobInfo.user = job.user ? job.user : "";
    return jobInfo;
}

JobInfo PrinterPosix::getJob(int jobId, const std::string &printer)
{
    // Checks if a printer was specified; otherwise, uses the default one
    std::string targetPrinter = printer.empty() ? getDefaultPrinterName() : printer;

    if (targetPrinter.empty())
    {
        throw std::runtime_error("No printer specified and no default printer is set.");
    }

    cups_job_t *jobs = nullptr;
    int numJobs = cupsGetJobs(&jobs, targetPrinter.c_str(), 0, CUPS_WHICHJOBS_ALL);

    for (int i = 0; i < numJobs; ++i)
    {
        if (jobs[i].id == jobId)
        {
            JobInfo job = parseJob(jobs[i], targetPrinter);
            cupsFreeJobs(numJobs, jobs);
            return job;
        }
    }

    cupsFreeJobs(numJobs, jobs);
    throw std::runtime_error("Job not found.");
}
