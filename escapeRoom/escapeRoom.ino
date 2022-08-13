#include<stdlib.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

//---------------------------------------------------------------------------------------------
// Buzzer
//---------------------------------------------------------------------------------------------

#define buzzpin 10

//---------------------------------------------------------------------------------------------
// Lâmpada RGB
//---------------------------------------------------------------------------------------------

#define ledRGB_R 11
#define ledRGB_G 13
#define ledRGB_B 12

//---------------------------------------------------------------------------------------------
// Teclado
//---------------------------------------------------------------------------------------------

const byte numRows = 4;
const byte numCols = 4;

char keymap[numRows][numCols] =
{
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[numRows] = {9, 8, 7, 6};
byte colPins[numCols] = {5, 4, 3, 2};

Keypad myKeypad = Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);

//---------------------------------------------------------------------------------------------
// LCD
//---------------------------------------------------------------------------------------------

LiquidCrystal_I2C lcd(0x26, 16, 2);

void iniciarLCD() {
  lcd.init();
  lcd.clear();
  lcd.backlight();
  lcd.home();
}

//---------------------------------------------------------------------------------------------
//Programa principal
//---------------------------------------------------------------------------------------------

struct perguntaAleatoria {
  String pergunta;
  int resposta;
};

struct variaveisGlobais {
  char opcaoJogo;
  struct perguntaAleatoria perguntaDesafioA;
  String senhaCorretaDesafioB;
  String teclasDigitadas;
  bool exibirLog;
};

struct variaveisGlobais GLOBAL;

// DESAFIO A
//---------------------------------------------------------------------------------------------

struct perguntaAleatoria gerarPerguntaAleatoria() {
  perguntaAleatoria pergunta;

  int primeiroNumeroAleatorio, segundoNumeroAleatorio;
  int operacao;
  String caracterOperacao;

  operacao = random(0, 2);
  primeiroNumeroAleatorio = random(0, 10);
  segundoNumeroAleatorio = random(0, 10);

  switch (operacao) {
    case 0: //adição
      pergunta.resposta = primeiroNumeroAleatorio + segundoNumeroAleatorio;
      caracterOperacao = " + ";
      break;

    case 1: //multiplicação
      pergunta.resposta = primeiroNumeroAleatorio * segundoNumeroAleatorio;
      caracterOperacao = " * ";
      break;
  }

  pergunta.pergunta = (String)primeiroNumeroAleatorio + caracterOperacao +
                      (String)segundoNumeroAleatorio  + " = ";

  return pergunta;
}

struct perguntaAleatoria gerarDesafioA() {
  lcd.clear();
  lcd.print("Responda:");
  lcd.setCursor(0, 1);

  perguntaAleatoria resultado = gerarPerguntaAleatoria();
  return resultado;
}

void gerarEExibirNovaPerguntaDesafioA() {
  GLOBAL.perguntaDesafioA = gerarDesafioA();
  lcd.print(GLOBAL.perguntaDesafioA.pergunta);
}

bool resolverDesafioA(perguntaAleatoria perguntaDesafioA, String keypressed) {
  int numero = keypressed.toInt();
  return (perguntaDesafioA.resposta == numero);
}

// DESAFIO B
//---------------------------------------------------------------------------------------------

void defineSenhaCorretaDesafioB() {
  bool definiuSenha;

  lcd.print("Define senha:");
  lcd.setCursor(0, 1);

  definiuSenha = false;
  while (!definiuSenha) {
    char keypressed = myKeypad.getKey();

    if (keypressed != NO_KEY) 
    {
      acionarSomELuzDaTeclaDigitada();      
      if (keypressed == '#')
      {
        definiuSenha = true;
        lcd.clear();
      }
      else
      {
        GLOBAL.senhaCorretaDesafioB += (String)keypressed;
        lcd.print(keypressed);
      }
    }
  }
}

void gerandoDesafioB() {
  lcd.clear();
  lcd.print("Senha:");
  lcd.setCursor(0, 1);
}

void gerarEExibirDesafioB() {
  gerandoDesafioB();
}

bool resolverDesafioB(String senhaCorretaDesafioB, String keypressed) {
  return (senhaCorretaDesafioB == keypressed);
}

// Led RGB
//---------------------------------------------------------------------------------------------

const int amarelo = 0;
const int verde = 1;
const int vermelho = 2;

void configurarLampada() {
  pinMode(ledRGB_R, OUTPUT);
  pinMode(ledRGB_G, OUTPUT);
  pinMode(ledRGB_B, OUTPUT);
}

void ligarLampada(int cor) {
  int red, green, blue;

  switch (cor) {
    case 0:
      red = 255;
      green = 255;
      blue = 0;
      break;
    case 1:
      red = 0;
      green = 255;
      blue = 0;
      break;
    case 2:
      red = 255;
      green = 0;
      blue = 0;
      break;
  }

  digitalWrite(ledRGB_R, red);
  digitalWrite(ledRGB_G, green);
  digitalWrite(ledRGB_B, blue);
}

