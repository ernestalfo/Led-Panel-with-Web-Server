//En construccion

#define SIZE_RAM_BYTECOLS 120
//#define LIM_COLS (SIZE_RAM_BYTECOLS - 5) * 8   //Dejando en limpio los ultimos 40 bits ( 5 * 8 = 40)
                                               //No recuerdo bien por que? parece que es para el efecto de salida....  
//#define LIM_COLS (SIZE_RAM_BYTECOLS - 10) * 8   //Dejando en limpio los ultimos 80 bits ( 10 * 8 = 80)  //v1.1d rev plus

//#define LIM_COLS (SIZE_RAM_BYTECOLS - 5) * 8  //v1.2b   
 #define LIM_COLS SIZE_RAM_BYTECOLS  * 8   //v1.2c Ya no hay que guardar espacio en la RAM                                       

byte ledsRAM[8][SIZE_RAM_BYTECOLS] = {0};

int FeedRow(byte row[], int index, byte supplier, int pos_i, int pos_f, int spaces);

int FeedCOLfromImg(int index, byte supplier);

//-----------------------------------------------------------------------------------------------------------------------
//index pasado como referencia porq se puede alterar dentro de la funcion
byte FetchByteInAlphabet (int &index, int fila, byte Font[][9], char *msg)
//byte FetchByteInAlphabet (int index, int fila, byte Font[][9], char *msg)
{
  byte c = msg[index];
  //uint8_t c = msg[index];
  if(c >= 0x20 && c <= 0x7f) //Caracteres Estandares 
    return pgm_read_byte(&(Font[c - 0x20][fila]));
  else
  {
    //Serial.print(F("Caracter no estandar: "));
    //Serial.println(c);
    switch(c)
    {
      ///*
        case 241: //'ñ'
                 return pgm_read_byte(&(Font[96][fila]));   
                 break;  
        case 225: //'á'
                 return pgm_read_byte(&(Font[97][fila]));   
                 break;  
        case 233: //'é'
                 return pgm_read_byte(&(Font[98][fila]));   
                 break;
        case 237: //'í'
                 return pgm_read_byte(&(Font[99][fila]));   
                 break;
        case 243: //'ó'
                 return pgm_read_byte(&(Font[100][fila]));   
                 break; 
        case 250: //'ú'
                 return pgm_read_byte(&(Font[101][fila]));   
                 break;
        case 252: //'ü'
                 return pgm_read_byte(&(Font[102][fila]));   
                 break;
         // */ //---> Esta codificación no creo que se use mucho, solo en algunos IDEs como por ej el IDE 1.6.11, 
        //el IDE 1.8.9 usa la misma codificación en el monitor serie que en el momento de la compilación (char *s = "ejemplo")
        //Aunque resulta que en el Proteus tambien se usa esta configuración
        
        case 195: //primer int común para ñ, á, é, í, ó, ú
               //Serial.println(F("2 bytes configuration..."));
               index++;
               if(msg[index] != '\0')
               {
                 c = msg[index];
                 //uint8_t c2 = msg[index];
                 //Serial.println(F("No se ha acabado la cadena"));
                 //Serial.println(c2);
                 if(fila == 8)  //OJO, huele a parche y es un parche, cdo se busca en la fila 8 es porq se quiere saber pos_i y pos_f
                  index--;         //por lo que luego se volvera a preguntar por otros bytes, los bytes con ctdo (de 0 a 7)
                 //switch(c2)
                 switch(c)
                 {
                    case 177: //'ñ'
                             return pgm_read_byte(&(Font[96][fila]));   
                             break;
                    case 161: //'á'
                             //Serial.println(F("a con tilde por aquI"));
                             return pgm_read_byte(&(Font[97][fila]));   
                             break;
                    case 169: //'é'
                             return pgm_read_byte(&(Font[98][fila]));   
                             break;
                    case 173: //'í'
                             return pgm_read_byte(&(Font[99][fila]));   
                             break;
                    case 179: //'ó'
                             return pgm_read_byte(&(Font[100][fila]));   
                             break;
                    case 186: //'ú'
                             return pgm_read_byte(&(Font[101][fila]));   
                             break;  
                    case 188: //'ü'
                             return pgm_read_byte(&(Font[102][fila]));   
                             break;
                   default: //No definido
                             //return posIposF = pgm_read_byte(&(Fonts[0x7f - 0x20][fila]));   
                             return pgm_read_byte(&(Font[95][fila]));   
                             break;
                 }//switch(c)
               }//if(msg[index]!= '\0')
             break;
        default: //No definido
                return pgm_read_byte(&(Font[95][fila]));   
                break;  
    }
  }//else  //caracteres no estandares
}
//-----------------------------------------------------------------------------------------------------------------------
//int ToMakeRAMwithAlphabet (char *cartel, byte RAM[][SIZE_RAM_BYTECOLS], ABC Alphabet, boolean space, byte pIpF)
/*
 *  char *cartel: Cadena de caracteres ASCII que se desea visualizar en el panel de LEDS, ej: "HELLO"
 * 
 *  byte RAM[][SIZE_RAM_BYTECOLS]: Arreglo donde se conformara la memoria de video del cartel 
 *  / SIZE_RAM_BYTECOLS: Tamaño de la RAM, cada byte almacena 8 columnas o sea SIZE_RAM_BYTECOLS * 8 = Cantidad Maxima de columnas
 *                                         //o mejor SIZE_RAM_BYTECOLS * 8 - 40 = Cantidad Maxima de columnas //si se quiere dejar la ultima parte en blanco...
 *  
 *  ABC Alphabet: Tipos de letras por asi decirlo, las letras estan guardadas en arreglos diferentes, algunos tienen 7 otros 8 filas para cada letra,
 *  serian 7 u 8 bytes para cada letra, mas un byte al final que se agrego para saber en que bit empieza la letra y en cual bit termina...
 *  
 *  boolean space: en true agrega una columna en blanco entre cada caracter.
 *  
 *  byte pIpF: Indica la pos inicial pI, columna o bit, donde comienza la letra y la pos final pF donde termina
 *  HIGH nibble --> pI: 0 -- 7 , LOW nibble --> pF: 0 -- 7, ej: 0x70
 *  Si pIpF == 0xff , la informacion de donde termina la letra se saca directamente del ultimo byte que define a cada letra en el arreglo especificado 
 *  por el tipo de Alfabeto o tipo de letra
 *  
 *  boolean inverse: 
 * 
 */
