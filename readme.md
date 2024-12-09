# InkPresser
A Node.js library for handling printers and print jobs, with easy integration across modern Node.js and Electron versions.

## Installation
InkPresser uses `node-gyp` for building native bindings during installation. Make sure your environment has the necessary dependencies, such as Python and a compatible C++ compiler (e.g., GCC on Linux, Build Tools for Visual Studio on Windows). 

Install it via GitHub:
```bash
npm i github:plantae-tecnologies/inkpresser
```

## Usage

Here’s how you can use:

### List Available Printers
```typescript
import { PrintManager } from 'inkpresser';
const manager = new PrintManager();

const printers = await manager.getPrinters();
// output: [{ name: "Printer1" }, { name: "Printer2" }]
```

### Get the Default Printer
```typescript
import { PrintManager } from 'inkpresser';
const manager = new PrintManager();

const defaultPrinter = await manager.getDefaultPrinter();
// output: { name: "DefaultPrinter" }
```

### Print a Document
```typescript
import { PrintManager } from 'inkpresser';
const manager = new PrintManager();

const printers = await manager.getPrinters();
const printer = printers[0];

const document = Buffer.from('Hello, printer!');
const jobId = await printer.printRaw(document, 'SampleDocument');
// output: 200
```

### Manage Print Jobs
```typescript
import { PrintManager } from 'inkpresser';
const manager = new PrintManager();

const printers = await manager.getPrinters();
const printer = printers[0];

const jobs = await printer.getJobs();
// output: [
//   { id: 1, printer: { name: "Printer1" }, document: "Document1", status: "queued", user: "user1" },
//   { id: 2, printer: { name: "Printer1" }, document: "Document2", status: "printing", user: "user1" }
// ]

const cancelResult = await jobs[0]?.cancel();
// output: true
```

### Get a Job by ID
```typescript
import { PrintManager } from 'inkpresser';
const manager = new PrintManager();

const printers = await manager.getPrinters();
const printer = printers[0];

const job = await printer.getJob(1);
// output: { id: 1, printer: { name: "Printer1" }, document: "Document1", status: "queued", user: "user1" }
```