#include <napi.h>
#include "printer.h"

Napi::Array ListPrinters(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
    Napi::Array printers = Napi::Array::New(env);

    std::vector<std::string> printerList = listPrinters();
    for (size_t i = 0; i < printerList.size(); i++) {
        printers.Set(i, Napi::String::New(env, printerList[i]));
    }

    return printers;
}

Napi::Boolean PrintRaw(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();

    if (info.Length() != 2 || !info[0].IsString() || !info[1].IsString()) {
        Napi::TypeError::New(env, "Expected two string arguments").ThrowAsJavaScriptException();
        return Napi::Boolean::New(env, false);
    }

    std::string printerName = info[0].As<Napi::String>();
    std::string data = info[1].As<Napi::String>();

    bool success = printRaw(printerName, data);
    return Napi::Boolean::New(env, success);
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set("listPrinters", Napi::Function::New(env, ListPrinters));
    exports.Set("printRaw", Napi::Function::New(env, PrintRaw));
    return exports;
}

NODE_API_MODULE(addon, Init)
