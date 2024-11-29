#include "PrinterWin.h"
#include <windows.h>
#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>

std::vector<std::string> PrinterWin::getPrinters()
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

    // Collect printer names
    std::vector<std::string> printers;
    for (DWORD i = 0; i < returned; ++i)
    {
        printers.emplace_back(printerInfo[i].pPrinterName);
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

bool PrinterWin::printRaw(const std::vector<uint8_t> &data, const std::string &printer)
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
    docInfo.pDocName = const_cast<char *>("Raw Print Job");
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

    // End the print page and the job
    EndPagePrinter(hPrinter);
    EndDocPrinter(hPrinter);

    // Close the printer
    ClosePrinter(hPrinter);

    // Return success if all data was written
    return bytesWritten == data.size();
}