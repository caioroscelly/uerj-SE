//PROJETO ESCRITO POR CAIO ROSCELLY
//CONEXAO DO ARDUINO MEGA + SENSOR DTH22 + RELE COM LAMPADA AO MODULO WIRELESS ESP01 

// Incluindo Bibliotecas a serem Utilizadas no Projeto
#include "WiFiEsp.h" // Biblioteca para conexao com ESP01 (ESP-8266 ESP01)
#include "SoftwareSerial.h" //Inclusao da Software serial a fim de serializar pinos
#define ledPin 7 //PINO LED
int statusLed = LOW; //VARIÁVEL QUE ARMAZENA O ESTADO ATUAL DO LED (LIGADO / DESLIGADO)


// PARTE DO CODIGO REFERENTE A CONEXAO DO ARDUINO COM ESP01
// Pinos seriais para conexao 
// Pino 6 é Rx do Arduino (se conecta ao Tx do ESP01)
// Pino 7 é Tx do Arduino (se conecta ao Rx do ESP01)
//SoftwareSerial Serial1 (10, 11); //PINOS QUE EMULAM A SERIAL, ONDE O PINO 6 É O RX E O PINO 7 É O TX
const byte rxPin = 10;
const byte txPin = 11;
// set up a new serial object
SoftwareSerial Serial01 (rxPin, txPin);
//SoftwareSerial Serial1(6, 7); 
//
//char ssid[] = "Lenovo_K6"; // Nome da rede sem fio a se conectar (SSID)
//char pass[] = "12345678"; // Senha da rede sem fio a se conectar
char ssid[] = "caio"; // Nome da rede sem fio a se conectar (SSID)
char pass[] = "caioemarcos0000"; // Senha da rede sem fio a se conectar
//
int status = WL_IDLE_STATUS; // Verifica o Status atribuido a rede Wi-Fi durante conexao, apos isso Wi-FI permanece
//Se não conseguir conectar o status fica em WL_NO_SHIELD. Se conseguir conectar o status fica em WL_CONNECTED
//
WiFiEspServer server(80); // Ao realizar a conexão, sobe um servidor no ESP01 na porta 2000
//
RingBuffer buf(8); //BUFFER PARA AUMENTAR A VELOCIDADE E REDUZIR A ALOCAÇÃO DE MEMÓRIA
// FIM DO CODIGO REFERENTE A CONEXAO DO ARDUINO COM ESP01




// PARTE DO CODIGO REFERENTE AO SENSOR DTH22 DE TEMPERATURA E UMIDADE
//INCLUSÃO DAS BIBLIOTECAS
#include <dht.h>
int statusTemperatureUmidity = 0; //VARIÁVEL QUE ARMAZENA O ESTADO ATUAL DO SENSOR DTH
//DEFINIÇÃO DE PINOS
#define pinSensor 8
//INTERVALO DE LEITURA
#define intervalo 2000
//CRIANDO VARIAVEIS E INSTANCIANDO OBJETOS
unsigned long delayIntervalo;
dht sensorDHT;




void setup(){
  pinMode(ledPin, OUTPUT); //DEFINE O PINO COMO SAÍDA (ledPin = PINO 13)
  digitalWrite(ledPin, LOW); //PINO 13 INICIA DESLIGADO
  Serial.begin(9600); //INICIALIZA A SERIAL
  Serial01.begin(9600); //INICIALIZA A SERIAL PARA O ESP8266
  //Serial1.begin(9600); //INICIALIZA A SERIAL PARA O ESP8266
  WiFi.init(&Serial01); //INICIALIZA A COMUNICAÇÃO SERIAL COM O ESP8266
  //WiFi.init(&Serial1); //INICIALIZA A COMUNICAÇÃO SERIAL COM O ESP8266
  WiFi.config(IPAddress(192,168,0,104)); //COLOQUE UMA FAIXA DE IP DISPONÍVEL DO SEU ROTEADOR


  
  //INÍCIO - VERIFICA SE O ESP8266 ESTÁ CONECTADO AO ARDUINO, CONECTA A REDE SEM FIO E INICIA O WEBSERVER
  if(WiFi.status() == WL_NO_SHIELD){
    while (true);
  }
  while(status != WL_CONNECTED){
    status = WiFi.begin(ssid, pass);
  }
  server.begin();
  //FIM - VERIFICA SE O ESP8266 ESTÁ CONECTADO AO ARDUINO, CONECTA A REDE SEM FIO E INICIA O WEBSERVER
}







