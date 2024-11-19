#include "printer.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>


#include <locale>
#include <codecvt>

#ifdef _WIN32
#include <windows.h>
#include <winspool.h>
#else
#include <cups/cups.h>
#endif

std::vector<std::string> listPrinters() {
    std::vector<std::string> printers;

#ifdef _WIN32
    DWORD needed, returned;
    EnumPrinters(PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS, nullptr, 2, nullptr, 0, &needed, &returned);
    BYTE* buffer = new BYTE[needed];
    if (EnumPrinters(PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS, nullptr, 2, buffer, needed, &needed, &returned)) {
        PRINTER_INFO_2* printerInfo = reinterpret_cast<PRINTER_INFO_2*>(buffer);
        for (DWORD i = 0; i < returned; i++) {
            printers.push_back(printerInfo[i].pPrinterName);
        }
    }
    delete[] buffer;
#else
    cups_dest_t *dests;
    int num_dests = cupsGetDests(&dests);
    for (int i = 0; i < num_dests; i++) {
        printers.push_back(dests[i].name);
    }
    cupsFreeDests(num_dests, dests);
#endif

    return printers;
}

bool printRaw(const std::string &printerName, const std::string &data) {

    int lineSpacing = 40;

    // Gera um arquivo temporário para envio
    std::string tempFile = "/tmp/temp_print_job";
    std::ofstream outFile(tempFile, std::ios::binary);
    if (!outFile) {
        return false;
    }

    // Adiciona comando ESC/POS para espaçamento entre linhas
    std::string escPosCommand;

    // Configura espaçamento entre linhas em n/72 polegadas
    escPosCommand.push_back(0x1B); // ESC
    escPosCommand.push_back(0x33); // '3'
    escPosCommand.push_back(static_cast<char>(lineSpacing)); // n (exemplo: 24 para 1/3 polegada)

    // Escreve o comando de controle no início do arquivo
    outFile.write(escPosCommand.data(), escPosCommand.size());

    // Grava os dados diretamente no arquivo (bruto)
    outFile.write(data.data(), data.size());
    outFile.close();

    // Adiciona as opções
    cups_option_t *options = nullptr;
    int num_options = 0;

    // Ativa o modo raw
    num_options = cupsAddOption("raw", "true", num_options, &options);

    // Envia o arquivo diretamente no modo raw
    int jobId = cupsPrintFile(printerName.c_str(), tempFile.c_str(), "Print Job", num_options, options);

    // Remove o arquivo temporário
    std::remove(tempFile.c_str());

    // Libera memória alocada para as opções
    cupsFreeOptions(num_options, options);

    return jobId > 0;
}