int ToMakeRAMwithAlphabet (char *cartel, byte RAM[][SIZE_RAM_BYTECOLS], ABC Alphabet, int spaces, byte pIpF, int offset) 
{
  int i = 0;
  int index_col = 0;
  //int Qrows = 8;
  /*
  Serial.println(F("deb: Hello... from making RAM ..."));
  Serial.println(cartel);
 */
 
  //for(int l = 0; l < Qrows; l++)//filas  
  for(int l = 0; l < 8; l++)//filas  
  {
   index_col = 0;
   i = 0;
   
   if(offset > 0) //desde 1 a 8 //mas de 8 no funcionara bien
    index_col = FeedRow(RAM[l], index_col, 0x00, offset - 1, 0, 0); //Insertando offset, en caso de que exista, en la fila.
    
    //while (cartel[i] != '\0' && (index_col + abs(pos_f - pos_i)) < SIZE_RAM_BYTECOLS * abs(pos_f - pos_i)) //Analizar despues creo deberia ser mejor: SIZE_RAM_BYTECOLS * 8
    //while (cartel[i] != '\0')
    while (cartel[i] != '\0' && index_col + 8 < LIM_COLS) //mejor implementación: index_col + abs(pos_f - pos_i)
    {
      byte posIposF, pos_i, pos_f;

      //Serial.println(i);
      
      switch(Alphabet)
      {
        case 0:  //Alphabet0  //SingleCharI
               if(pIpF == 0xff)
                 posIposF = FetchByteInAlphabet (i, 8, Alphabet0, cartel);//El ultimo byte tiene la informacion de donde comenzar a leer y donde terminar (cuales bits)
               else
               posIposF = pIpF;
               pos_i = posIposF >> 4;
               pos_f = posIposF & B00001111;
               index_col = FeedRow(RAM[l], index_col, FetchByteInAlphabet (i, l, Alphabet0, cartel), pos_i, pos_f, spaces);
               break;
        case 1:  //Alphabet1  //DoubleCharI
               if(pIpF == 0xff)
                 posIposF = FetchByteInAlphabet (i, 8, Alphabet1, cartel);//El ultimo byte tiene la informacion de donde comenzar a leer y donde terminar (cuales bits)
               else
               posIposF = pIpF;
               pos_i = posIposF >> 4;
               pos_f = posIposF & B00001111;
               index_col = FeedRow(RAM[l], index_col, FetchByteInAlphabet (i, l, Alphabet1, cartel), pos_i, pos_f, spaces);
               break;
        case 2: //Alphabet2  //DoubleCharII
               if(pIpF == 0xff)
                 posIposF = FetchByteInAlphabet (i, 8, Alphabet2, cartel);//El ultimo byte tiene la informacion de donde comenzar a leer y donde terminar (cuales bits)
               else
               posIposF = pIpF;
               pos_i = posIposF >> 4;
               pos_f = posIposF & B00001111;
               index_col = FeedRow(RAM[l], index_col, FetchByteInAlphabet (i, l, Alphabet2, cartel), pos_i, pos_f, spaces);
               break;
        case 3: //Alphabet3  //SingleCharII               
               if(pIpF == 0xff)
                 posIposF = FetchByteInAlphabet (i, 8, Alphabet3, cartel);//El ultimo byte tiene la informacion de donde comenzar a leer y donde terminar (cuales bits)
               else
               posIposF = pIpF;
               pos_i = posIposF >> 4;
               pos_f = posIposF & B00001111;
               index_col = FeedRow(RAM[l], index_col, FetchByteInAlphabet (i, l, Alphabet3, cartel), pos_i, pos_f, spaces);
               break;

      }// switch(Alphabet)
      i++;
   }//while
  }//for
 //Serial.println("grettings from MakeRAM");
 return index_col; //Retornando el numero de columnas del cartel 
}//int ToMakeRam(...)
/*
 * En esta ultima version se ha tratado de escribir de forma mas limpia int ToMakeRam(...), se ha logrado, no obstante se pudiera seguir mejorando
 * y también se pudiera mejorar con respecto al tiempo de ejecuccion
 */
 //---------------------------------------------------------------------------------------------------------------------
 //int PutCharInRAM(...)
 //---------------------------------------------------------------------------------------------------------------------
