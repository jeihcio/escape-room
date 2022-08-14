# Escape Room
Projeto desenvolvido para Arduino Uno

## Instruções
Instalar a biblioteca "keypad.zip" que está na pasta [bibliotecas](/Bibliotecas)

### Instalação
Sketch > Incluir Biblioteca > Adicionar biblioteca .ZIP

#### Atenção
Na configuração do LCD pode ser que seja necessário alterar o endereço. No código está:

```LiquidCrystal_I2C lcd(0x27, 16, 2);```

Mas o 0x27 pode variarar para 0x26, 0x20 etc.

## Tinkercad
Tinkercad é um simulador online para Arduino; no link abaixo contém todo o projeto rodando e funcionando. Além de todo o esquema de montagem dos módulos do Arduino Uno e o código fonte. <br/><br/>
[Visualizar o projeto](https://www.tinkercad.com/things/5OiIE5SMJbv-escape-room)
