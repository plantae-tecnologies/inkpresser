const addon = require('./build/Release/addon');
const fs = require('node:fs');

function listPrinters() {
    return addon.listPrinters();
}

function printRaw(printerName, rawData) {
    return addon.printRaw(printerName, rawData);
}


console.log("Impressoras disponíveis:", listPrinters());

const success = printRaw("LX-350", fs.readFileSync('./print.txt', 'utf8'));


console.log("Impressão:", success ? "Sucesso" : "Falhou");