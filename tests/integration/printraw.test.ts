import { describe, it, expect } from 'vitest';
import { PrintManager } from '../../src/index.ts';
import fs from 'fs';
import iconv from 'iconv-lite';

function parseEscPosCommands(text: string): string {
    return text.replace(/\\x([0-9A-Fa-f]{2})/g, (match: string, hex: string) =>
        String.fromCharCode(parseInt(hex, 16))
    );
}

describe('Integration: Printer printRaw', () => {
    it('should send an ESC/POS print job to the default printer', async () => {
        const manager = new PrintManager();
        const printer = await manager.getDefaultPrinter();

        expect(printer).not.toBeNull();

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

        // Check if the return value is a number (possibly a job ID)
        expect(printResult).toBeTypeOf('number');

        // Verify if the job was added to the queue
        const job = await printer.getJob(printResult);

        expect(job).toBeDefined();
        expect(job?.document).toBe(documentName);
    });
});
