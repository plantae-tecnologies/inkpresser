import addon from './index.cjs';

try {
    
    const data = Buffer.from("Hello, printer!\n");
    const success = addon.printRaw(data);

    console.log('Print success:', success);

} catch (error) {
    console.error('Error:', error.message);
}
