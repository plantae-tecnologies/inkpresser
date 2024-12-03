#include <napi.h>
#include "PrinterBuilder.h"

Napi::Value getDefaultPrinterName(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    try
    {
        // Create a PrinterBuilder instance
        auto builder = PrinterBuilder::Create();
        std::string defaultPrinter = builder->getDefaultPrinterName();

        // If no default printer is found, return null
        if (defaultPrinter.empty())
        {
            return env.Null();
        }

        // Return the default printer name as a string
        return Napi::String::New(env, defaultPrinter);
    }
    catch (const std::exception &e)
    {
        Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
        return env.Undefined();
    }
}

Napi::Value getPrinters(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    try
    {
        // Create a PrinterBuilder instance
        auto builder = PrinterBuilder::Create();
        auto printers = builder->getPrinters();

        // Create a Napi array from the list of printers
        Napi::Array result = Napi::Array::New(env, printers.size());
        for (size_t i = 0; i < printers.size(); ++i)
        {
            result[i] = Napi::String::New(env, printers[i]);
        }
        // Return the array of printers
        return result;
    }
    catch (const std::exception &e)
    {
        Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
        return env.Undefined();
    }
}

Napi::Value printRaw(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    // Validate if the first argument (data) is a buffer
    if (info.Length() < 1 || !info[0].IsBuffer())
    {
        Napi::TypeError::New(env, "Expected a Buffer (data) as the first argument").ThrowAsJavaScriptException();
        return env.Undefined();
    }

    // Retrieve the data as a Buffer
    Napi::Buffer<uint8_t> buffer = info[0].As<Napi::Buffer<uint8_t>>();
    std::vector<uint8_t> data(buffer.Data(), buffer.Data() + buffer.Length());

    // Check if the second argument (printer) was provided
    std::string printer = "";
    if (info.Length() > 1 && info[1].IsString())
    {
        printer = info[1].As<Napi::String>();
    }

    try
    {
        auto builder = PrinterBuilder::Create();
        int jobId = builder->printRaw(data, printer);
        return Napi::Number::New(env, jobId);
    }
    catch (const std::exception &e)
    {
        Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
        return env.Undefined();
    }
}

Napi::Value getJob(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() < 1 || !info[0].IsNumber()) {
        Napi::TypeError::New(env, "Expected a jobId (number) as the first argument").ThrowAsJavaScriptException();
        return env.Undefined();
    }

    int jobId = info[0].As<Napi::Number>().Int32Value();

    std::string printer = "";
    if (info.Length() > 1 && info[1].IsString()) {
        printer = info[1].As<Napi::String>();
    }

    try {
        auto builder = PrinterBuilder::Create();
        JobInfo jobInfo = builder->getJob(jobId, printer);

        Napi::Object jsObject = Napi::Object::New(env);
        jsObject.Set("id", Napi::Number::New(env, jobInfo.id));
        jsObject.Set("printer", Napi::String::New(env, jobInfo.printer));
        jsObject.Set("document", Napi::String::New(env, jobInfo.document));
        jsObject.Set("status", Napi::String::New(env, jobInfo.status));
        jsObject.Set("user", Napi::String::New(env, jobInfo.user));

        return jsObject;
    } catch (const std::exception& e) {
        Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
        return env.Undefined();
    }
}

// Initialization method to export module functions
Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports.Set("getDefaultPrinterName", Napi::Function::New(env, getDefaultPrinterName));
    exports.Set("getPrinters", Napi::Function::New(env, getPrinters));
    exports.Set("printRaw", Napi::Function::New(env, printRaw));
    exports.Set("getJob", Napi::Function::New(env, getJob));
    return exports;
}

NODE_API_MODULE(addon, Init)
