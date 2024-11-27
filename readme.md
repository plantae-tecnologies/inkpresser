# Print Node Api



## Build

```bash
npm run rebuild
```

## Notes

Notas sobre o desenvolvimento

### Node-api
A migração do NAN para a Node-API aconteceu porque esta última oferece uma abstração estável e independente da versão do Node.js, reduzindo a necessidade de reescrever ou recompilar módulos nativos a cada atualização do Node.

### Import .node
Arquivos .node são módulos nativos compilados que seguem o padrão CommonJS e não suportam o sistema ES Modules (ESM) diretamente. Como o Node.js trata ESM e CJS de forma diferente, tentar importar diretamente um .node em um ambiente ESM gera erros.

Para contornar isso, usamos um arquivo intermediário (index.cjs) que importa o .node usando CommonJS e exporta suas funcionalidades de maneira compatível com os ES Modules. Isso garante que tanto o ambiente ESM quanto o CJS consigam acessar o módulo nativo sem problemas.



### Charsets
Tive bastante dificuldade em ler um arquivo em js e fazer a impressão correta com caracteres especiais. O que acontece é que geralmente arquivos ficam salvos no pc em `utf-8` e a impressora está configurada em `pc-860`. Ao tentar enviar uma impressão em modo raw, não existe tratamento de charset, por isso a impressora não imprime corretamente.

É necessário carregar o arquivo de template e fazer conversão usando algo como a lib `iconv-lite` para transformar o texto em um `buffer` e não perder a codificação ao chegar no c++;


### Comandos ESC/P
Durante os teste notei que tem alguns padrões de comandos `ESC/P`, para ativar e desativar o negrito usamos respectivamente `\x1B\x45\x01` e `\x1B\x45\x00`, porém esses comandos não são interpretados pela nossa impressora, ela apenas interpreta `\x1B\x45` e `\x1B\x46` para fazer a mesma coisa.
Por esse motivo a antiga forma de imprimir já não funcionava mais ou funcionava em alguns clientes, pois enviamos comandos especificos para determinadas impressoras.

# Impressão windows
Durante os testes de impressão no Windows, foi necessário instalar o driver original da Epson ([link](https://ftp.epson.com/drivers/epson15229.exe)). Clientes com outras impressoras podem precisar de configurações adicionais, como instalação de drivers, algo fora do escopo do nosso código.