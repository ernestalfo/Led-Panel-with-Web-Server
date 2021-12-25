//clase base para los efectos de entrada
/************************************
 * Clase Base EntranceScroll  ********
 * **********************************
 */
class EntranceScroll
{
  public:
  unsigned long int t_last;
  TCartel *Target;
  int Tscrll;
  int state;
  EntranceScroll(TCartel *MiCartel,  int tS)
  {
    t_last = millis();
    Tscrll = tS;
    state = 0;  //estado incial
    Target = MiCartel;
  };
  virtual boolean Update(unsigned long int t) = 0;
  void SetInitialTime(unsigned long int t);
};
//------------------------------------------------------------------------------------------------------------------------------------------------
EntranceScroll *MyEntrance = NULL;
//------------------------------------------------------------------------------------------------------------------------------------------------
void EntranceScroll::SetInitialTime(unsigned long int t)
{
  t_last = t;
  state = 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------
//Entra por la derecha y camina pa la izq, pero ENTRA POR LA DERECHA
class EntranceRight: public EntranceScroll
{
  public:
  EntranceRight(TCartel *MiCartel,  int tS)
  : EntranceScroll(MiCartel, tS)
  {
    if(Target != NULL) //buscando condicciones para el estado cero, state = 0
    {
      Target -> bit_col_inic = -DISPLAY_COLS; 
      //Target -> marquesina = false; //Ya viene en falso pero bueno...
      Target -> delta_col = 1;
    }
  };
  virtual boolean Update(unsigned long int t);
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------
boolean EntranceRight::Update(unsigned long int t)
{
  //Serial.println(F("AAAAAA - Hello from EntranceScroll Update(t)"));
  if(Target != NULL)
  {
   //Serial.println(F("BBB - Hello from EntranceScroll Update(t)"));
   switch(state)
   {
    case 0: //Estado Inicial , se esta trayendo el msj a posicion, en col incicial cero.
            if(Target -> bit_col_inic == 0)
            {
              scrolling = false;
              //Target ->  marquesina = true; 
              //Target ->delta_col = 0;
              state = 1; //Msj en posición.
              return false;
            }
            else if(((unsigned long int)(t - t_last)) > Tscrll)
            {
              t_last = t;
              scrolling = true;
              //Serial.println(F("Scrolling from Entrance"));
            }
            break;
    case 1: //Empty State // o Ninguno
            return false;
            break;  //Esta demas pero bueno....
   }//switch
   return true;
  }//if(Target != NULL)
  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------
//Entra por la izq y camina pa la derecha, pero ENTRA POR LA IZQ
class EntranceLeft: public EntranceScroll
{
  public:
  EntranceLeft(TCartel *MiCartel,  int tS)
  : EntranceScroll(MiCartel, tS)
  {
    if(Target != NULL) //buscando condicciones para el estado cero, state = 0
    {
      Target -> bit_col_inic = Target -> Qcols; 
      //Target -> marquesina = true; //Probando....
      //Target -> pls = true;
      Target -> delta_col = -1;
    }
  };
  virtual boolean Update(unsigned long int t);
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------
boolean EntranceLeft::Update(unsigned long int t)
{
  //Serial.println(F("AAAAAA - Hello from EntranceScroll Update(t)"));
  if(Target != NULL)
  {
   //Serial.println(F("BBB - Hello from EntranceScroll Update(t)"));
   switch(state)
   {
    case 0: //Estado Inicial , se esta trayendo el msj a posicion, en col incicial cero.
            if(Target -> bit_col_inic == 0)
            {
              scrolling = false;
              //Target ->  marquesina = true; 
              //Target ->delta_col = 0;
              //Target -> pls = false;
              state = 1; //Msj en posición.
              return false;
            }
            else if(((unsigned long int)(t - t_last)) > Tscrll)
            {
              t_last = t;
              scrolling = true;
              //Serial.println(F("Scrolling from Entrance"));
            }
            break;
    case 1: //Empty State // o Ninguno
            return false;
            break;  //Esta demas pero bueno....
   }//switch
   return true;
  }//if(Target != NULL)
  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------
