import { describe, it, expect } from 'vitest';
import { Printer, PrintManager } from '../../src/index';

describe('Integration: Retrieve Non-Existent Print Job', () => {
    it('should return null for a non-existent job ID', async () => {
        const manager = new PrintManager();
        const printer = await manager.getDefaultPrinter();

        expect(printer).not.toBeNull();
        expect(printer).toBeInstanceOf(Printer);

        const nonExistentJobId = 999999;

        const job = await printer.getJob(nonExistentJobId);

        expect(job).toBeNull();
    });
});
