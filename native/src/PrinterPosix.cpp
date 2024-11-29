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
    const char *defaultPrinter = cupsGetDefault();

    // Return an empty string if no default printer is set
    if (defaultPrinter == nullptr)
    {
        return "";
    }

    return string(defaultPrinter);
}

bool PrinterPosix::printRaw(const vector<uint8_t> &data, const string &printer)
{
    // Verifica se uma impressora foi especificada, caso contrário, usa a padrão
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