void loop(){


// FAZ O SENSOR DTH22 LER O VALOR DE TEMPERATURA E UMIDADE DO AMBIENTE A CADA INICIO DE LOOP
  if ( (millis() - delayIntervalo) > intervalo ) {
            //LEITURA DOS DADOS
            unsigned long start = micros();
            int chk = sensorDHT.read22(pinSensor);
             unsigned long stop = micros();
            delayIntervalo = millis();};


//INICIA WEBSERVER			
  WiFiEspClient client = server.available(); //ATENDE AS SOLICITAÇÕES DO CLIENTE

  if (client) { //SE CLIENTE TENTAR SE CONECTAR, FAZ
    buf.init(); //INICIALIZA O BUFFER
    while (client.connected()){ //ENQUANTO O CLIENTE ESTIVER CONECTADO, FAZ
      if(client.available()){ //SE EXISTIR REQUISIÇÃO DO CLIENTE, FAZ
        char c = client.read(); //LÊ A REQUISIÇÃO DO CLIENTE
        buf.push(c); //BUFFER ARMAZENA A REQUISIÇÃO

        //IDENTIFICA O FIM DA REQUISIÇÃO HTTP E ENVIA UMA RESPOSTA
        if(buf.endsWith("\r\n\r\n")) {
          sendHttpResponse(client);
          break;
        }
        if(buf.endsWith("GET /H")){ //SE O PARÂMETRO DA REQUISIÇÃO VINDO POR GET FOR IGUAL A "H", FAZ 
            digitalWrite(ledPin, HIGH); //ACENDE O LED
            statusLed = 1; //VARIÁVEL RECEBE VALOR 1(SIGNIFICA QUE O LED ESTÁ ACESO)
        }else if (buf.endsWith("GET /T")){
          //INICIAS DTH22
          statusTemperatureUmidity = 1;
    }else if (buf.endsWith("GET /O")){
          //INICIAS DTH22
          statusOLED = 1;
          
    }
        else{ //SENÃO, FAZ
          if (buf.endsWith("GET /L")) { //SE O PARÂMETRO DA REQUISIÇÃO VINDO POR GET FOR IGUAL A "L", FAZ
                  digitalWrite(ledPin, LOW); //APAGA O LED
                  statusLed = 0; //VARIÁVEL RECEBE VALOR 0(SIGNIFICA QUE O LED ESTÁ APAGADO)
          }
        }
      }
    }
    client.stop(); //FINALIZA A REQUISIÇÃO HTTP E DESCONECTA O CLIENTE
  }
}


