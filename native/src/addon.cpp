#include <napi.h>
#include "PrinterBuilder.h"

Napi::Value getDefaultPrinterName(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    try {
        auto builder = PrinterBuilder::Create();
        std::string defaultPrinter = builder->getDefaultPrinterName();

        if (defaultPrinter.empty()) {
            return env.Null();
        }

        return Napi::String::New(env, defaultPrinter);

    } catch (const std::exception& e) {
        Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
        return env.Undefined();
    }
}

Napi::Value getPrinters(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    try {
        auto builder = PrinterBuilder::Create();
        auto printers = builder->getPrinters();

        // Criar um array NAPI a partir da lista de impressoras
        Napi::Array result = Napi::Array::New(env, printers.size());
        for (size_t i = 0; i < printers.size(); ++i) {
            result[i] = Napi::String::New(env, printers[i]);
        }
        return result;

    } catch (const std::exception& e) {
        Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
        return env.Undefined();
    }
}

Napi::Value printRaw(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() < 2 || !info[0].IsString() || !info[1].IsBuffer()) {
        Napi::TypeError::New(env, "Expected a string (printer) and a Buffer (data)").ThrowAsJavaScriptException();
        return env.Undefined();
    }

    std::string printer = info[0].As<Napi::String>();

    // Dados como Buffer
    Napi::Buffer<uint8_t> buffer = info[1].As<Napi::Buffer<uint8_t>>();
    std::vector<uint8_t> data(buffer.Data(), buffer.Data() + buffer.Length());

    try {
        auto builder = PrinterBuilder::Create();
        bool success = builder->printRaw(printer, data);
        return Napi::Boolean::New(env, success);
    } catch (const std::exception& e) {
        Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
        return env.Undefined();
    }
}

// Método Init para exportar os métodos do módulo
Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set("getDefaultPrinterName", Napi::Function::New(env, getDefaultPrinterName)); 
    exports.Set("getPrinters", Napi::Function::New(env, getPrinters));
    exports.Set("printRaw", Napi::Function::New(env, printRaw));
    return exports;
}

NODE_API_MODULE(addon, Init)
