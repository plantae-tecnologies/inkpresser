import { Printer } from './printer.ts';
import { JobInfo } from './types.ts';
import bindings from './bindings.ts';

export class Job {
    id: number;
    printer: Printer;
    document: string;
    status: string;
    user: string;

    constructor(params: JobInfo) {
        this.id = params.id;
        this.printer = params.printer;
        this.document = params.document;
        this.status = params.status;
        this.user = params.user;
    }

    async cancel(): Promise<boolean> {
        return bindings.cancelJob(this.id, this.printer.name);
    }
}
