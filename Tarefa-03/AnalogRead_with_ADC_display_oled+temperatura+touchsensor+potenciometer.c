#include "U8glib.h"

//INCLUSÃO DAS BIBLIOTECAS
#include <dht.h>

//DEFINIÇÃO DE PINOS
#define pinSensor 7 // PINOS DO SENSOR DE UMIDADE E TEMPERATURA
#define sensorPin 2 // capactitive touch sensor - Arduino Digital pin D1

//INTERVALO DE LEITURA
#define intervalo 2000

//CRIANDO VARIAVEIS E INSTANCIANDO OBJETOS
unsigned long delayIntervalo;
dht sensorDHT;

//#################################

unsigned long time;


//VARIAVEIS DO POTENCIOMETRO
const int pinoPOT = A0; //PINO ANALÓGICO UTILIZADO PELO POTENCIÔMETRO
int leituraA0 = 0; //VARIÁVEL QUE ARMAZENA O VALOR LIDO NO PINO ANALÓGICO
float tensao = 0;//Valor convertido em volts (V)
int sensorValue;
//const int pinoPOT2 = CHO;
//controlador SSD1306 com interface I2C (U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK)), correspondente ao nosso display.
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);  // Display which does not send AC
 
void draw() 
{
  //Comandos graficos para o display devem ser colocados aqui
  
  //Seleciona a fonte de texto
  u8g.setFont(u8g_font_8x13B);
  //Linha superior - temperatura 
  //u8g.drawStr( 5, 15, "23");
  u8g.setPrintPos(5, 20); 
  u8g.print(sensorDHT.temperature, 1); 
  u8g.drawCircle(40,12,3);
  u8g.drawStr( 45, 20, "C");
 
  //Hora
  u8g.setFont(u8g_font_6x13B);
  u8g.drawStr( 5, 43, "Caio Roscelly");
  u8g.setFont(u8g_font_5x7);
  u8g.drawStr( 5, 52, "TR02-Software Embarcado");
  u8g.drawStr( 5, 60, "Time:");
  u8g.setPrintPos(50, 60); 
  time = millis();
  u8g.print(time);


  //Texto - AM
  u8g.setFont(u8g_font_5x7);
 // u8g.setPrintPos(70, 27); 
  //u8g.println(sensorValue, 0); 
  //u8g.drawStr( 90, 27, "Resolut");
  //u8g.drawStr( 115, 33, "AM");
  
  //moldura nome e timer
  // pixel da esquerda, pixel de cima, pixel da direita, pixel de baixo, aresta arrendodada
  u8g.drawRFrame(0,28, 128, 36, 4);
  //Desenho bateria
  //u8g.drawRFrame(105, 3, 20,12 , 0);
  //u8g.drawBox(125, 6, 2,6);
  //u8g.drawBox(107, 5, 4,8);
  //u8g.drawBox(114, 5, 4,8);
  //Desenho DA TEMPERATURA
  u8g.setFont(u8g_font_6x13B);
  u8g.setPrintPos(75, 15); 
  u8g.print(tensao, 1); 
  u8g.drawStr( 95, 15, "Volts"); 
  //u8g.drawVLine(99,0, 15);
  //u8g.drawVLine(98,0, 15);
  //u8g.drawVLine(96,4, 11);
  //u8g.drawVLine(95,4, 11);
  //u8g.drawVLine(93,8, 7);
  //u8g.drawVLine(92,8, 7);
  //u8g.drawVLine(90,12, 3);
  //u8g.drawVLine(89,12, 3);
}

//DESENHO 2 PARA QUANDO EU QUISER "DESLIGAR A TELA" e mandar tocar no touch a fim de ligar a tela de novo
void drawTwo() 
{
  //Comandos graficos para o display devem ser colocados aqui
  //Hora
  //u8g.setFont(u8g_font_6x13B);
  //u8g.drawStr( 20, 25, "Out Of Service");
  //u8g.setFont(u8g_font_5x7);
  //u8g.drawStr( 5, 45, "Toque no sensor e segure para ligar");
  //u8g.drawStr( 5, 55, "para ligar o dispositivo");

 
}///////////TERMINA DESENHO 2
 
void setup(void) 
{
  Serial.begin(9600);
  
  
  
  
  
  ADMUX = 0; //ADMUX = ANALOG DIGITAL MULTIPLEXER SELECTION REGISTER
  ADMUX |= (1 << REFS0); //REFS0 = reference selection bits // These bits select the voltage reference for the ADC, as shown in Table 74. If these bits are changed during a conversion, the change will not go in effect until this conversion is complete (ADIF in ADCSRA is set). The internal voltage reference options may not be used if an external reference voltage is being applied to the AREF pin.
  ADCSRA |= (1<< ADEN); // ADCSRA = ADC Control and Status Registe A  // ADEN = ANALOG DIGITAL CONVERSION ENABLED
  pinMode(pinoPOT, INPUT); //DEFINE O PINO COMO ENTRADA

   pinMode(sensorPin, INPUT); // definindo pino do touch sensor como input

   
   
   
    //IMPRIMINDO INFORMAÇÕES SOBRE A BIBLIOTECA
  Serial.print("VERSAO DA BIBLIOTECA: ");
  Serial.println(DHT_LIB_VERSION);
  Serial.println();
  Serial.println("Status,\tTempo(uS),\tUmidade(%),\tTemperatura(C)");
  
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255,255,255);
  }
}
 
void loop(void) 
{
   int senseValue = digitalRead(sensorPin);
  
  if (senseValue == LOW){ 
   u8g.firstPage();  
  do
  {
    draw();
  } while( u8g.nextPage() );
   
  delay(100);// ATUALIZA O DISPLAY A CADA 500ms
  
if ( (millis() - delayIntervalo) > intervalo ) {
      //LEITURA DOS DADOS
      unsigned long start = micros();
      int chk = sensorDHT.read22(pinSensor);
      unsigned long stop = micros();
  
    
    
      // EXIBINDO DADOS LIDOS
      Serial.print(stop - start);
      Serial.print(", \t\t");
      Serial.print(sensorDHT.humidity, 1 /*FORMATAÇÃO PARA UMA CASA DECIMAL*/);
      Serial.print(",\t\t");
      Serial.println(sensorDHT.temperature, 1 /*FORMATAÇÃO PARA UMA CASA DECIMAL*/);

      delayIntervalo = millis();
   
    };
  
  
  
  
  //the loop routine runs over and over again forever
   // Leitura CH0
  //ADMUX   = 0; // usa o canal 0
  //ADMUX  |= (1 << REFS0);       // usa Vcc como referencia
  //ADCSRA |= (1 << ADEN);        // habilita o ADC               
  ADCSRA |= (1 << ADSC);        // inicia a conversao
  while(ADCSRA & (1 << ADSC));  // espera a conversao
  int CH0 = ADCL;               // tem que ser lido antes do ADCH
  CH0 = (ADCH << 8) + CH0;         // valor de 2 bytes
  

  // leituraA0 = analogRead(pinoPOT);//LÊ O VALOR NO PINO ANALÓGICO (VALOR LIDO EM BITS QUE VAI DE 0 A 1023)  
  //int sensorValue = analogRead(A0); //read the input on analog pin 0

  tensao = CH0 * (5.0/1023.0); //Converter valores em volts
  Serial.println(CH0);
 

 
  }
  else{
    u8g.firstPage();  
  do
  {
    drawTwo();
  } while( u8g.nextPage() );
   
  delay(50);
    Serial.println("not touched");
  } 
  
  
  delay(500);// ATUALIZA O DISPLAY A CADA 500ms
  
}