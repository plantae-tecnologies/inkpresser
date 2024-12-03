import addon from './index.cjs';

try {
    const success = addon.cancelJob(4);
    console.log(`Cancelamento ${success ? "bem-sucedido" : "falhou"}`);
} catch (err) {
    console.error("Erro ao cancelar o job:", err);
}
