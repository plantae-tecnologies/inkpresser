import bindings from './bindings.ts';
import { PrinterInfo } from './types.ts';
import { Printer } from './printer.ts';

export class PrintManager {
    async getPrinters(): Promise<Printer[]> {
        const printers: PrinterInfo[] = bindings.getPrinters();
        return printers.map(printer => new Printer(printer));
    }

    async getDefaultPrinter(): Promise<Printer> {
        const defaultPrinterName: string = bindings.getDefaultPrinterName();
        return new Printer({ name: defaultPrinterName });
    }
}
