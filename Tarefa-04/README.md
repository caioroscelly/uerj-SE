# Tarefa 04 - Temporizadores -Timers 
## Software Embarcado - UERJ 
### Caio Roscelly Barros Fagundes
 
--> Entender e criar um Timer para contar o tempo usando arduíno e aplicar esse Timer no Trabalho 02

--> Substituir a função millis ou delay (ou qualquer outra função que conte e use o tempo) pelo TIMER1

--> Criar um Timer2 para capturar o tempo de uma entrada e gerar uma forma de onda

--> No arquivo chamado "Projeto02_com_timer.cc", encontra-se a primeira parte, que é criar o TIMER1 e substituir a função delay(1000) pelo Timer1. Eu usava a função delay meu projeto para atualizar a tela do display OLED a cada 1 segundo, então, agora quem faz essa lógica é o TIMER 1 que muda o bit de TIFR1 (clear timer on overflow) a cada 1 segundo (TCNT1  = 0x48E4 - inicia timer com valor para que estouro ocorra em 1 segundo)

--> Timer2 em andamento

