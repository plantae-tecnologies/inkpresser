import { describe, it, expect, vi } from 'vitest';
import { PrintManager } from '../src/print-manager.ts';
import { Printer } from '../src/printer.ts';

vi.mock('../src/bindings.ts', () => ({
    default: {
        getPrinters: vi.fn(() => [{ name: 'Printer1' }, { name: 'Printer2' }]),
        getDefaultPrinterName: vi.fn(() => 'DefaultPrinter'),
    },
}));

describe('PrintManager', () => {

    it('should return a list of "Printer" instances', async () => {
        const printManager = new PrintManager();
        const printers = await printManager.getPrinters();

        expect(printers[0]).toBeInstanceOf(Printer);
        expect(printers[0].name).toBe('Printer1');

        expect(printers[1]).toBeInstanceOf(Printer);
        expect(printers[1].name).toBe('Printer2');
    });

    it('should return the default printer as an instance of "Printer"', async () => {
        const printManager = new PrintManager();
        const defaultPrinter = await printManager.getDefaultPrinter();

        expect(defaultPrinter).toBeInstanceOf(Printer);
        expect(defaultPrinter.name).toBe('DefaultPrinter');
    });
});
