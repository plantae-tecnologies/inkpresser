import addon from './index.cjs';
import fs from 'fs';
import iconv from 'iconv-lite';

// Função para interpretar os comandos ESC/POS (\xXX)
function parseEscPosCommands(text) {
    return text.replace(/\\x([0-9A-Fa-f]{2})/g, (match, hex) =>
        String.fromCharCode(parseInt(hex, 16))
    );
}

try {
    // Leia o conteúdo do arquivo como string
    const fileContent = fs.readFileSync('./storage/fast.tpl', 'utf-8');

    // Converta os comandos ESC/POS
    const parsedContent = parseEscPosCommands(fileContent);

    // Converta o conteúdo para CP860
    const convertedContent = iconv.encode(parsedContent, 'CP860');

    const success = addon.printRaw(convertedContent, 'Teste', 'L3150');

    console.log('Print success:', success);

} catch (error) {
    console.error('Error:', error.message);
}
