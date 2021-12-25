#include <avr/pgmspace.h> 
#include "setletras.h"
#include "HWpanel.h"
#include "images.h"
#include "videoram.h"
#include "utilities.h"

volatile boolean scrolling = false;  //Pudiera pasarse a TCartel, analizar...
//------------------------------------------------------------------------------------------------------------------------
class TCartel{
  public:
  char *str;     //v1.1c moved here from TText
  int offset;    //v1.1c moved here from TText
  int indexFila;
  int bit_col_inic;
  boolean inverse;
  int delta_col;   //v...BETA_5
  int Qcols;
  int actualQcols; //v1.1d plus revision  //Sera usado en dual scroll efect
  boolean marquesina; //new in v1.d2
  boolean pls;
  int IndexLastByte; //new in v1.2c
  boolean ToggleInverse;  //new in v1.2c
  //int loops_show;
  TCartel(char *s, boolean inv){
     str = s;
     bit_col_inic = 0;
     Qcols = 0;
     actualQcols = 0; //v1.1d rev plus
     indexFila = 0;
     delta_col = 0;
     inverse = inv;
     offset = 0; //added v1.1c
     marquesina = false; //added v1.1d2
     IndexLastByte = 0; //added v1.2c
     ToggleInverse = false;
     //pls = false;  //v1.6b
    };
  void Refresh();  //v1.1c se quito virtual puro y hasta virtual
  virtual void Display() = 0;   
  void ApagarFila();
  //v1.6b:
  void RefreshNinja();
  void EncenderPrevFila();
};
//------------------------------------------------------------------------------------------------------------------------
TCartel* currCartel = NULL;
//------------------------------------------------------------------------------------------------------------------------
void TCartel::ApagarFila()
{
  if(indexFila > 0)
     digitalWrite(filas_pins[indexFila - 1], LOW);  
   else
     digitalWrite(filas_pins[7], LOW);  
}
//-----------------------------------------------------------------------------------------------------------------------
void TCartel::Refresh()
{
   for(int bit_col = bit_col_inic + (DISPLAY_COLS - 1); bit_col >=  bit_col_inic; bit_col-=8)
   //for(int bit_col = bit_col_inic + 39; bit_col >=  bit_col_inic; bit_col-=8)
   {
    int tmp = bit_col;   //its a good one

    if(bit_col >= Qcols && marquesina)
       bit_col = bit_col - Qcols;  //empatando final con ppo...  //Los carteles deben tener min 40 cols //DISPLAY_COLS

    byte SendB = 0;
    
    int curr_byte = bit_col / 8;  //v1.2c moved here
    int curr_bit = bit_col % 8;   //v1.2c

    //if(bit_col >= 0 &&  curr_byte <= IndexLastByte)  //v1.2c  //comentareado en v1.6b
    if(bit_col >= 0)
    {
     byte aux1 = 0, aux2 = 0;
     uint8_t k = 7 - curr_bit;

      if(k == 0)
       SendB = ledsRAM[indexFila][curr_byte];
      else  //Uniendo ppo con final //valido para left scroll, q al final es el sentido en que uno lee //no existe right scrool solo, si exise dual scroll 
      {  
        //No solo se une ppo con el final sino que se combinan dos bytes en uno para poder ser mandados via SPI, y sin tener que rotar la RAM
        //este es un pequeño know how...   
        aux1 = ledsRAM[indexFila][curr_byte];
      
        int bF = (Qcols - 1) / 8;
      
        int POSTbyte =  curr_byte - 1; 
       
        if(POSTbyte < 0)
          POSTbyte = bF;  //Uniendo ppo con el final

        if(bit_col > 6 || marquesina) //sino se quedara con aux2 = 0, pues se acabo la RAM, y todo esta en cero, seguro debido a un right scroll en efecto de salida, aqui no se debe unir ppo con final como en left scroll
        //if(bit_col > 6 || marquesina || pls)
        //if(marquesina)
        {
         aux2 = ledsRAM[indexFila][POSTbyte];
         aux2 = aux2 << (8 - k);
        }
        
        aux1 = aux1 >> k;
        
        SendB = aux1 + aux2;
      }//else // k!=0
    }//if(bit_col >= 0)
    
    if(inverse)
     SendB = ~SendB;
   
     SPI.transfer(SendB); //Enviando a los registros de desplazamiento vía SPI
     bit_col = tmp;  //... good one
  }//for //bit_col

   ///* //---> Descomentareado en simulacion, simulacion en Proteus
   //En ver 1.6b se utilizara sin comentarerar
   if(indexFila > 0)
     digitalWrite(filas_pins[indexFila - 1], LOW);  //Apagando fila anterior
   else
     digitalWrite(filas_pins[7], LOW);  //Apagando fila anterior
   
   delayMicroseconds(8);  //Esperando que se apague la fila anterior antes de encender la nueva o actual
   //*/

   PORTB = PORTB | B00000100;  //RCK --> HIGH
   PORTB = PORTB & B11111011;  //RCK --> LOW

   digitalWrite(filas_pins[indexFila], HIGH);  //Encendiendo fila actual
   
   indexFila++;
   if(indexFila == 8)
   {
    indexFila = 0;
    if(scrolling)
    {
     bit_col_inic += delta_col;
     scrolling = false;
     if(bit_col_inic >= Qcols && marquesina)  //v1.2c
      bit_col_inic = 0;
    }//if(scrolling)
    if(ToggleInverse) //added in v1.2c to create a new efect called Blink...
    {
      inverse = !inverse;
      ToggleInverse = false;
    }
    /*
    if(++precisionCounter == 5000)  //precisionCounter = 5000 --> Elapsed Time = 1.8ms * 8 * 5000  --> 1 min y 12 seg  //Prueba: OK
    {
      precisionCounter = 0;
      Serial.println(F("T"));
    }
    */
   }//if(indexFila == 8)
}//void TCartel::Refresh()
//------------------------------------------------------------------------------------------------------------------------
void TCartel::RefreshNinja()
{
   for(int bit_col = bit_col_inic + (DISPLAY_COLS - 1); bit_col >=  bit_col_inic; bit_col-=8)
   //for(int bit_col = bit_col_inic + 39; bit_col >=  bit_col_inic; bit_col-=8)
   {
    int tmp = bit_col;   //its a good one

    if(bit_col >= Qcols && marquesina)
       bit_col = bit_col - Qcols;  //empatando final con ppo...  //Los carteles deben tener min 40 cols //DISPLAY_COLS

    byte SendB = 0;
    
    int curr_byte = bit_col / 8;  //v1.2c moved here
    int curr_bit = bit_col % 8;   //v1.2c

    //if(bit_col >= 0 &&  curr_byte <= IndexLastByte)  //v1.2c
    if(bit_col >= 0)
    {
     byte aux1 = 0, aux2 = 0;
     uint8_t k = 7 - curr_bit;

      if(k == 0)
       SendB = ledsRAM[indexFila][curr_byte];
      else  //Uniendo ppo con final //valido para left scroll, q al final es el sentido en que uno lee //no existe right scrool solo, si exise dual scroll 
      {  
        //No solo se une ppo con el final sino que se combinan dos bytes en uno para poder ser mandados via SPI, y sin tener que rotar la RAM
        //este es un pequeño know how...   
        aux1 = ledsRAM[indexFila][curr_byte];
      
        int bF = (Qcols - 1) / 8;
      
        int POSTbyte =  curr_byte - 1; 
       
        if(POSTbyte < 0)
          POSTbyte = bF;  //Uniendo ppo con el final

        if(bit_col > 6 || marquesina)  //sino se quedara con aux2 = 0, pues se acabo la RAM, y todo esta en cero, seguro debido a un right scroll en efecto de salida, aqui no se debe unir ppo con final como en left scroll
        //if(bit_col > 6 || marquesina || pls)
        //if(marquesina)
        {
         aux2 = ledsRAM[indexFila][POSTbyte];
         aux2 = aux2 << (8 - k);
        }
        
        aux1 = aux1 >> k;
        
        SendB = aux1 + aux2;
      }//else // k!=0
    }//if(bit_col >= 0)
    
    if(inverse)
     SendB = ~SendB;
   
     SPI.transfer(SendB); //Enviando a los registros de desplazamiento vía SPI
     bit_col = tmp;  //... good one
  }//for //bit_col

  //Se dara a RCK y se encendera la fila despues cdo se llame a :EncenderPrevFila() desde la IT del timer
  PORTB = PORTB | B00000100;  //RCK --> HIGH
  PORTB = PORTB & B11111011;  //RCK --> LOW  //Es mejor hacerlo aqui, en definitiva la fila se apago con anterioridad
   
   indexFila++;
   if(indexFila == 8)
   {
    indexFila = 0;
    if(scrolling)
    {
     bit_col_inic += delta_col;
     scrolling = false;
     if(bit_col_inic >= Qcols && marquesina)  //v1.2c
      bit_col_inic = 0;
    }//if(scrolling)
    if(ToggleInverse) //added in v1.2c to create a new efect called Blink...
    {
      inverse = !inverse;
      ToggleInverse = false;
    }
   }//if(indexFila == 8)
}//void TCartel::RefreshNinja()
//------------------------------------------------------------------------------------------------------------------------
//Encendiendo Fila Previa
void TCartel::EncenderPrevFila()
{
   //PORTB = PORTB | B00000100;  //RCK --> HIGH
   //PORTB = PORTB & B11111011;  //RCK --> LOW   //Es mejor hacerlo en el RefreshNinja()

   //digitalWrite(filas_pins[indexFila], HIGH);  //Encendiendo fila actual
   if(indexFila > 0)
     digitalWrite(filas_pins[indexFila - 1], HIGH);  
   else
     digitalWrite(filas_pins[7], HIGH);  
}
//------------------------------------------------------------------------------------------------------------------------
class TImage : public TCartel{
 public:
 boolean IsFrame; //added v1.1d
 TImage(char *, boolean);
 TImage(char *, boolean, boolean);
 virtual void Display();
 void BuildRAM();
 //virtual void Refresh();
};
//------------------------------------------------------------------------------------------------------------------------
TImage::TImage(char *texto, boolean inv)
:TCartel(texto, inv)
{
 IsFrame = false;
}
//------------------------------------------------------------------------------------------------------------------------
TImage::TImage(char *texto, boolean inv, boolean animation)
:TCartel(texto, inv)
{
 IsFrame = animation;
}
//------------------------------------------------------------------------------------------------------------------------
void TImage::BuildRAM()
{
  int num = 0;
  //int ctd_ch = GetQtyCh(str);
    int i = 0;
    Qcols = offset;  //Desplazando....
    while(str[i] != '\0')
    //while(i < ctd_ch)
    {
      if(str[i] == ':')
      {
        if(!FetchNumber(str, i, num)) //i, num son pasados por referencia
         //Serial.println("Salida aprubta jejjee");
          return; //cad no valida para ser convertida a int
        else
        {
         switch(num) 
         {//imágenes aqui
          case 0: Qcols = ToMakeRAMwithImage (IM_HappyFace, Qcols, IsFrame); 
                  break;  
          case 1: Qcols = ToMakeRAMwithImage (IM_CopaA_1, Qcols, IsFrame); 
                  break;
          case 2: Qcols = ToMakeRAMwithImage (IM_CopaA_2, Qcols, IsFrame); 
                  break;
          case 3: Qcols = ToMakeRAMwithImage (IM_CopaA_3, Qcols, IsFrame); 
                  break;
          case 4: Qcols = ToMakeRAMwithImage (IM_CopaA_4, Qcols, IsFrame); 
                  break;
          case 5: Qcols = ToMakeRAMwithImage (IM_TVinos, Qcols, IsFrame); //cartel: Vinos
                  break;
          case 6: Qcols = ToMakeRAMwithImage (IM_TValois, Qcols, IsFrame); //cartel: Valois
                  break;
          case 7: Qcols = ToMakeRAMwithImage (IM_CopaB_0, Qcols, IsFrame);
                  break;
          case 8: Qcols = ToMakeRAMwithImage (IM_CopaB_1, Qcols, IsFrame); 
                  break;  
          case 9: Qcols = ToMakeRAMwithImage (IM_CopaB_2, Qcols, IsFrame); 
                  break;
          case 10: Qcols = ToMakeRAMwithImage (IM_CopaB_3, Qcols, IsFrame); 
                  break;
          case 11: Qcols = ToMakeRAMwithImage (IM_HappyFace2, Qcols, IsFrame); 
                  break;
          case 12: Qcols = ToMakeRAMwithImage (IM_Heart_0, Qcols, IsFrame); 
                  break;
          case 13: Qcols = ToMakeRAMwithImage (IM_Heart_1, Qcols, IsFrame); 
                  break;
          case 14: Qcols = ToMakeRAMwithImage (IM_Heart_2, Qcols, IsFrame); 
                  break;
          case 15: Qcols = ToMakeRAMwithImage (MasterORG, Qcols, IsFrame); 
                  break;
          case 16: Qcols = ToMakeRAMwithImage (Master_0, Qcols, IsFrame);   //M invertida
                  break;
          case 17: Qcols = ToMakeRAMwithImage (Master_1, Qcols, IsFrame); 
                  break; 
          case 18: Qcols = ToMakeRAMwithImage (Master_2, Qcols, IsFrame); 
                  break;  
          case 19: Qcols = ToMakeRAMwithImage (Master_3, Qcols, IsFrame); 
                  break;
          case 20: Qcols = ToMakeRAMwithImage (Master_4, Qcols, IsFrame); 
                  break;
          case 21: Qcols = ToMakeRAMwithImage (Master_5, Qcols, IsFrame); 
                  break;   
          case 22: Qcols = ToMakeRAMwithImage (MovilORG, Qcols, IsFrame); 
                  break;  
          case 23: Qcols = ToMakeRAMwithImage (Movil_0, Qcols, IsFrame);  //M invertida
                  break; 
          case 24: Qcols = ToMakeRAMwithImage (ClockOFF, Qcols, IsFrame); 
                  break;  
          case 25: Qcols = ToMakeRAMwithImage (Espacio, Qcols, IsFrame); 
                  break;
          case 26: Qcols = ToMakeRAMwithImage (V, Qcols, IsFrame); 
                  break;
          case 27: Qcols = ToMakeRAMwithImage (Vinos2, Qcols, IsFrame); 
                  break;
          case 28: Qcols = ToMakeRAMwithImage (Vinos3, Qcols, IsFrame); 
                  break;
          case 29: Qcols = ToMakeRAMwithImage (Vinos4, Qcols, IsFrame); //cartel: Vinos
                  break;
          case 30: Qcols = ToMakeRAMwithImage (Vinos5, Qcols, IsFrame); //cartel: Valois
                  break;
          case 31: Qcols = ToMakeRAMwithImage (Vinos6, Qcols, IsFrame);
                  break;
          case 32: Qcols = ToMakeRAMwithImage (Vinos7, Qcols, IsFrame); 
                  break;  
          case 33: Qcols = ToMakeRAMwithImage (Valois2, Qcols, IsFrame); 
                  break;
          case 34: Qcols = ToMakeRAMwithImage (Valois3, Qcols, IsFrame); 
                  break;
          case 35: Qcols = ToMakeRAMwithImage (Valois4, Qcols, IsFrame); 
                  break;
          case 36: Qcols = ToMakeRAMwithImage (Valois5, Qcols, IsFrame); 
                   break;
          case 37: Qcols = ToMakeRAMwithImage (Valois6, Qcols, IsFrame); 
                   break;
          case 38: Qcols = ToMakeRAMwithImage (Valois7, Qcols, IsFrame); 
                   break;
         }//switch(num) 
        }
      }//if(str[i] == ':')
      else
       i++;
    }//while...
}
//------------------------------------------------------------------------------------------------------------------------
void TImage::Display()
{
  if(Qcols == 0)
  {
    BuildRAM();
    if(Qcols > 0 && Qcols < DISPLAY_COLS)
    {
     if(offset == 0)
     {
      offset = DISPLAY_COLS/2 - Qcols/2;
      ClearRAM();
      BuildRAM();
     }
     Qcols = DISPLAY_COLS;
    }
    actualQcols = Qcols; //v1.1d rev plus
    if(Qcols % 8 != 0) //Esto sirve para q Qcols sea multiplo de 8 //ver BETA_g with ending //Adding efect: blending begining with ending
    {
     int b = Qcols / 8.0;
     Qcols = (b + 1) * 8;
    }
    IndexLastByte = Qcols / 8;  //added v1.2c
  }//if(Qcols == 0)
}//void TImage::Display()
//------------------------------------------------------------------------------------------------------------------------
class TText : public TCartel{
  public:  //mejor.. sin tener q hacer friends o mas...
  //char *str;   //Movido a la clase base TCartel
  //String str;
  ABC TipoLetra;
  int spaces;
  //int offset;   //Movido a la clase base TCartel
  TText(char *, ABC, int , boolean);
  TText(char *texto, ABC letra, int SbCh, boolean inv, int off);
  void Display();
};
//----------------------------------------------------------------------------------------------------------------------------------------
TText::TText(char *texto, ABC letra, int SbCh, boolean inv)
:TCartel(texto, inv)
{
  TipoLetra = letra;
  spaces = SbCh;
  offset = 0;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
TText::TText(char *texto, ABC letra, int SbCh, boolean inv, int off)
:TCartel(texto, inv)
{
  TipoLetra = letra;
  spaces = SbCh;
  inverse = inv;
  offset = off;
  //Serial.println("Aqui creando un nuevo objeto TText");
}
//-------------------------------------------------------------------------------------------------------------------------------------------
/*
 Scan Rows
 Top to Bottom
 LSB at right
*/
//Actualmente entre el tiempo de refresh y el tiempo de la IT toma mas 1ms porq no fue posible contar 10 seg con presicion usando como base 1ms mientras se debuggeaba (old comment)
//En version *** FW_8x40Leds_vBETA_g *** , usando SPI ya fue posible poner el tiempo de refrescamineto en 1ms
void TText::Display()
{
 if(Qcols == 0)
 {
  Qcols = ToMakeRAMwithAlphabet(str, ledsRAM, TipoLetra, spaces, 0xff, offset); 
  /*
  Serial.print(F("deb: Display Function - Qcols: "));
  Serial.println(Qcols);
  */
  //if(Qcols <= 40)
  //if(Qcols < 40) //v1.1d
  if(Qcols < DISPLAY_COLS) //DISPLAY_COLS v1.2c
  {
   if(offset == 0)
   {  
    //Añadiendo auto centrado...
    //uint8_t tmp_cols = 20 - (Qcols-spaces)/2; // Qcols-spaces --> Quitando los ultimas cols del espacio
    uint8_t tmp_cols = DISPLAY_COLS/2 - (Qcols-spaces)/2; // Qcols-spaces --> Quitando los ultimas cols del espacio
    uint8_t tmp_chars = tmp_cols / (3 + spaces);
    offset = tmp_cols % (3 + spaces);

    if(tmp_chars > 0)
    {
     char *str_add = new char[tmp_chars];
     for(int j = 0; j < tmp_chars; j++)
      str_add[j] = ' ';
     
     int i = 0;
     while(str[i] != '\0'){i++;} //midiendo longitud(de caracteres)de la cadena actual

     char *tmp_str = new char[tmp_chars + i + 1];
     tmp_str[tmp_chars + i] = '\0';
     
     int j;
     for(j = 0; j < tmp_chars; j++)
      tmp_str[j] = str_add[j]; //Añadiendo los espacios en blanco necesarios // los espacios son de tres columnas mas las q deja la funcion...
      
     for(int l = j, k = 0; l < tmp_chars + i; l++, k++)
      tmp_str[l] = str[k];
     j = 0;
     while(j <= tmp_chars + i)  //<= para que se agregue el caracter de fin de cadena...
     {
      str[j] = tmp_str[j];
      j++;  
     }
     delete [] tmp_str;
     delete [] str_add;
    }//if(tmp_chars > 0)
     ClearRAM();
     Qcols = ToMakeRAMwithAlphabet(str, ledsRAM, TipoLetra, spaces, 0xff, offset); 
     /*
     Serial.print(F("Qcols despues de autocentrado: "));
     Serial.println(Qcols);
     */
   }//if(offset == 0)
   Qcols = DISPLAY_COLS;
  }//if(Qcols < 40)
  actualQcols = Qcols; //v1.1d rev plus
  if(Qcols % 8 != 0) //Esto sirve para q Qcols sea multiplo de 8 //ver BETA_g with ending //Adding efect: blending begining with ending
  {
    int b = Qcols / 8.0;
    Qcols = (b + 1) * 8;
  }
  //IndexLastByte = Qcols / 8;
  IndexLastByte = (Qcols - 1) / 8;  //rev v1.6
 }//if(Qcols == 0)
}//TText::Display()
//---------------------------------------------------------------------------------------------------------------------------------------------------
