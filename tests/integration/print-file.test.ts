import { describe, expect, it } from 'vitest';
import { Printer, PrintManager } from '../../src/index';
import * as fs from 'fs';
import * as path from 'path';

describe('Integration: Print Files from Fixtures', () => {
    it('should send a PDF print job and verify the job ID', async () => {
        const manager = new PrintManager();
        const printer = await manager.getDefaultPrinter();

        expect(printer).not.toBeNull();
        expect(printer).toBeInstanceOf(Printer);

        // Carregar o PDF de teste
        const filePath = path.resolve('./tests/fixtures/test-pdf.pdf');
        const fileData = fs.readFileSync(filePath);

        console.log(fileData);

        // Nome do documento para impressão
        const documentName = `Test PDF Document ${Date.now()}`;

        // Enviar o job usando o método print
        const jobId = await printer.print(fileData, documentName);
        expect(jobId).toBeTypeOf('number');
        console.log(`Print job submitted successfully. Job ID: ${jobId}`);

        // (Opcional) Verificar se o job foi adicionado à fila
        const job = await printer.getJob(jobId);
        expect(job).not.toBeNull();
        expect(job?.document).toBe(documentName);
    });

});
