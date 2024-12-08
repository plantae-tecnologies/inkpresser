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
        auto builder = PrinterBuilder::Create();
        auto printers = builder->getPrinters();

        // Create a Napi array from the list of printers
        Napi::Array result = Napi::Array::New(env, printers.size());
        for (size_t i = 0; i < printers.size(); ++i)
        {
            Napi::Object printerObj = Napi::Object::New(env);
            printerObj.Set("name", printers[i].name);
            result[i] = printerObj;
        }

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

    // Validate input: Expect data (Buffer), documentName (string), and optionally printer (string)
    if (info.Length() < 2 || !info[0].IsBuffer() || !info[1].IsString())
    {
        Napi::TypeError::New(env, "Expected a Buffer (data) and a string (documentName), and optionally a string (printer)").ThrowAsJavaScriptException();
        return env.Undefined();
    }

    // Get the data as a Buffer
    Napi::Buffer<uint8_t> buffer = info[0].As<Napi::Buffer<uint8_t>>();
    std::vector<uint8_t> data(buffer.Data(), buffer.Data() + buffer.Length());

    // Get the document name
    std::string documentName = info[1].As<Napi::String>();

    // Get the printer name (optional)
    std::string printer = "";
    if (info.Length() > 2 && info[2].IsString())
    {
        printer = info[2].As<Napi::String>();
    }

    try
    {
        auto builder = PrinterBuilder::Create();
        int jobId = builder->printRaw(data, documentName, printer);
        return Napi::Number::New(env, jobId);
    }
    catch (const std::exception &e)
    {
        Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
        return env.Undefined();
    }
}

Napi::Value getJobs(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    std::string printer = "";
    if (info.Length() > 0 && info[0].IsString())
    {
        printer = info[0].As<Napi::String>();
    }

    try
    {
        auto builder = PrinterBuilder::Create();
        auto jobs = builder->getJobs(printer);

        Napi::Array result = Napi::Array::New(env, jobs.size());
        for (size_t i = 0; i < jobs.size(); ++i)
        {
            Napi::Object jobObj = Napi::Object::New(env);
            jobObj.Set("id", Napi::Number::New(env, jobs[i].id));
            jobObj.Set("printer", Napi::String::New(env, jobs[i].printer));
            jobObj.Set("document", Napi::String::New(env, jobs[i].document));
            jobObj.Set("status", Napi::String::New(env, jobs[i].status));
            jobObj.Set("user", Napi::String::New(env, jobs[i].user));
            result[i] = jobObj;
        }

        return result;
    }
    catch (const std::exception &e)
    {
        Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
        return env.Undefined();
    }
}

Napi::Value getJob(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    if (info.Length() < 1 || !info[0].IsNumber())
    {
        Napi::TypeError::New(env, "Expected a jobId (number) as the first argument").ThrowAsJavaScriptException();
        return env.Undefined();
    }

    int jobId = info[0].As<Napi::Number>().Int32Value();

    std::string printer = "";
    if (info.Length() > 1 && info[1].IsString())
    {
        printer = info[1].As<Napi::String>();
    }

    try
    {
        auto builder = PrinterBuilder::Create();
        JobInfo jobInfo = builder->getJob(jobId, printer);

        Napi::Object jsObject = Napi::Object::New(env);
        jsObject.Set("id", Napi::Number::New(env, jobInfo.id));
        jsObject.Set("printer", Napi::String::New(env, jobInfo.printer));
        jsObject.Set("document", Napi::String::New(env, jobInfo.document));
        jsObject.Set("status", Napi::String::New(env, jobInfo.status));
        jsObject.Set("user", Napi::String::New(env, jobInfo.user));

        return jsObject;
    }
    catch (const std::exception &e)
    {
        Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
        return env.Undefined();
    }
}

Napi::Value cancelJob(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    if (info.Length() < 1 || !info[0].IsNumber())
    {
        Napi::TypeError::New(env, "Expected a jobId (number) as the first argument").ThrowAsJavaScriptException();
        return env.Undefined();
    }

    int jobId = info[0].As<Napi::Number>().Int32Value();

    std::string printer = "";
    if (info.Length() > 1 && info[1].IsString())
    {
        printer = info[1].As<Napi::String>();
    }

    try
    {
        auto builder = PrinterBuilder::Create();
        bool success = builder->cancelJob(jobId, printer);
        return Napi::Boolean::New(env, success);
    }
    catch (const std::exception &e)
    {
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
    exports.Set("getJobs", Napi::Function::New(env, getJobs));
    exports.Set("getJob", Napi::Function::New(env, getJob));
    exports.Set("cancelJob", Napi::Function::New(env, cancelJob));
    return exports;
}

NODE_API_MODULE(addon, Init)
