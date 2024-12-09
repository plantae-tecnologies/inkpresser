# Notas de Desenvolvimento

Este documento reúne as principais dificuldades e soluções encontradas durante o desenvolvimento do projeto, organizadas por temas para facilitar o entendimento e futuras consultas.

## Node-API
A migração do **NAN** para a **Node-API** foi realizada devido às vantagens oferecidas por esta última:
- **Estabilidade**: A Node-API fornece uma abstração estável e independente da versão do Node.js.
- **Manutenção Reduzida**: Reduz a necessidade de reescrever ou recompilar módulos nativos a cada atualização do Node.js.

## Problemas com Charsets
Ao imprimir arquivos contendo caracteres especiais, surgiram dificuldades relacionadas ao uso de diferentes charsets:
- Arquivos geralmente estão salvos em **UTF-8**.
- Muitas impressoras estão configuradas para **PC-860** ou outros charsets.

### Solução
- **Conversão de Charsets**: Antes de enviar o texto em modo raw para a impressora, é necessário convertê-lo para o charset correto. 
- Utilize alguma biblioteca como `iconv-lite` para realizar a conversão e gerar um `Buffer` com a codificação correta antes de enviar o texto ao C++.

## Comandos ESC/P
Os comandos **ESC/P** (ou **ESC/POS**) seguem diferentes padrões dependendo do modelo e fabricante da impressora. Essa variação pode causar problemas de compatibilidade ao enviar comandos que não são reconhecidos por determinadas impressoras.

### Exemplo
- O comando para ativar/desativar negrito varia entre modelos:
  - Algumas impressoras utilizam: `\x1B\x45\x01` e `\x1B\x45\x00`.
  - Outras reconhecem apenas: `\x1B\x45` e `\x1B\x46`.

### Solução
- Identifique o padrão de comandos suportado pela impressora antes de enviar os dados.
- Sempre que possível, utilize comandos amplamente compatíveis para aumentar a portabilidade do código entre diferentes modelos.

## Builds para Node.js v12
A compilação para versões antigas do Node.js, como a **v12**, exige atenção especial devido à dependência do **Python 2.7** por algumas bibliotecas nativas. Certifique-se de configurar corretamente o ambiente, conforme o sistema operacional.

### Solução

#### Windows
1. Instale o Python 2.7 a partir do site oficial ([Download Python 2.7](https://www.python.org/downloads/release/python-2718/)).
2. Configure o caminho do Python com o comando:
   ```sh
   npm config set python "C:\Python27\python.exe"
   ```

#### Linux
1. Instale o Python 2.7 utilizando o gerenciador de pacotes da sua distribuição. Exemplo no Ubuntu:
   ```sh
   sudo apt-get install python2.7
   ```
2. Configure o caminho do Python com o comando:
   ```sh
   npm config set python /usr/bin/python2.7
   ```

## Impressão no Windows
Durante os testes de impressão no Windows, foi constatado que:
- É necessário instalar o driver oficial da impressora para garantir o funcionamento correto.
- No caso das impressoras Epson, o driver pode ser obtido [aqui](https://ftp.epson.com/drivers/epson15229.exe).

### Observações
- Impressoras de outros fabricantes podem exigir configurações adicionais, como a instalação de drivers específicos.
- Essas configurações estão fora do escopo do código, sendo responsabilidade do usuário.
