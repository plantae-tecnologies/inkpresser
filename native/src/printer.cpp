#include "printer.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>

#include <locale>
#include <codecvt>

#ifdef _WIN32
#include <windows.h>
#include <winspool.h>
#else
#include <cups/cups.h>
#endif

std::vector<std::string> listPrinters()
{
    std::vector<std::string> printers;

#ifdef _WIN32
    DWORD needed, returned;
    EnumPrinters(PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS, nullptr, 2, nullptr, 0, &needed, &returned);
    BYTE *buffer = new BYTE[needed];
    if (EnumPrinters(PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS, nullptr, 2, buffer, needed, &needed, &returned))
    {
        PRINTER_INFO_2 *printerInfo = reinterpret_cast<PRINTER_INFO_2 *>(buffer);
        for (DWORD i = 0; i < returned; i++)
        {
            printers.push_back(printerInfo[i].pPrinterName);
        }
    }
    delete[] buffer;
#else
    cups_dest_t *dests;
    int num_dests = cupsGetDests(&dests);
    for (int i = 0; i < num_dests; i++)
    {
        printers.push_back(dests[i].name);
    }
    cupsFreeDests(num_dests, dests);
#endif

    return printers;
}

bool printRaw(const std::string &printerName, const std::string &data)
{
    // Gera um arquivo temporário para envio
    std::string tempFile = "/tmp/temp_print_job";
    std::ofstream outFile(tempFile, std::ios::binary);
    if (!outFile)
    {
        return false;
    }

    // Grava os dados diretamente no arquivo (bruto)
    outFile.write(data.data(), data.size());
    outFile.close();

    // Adiciona as opções do CUPS
    cups_option_t *options = nullptr;
    int num_options = 0;

    // Ativa o modo raw
    num_options = cupsAddOption("raw", "true", num_options, &options);

    // Configura o espaçamento entre linhas via opção do CUPS
    num_options = cupsAddOption("lineSpacing", "1", num_options, &options);

    // Envia o arquivo diretamente no modo raw
    int jobId = cupsPrintFile(printerName.c_str(), tempFile.c_str(), "Print Job", num_options, options);

    // Remove o arquivo temporário
    std::remove(tempFile.c_str());

    // Libera memória alocada para as opções
    cupsFreeOptions(num_options, options);

    return jobId > 0;
}

int printFile(const std::string &filename, const std::string &docname, const std::string &printerName, const std::map<std::string, std::string> &options)
{
    // Converte as opções do mapa C++ para o formato de opções do CUPS
    cups_option_t *cupsOptions = nullptr;
    int numOptions = 0;

    for (const auto &option : options)
    {
        numOptions = cupsAddOption(option.first.c_str(), option.second.c_str(), numOptions, &cupsOptions);
    }

    // Envia o arquivo para impressão
    int jobId = cupsPrintFile(printerName.c_str(), filename.c_str(), docname.c_str(), numOptions, cupsOptions);

    // Libera as opções alocadas
    cupsFreeOptions(numOptions, cupsOptions);

    // Verifica se o jobId é válido
    if (jobId == 0)
    {
        throw std::runtime_error("Failed to submit print job"); // Lança uma exceção em caso de erro
    }

    return jobId; // Retorna o ID do trabalho em caso de sucesso
}