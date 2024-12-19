# 🖨️ InkPresser

InkPresser is a library for managing printers and handling print jobs. It provides integration with Node.js and Electron applications, supporting multiple operating systems and compatible with a wide range of Node.js and Electron versions.

## 🚀 Installation

We use `node-gyp` for building native bindings during installation. Ensure your environment has the necessary dependencies, such as Python and a compatible C++ compiler (e.g., GCC on Linux, Build Tools for Visual Studio on Windows).

For detailed instructions, refer to the [node-gyp documentation](https://github.com/nodejs/node-gyp).

```bash
npm install @plantae-tech/inkpresser
```

## 📖 Usage

Here’s how you can use InkPresser in your project:

### List Available Printers
```typescript
import { PrintManager } from '@plantae-tech/inkpresser';
const manager = new PrintManager();

const printers = await manager.getPrinters();
// output: [
//   { name: "Printer1", isDefault: false },
//   { name: "Printer2", isDefault: true }
// ]
```

### Get the Default Printer
```typescript
import { PrintManager } from '@plantae-tech/inkpresser';
const manager = new PrintManager();

const defaultPrinter = await manager.getDefaultPrinter();
// output: { name: "Printer2", isDefault: true }
```

### Print a Document
```typescript
import { PrintManager } from '@plantae-tech/inkpresser';
const manager = new PrintManager();
const printer = await manager.getDefaultPrinter();

const document = Buffer.from('Hello, printer!');
const jobId = await printer.printRaw(document, 'SampleDocument');
// output: 200
```

### Manage Print Jobs
```typescript
import { PrintManager } from '@plantae-tech/inkpresser';
const manager = new PrintManager();
const printer = await manager.getDefaultPrinter();

const jobs = await printer.getJobs();
// output: [
//   { id: 1, printer: { name: "Printer1", isDefault: true }, document: "Document1", status: "queued", user: "user1" },
//   { id: 2, printer: { name: "Printer1", isDefault: true }, document: "Document2", status: "printing", user: "user1" }
// ]

const cancelResult = await jobs[0]?.cancel();
// output: true
```

### Get a Job by ID
```typescript
import { PrintManager } from '@plantae-tech/inkpresser';
const manager = new PrintManager();
const printer = await manager.getDefaultPrinter();

const job = await printer.getJob(1);
// output: { id: 1, printer: { name: "Printer2", isDefault: true }, document: "Document1", status: "queued", user: "user1" }
```