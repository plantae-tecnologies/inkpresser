import { describe, it, expect } from 'vitest';
import { PrintManager } from '../../src/index.ts';

describe('Integration: PrintManager', () => {
    it('should list the printers available on the system', async () => {
        const manager = new PrintManager();
        const printers = await manager.getPrinters();

        // Verify the result is an array
        expect(printers).toBeInstanceOf(Array);

        // Check if each printer has a "name" property
        for (const printer of printers) {
            expect(printer).toHaveProperty('name');
        }
    });
});
