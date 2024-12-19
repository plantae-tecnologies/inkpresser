import bindings from './bindings';
import { PrinterInfo } from './types';
import { Printer } from './printer';

export class PrintManager {
    async getPrinters(): Promise<Printer[]> {
        const printers: PrinterInfo[] = bindings.getPrinters();
        return printers.map(printer => new Printer(printer));
    }

    async getDefaultPrinter(): Promise<Printer> {
        const defaultPrinterName: string = bindings.getDefaultPrinterName();
        return new Printer({ name: defaultPrinterName, isDefault: true });
    }
}
