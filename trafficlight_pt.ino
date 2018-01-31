// pino no Arduino onde está ligado o LED vermelho dos carros
const byte CARROS_PINO_VERMELHO = 6;
// pino no Arduino onde está ligado o LED amarelo dos carros
const byte CARROS_PINO_AMARELO = 5;
// pino no Arduino onde está ligado o LED verde dos carros
const byte CARROS_PINO_VERDE = 4;
// pino no Arduino onde está ligado o LED vermelho dos peoes
const byte PEOES_PINO_VERMELHO = 3;
// pino no Arduino onde está ligado o LED verde dos peoes
const byte PEOES_PINO_VERDE = 2;

// não ligar nada a este pino do Arduino
const byte PINO_VAZIO = 7;

// durante quanto tempo deverá estar ligado o verde para os carros (segundos)
const int CARROS_TEMPO_VERDE = 6;
// durante quanto tempo deverá piscar o amarelo para os carros (segundos)
const int CARROS_TEMPO_PISCA_AMARELO = 2;
// durante quanto tempo deverá ser a espera entre o vermelho para os carros e o verde para os peões (segundos)
const int PEOES_TEMPO_ANTES_VERDE = 1;
// durante quanto tempo deverá estar ligado o verde para os peoões (segundos)
const int PEOES_TEMPO_VERDE = 4;
// durante quanto tempo deverá piscar o verde para os peões (segundos)
const int PEOES_TEMPO_PISCA_VERDE = 2;
// durante quanto tempo deverá ser a espera entre o vermelho para os peões e o verde para os carros (segundos)
const int CARROS_TEMPO_ANTES_VERDE = 1;

// quanto tempo tem o ciclo de piscas (milissegundos)
const int TEMPORIZACAO_PISCAS = 500;

// quanto tempo tem o ciclo no total
const int TEMPO_TOTAL = (CARROS_TEMPO_VERDE +
  CARROS_TEMPO_PISCA_AMARELO +
  PEOES_TEMPO_ANTES_VERDE +
  PEOES_TEMPO_VERDE +
  PEOES_TEMPO_PISCA_VERDE +
  CARROS_TEMPO_ANTES_VERDE) * 1000;

// guardar a última vez que o loop correu (milissegundos)
unsigned long ultimoTempo = 0;
// guardar o ponto no ciclo em que nos encontramos (milissegundos)
int tempoDecorrido = 0;
// guardar o ponto no ciclo de piscas em que nos encontramos (milissegundos)
int tempoDecorridoPisca = 0;

// Configuração inical - isto só corre uma vez, ao iniciar
void setup() {
  Serial.begin(9600);

  // marcar os pinos como sendo de saída
  pinMode(CARROS_PINO_VERMELHO, OUTPUT);
  pinMode(CARROS_PINO_AMARELO, OUTPUT);
  pinMode(CARROS_PINO_VERDE, OUTPUT);
  pinMode(PEOES_PINO_VERMELHO, OUTPUT);
  pinMode(PEOES_PINO_VERDE, OUTPUT);
  pinMode(PINO_VAZIO, OUTPUT);
}

void calcularTemposCiclos() {
  unsigned long agora = millis();
  // calcular o tempo no ciclo
  tempoDecorrido = (tempoDecorrido + (agora - ultimoTempo)) % TEMPO_TOTAL;
  // calcular o tempo no ciclo dos piscas
  tempoDecorridoPisca = tempoDecorrido % (TEMPORIZACAO_PISCAS * 2);
  // guardar o tempo actual para o próximo loop
  ultimoTempo = agora;
}

// desligar todos os LED
void desligarTudo() {
  digitalWrite(CARROS_PINO_VERMELHO, LOW);
  digitalWrite(CARROS_PINO_AMARELO, LOW);
  digitalWrite(CARROS_PINO_VERDE, LOW);
  digitalWrite(PEOES_PINO_VERMELHO, LOW);
  digitalWrite(PEOES_PINO_VERDE, LOW);
  digitalWrite(PINO_VAZIO, LOW);
}

// que luz deverá estar acesa para os carros
byte carrosLuzActual() {
  // se deverá ser a verde
  if (tempoDecorrido / 1000 < CARROS_TEMPO_VERDE) {
    return CARROS_PINO_VERDE;
  // se não deverá estar verde, mas deverá piscar no amarelo
  } else if (tempoDecorrido / 1000 < CARROS_TEMPO_VERDE + CARROS_TEMPO_PISCA_AMARELO) {
    // se está na parte ligada do ciclo dos piscas
    if (tempoDecorridoPisca < TEMPORIZACAO_PISCAS) {
      return CARROS_PINO_AMARELO;
    } else {
      return PINO_VAZIO;
    }
  } else {
    return CARROS_PINO_VERMELHO;
  }
}

// que luz deverá estar acesa para os peões
byte peoesLuzActual() {
  if (tempoDecorrido / 1000 < CARROS_TEMPO_VERDE + CARROS_TEMPO_PISCA_AMARELO + PEOES_TEMPO_ANTES_VERDE) {
    return PEOES_PINO_VERMELHO;
  } else if (tempoDecorrido / 1000 < CARROS_TEMPO_VERDE + CARROS_TEMPO_PISCA_AMARELO + PEOES_TEMPO_ANTES_VERDE + PEOES_TEMPO_VERDE) {
    return PEOES_PINO_VERDE;
  } else if (tempoDecorrido / 1000 < CARROS_TEMPO_VERDE + CARROS_TEMPO_PISCA_AMARELO + PEOES_TEMPO_ANTES_VERDE + PEOES_TEMPO_VERDE + PEOES_TEMPO_PISCA_VERDE) {
    if (tempoDecorridoPisca < TEMPORIZACAO_PISCAS) {
      return PEOES_PINO_VERDE;
    } else {
      return PINO_VAZIO;
    }
  } else {
    return PEOES_PINO_VERMELHO;
  }
}

void loop() {
  // calcular em que ponto dos ciclos estamos
  calcularTemposCiclos();

  // desligar todos os LED
  // (nada de preocupações, vamos ligar alguns a seguir)
  desligarTudo();

  // ligar a luz correcta para os carros
  digitalWrite(carrosLuzActual(), HIGH);
  // ligar a luz correcta para os peões
  digitalWrite(peoesLuzActual(), HIGH);
}