//MÉTODO DE RESPOSTA A REQUISIÇÃO HTTP DO CLIENTE
void sendHttpResponse(WiFiEspClient client){
  client.println("HTTP/1.1 200 OK"); //ESCREVE PARA O CLIENTE A VERSÃO DO HTTP
  client.println("Content-Type: text/html"); //ESCREVE PARA O CLIENTE O TIPO DE CONTEÚDO(texto/html)
  client.println("");
  client.println("<!DOCTYPE HTML>"); //INFORMA AO NAVEGADOR A ESPECIFICAÇÃO DO HTML
  client.println("<html>"); //ABRE A TAG "html"
  client.println("<head>"); //ABRE A TAG "head"
  client.println("<link rel='icon' type='image/png' href=''/>"); //DEFINIÇÃO DO ICONE DA PÁGINA
  client.println("<link rel='stylesheet' type='text/css' href='https://blogmasterwalkershop.com.br/arquivos/artigos/sub_wifi/webpagecss.css' />"); //REFERENCIA AO ARQUIVO CSS (FOLHAS DE ESTILO)
  client.println("<title>Projeto Final Software Embarcado - Mestrado PEL UERJ 2019.2</title>"); //ESCREVE O TEXTO NA PÁGINA
  client.println("<h1>Caio Roscelly Barros Fagundes</h1>"); //ESCREVE O TEXTO NA PÁGINA
  client.println("<h5>Mestrando do Programa de Pos Graduacao em Engenharia Eletronica PEL UERJ</h5>"); //ESCREVE O TEXTO NA PÁGINA
  client.println("</head>"); //FECHA A TAG "head"
  
  //AS LINHAS ABAIXO CRIAM A PÁGINA HTML
  client.println("<body>"); //ABRE A TAG "body"
  client.println("<p style='line-height:2'><font>Modulo WiFi ESP8266 para Arduino</font></p>"); //ESCREVE O TEXTO NA PÁGINA
  client.println("<font>TESTE</font>"); //ESCREVE O TEXTO NA PÁGINA
  
  if (statusLed == HIGH && statusTemperatureUmidity == 0){ //SE VARIÁVEL FOR IGUAL A HIGH (1), FAZ
    client.println("<p style='line-height:0'><font color='green'>LIGADO</font></p>"); //ESCREVE "LIGADO" NA PÁGINA
    client.println("<a href=\"/L\">APAGAR</a>"); //COMANDO PARA APAGAR O LED (PASSA O PARÂMETRO /L)
    client.println("<hr />"); //TAG HTML QUE CRIA UMA LINHA NA PÁGINA
    client.println("<p style='line-height:0'><font color='green'>TEMPERATURA:</font></p>"); //ESCREVE "LIGADO" NA PÁGINA
    // client.println(sensorDHT.temperature, 2); //ESCREVE "LIGADO" NA PÁGINA
     client.println("<p style='line-height:0'><font color='green'>UMIDADE:</font></p>"); //ESCREVE "LIGADO" NA PÁGINA
    //client.println(sensorDHT.humidity, 2); //FORMATAÇÃO PARA UMA CASA DECIMAL/); //ESCREVE "LIGADO" NA PÁGINA
    client.println("<a href=\"/T\">GET Temp Umi</a>"); 
  }
  else if (statusLed == HIGH && statusTemperatureUmidity == 1){ //SE VARIÁVEL FOR IGUAL A HIGH (1), FAZ
    client.println("<p style='line-height:0'><font color='green'>LIGADO</font></p>"); //ESCREVE "LIGADO" NA PÁGINA
    client.println("<a href=\"/L\">APAGAR</a>"); //COMANDO PARA APAGAR O LED (PASSA O PARÂMETRO /L)
    client.println("<hr />"); //TAG HTML QUE CRIA UMA LINHA NA PÁGINA
    client.println("<p style='line-height:0'><font color='green'>TEMPERATURA:</font></p>"); //ESCREVE "LIGADO" NA PÁGINA
     client.println(sensorDHT.temperature, 2); //ESCREVE "LIGADO" NA PÁGINA
     client.println("<p style='line-height:0'><font color='green'>UMIDADE:</font></p>"); //ESCREVE "LIGADO" NA PÁGINA
    client.println(sensorDHT.humidity, 2); //FORMATAÇÃO PARA UMA CASA DECIMAL/); //ESCREVE "LIGADO" NA PÁGINA
    client.println("<a href=\"/T\">GET Temp Umi</a>");    
    }
    else if (statusLed == LOW && statusTemperatureUmidity == 0){ //SE VARIÁVEL FOR IGUAL A HIGH (1), FAZ
    client.println("<p style='line-height:0'><font color='green'>APAGADO</font></p>"); //ESCREVE "LIGADO" NA PÁGINA
    client.println("<a href=\"/H\">ACENDER</a>"); //COMANDO PARA APAGAR O LED (PASSA O PARÂMETRO /L)
    client.println("<hr />"); //TAG HTML QUE CRIA UMA LINHA NA PÁGINA
    client.println("<p style='line-height:0'><font color='green'>TEMPERATURA:</font></p>"); //ESCREVE "LIGADO" NA PÁGINA
    // client.println(sensorDHT.temperature, 2); //ESCREVE "LIGADO" NA PÁGINA
     client.println("<p style='line-height:0'><font color='green'>UMIDADE:</font></p>"); //ESCREVE "LIGADO" NA PÁGINA
    //client.println(sensorDHT.humidity, 2); //FORMATAÇÃO PARA UMA CASA DECIMAL/); //ESCREVE "LIGADO" NA PÁGINA
    client.println("<a href=\"/T\">GET Temp Umi</a>");
    client.println("<hr />"); //TAG HTML QUE CRIA UMA LINHA NA PÁGINA
    client.println("<hr />"); //TAG HTML QUE CRIA UMA LINHA NA PÁGINA
    }  
  else if (statusLed == LOW && statusTemperatureUmidity == 1){ //SE VARIÁVEL FOR IGUAL A LOW (0), FAZ
    client.println("<p style='line-height:0'><font color='red'>DESLIGADO</font></p>"); //ESCREVE "DESLIGADO" NA PÁGINA
    client.println("<a href=\"/H\">ACENDER</a>"); //COMANDO PARA ACENDER O LED (PASSA O PARÂMETRO /H)
     client.println("<hr />"); //TAG HTML QUE CRIA UMA LINHA NA PÁGINA
    client.println("<p style='line-height:0'><font color='green'>TEMPERATURA:</font></p>"); //ESCREVE "LIGADO" NA PÁGINA
     client.println(sensorDHT.temperature, 2); //ESCREVE "LIGADO" NA PÁGINA
     client.println("<p style='line-height:0'><font color='green'>UMIDADE:</font></p>"); //ESCREVE "LIGADO" NA PÁGINA
    client.println(sensorDHT.humidity, 2); //FORMATAÇÃO PARA UMA CASA DECIMAL/); //ESCREVE "LIGADO" NA PÁGINA
    client.println("<a href=\"/T\">GET Temp Umi</a>"); 
    }else{
  }
  statusTemperatureUmidity = 0; // SETA NOVAMENTE A VARIAVEL QUE LE A ATEMPERATURA PARA ZERO PARA NA TELA NAO FICAR MOSTRANDO A TEMPERATURA TODA HORA. SO DEVE APARECER QUANDO CLICAR EM GET TEMP
  client.println("<hr />"); //TAG HTML QUE CRIA UMA LINHA NA PÁGINA
  client.println("<hr />"); //TAG HTML QUE CRIA UMA LINHA NA PÁGINA
  client.println("</body>"); //FECHA A TAG "body"
  client.println("</html>"); //FECHA A TAG "html"
  delay(1000); //INTERVALO DE 1 MILISSEGUNDO


//FINALIZA CLIENTE WEBSERVER




}