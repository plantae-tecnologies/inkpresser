#include <napi.h>
#include "PrinterBuilder.h"

// Função para listar impressoras
Napi::Value ListPrinters(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    try {
        // Usar o Builder para instanciar a classe correta
        auto builder = PrinterBuilder::Create();
        auto printers = builder->ListPrinters();

        // Criar um array NAPI a partir da lista de impressoras
        Napi::Array result = Napi::Array::New(env, printers.size());
        for (size_t i = 0; i < printers.size(); ++i) {
            result[i] = Napi::String::New(env, printers[i]);
        }
        return result;
    } catch (const std::exception& e) {
        // Lançar exceção em caso de erro
        Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
        return env.Undefined();
    }
}

Napi::Value PrintRaw(const Napi::CallbackInfo& info) {
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
        bool success = builder->PrintRaw(printer, data);
        return Napi::Boolean::New(env, success);
    } catch (const std::exception& e) {
        Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
        return env.Undefined();
    }
}

// Método Init para exportar os métodos do módulo
Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set("listPrinters", Napi::Function::New(env, ListPrinters));
    exports.Set("printRaw", Napi::Function::New(env, PrintRaw));
    return exports;
}

NODE_API_MODULE(addon, Init)
