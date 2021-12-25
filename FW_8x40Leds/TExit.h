//clase base para los efectos de salida
//En estos efectos se limpia la imagen en la pantalla, o sea al terminar de salir el cartel la pantalla se queda en blanco
//************************
//*** Clase base Exit ****
//************************
class ExitScroll
{
  public:
  unsigned long int t_last;
  TCartel *Target;
  int Tscrll;
  int state;
  ExitScroll(TCartel *MiCartel,  int tS)
  {
    Target = MiCartel;
    Tscrll = tS;
    state = 0; //estado inicial
    if(Target != NULL)
     Target -> marquesina = false;  //v1.1d2
  };
  virtual boolean Update(unsigned long int t) = 0;
  void SetInitialTime(unsigned long int t);
};
//------------------------------------------------------------------------------------------------------------------------------------------------
ExitScroll *MyExit = NULL;
//-------------------------------------------------------------------------------------------------------------------------------------------------
void ExitScroll::SetInitialTime(unsigned long int t)
{
  t_last = t;
  state = 0;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------
//*** Exit Left ****
//********************
class ExitScrollLeft : public ExitScroll
{
  public:
  ExitScrollLeft(TCartel *MiCartel,  int tS);
  boolean Update(unsigned long int t);
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ExitScrollLeft::ExitScrollLeft(TCartel *MiCartel,  int tS)
:ExitScroll(MiCartel, tS)
{
  if(Target != NULL)
  {
   //Target -> bit_col_inic = (SIZE_RAM_BYTECOLS * 8 - 88) + Target -> Qcols - Target -> actualQcols; //v1.1d rev plus
   //pudiera borrarse primero los proximos 40 bytes, pero bueno... //se asume qu estan limpios
   //Target -> Qcols = Target -> bit_col_inic + 80; //Creo que ya no es necesario, probemos...  //v1.2c
   Target -> delta_col = 1; //Left
  }//if(Target != NULL)
  t_last = millis();
}//ExitScroll::ExitScrollLeft(TCartel *MiCartel,  int tS)
//----------------------------------------------------------------------------------------------------------
boolean ExitScrollLeft::Update(unsigned long int t)
{
  if(Target != NULL)
  {
   switch(state)
   {
    case 0: //Estado inicial
            if(Target -> bit_col_inic == Target -> Qcols)  //v1.2c
            //if(Target -> bit_col_inic == Target -> Qcols - 40)
            //if(Target -> bit_col_inic == Target -> actualQcols - 40)  //v1.1d rev plus  //wrong
            {
              Target ->delta_col = 0;
              state = 1;
              //Serial.println("finito...");
              return false;
            }
            if(((unsigned long int)(t - t_last)) > Tscrll)
            {
              t_last = t;
              Target ->delta_col = 1;
              scrolling = true; //variable global
            }
            break;
    case 1: //Empty State
            return false;
            break;
   }//switch
   return true;
  }//if(Target != NULL)
  return false;
}//boolean ExitScroll::Update(unsigned long int t)
//----------------------------------------------------------------------------------------------------------
//*** Exit Right ***
//******************
class ExitScrollRight : public ExitScroll
{
  public:
  ExitScrollRight(TCartel *MiCartel,  int tS);
  boolean Update(unsigned long int t);
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ExitScrollRight::ExitScrollRight(TCartel *MiCartel,  int tS)
:ExitScroll(MiCartel, tS)
{
  if(Target != NULL)
  {
    int byteCol = Target -> bit_col_inic / 8;
    int bitCol = Target -> bit_col_inic % 8;
       
    for(int i = byteCol - 1; i >= 0 && i > byteCol - 6;i--)  //revisar q solo borre lo necesario nada mas ---  done!
     for(int row = 0; row < 8; row++)
      ledsRAM[row][i] = 0; //Clearing...

    if(bitCol != 0)
    {
      byte mask = B11111111 >> bitCol;
      
      for(int row = 0; row < 8; row++)
        ledsRAM[row][byteCol] = mask & ledsRAM[row][byteCol]; //Clearing...
    }
       
   Target -> Qcols = Target -> bit_col_inic + DISPLAY_COLS;
   Target -> IndexLastByte = Target -> Qcols / 8;   //por buena practica, cada vez que se afecta Qcol que se actualice IndexLastByte
   Target -> delta_col = -1; //Right
  }//if(Target != NULL)
  t_last = millis();
}//ExitScroll::ExitScrollRight(TCartel *MiCartel,  int tS)
//----------------------------------------------------------------------------------------------------------
boolean ExitScrollRight::Update(unsigned long int t)
{
  if(Target != NULL)
  {
   switch(state)
   {
    case 0: //Estado inicial
            if(Target -> bit_col_inic == Target -> Qcols - (DISPLAY_COLS*2))
            //if(Target -> bit_col_inic == Target -> actualQcols - 80)  //1.1d rev plus  //wrong
            {
              Target ->delta_col = 0;
              state = 1;
              //Serial.println("finito...");
              return false;
            }
            if(((unsigned long int)(t - t_last)) > Tscrll)
            {
              t_last = t;
              Target ->delta_col = -1;
              scrolling = true; //variable global
            }
            break;
    case 1: //Empty State
            return false;
            break;
   }//switch
   return true;
  }//if(Target != NULL)
  return false;
}//boolean ExitScrollRight::Update(unsigned long int t)
//----------------------------------------------------------------------------------------------------------
//*** Exit Right II / Exit Right B ***
//*****************************************
//Added in ver 1.1d2
class ExitScrollRightB : public ExitScroll
{
  public:
  ExitScrollRightB(TCartel *MiCartel,  int tS);
  boolean Update(unsigned long int t);
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ExitScrollRightB::ExitScrollRightB(TCartel *MiCartel,  int tS)
:ExitScroll(MiCartel, tS)
{
  if(Target != NULL)
  {
   //-- por ahora mas nada que implementar aqui
   //No borra nada porq esta vez le da salida al cartel completo, no solo a lo q esta en pantalla
   Target -> delta_col = -1; //Right
  }//if(Target != NULL)
  t_last = millis();
}//ExitScroll::ExitScrollRightB(TCartel *MiCartel,  int tS)
//----------------------------------------------------------------------------------------------------------
boolean ExitScrollRightB::Update(unsigned long int t)
{
  if(Target != NULL)
  {
   switch(state)
   {
    case 0: //Estado inicial
            if(Target -> bit_col_inic == -DISPLAY_COLS) //Condiccion de parada
            {
              Target ->delta_col = 0;
              state = 1;
              //Serial.println("finito...");
              return false;
            }
            if(((unsigned long int)(t - t_last)) > Tscrll)
            {
              t_last = t;
              Target ->delta_col = -1;
              scrolling = true; //variable global
            }
            break;
    case 1: //Empty State
            return false;
            break;
   }//switch
   return true;
  }//if(Target != NULL)
  return false;
}//boolean ExitScrollRightB::Update(unsigned long int t)
//----------------------------------------------------------------------------------------------------------
//#endif
