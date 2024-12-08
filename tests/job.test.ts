import { beforeEach, describe, it, expect, vi } from 'vitest';
import bindings from '../src/bindings.ts';
import { Printer } from '../src/printer.ts';
import { Job } from '../src/job.ts';

vi.mock('../src/bindings.ts', () => ({
    default: {
        cancelJob: vi.fn((jobId, printerName) => true)
    },
}));

describe('Job', () => {

    beforeEach(() => {
        vi.mocked(bindings.cancelJob).mockClear();
    });

    it('should initialize with the correct properties', () => {

        const printerName = 'TestPrinter';
        const printer = new Printer({ name: printerName });
        
        const jobParams = {
            id: 1,
            printer,
            document: 'TestDocument',
            status: 'completed',
            user: 'TestUser',
        };
        const job = new Job(jobParams);

        expect(job.id).toBe(jobParams.id);
        expect(job.printer).toBe(printer);
        expect(job.document).toBe(jobParams.document);
        expect(job.status).toBe(jobParams.status);
        expect(job.user).toBe(jobParams.user);
    });

    it('should cancel a job and return true if successful', async () => {

        const printerName = 'TestPrinter';
        const printer = new Printer({ name: printerName });

        const jobParams = {
            id: 1,
            printer,
            document: 'TestDocument',
            status: 'printing',
            user: 'TestUser',
        };
        const job = new Job(jobParams);

        const result = await job.cancel();

        expect(result).toBe(true);
        expect(bindings.cancelJob).toHaveBeenCalledWith(jobParams.id, printerName);
    });

    it('should cancel a job and return false if unsuccessful', async () => {

        vi.mocked(bindings.cancelJob).mockReturnValueOnce(false);

        const printerName = 'TestPrinter';
        const printer = new Printer({ name: printerName });

        const jobParams = {
            id: 2,
            printer,
            document: 'TestDocument',
            status: 'printing',
            user: 'TestUser',
        };
        const job = new Job(jobParams);

        const result = await job.cancel();

        expect(result).toBe(false);
        expect(bindings.cancelJob).toHaveBeenCalledWith(jobParams.id, printerName);
    });

    it('should handle errors gracefully when trying to cancel a job', async () => {
        vi.mocked(bindings.cancelJob).mockImplementationOnce(() => {
            throw new Error('Failed to cancel job');
        });

        const printerName = 'TestPrinter';
        const printer = new Printer({ name: printerName });

        const jobParams = {
            id: 3,
            printer,
            document: 'TestDocument',
            status: 'printing',
            user: 'TestUser',
        };
        const job = new Job(jobParams);

        await expect(job.cancel()).rejects.toThrow('Failed to cancel job');
        expect(bindings.cancelJob).toHaveBeenCalledWith(jobParams.id, printerName);
    });
});
