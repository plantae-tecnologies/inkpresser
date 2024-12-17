import { JobInfo, PrinterInfo } from './types';
import bindings from './bindings';
import { Job } from './job';

export class Printer {
    name: string;

    constructor(params: PrinterInfo) {
        this.name = params.name;
    }

    async print(data: Uint8Array, documentName: string): Promise<number> {
        return bindings.print(data, documentName, this.name);
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
