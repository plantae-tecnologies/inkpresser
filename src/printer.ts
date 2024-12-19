import { JobInfo, PrinterInfo } from './types';
import bindings from './bindings';
import { Job } from './job';

export class Printer {
    name: string;
    isDefault: boolean | null;

    constructor(params: PrinterInfo) {
        this.name = params.name;
        this.isDefault = params.isDefault ?? null;
    }

    async printRaw(data: Uint8Array, documentName: string): Promise<number> {
        return bindings.printRaw(data, documentName, this.name);
    }

    async getJobs(): Promise<Job[]> {
        const jobsInfo: JobInfo[] = bindings.getJobs(this.name);
        return jobsInfo.map(jobInfo => new Job({ ...jobInfo, printer: this }));
    }

    async getJob(jobId: number): Promise<Job | null> {
        const jobInfo: JobInfo = bindings.getJob(jobId, this.name);
        if (!jobInfo) {
            return null;
        }

        return new Job({ ...jobInfo, printer: this });
    }
}
