import addon from './index.cjs';

try {
    
    const data = Buffer.from('Hello, printer!');
    const success = addon.printRaw(data, 'LX-350');

    console.log('Print success:', success);

} catch (error) {
    console.error('Error:', error.message);
}
