#include "PrinterBuilder.h"
#ifdef _WIN32
#include "PrinterWin.h"
#else
#include "PrinterPosix.h"
#endif
#include <memory>

std::shared_ptr<PrinterInterface> PrinterBuilder::Create()
{
#ifdef _WIN32
    return std::make_shared<PrinterWin>();
#else
    return std::make_shared<PrinterPosix>();
#endif
}
