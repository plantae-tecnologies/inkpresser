const fs = require('fs');
const addon = require('./build/Release/addon');

function parseEscPosCommands(text) {
    return text.replace(/\\x([0-9A-Fa-f]{2})/g, (match, hex) =>
        String.fromCharCode(parseInt(hex, 16))
    );
}

try {
    // Leia o conteúdo do arquivo como string
    const fileContent = fs.readFileSync('./template-fast.txt', 'utf-8');

    // Converta o conteúdo do arquivo, processando os comandos ESC/POS
    const parsedContent = parseEscPosCommands(fileContent);

    const success = addon.printRaw('LX-350', parsedContent);

    console.log('Print success:', success);

} catch (error) {
    console.error('Error:', error.message);
}
