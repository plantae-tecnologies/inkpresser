#ifndef PRINTER_BUILDER_H
#define PRINTER_BUILDER_H

#include "PrinterInterface.h"
#include <memory>

class PrinterBuilder {
public:
    static std::shared_ptr<PrinterInterface> Create();
};

#endif
