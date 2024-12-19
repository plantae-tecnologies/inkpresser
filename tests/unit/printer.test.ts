import { beforeEach, describe, it, expect, vi } from 'vitest';
import bindings from '../../src/bindings';
import { Printer } from '../../src/printer';
import { Job } from '../../src/job';

vi.mock('../../src/bindings', () => ({
    default: {
        printRaw: vi.fn((data, documentName, printerName) => 1),
        getJobs: vi.fn((printerName) => []),
        getJob: vi.fn((jobId, printerName) => null)
    },
}));

describe('Printer', () => {

    beforeEach(() => {
        vi.mocked(bindings.printRaw).mockClear();
        vi.mocked(bindings.getJobs).mockClear();
        vi.mocked(bindings.getJob).mockClear();
    });

    it('should initialize with attributes', () => {
        const printerName = 'TestPrinter';
        const printer = new Printer({ name: printerName, isDefault: true });

        expect(printer.name).toBe(printerName);
        expect(printer.isDefault).toBe(true);
    });

    it('should allow instantiation without specifying isDefault attribute', () => {
        const printerName = 'TestPrinter';
        const printer = new Printer({ name: printerName });

        expect(printer.name).toBe(printerName);
        expect(printer.isDefault).toBeNull();
    });

    it('should send a print job and return the job ID', async () => {
        const printerName = 'Printer';
        const jobData = new Uint8Array([1, 2, 3, 4]);
        const jobName = 'Print test';

        const printer = new Printer({ name: printerName });
        const jobId = await printer.printRaw(jobData, jobName);

        expect(jobId).toBeTypeOf('number');
        expect(bindings.printRaw).toHaveBeenCalledWith(jobData, jobName, printerName);
    });

    it('should return all jobs of the printer as instances of "Job"', async () => {

        const printerName = 'Printer';

        vi.mocked(bindings.getJobs).mockReturnValueOnce([
            { id: 1, printer: printerName, document: 'Doc1', status: 'completed', user: 'User1' },
            { id: 2, printer: printerName, document: 'Doc2', status: 'printing', user: 'User1' },
        ]);

        const printer = new Printer({ name: printerName });
        const jobs = await printer.getJobs();

        expect(jobs).toBeInstanceOf(Array);
        expect(jobs[0]).toBeInstanceOf(Job);
        expect(jobs[1]).toBeInstanceOf(Job);
        expect(bindings.getJobs).toHaveBeenCalledWith(printerName);
    });

    it('should return an empty array if there are no jobs', async () => {
        const printerName = 'Printer';
        const printer = new Printer({ name: printerName });
    
        const jobs = await printer.getJobs();
    
        expect(jobs).toEqual([]);
        expect(bindings.getJobs).toHaveBeenCalledWith(printerName);
    });

    it('should return a specific job by ID', async () => {

        const printerName = 'Printer';

        vi.mocked(bindings.getJob).mockReturnValueOnce({ id: 1, printer: printerName, document: 'Doc1', status: 'completed', user: 'User1' });

        const printer = new Printer({ name: printerName });

        const jobId = 1;
        const job = await printer.getJob(jobId);

        expect(job).toBeInstanceOf(Job);
        expect(job?.id).toBe(jobId);
        expect(bindings.getJob).toHaveBeenCalledWith(jobId, printerName);
    });

    it('should return null if a specific job does not exist', async () => {
        const printerName = 'Printer';
        const printer = new Printer({ name: printerName });

        const jobId = 999;
        const job = await printer.getJob(jobId);

        expect(job).toBeNull();
        expect(bindings.getJob).toHaveBeenCalledWith(jobId, printerName);
    });
});