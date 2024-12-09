import { Printer } from './printer';

export type PrinterInfo = {
    name: string;
};

export interface JobInfo {
    id: number;
    printer: Printer;
    document: string;
    status: string;
    user: string;
}
