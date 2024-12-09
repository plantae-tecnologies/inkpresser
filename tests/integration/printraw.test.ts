import { describe, it, expect } from 'vitest';
import { Job, Printer, PrintManager } from '../../src/index';
import fs from 'fs';
import iconv from 'iconv-lite';

function parseEscPosCommands(text: string): string {
    return text.replace(/\\x([0-9A-Fa-f]{2})/g, (match: string, hex: string) =>
        String.fromCharCode(parseInt(hex, 16))
    );
}

describe('Integration: Printer printRaw', () => {
    it('should send an ESC/POS print job to the default printer and cancel it', async () => {
        const manager = new PrintManager();
        const printer = await manager.getDefaultPrinter();
        expect(printer).not.toBeNull();
        expect(printer).toBeInstanceOf(Printer);

        // Read the ESC/POS template file from the fixtures directory
        const filePath = './tests/fixtures/fast.txt';
        const fileContent = fs.readFileSync(filePath, 'utf-8');

        // Convert ESC/POS commands (like \x1B) to their actual byte sequences
        const parsedContent = parseEscPosCommands(fileContent);

        // Encode to CP860 to handle special characters properly (e.g., accented letters).
        const convertedContent = iconv.encode(parsedContent, 'CP860');

        // Attempt to print the document
        const documentName = 'Integration Test: printRaw';
        const printResult = await printer.printRaw(convertedContent, documentName);

        // Check if the return value is a number
        expect(printResult).toBeTypeOf('number');

        // Verify if the job was added to the queue
        const job = await printer.getJob(printResult);
        expect(job).not.toBeNull();
        expect(job).toBeInstanceOf(Job);
        expect(job?.document).toBe(documentName);

        // Attempt to cancel the job
        if (job) {
            const cancelResult = await job.cancel();
            
            // Verify the job was successfully canceled
            expect(cancelResult).toBe(true);

            // Verify that the job no longer exists or its status is canceled
            const canceledJob = await printer.getJob(printResult);
            expect(canceledJob?.status).toBe('canceled');
        }
    });
});
