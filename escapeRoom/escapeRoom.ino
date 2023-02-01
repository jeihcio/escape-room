#include <stdlib.h>
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
        {'*', '0', '#', 'D'}};

byte rowPins[numRows] = {5, 4, 3, 2};
byte colPins[numCols] = {9, 8, 7, 6};

Keypad myKeypad = Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);

//---------------------------------------------------------------------------------------------
// LCD
//---------------------------------------------------------------------------------------------

LiquidCrystal_I2C lcd(0x27, 16, 2); // dependendo o endereço pode variarar para 0x26, 0x20 etc.

void iniciarLCD()
{
  lcd.init();
  lcd.clear();
  lcd.backlight();
  lcd.home();
}

//---------------------------------------------------------------------------------------------
// Programa principal
//---------------------------------------------------------------------------------------------

struct perguntaAleatoria
{
  String pergunta;
  int resposta;
};

struct tempoLimite
{
  int minutos;
  int segundos;
  unsigned long tempoTotalEmMilissegundos;
  unsigned long inicioDoContador;
  unsigned long tempoFinal;
};

struct variaveisGlobais
{
  char opcaoJogo;
  struct perguntaAleatoria perguntaDesafioA;
  String senhaCorretaDesafioB;
  String senhaCorretaDesafioC;
  String teclasDigitadas;
  struct tempoLimite tempoLimiteDesafioC;
  bool exibirLog;
};

struct variaveisGlobais GLOBAL;

void limparResposta()
{
  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.setCursor(0, 1);

  GLOBAL.teclasDigitadas = "";
}

// MÉTODOS EM COMUM DOS DESAFIOS
//---------------------------------------------------------------------------------------------

bool verificarSeSenhaDigitadaECorreta(String senhaCorreta, String keypressed)
{
  return (senhaCorreta == keypressed);
}

// DESAFIO A
//---------------------------------------------------------------------------------------------

struct perguntaAleatoria gerarPerguntaAleatoria()
{
  perguntaAleatoria pergunta;

  int primeiroNumeroAleatorio, segundoNumeroAleatorio;
  int operacao;
  String caracterOperacao;

  operacao = random(0, 2);
  primeiroNumeroAleatorio = random(0, 10);
  segundoNumeroAleatorio = random(0, 10);

  switch (operacao)
  {
  case 0: // adição
    pergunta.resposta = primeiroNumeroAleatorio + segundoNumeroAleatorio;
    caracterOperacao = " + ";
    break;

  case 1: // multiplicação
    pergunta.resposta = primeiroNumeroAleatorio * segundoNumeroAleatorio;
    caracterOperacao = " * ";
    break;
  }

  pergunta.pergunta = (String)primeiroNumeroAleatorio + caracterOperacao +
                      (String)segundoNumeroAleatorio + " = ";

  return pergunta;
}

struct perguntaAleatoria gerarDesafioA()
{
  lcd.clear();
  lcd.print("Responda:");
  lcd.setCursor(0, 1);

  perguntaAleatoria resultado = gerarPerguntaAleatoria();
  return resultado;
}

void gerarEExibirNovaPerguntaDesafioA()
{
  GLOBAL.perguntaDesafioA = gerarDesafioA();
  lcd.print(GLOBAL.perguntaDesafioA.pergunta);
}

bool resolverDesafioA(perguntaAleatoria perguntaDesafioA, String keypressed)
{
  int numero = keypressed.toInt();
  return (perguntaDesafioA.resposta == numero);
}

// DESAFIO B
//---------------------------------------------------------------------------------------------

