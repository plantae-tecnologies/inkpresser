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
        cupsFreeDests(num_destinations, destinations);
        return {};
    }

    vector<string> printers;
    printers.reserve(num_destinations);
    for (int i = 0; i < num_destinations; ++i)
    {
        printers.emplace_back(destinations[i].name);
    }

    cupsFreeDests(num_destinations, destinations);
    return printers;
}

string PrinterPosix::getDefaultPrinterName()
{
    const char* defaultPrinter = cupsGetDefault();

    if (defaultPrinter == nullptr) {
        return "";
    }

    return string(defaultPrinter);
}

bool PrinterPosix::printRaw(const string &printer, const vector<uint8_t> &data)
{
    // Criar um novo trabalho de impressão
    int job_id = cupsCreateJob(CUPS_HTTP_DEFAULT, printer.c_str(), "Raw Print Job", 0, nullptr);
    if (job_id == 0)
    {
        throw runtime_error("Failed to create print job: " + string(cupsLastErrorString()));
    }

    // Iniciar o documento para o trabalho de impressão
    if (HTTP_CONTINUE != cupsStartDocument(CUPS_HTTP_DEFAULT, printer.c_str(), job_id, "Raw Print Job", CUPS_FORMAT_RAW, 1))
    {
        throw runtime_error("Failed to start document: " + string(cupsLastErrorString()));
    }

    // Enviar os dados diretamente
    if (HTTP_CONTINUE != cupsWriteRequestData(CUPS_HTTP_DEFAULT, reinterpret_cast<const char *>(data.data()), data.size()))
    {
        cupsFinishDocument(CUPS_HTTP_DEFAULT, printer.c_str());
        throw runtime_error("Failed to send print data: " + string(cupsLastErrorString()));
    }

    if (IPP_STATUS_OK != cupsFinishDocument(CUPS_HTTP_DEFAULT, printer.c_str()))
    {
        throw runtime_error("Failed to finish document: " + string(cupsLastErrorString()));
    }

    return true;
}
