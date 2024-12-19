import { describe, it, expect, vi } from 'vitest';
import { PrintManager } from '../../src/print-manager';
import { Printer } from '../../src/printer';

vi.mock('../../src/bindings', () => ({
    default: {
        getPrinters: vi.fn(() => [{
            name: 'Printer1',
            isDefault: true,
        },
        {
            name: 'Printer2',
            isDefault: false,
        }]),
        getDefaultPrinterName: vi.fn(() => 'DefaultPrinter'),
    },
}));

describe('PrintManager', () => {

    it('should return a list of "Printer" instances', async () => {
        const printManager = new PrintManager();
        const printers = await printManager.getPrinters();

        expect(printers[0]).toBeInstanceOf(Printer);
        expect(printers[0].name).toBe('Printer1');
        expect(printers[0].isDefault).toBe(true);

        expect(printers[1]).toBeInstanceOf(Printer);
        expect(printers[1].name).toBe('Printer2');
        expect(printers[1].isDefault).toBe(false);
    });

    it('should return the default printer as an instance of "Printer"', async () => {
        const printManager = new PrintManager();
        const defaultPrinter = await printManager.getDefaultPrinter();

        expect(defaultPrinter).toBeInstanceOf(Printer);
        expect(defaultPrinter.name).toBe('DefaultPrinter');
        expect(defaultPrinter.isDefault).toBe(true);
    });
});
