#include "PrinterWin.h"
#include <windows.h>
#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>

std::vector<std::string> PrinterWin::getPrinters() {
    DWORD needed = 0, returned = 0;

    // Query the required size of the buffer
    EnumPrinters(PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS, nullptr, 2, nullptr, 0, &needed, &returned);

    // No printers found
    if (needed == 0) {
        return {};
    }

    // Allocate buffer and retrieve printer information
    std::vector<BYTE> buffer(needed);
    auto printerInfo = reinterpret_cast<PRINTER_INFO_2*>(buffer.data());
    if (!EnumPrinters(PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS, nullptr, 2, buffer.data(), needed, &needed, &returned)) {
        throw std::runtime_error("Failed to enumerate printers.");
    }

    // Collect printer names
    std::vector<std::string> printers;
    for (DWORD i = 0; i < returned; ++i) {
        printers.emplace_back(printerInfo[i].pPrinterName);
    }

    return printers;
}

bool PrinterWin::printRaw(const std::string& printer, const std::vector<uint8_t>& data) {
    // Open the printer
    HANDLE hPrinter = nullptr;
    if (!OpenPrinterA(const_cast<char*>(printer.c_str()), &hPrinter, nullptr)) {
        throw std::runtime_error("Failed to open printer: " + printer);
    }

    // Configure the print job
    DOC_INFO_1A docInfo = {};
    docInfo.pDocName = const_cast<char*>("Raw Print Job");
    docInfo.pOutputFile = nullptr;
    docInfo.pDatatype = const_cast<char*>("RAW");

    // Start the print job
    DWORD jobId = StartDocPrinterA(hPrinter, 1, reinterpret_cast<BYTE*>(&docInfo));
    if (jobId == 0) {
        ClosePrinter(hPrinter);
        throw std::runtime_error("Failed to start print job on printer: " + printer);
    }

    // Start the print page
    if (!StartPagePrinter(hPrinter)) {
        EndDocPrinter(hPrinter);
        ClosePrinter(hPrinter);
        throw std::runtime_error("Failed to start printer page on printer: " + printer);
    }

    // Write data to the printer
    DWORD bytesWritten = 0;
    if (!WritePrinter(hPrinter, const_cast<uint8_t*>(data.data()), static_cast<DWORD>(data.size()), &bytesWritten)) {
        EndPagePrinter(hPrinter);
        EndDocPrinter(hPrinter);
        ClosePrinter(hPrinter);
        throw std::runtime_error("Failed to write data to printer: " + printer);
    }

    // End the print page and the job
    EndPagePrinter(hPrinter);
    EndDocPrinter(hPrinter);

    // Close the printer
    ClosePrinter(hPrinter);

    // Return success if all data was written
    return bytesWritten == data.size();
}