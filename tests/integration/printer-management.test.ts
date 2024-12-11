import { describe, it, expect } from 'vitest';
import { Job, Printer, PrintManager } from '../../src/index';
import fs from 'fs';
import iconv from 'iconv-lite';

function parseEscPosCommands(text: string): string {
    return text.replace(/\\x([0-9A-Fa-f]{2})/g, (_, hex: string) =>
        String.fromCharCode(parseInt(hex, 16))
    );
}

describe('Integration: Print System', () => {
    it('should list all printers', async () => {
        const manager = new PrintManager();
        const printers = await manager.getPrinters();

        expect(printers).toBeInstanceOf(Array);
        printers.forEach((printer) => {
            expect(printer).toBeInstanceOf(Printer);
            expect(printer).toHaveProperty('name');
        });
    });

    it('should retrieve the default printer', async () => {
        const manager = new PrintManager();
        const printer = await manager.getDefaultPrinter();

        expect(printer).not.toBeNull();
        expect(printer).toBeInstanceOf(Printer);
    });

    it('should send an ESC/POS print job, verify it is queued, and cancel it', async () => {
        const manager = new PrintManager();
        const printer = await manager.getDefaultPrinter();
        expect(printer).not.toBeNull();
    
        // Prepare the print job content
        const filePath = './tests/fixtures/fast.txt';
        const fileContent = fs.readFileSync(filePath, 'utf-8');
        const parsedContent = parseEscPosCommands(fileContent);
        const convertedContent = iconv.encode(parsedContent, 'CP860');
    
        // Send the print job
        const documentName = `Test Document ${Date.now()}`;
        const jobId = await printer.printRaw(convertedContent, documentName);
        expect(jobId).toBeTypeOf('number');
    
        // Verify the job is queued
        const job = await printer.getJob(jobId);
        expect(job).not.toBeNull();
        expect(job).toBeInstanceOf(Job);
        expect(job?.document).toBe(documentName);
    
        // Cancel the print job
        if (job) {
            const cancelResult = await job.cancel();
            expect(cancelResult).toBe(true);
    
            // Verify the job has been removed from the queue
            const canceledJob = await printer.getJob(jobId);
            expect(canceledJob).toBeNull();
        }
    });    

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
