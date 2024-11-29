#include "PrinterBuilder.h"
#include "PrinterWin.h"
#include "PrinterPosix.h"
#include <memory>

std::shared_ptr<PrinterInterface> PrinterBuilder::Create()
{
#ifdef _WIN32
    return std::make_shared<PrinterWin>();
#else
    return std::make_shared<PrinterPosix>();
#endif
}
