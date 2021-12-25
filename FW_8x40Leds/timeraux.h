//#include "matrixaux.h"
#include "Carteles.h"

//v1.4:
byte PulseCounter = 0;
byte DesiredPulse_k = 4;  //4 --> 100% de Intensidad
//byte DesiredPulse_k = 1;  //1 --> 25% de Intensidad
//byte DesiredPulse_k = 3;  //3 --> 75% de Intensidad

//Timers y otros
//Timer1 16 bits
//Se recomiendan 70Hz por fila, aunq no menos de 2ms segun esa misma fuente
//OCR1A = 3600;   //1.8 ms
//OCR1A = 4000;   //2 ms
void InicializarTimer()  //Debe estar entre 1ms y 2ms
{
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  //OCR1A = 3600;            // compare match register 16MHz/8/0.5 KHz   //base de tiempo 0.5 uS Incrementando o Disminuyendo OCR1A se aumenta o se disminuye 0.5 uS
  OCR1A = 900; //v1.4 subdivideiendo el refresh para jugar con la intensidad 4 * 900 = 3600
  //simulacion:
  //OCR1A = 3600;  
  //OCR1A = 900;
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS11);    // 8 prescaler 
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
}//void InicializarTimer()
//-----------------------------------------------------------------------------------------------------------------------------------------------
ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{
 ///*
 if(++PulseCounter == DesiredPulse_k && DesiredPulse_k < 4)
 {
  if(currCartel != NULL)
  {
    currCartel->ApagarFila();
    currCartel->RefreshNinja();
  }
 }
 if(PulseCounter == 4)
 {
  PulseCounter = 0;
  if(currCartel != NULL)
  {
   if(DesiredPulse_k < 4)
     currCartel->EncenderPrevFila();
   else
    currCartel->Refresh();
  }
 }
 //*/ //--> Comentareado para simulacion en Proteus
 //simulacion:
 /*
 if(currCartel != NULL)
  currCartel->Refresh();
  */
}
