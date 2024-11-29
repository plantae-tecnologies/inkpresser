import addon from './index.cjs';

try {
    const defaultPrinter = addon.getDefaultPrinterName();
    console.log('Default Printer:', defaultPrinter || 'No default printer set');
} catch (error) {
    console.error('Error:', error.message);
}