/*-----------------------------------------------------------------------------------------------------------------------
 * FeedRow alimenta una fila con el byte en supplier
 * los bits son colocados en la fila empezando por index hasta el final, incrementandose index durante el proceso
 * los bits en supplier son cogidos desde una posicion inical: pos_i hasta una posicion final: pos_f
 * No importa como venga la letra es colocada de forma tal que LSB queda a la derecha (visualmente en el Panel Led / Matrix Led)
 */
//int FeedRow(byte row[], int index, byte supplier, int pos_i, int pos_f, boolean space_BtwCh = true, boolean Opt_sizeCH = true)
int FeedRow(byte row[], int index, byte supplier, int pos_i, int pos_f, int spaces_BtwCh)
{
  int indx_curr_byte = index / 8;
  int indx_curr_bit = index % 8;
  byte aux;

 /*
  Serial.println(F("FeedRow - Function"));
  Serial.print(F("char: "));
  Serial.println(supplier);
  Serial.print(F("inicio: "));
  Serial.println(pos_i);
  Serial.print(F("final: "));
  Serial.println(pos_f);
  Serial.print(F("spaces_BtwCh: "));
  Serial.println(spaces_BtwCh);
 */

  //if(pos_i < pos_f) //Cuando la letra viene de un Alfabeto con el LSB a la izquierda
  if(pos_i <= pos_f)  //para cuando la letra o signo, simbolo etc, tenga una sola colummna
  {
    while(pos_i <= pos_f)  //Cuando la letra viene de un Alfabeto con el LSB a la izquierda
    {
      aux = row[indx_curr_byte];

      supplier & (1 << pos_i) ? aux = aux | (1 << (7 - indx_curr_bit)) : aux = aux &  ~ (1 << (7 - indx_curr_bit)) ;// supplier & (1 << pos_i) ... pos_i
        
      row[indx_curr_byte] = aux;
      pos_i ++;
      index++;
      indx_curr_byte = index / 8;
      indx_curr_bit = index % 8;
    }
    //Lines below added for fit with ToMakeRam_Alpha2
     while(spaces_BtwCh-- > 0)
     {
      //aux = row[indx_curr_byte];
      //aux = aux &  ~ (1 << (7 - indx_curr_bit));  //agregando un bit mas en cero, para espacio entre caracteres
      index++;  //Si la RAM estaba limpia con esto basta y no hacen falta las dos lineas de arriba que he hecho faltarian mas lineas si se quisiera hacer lo q dice ahi
     }
  }//if(pos_i < pos_f)
  else {
    while(pos_i >= pos_f)  //Cuando la letra viene de un Alfabeto con el LSB a la derecha
    {
      aux = row[indx_curr_byte];
      
      supplier & (1 << pos_i) ? aux = aux | (1 << (7 - indx_curr_bit)) : aux = aux &  ~ (1 << (7 - indx_curr_bit)) ; // supplier & (1 << pos_i) ... pos_i
      
      row[indx_curr_byte] = aux;
      pos_i --;
      index++;
      indx_curr_byte = index / 8;
      indx_curr_bit = index % 8;
    } 
     while(spaces_BtwCh-- > 0)
     {
      //aux = row[indx_curr_byte];
      //aux = aux &  ~ (1 << (7 - indx_curr_bit));  //agregando un bit mas en cero, para espacio entre caracteres
      index++;
     }
  }//else
 return index;
}//int FeedRow(...)
//-------------------------------------------------------------------------------------------------------------------------
//El LSB estará en la fila 0, fila 0 aqui es TOP... TOP to Bottom
int FeedCol(int index, byte supplier)
{
  int indx_curr_byte = index / 8;
  int indx_curr_bit = index % 8;
  byte aux;
 /*
  Serial.print(F("supplier byte: "));
  Serial.println(supplier, HEX);
  */
  for(int fila = 0; fila < 8; fila++)
  {
     aux = ledsRAM[fila][indx_curr_byte];
     
     supplier & (1 << fila) ? aux = aux | (1 << (7 - indx_curr_bit)) : aux = aux &  ~ (1 << (7 - indx_curr_bit));
     //supplier & (1 << fila) ? aux = aux | (1 << indx_curr_bit) : aux = aux &  ~ (1 <<  indx_curr_bit);
     
     ledsRAM[fila][indx_curr_byte] = aux;
     /*
     Serial.print(F("fila: "));
     Serial.print(fila);
     Serial.print(F(" byte"));
     Serial.println(indx_curr_byte);
     */
  }  
  //Serial.println(index);
  return ++index;
}
//-------------------------------------------------------------------------------------------------------------------------
//v1.1d añadido boolean 'frame' para indicar si se esta construyendo un frame de una animacion o no
//esto permite conformar frames con imagenes guardadas en arreglos de menos de 40 columnas
//y de esta froma se centran automaticmente y ocupan las 40 columnas.
int ToMakeRAMwithImage (byte IMG_info[], int indexCol, boolean frame)  
{
   int ctd_bytes = pgm_read_byte(&IMG_info[0]);//el 1er byte nos dira la ctd de bytes de la imagen
  
   if(frame)
    if(ctd_bytes < DISPLAY_COLS)
    {
      indexCol = indexCol + DISPLAY_COLS/2 - ctd_bytes / 2;
    }

   int i = 1;
   if((indexCol + ctd_bytes) < LIM_COLS)
    while(ctd_bytes > 0)
    {
     indexCol = FeedCol(indexCol, pgm_read_byte(&IMG_info[i]));
     ctd_bytes--;
     i++;
    }
    
   if(frame)
    if(indexCol % DISPLAY_COLS != 0)
    {
     int b = indexCol / DISPLAY_COLS;
     indexCol = (b + 1) * DISPLAY_COLS;
    } 
    
   return indexCol; //retornando ctd de columnas
}
//--------------------------------------------------------------------------------------------------------------------------------------
//En v1.2c ya no hace falta borrar toda la RAM, porque los efectos tanto de presentacion como de salida no dependen de bytes dejados en cero, ni antes ni despues 
//del mensaje en la memoria RAM, por lo que cada mensaje sobrescibira lo que necesite, y el resto que quede en la RAM con informacion de msjs anteriores no sera mostrado
//pues los efectos ya no sobrepasan Qcols (la ctd de cols del msj actualmente siendo mostrado).
//Solo en el caso de que el msj ocupe menos cols que DISPLAY_COLS, sera necesario limpiar el resto de los bytes hasta que los primeros DISPLAY_COLS bits 
//(o sea los primeros 40 bits, DISPLAY_COLS = 40) de la RAM queden limpios.
//v1.2d: vuelta atras a la implentacion anterior
void ClearRAM()
{
  for(int i = 0; i < 8; i++)
   for(int j = 0; j < SIZE_RAM_BYTECOLS; j++)
   //for(int j = 0; j < DISPLAY_COLS / 8; j++)  //v1.2c
    ledsRAM[i][j] = 0;
}
//------------------------------------------------------------------------------------------------------------------------------------------
//Uso para debuggeo...
/*
void DisplayRAM()
{
  Serial.println(F("Volcando RAM (primeros 8 x 5 bytes):"));
  Serial.println(F("-------------------------------------"));
  for(int i = 0; i < 8; i++)
  {
   for(int j = 0; j < 5; j++)
   {
    Serial.print(ledsRAM[i][j], BIN);
    Serial.print(" ");
   }
   Serial.println();
  }
}
*/
