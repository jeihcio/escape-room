# Escape Room
Convertendo o projeto para o Arduino Nano

## Instruções
Seguir as instruções primárias da branch [main](https://github.com/jeihcio/escape-room/tree/main)

### Configurações da IDE

__Ferramentas__ > 
  - Placa: Arduino Nano
  - Processador: ATMega328p (Old Bootloader)
  - Porta: COM4
  
  ## Hardware
  
  Para essa versão do projeto estamos usando uma versão diferente de teclado que está no [Tinkercad](https://www.tinkercad.com/things/5OiIE5SMJbv-escape-room). A mudança é mínima, apenas alteramos as portas de conexão. 
  
 ```
 byte rowPins[numRows] = {5, 4, 3, 2};
 byte colPins[numCols] = {9, 8, 7, 6};
 ```

Caso queira usar outro teclado, basta alterar nesse trecho de código, ou simplesmente mudar suas conexões no circuito. 