void defineSenhaCorretaDesafioB()
{
  bool definiuSenha;

  lcd.print("Define senha:");
  lcd.setCursor(0, 1);

  definiuSenha = false;
  while (!definiuSenha)
  {
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

void gerandoDesafioB()
{
  lcd.clear();
  lcd.print("Senha:");
  lcd.setCursor(0, 1);
}

void gerarEExibirDesafioB()
{
  gerandoDesafioB();
}

// DESAFIO C
//---------------------------------------------------------------------------------------------

long calcularTempoTotalEmMilissegundos(int minutos, int segundos)
{
  unsigned long segundosEmUmMinuto = 60;
  unsigned long milissegundosEmUmSegundo = 1000;

  unsigned long valoresEmSegundos = (minutos * segundosEmUmMinuto + segundos);
  unsigned long valoresEmMilissegundos = valoresEmSegundos * milissegundosEmUmSegundo;

  return valoresEmMilissegundos;
}

String getTempoLimiteDoDesafioC(String textoInferiorLCD)
{
  bool definiuTempo;
  String resultado;

  lcd.print("Define o tempo:");
  lcd.setCursor(0, 1);
  lcd.print(textoInferiorLCD);

  definiuTempo = false;
  while (!definiuTempo)
  {
    char keypressed = myKeypad.getKey();

    if (keypressed != NO_KEY)
    {
      acionarSomELuzDaTeclaDigitada();
      if (keypressed == '#')
      {
        definiuTempo = true;
        lcd.clear();
      }
      else
      {
        resultado += (String)keypressed;
        lcd.print(keypressed);
      }
    }
  }

  return resultado;
}

void defineTempoLimiteDoDesafioC()
{
  String minutos = getTempoLimiteDoDesafioC("Minutos: ");
  String segundos = getTempoLimiteDoDesafioC("Segundos: ");

  GLOBAL.tempoLimiteDesafioC.minutos = atoi(minutos.c_str());
  GLOBAL.tempoLimiteDesafioC.segundos = atoi(segundos.c_str());

  long tempoTotalEmMilissegundos = calcularTempoTotalEmMilissegundos(GLOBAL.tempoLimiteDesafioC.minutos, GLOBAL.tempoLimiteDesafioC.segundos);
  GLOBAL.tempoLimiteDesafioC.tempoTotalEmMilissegundos = tempoTotalEmMilissegundos;
}

void defineSenhaCorretaDesafioC()
{
  defineSenhaCorretaDesafioB();
  GLOBAL.senhaCorretaDesafioC = GLOBAL.senhaCorretaDesafioB;
}

void gerarEExibirDesafioC()
{
  gerandoDesafioB();
}

bool resolverDesafioC(String senhaCorretaDesafioc, String keypressed)
{
  return verificarSeSenhaDigitadaECorreta(senhaCorretaDesafioc, keypressed);
}

void exibirMensagemTempoExcedido()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Excedeu o limite");
  lcd.setCursor(0, 1);
  lcd.print("De tempo !");
}

// Led RGB
//---------------------------------------------------------------------------------------------

const int amarelo = 0;
const int verde = 1;
const int vermelho = 2;

void configurarLampada()
{
  pinMode(ledRGB_R, OUTPUT);
  pinMode(ledRGB_G, OUTPUT);
  pinMode(ledRGB_B, OUTPUT);
}

void ligarLampada(int cor)
{
  int red, green, blue;

  switch (cor)
  {
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

void desligarLampada()
{
  digitalWrite(ledRGB_R, 0);
  digitalWrite(ledRGB_G, 0);
  digitalWrite(ledRGB_B, 0);
}

void ligarLampadaTemporariamente(int cor, int tempo)
{
  ligarLampada(cor);
  delay(tempo);
  desligarLampada();
}

// Buzzer
//---------------------------------------------------------------------------------------------

void acionarSom(int nota, int duracao)
{
  tone(buzzpin, nota, duracao);
}

void somErro()
{
  int duracao = 500;
  int nota = 494; // b4

  for (int x = 0; x <= 6; x++)
  {
    acionarSom(nota, duracao);
    ligarLampadaTemporariamente(vermelho, duracao);
    delay(duracao);
  }
}

void somAcerto()
{
  int duracao = 250;
  int nota = 494; // b4

  for (int x = 0; x < 3; x++)
  {
    acionarSom(nota, duracao);
    ligarLampadaTemporariamente(verde, duracao);
    delay(duracao);
  }

  duracao = 1500;
  ligarLampada(verde);
  acionarSom(nota, duracao);
  delay(duracao);
  desligarLampada();
}

// Utilitários
//---------------------------------------------------------------------------------------------

void log(String frase)
{
  if (GLOBAL.exibirLog)
  {
    Serial.println(frase);
  }
}

void log(String frase, bool espacamentoNoInicio)
{
  String linha = "   " + frase;
  log(linha);
}

void exibirResultado(bool resultado)
{
  lcd.clear();
  delay(100);
  barraDeProgresso("Verificando...", 300);
  lcd.clear();

  if (resultado)
  {
    lcd.print("Acertou !");
    somAcerto();
  }
  else
  {
    lcd.print("Errou !");
    somErro();
  }
}

void acionarSomELuzDaTeclaDigitada()
{
  int duracao = 15;
  int nota = 494; // b4

  acionarSom(nota, duracao);
  ligarLampadaTemporariamente(amarelo, duracao);
}

void barraDeProgresso(String texto, int tempoDelay)
{
  int tamanhoDisplay = 16;
  int retangulo = 1;

  lcd.print(texto);
  lcd.setCursor(0, 1);

  for (int i = 0; i < tamanhoDisplay; i++)
  {
    lcd.write(retangulo);
    delay(tempoDelay);
  }
}

void travarSistema()
{
  while (true)
  {
    delay(1);
  }
}

// Carregamento inicial
//---------------------------------------------------------------------------------------------

void inicializarVariaveisGlobais()
{
  GLOBAL.opcaoJogo = 0x00;
  GLOBAL.perguntaDesafioA.pergunta = "";
  GLOBAL.perguntaDesafioA.resposta = 0;
  GLOBAL.senhaCorretaDesafioB = "";
  GLOBAL.senhaCorretaDesafioC = "";

  GLOBAL.tempoLimiteDesafioC.inicioDoContador = 0;
  GLOBAL.tempoLimiteDesafioC.tempoFinal = 0;
  GLOBAL.tempoLimiteDesafioC.minutos = 0;
  GLOBAL.tempoLimiteDesafioC.segundos = 0;
  GLOBAL.tempoLimiteDesafioC.tempoTotalEmMilissegundos = 0;
}

void carregandoJogo()
{
  lcd.clear();
}

void configurarAOpcoesDoJogo()
{
  lcd.print("Opcao de jogo:");
  lcd.setCursor(0, 1);
  lcd.print("A,B,C: ");

  bool escolheuOpcao = false;
  while (!escolheuOpcao)
  {
    char teclaDigitada = myKeypad.getKey();

    if (teclaDigitada != NO_KEY)
    {
      acionarSomELuzDaTeclaDigitada();

      if ((teclaDigitada == 'A') ||
          (teclaDigitada == 'B') ||
          (teclaDigitada == 'C'))
      {
        escolheuOpcao = true;
        GLOBAL.opcaoJogo = teclaDigitada;

        lcd.print(teclaDigitada);
        delay(1000);
        lcd.clear();

        if (teclaDigitada == 'A')
        {
          gerarEExibirNovaPerguntaDesafioA();
        }
        else if (teclaDigitada == 'B')
        {
          defineSenhaCorretaDesafioB();
          barraDeProgresso("Iniciando jogo", 50);
          gerarEExibirDesafioB();
        }
        else if (teclaDigitada == 'C')
        {
          log("DESAFIO C");

          defineTempoLimiteDoDesafioC();

          // Exibindo o tempo limite
          log("Tempo limite:", true);
          log("   Minutos: " + (String)GLOBAL.tempoLimiteDesafioC.minutos, true);
          log("   Segundos: " + (String)GLOBAL.tempoLimiteDesafioC.segundos, true);
          log("   Total: " + (String)GLOBAL.tempoLimiteDesafioC.tempoTotalEmMilissegundos + " milissegundos", true);

          defineSenhaCorretaDesafioC();
          barraDeProgresso("Iniciando jogo", 50);

          // iniciar contador
          GLOBAL.tempoLimiteDesafioC.inicioDoContador = millis();
          GLOBAL.tempoLimiteDesafioC.tempoFinal = GLOBAL.tempoLimiteDesafioC.inicioDoContador + GLOBAL.tempoLimiteDesafioC.tempoTotalEmMilissegundos;

          log("inicioDoContador: " + (String)GLOBAL.tempoLimiteDesafioC.inicioDoContador);
          log("tempoFinal: " + (String)GLOBAL.tempoLimiteDesafioC.tempoFinal);

          gerarEExibirDesafioC();
        }
      }
    }
  }
}

// Métodos padrões do Arduino
//---------------------------------------------------------------------------------------------

void setup()
{

#ifdef ARDUINO_AVR_NANO
  // código específico para o Arduino Nano
  GLOBAL.exibirLog = false;
#else
  // código para outras placas
  GLOBAL.exibirLog = true;
#endif

  Serial.begin(9600);

  log("INICIANDO O SETUP");
  log("Configurou a porta serial", true);

  log("Iniciando as variaveis globais", true);
  inicializarVariaveisGlobais();

  log("Gerando a semente para o random", true);
  randomSeed(analogRead(0));

  log("Iniciando o LCD", true);
  iniciarLCD();

  log("Configurando o buzzpin", true);
  pinMode(buzzpin, OUTPUT);

  log("Configurando a lampada", true);
  configurarLampada();
  desligarLampada();

  log("Carregando o jogo", true);
  carregandoJogo();

  log("Iniciando as opcoes do jogo", true);
  configurarAOpcoesDoJogo();

  log("TERMINOU O SETUP");
  log("");
}

void loop()
{
  char keypressed = myKeypad.getKey();

  // teclado
  if (keypressed != NO_KEY)
  {
    // log((String)keypressed);

    acionarSomELuzDaTeclaDigitada();
    bool resposta;

    if (keypressed == '#') // simula a tecla enter
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
        resposta = verificarSeSenhaDigitadaECorreta(GLOBAL.senhaCorretaDesafioB, GLOBAL.teclasDigitadas);
        exibirResultado(resposta);

        delay(1500);
        gerarEExibirDesafioB();
      }
      else if (GLOBAL.opcaoJogo == 'C')
      {
        delay(800);
        resposta = resolverDesafioC(GLOBAL.senhaCorretaDesafioC, GLOBAL.teclasDigitadas);
        exibirResultado(resposta);

        delay(1500);
        gerarEExibirDesafioC();
      }

      GLOBAL.teclasDigitadas = "";
    }
    else if (keypressed == '*') // simula a tecla de apagar
    {
      limparResposta();
      if (GLOBAL.opcaoJogo == 'A')
      {
        lcd.print(GLOBAL.perguntaDesafioA.pergunta);
      }
    }
    else
    {
      lcd.print(keypressed);
      GLOBAL.teclasDigitadas += (String)keypressed;
    }
  }

  // cronômetro
  if (GLOBAL.opcaoJogo == 'C')
  {
    unsigned long tempoAtual = millis();
    if (tempoAtual >= GLOBAL.tempoLimiteDesafioC.tempoFinal)
    {
      log("Acabou o tempo!");
      log("Tempo atual: " + (String)tempoAtual);

      exibirMensagemTempoExcedido();
      travarSistema();
    }
  }
}
