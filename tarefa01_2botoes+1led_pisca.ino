#define LED 6
#define BUT_SPEEDUP 10
#define BUT_SLOWDOWN 11

//Definindo as variaveis do meu projeto
int stateSpeedUpButton;
int stateSlowDownButton;
int blinkSpeed = 1000;
unsigned long old;
int state = 1;
int delayWhenPressBut = 200;


//ledBlink vai ser a funcao que faz o led piscar e blinkSpeed a velocidade de pisca
//now = millis, ou seja, eh o tempo que a o script loop comecou a rodar em cada iteracao
//millis - Retorna o numero de milissegundos passados desde que a placa Arduino comecou a executar o programa atual
void ledBlink (int blinkSpeed) {
  unsigned long now = millis();
  if (now >= old+blinkSpeed) {
    state = !state;
    old = now;
    digitalWrite(LED, state);
  } 
}

void setup() {
  // put your setup code here, to run once:
  //Definindo os componentes do projeto como entrada ou saida
  // o led eh saida
  pinMode(LED, OUTPUT);
  // os botes sao entrada
  pinMode(BUT_SPEEDUP, INPUT);
  pinMode(BUT_SLOWDOWN, INPUT);
}


void loop() {
  //definindo variavel now como sendo = millis para fazer o led piscar +rapido ou +devagar
  // essa variavel sera usada ao longo do projeto, portanto, sem ela o led nao pisca
  unsigned long now = millis();

  // INICIANDO O ESTADO INICIAL DO LED QUE VAI SER --> PISCAR A CADA 1 SEGUNDO (1000 ms)
  ledBlink(blinkSpeed);

  
  // PRIMEIRO VOU VERIFICAR SE OS 2 BOTOES FORAM PRESSIONADOS JUNTOS 
  //(JUNTOS LEIA-SE PRESSIONADOS AO MESMO TEMPO DENTRO DE UM INTERVALO DE 500ms)
  //se but1 for pressionado E but2 for pressionado E but2 for pressionado em menos de 500ms depois
  //ou se but2 for pressionado E but1 for pressionado E but1 for pressionado em menos de 500ms depois
  if ( stateSpeedUpButton &&  stateSlowDownButton && 
     (now - stateSpeedUpButton < 501) &&
     (now - stateSlowDownButton < 501) ) { 
    digitalWrite(LED, LOW);   //se os 2 botoes forem apertados ao "mesmo" tempo, entao apaga o led                                          
  //while(0) em C significa -- nunca entre em loop pois o if statement acima eh falso
  //while(1) em C significa -- entre em loop caso o if statement acima for verdadeiro
  while(1);
  }

  
  //Senao, se os 2 botoes nao forem apertados ao mesmo tempo --> 
  //aumentar velocidade de pisca quanto apertar but1 OU diminuir velocidade de piscar quando apertar but2
  else {
	  
    //INICIO DA LOGICA DE PROGRAMACAO DO BOTAO1 = ACELERAR VELOCIDADE DE PISCA
    if( stateSpeedUpButton ) {	// Verificando o estado do but1. Se o botao nao foi pressionado ou nao estiver pressionado mudo estado dele para um estado inicial zero que eh um estado de "nao pressionado"
      if(digitalRead(BUT_SPEEDUP)) {
        stateSpeedUpButton = 0;
      }
    }    
	//Se houver a negacao do estado inicial do botao, ou seja, se ele sair de zero e se tornar 1, entao aplico a mudanca de estado, que nada mais eh do que aumentar a velocidade do led pisca
    else if(!digitalRead(BUT_SPEEDUP)) {
      stateSpeedUpButton = now;			//Comeca a contar o tempo desde quando o botao foi pressionado para aplicar na funcao (millis - delay)
      blinkSpeed = blinkSpeed - delayWhenPressBut;			// estamos mudando a velocidade de pisca, assim , aumentando a frequencia de pisca
    }//FIM DA LOGICA DE PROGRAMACAO DO BOTAO1 
	 
	 //--------------------------------------------------------------------------------------
	 	 
	//INICIO LOGICA DE PROGRAMACAO DO BOTAO2 = DESACELERAR VELOCIDADE DE PISCA
    if( stateSlowDownButton ) { 	// Aplicando a mesma logica descrita no if/elseif statement acima para o but2
      if(digitalRead(BUT_SLOWDOWN)) {
        stateSlowDownButton = 0;
      }
    }
    else if(!digitalRead(BUT_SLOWDOWN)) {
      stateSlowDownButton = now;
      blinkSpeed = blinkSpeed + delayWhenPressBut;
    }//FIM DA LOGICA DE PROGRAMACAO DO BOTAO2 
 
  
  } //Finalizando else statement. Logica de acelerar e desacelerar velocidade pisca do led terminou

  
} // terminou o loop
