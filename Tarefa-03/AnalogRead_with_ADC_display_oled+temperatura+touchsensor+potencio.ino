//Analog Read with Serial Monitor
const int pinoPOT = A0; //PINO ANALÓGICO UTILIZADO PELO POTENCIÔMETRO
int leituraA0 = 0; //VARIÁVEL QUE ARMAZENA O VALOR LIDO NO PINO ANALÓGICO
float tensao = 0;//Valor convertido em volts (V)


void setup() {
  //the setup routine runs once when you press reset

  Serial.begin(9600); //initialize serial communication at 9600 bits per second
  ADMUX = 0; //ADMUX = ANALOG DIGITAL MULTIPLEXER SELECTION REGISTER
  ADMUX |= (1 << REFS0); //REFS0 = reference selection bits // These bits select the voltage reference for the ADC, as shown in Table 74. If these bits are changed during a conversion, the change will not go in effect until this conversion is complete (ADIF in ADCSRA is set). The internal voltage reference options may not be used if an external reference voltage is being applied to the AREF pin.
  ADCSRA |= (1<< ADEN); // ADCSRA = ADC Control and Status Registe A  // ADEN = ANALOG DIGITAL CONVERSION ENABLED
 
  pinMode(pinoPOT, INPUT); //DEFINE O PINO COMO ENTRADA

}

void loop() {
  //the loop routine runs over and over again forever
    ADCSRA |= (1 << ADSC); //ADSC: ADC Start conversion
    while (ADCSRA & (1 << ADSC));
    int v = (ADCH << 8) + v; // ADC DATA REGISTER (ADCH/ADCL)

  // leituraA0 = analogRead(pinoPOT);//LÊ O VALOR NO PINO ANALÓGICO (VALOR LIDO EM BITS QUE VAI DE 0 A 1023)  
  int sensorValue = analogRead(A0); //read the input on analog pin 0

  tensao = sensorValue * (5.0/1023.0); //Converter valores em volts

  Serial.print(",\t\t");
  Serial.print(sensorValue, 1 /*FORMATAÇÃO PARA UMA CASA DECIMAL*/);
  Serial.print(",\t\t");
  Serial.println(tensao, 1 /*FORMATAÇÃO PARA UMA CASA DECIMAL*/);
  //Serial.println(sensorValue); //print out the value you read
  //Serial.println(pinoPOT); //print out the value you read
  //Serial.println(leituraA0); //print out the value you read



  delay(500); //delay in between reads for stability
}
