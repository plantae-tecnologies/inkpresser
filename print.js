const addon = require('./build/Release/addon');

try {

    const success = addon.printRaw('LX-350', 'Hello, printer!');
    console.log('Print success:', success);

} catch (error) {
    console.error('Error:', error.message);
}