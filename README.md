# Escape Room
Projeto desenvolvido para Arduino Uno

![Esquema](/EsquemaEletronico.png)

## Instruções
Instalar a biblioteca "keypad.zip" que está na pasta [bibliotecas](/Bibliotecas)

### Instalação
Sketch > Incluir Biblioteca > Adicionar biblioteca .ZIP

#### Atenção
Na configuração do LCD pode ser que seja necessário alterar o endereço. No código está:

```LiquidCrystal_I2C lcd(0x27, 16, 2);```

Mas o 0x27 pode variarar para 0x26, 0x20 etc.

### Componentes necessários

- 1 Arduino Uno R3
- 1 Keypad 4x4
- 1 Piezo
- 1 LED RGB
- 3 100 Ω Resistor
- 1 LCD 16 x 2 (Com I2C)
- 1 250 kΩ Potentiometer

# Modelagem 3d do case do projeto

![Modelagem](/case.png)

### Lista de parafusos

| QTD | Circuferência | Comprimento |
| --: | --- | --- |
| 4 | 4mm | 50mm |
| 1 | 2mm | 01mm |

## Tinkercad
Tinkercad é um simulador online para Arduino; no link abaixo contém todo o projeto rodando e funcionando. Além de todo o esquema de montagem dos módulos do Arduino Uno, lista de componentes utilizados e o código fonte. <br/>
[Visualizar o circuito do projeto](https://www.tinkercad.com/things/5OiIE5SMJbv-escape-room)

Além do simulador o Tinkercad contém uma ferramenta para modelagem 3d, no link abaixo contém a modelagem da caixa do projeto.<br/>
[Visualizar a modelagem do case](https://www.tinkercad.com/things/iyig3Z6EOqQ-case-para-escape-room)