void desligarLampada() {
  digitalWrite(ledRGB_R, 0);
  digitalWrite(ledRGB_G, 0);
  digitalWrite(ledRGB_B, 0);
}

void ligarLampadaTemporariamente(int cor, int tempo) {
  ligarLampada(cor);
  delay(tempo);
  desligarLampada();
}

// Buzzer
//---------------------------------------------------------------------------------------------

void acionarSom(int nota, int duracao) {
  tone(buzzpin, nota, duracao);
}

void somErro() {
  int duracao = 500;
  int nota = 494; // b4

  for (int x = 0; x <= 6; x++) {
    acionarSom(nota, duracao);
    ligarLampadaTemporariamente(vermelho, duracao);
    delay(duracao);
  }
}

void somAcerto() {
  int duracao = 250;
  int nota = 494; // b4

  for (int x = 0; x < 3; x++) {
    acionarSom(nota, duracao);
    ligarLampadaTemporariamente(verde, duracao);
    delay(duracao);
  }

  duracao = 1500;
  ligarLampadaTemporariamente(verde, duracao);
  acionarSom(nota, duracao);
}

// Utilitários
//---------------------------------------------------------------------------------------------

void log(String frase) {
  if (GLOBAL.exibirLog) {
    Serial.println(frase);
  }
}

void exibirResultado(bool resultado) {
  lcd.clear();
  delay(700);

  if (resultado) {
    lcd.print("Acertou !");
    somAcerto();
  } else {
    lcd.print("Errou !");
    somErro();
  }
}

void acionarSomELuzDaTeclaDigitada() {
  int duracao = 15;
  int nota = 494; // b4

  acionarSom(nota, duracao);
  ligarLampadaTemporariamente(amarelo, duracao);
}

void barraDeProgresso(String texto, int tempoDelay) {
  int tamanhoDisplay = 16;
  int retangulo = 1;

  lcd.print(texto);
  lcd.setCursor(0, 1);

  for (int i = 0; i < tamanhoDisplay; i++) {
    lcd.write(retangulo);
    delay(tempoDelay);
  }
}

// Carregamento inicial
//---------------------------------------------------------------------------------------------

void inicializarVariaveisGlobais() {
  GLOBAL.opcaoJogo = 0x00;
  GLOBAL.perguntaDesafioA.pergunta = "";
  GLOBAL.perguntaDesafioA.resposta = 0;
  GLOBAL.senhaCorretaDesafioB = "";
  GLOBAL.exibirLog = true;
}

void carregandoJogo() {
  barraDeProgresso("Carregando...", 400);
  lcd.clear();
}

void setOpcoesDeJogo() {
  lcd.print("Opcao de jogo:");
  lcd.setCursor(0, 1);
  lcd.print("A,B: ");

  bool escolheuOpcao = false;
  while (!escolheuOpcao) {
    char teclaDigitada = myKeypad.getKey();

    if (teclaDigitada != NO_KEY) {
      acionarSomELuzDaTeclaDigitada();

      if ((teclaDigitada == 'A') || (teclaDigitada == 'B')) {
        escolheuOpcao = true;
        GLOBAL.opcaoJogo = teclaDigitada;

        lcd.print(teclaDigitada);
        delay(1000);
        lcd.clear();

        if (teclaDigitada == 'A') {
          gerarEExibirNovaPerguntaDesafioA();
        }
        else if (teclaDigitada == 'B') {
          defineSenhaCorretaDesafioB();
          barraDeProgresso("Iniciando jogo", 100);
          gerarEExibirDesafioB();
        }
      }
    }
  }
}

// Métodos padrões do Arduino
//---------------------------------------------------------------------------------------------

void setup()
{
  Serial.begin(9600);
  inicializarVariaveisGlobais();

  log("Iniciando o setup");
  iniciarLCD();
  pinMode(buzzpin, OUTPUT);
  configurarLampada();
  desligarLampada();

  carregandoJogo();
  setOpcoesDeJogo();
  log("Terminou o setup");
}

void loop() {
  char keypressed = myKeypad.getKey();

  if (keypressed != NO_KEY)
  {
    acionarSomELuzDaTeclaDigitada();
    bool resposta;

    if (keypressed == '#')
    {
      if (GLOBAL.opcaoJogo == 'A')
      {
        delay(800);
        resposta = resolverDesafioA(GLOBAL.perguntaDesafioA, GLOBAL.teclasDigitadas);
        exibirResultado(resposta);

        delay(1500);
        gerarEExibirNovaPerguntaDesafioA();
      }
      else if (GLOBAL.opcaoJogo == 'B')
      {
        delay(800);
        resposta = resolverDesafioB(GLOBAL.senhaCorretaDesafioB, GLOBAL.teclasDigitadas);
        exibirResultado(resposta);

        delay(1500);
        gerarEExibirDesafioB();
      }

      GLOBAL.teclasDigitadas = "";
    }
    else
    {
      lcd.print(keypressed);
      GLOBAL.teclasDigitadas += (String)keypressed;
    }
  }
}
