//clase base para los efectos...
/************************************
 * Clase Base Display Efect  ********
 * **********************************
 */
class DisplayEfect
{ 
  public:
  unsigned long int t_last;
  int reps;
  int loops_show;
  TCartel *Target;
  unsigned long int TexpI;  //tiempo de exposicion inicial antes de comenzar el scrolling, pero sera tambien el tiempo de exposicion total en efecto estacionario
  //int TexpF;
  unsigned long int TexpF;  //v1.3
  int Tscrll;
  boolean trigger;
  int state;
  //DisplayEfect(TCartel *MiCartel, int tI, int tF, int tS, int Nloops);
  DisplayEfect(TCartel *MiCartel, unsigned long int tI, unsigned long int tF, int tS, int Nloops);  //v1.3
  DisplayEfect(TCartel *MiCartel,unsigned long int tI);
  virtual boolean Update(unsigned long int t) = 0;
  void SetInitialTime(unsigned long int t);
  
};
//---------------------------------------------------------------------------------------------------------------------------------------------
DisplayEfect *MiEfecto = NULL;
//----------------------------------------------------------------------------------------------------------------------------------------------
//Este constructor lo usa DisplayEfectStatic
DisplayEfect::DisplayEfect(TCartel *MiCartel,unsigned long int tI)
{
  t_last = millis();
  state = 0;
  Target = MiCartel;
  TexpI = tI; //t en ms
  if(Target != NULL)
  {
   Target -> bit_col_inic = 0;  //para que se corresponda con el estado cero, 0
   Target -> delta_col = 0;
  }
}
//----------------------------------------------------------------------------------------------------------------------------------------------
//DisplayEfect::DisplayEfect(TCartel *MiCartel,int tI, int tF, int tS, int Nloops)
DisplayEfect::DisplayEfect(TCartel *MiCartel,unsigned long int tI,unsigned long int tF, int tS, int Nloops) //v1.3
{
  t_last = millis();
  Target = MiCartel;
  reps = 0;
  if(Nloops < 1)
    loops_show = 1;  //v1.2 Validacion añadida
  else
   loops_show = Nloops;   
  trigger = false;
  TexpI = tI; //t en ms
  TexpF = tF;
  state = 0;
  Tscrll = tS;
  if(Target != NULL)
  {
   Target -> bit_col_inic = 0;  //para que se corresponda con el estado cero, 0
   Target -> delta_col = 0;
  }
}
//----------------------------------------------------------------------------------------------------------------------------------------------
void DisplayEfect::SetInitialTime(unsigned long int t)
{
  t_last = t;
  state = 0; //Added...
  if(Target != NULL)
  {
   Target -> bit_col_inic = 0;  //para que se corresponda con el estado cero, 0
   Target -> delta_col = 0;
  }
}
//----------------------------------------------------------------------------------------------------------------------------------------------
/*********************************************
 *  Display Efect Left (Left Scroll)  ********
 * *******************************************
 */
class DisplayEfectLeft : public DisplayEfect
{
  public:
  DisplayEfectLeft(TCartel *MiCartel,unsigned long int tI, unsigned long int tF, int tS, int Nloops );
  virtual boolean Update(unsigned long int t);
};
//----------------------------------------------------------------------------------------------------------------------------------------------
DisplayEfectLeft::DisplayEfectLeft(TCartel *MiCartel,unsigned long int tI, unsigned long int tF, int tS, int Nloops)
:DisplayEfect(MiCartel, tI, tF, tS, Nloops)
{
 //Todo ya se hace en el constructor de la clase base...  //ya no v1.d2
 if(Target != NULL)
  Target -> marquesina = true;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
boolean DisplayEfectLeft::Update(unsigned long int t)
{
  if(Target != NULL)
  {
   switch(state)
   {
    case 0: //Estado inicial
            if(((unsigned long int)(t - t_last)) > TexpI)
            {
              t_last = t;
              state = 1; // state++;
              Target ->delta_col = 1;
              scrolling = true; //variable global
              //Serial.println("Initial Show Time Elpased");
            }
            break;
    case 1: //Estado Normal //Scrolling
            if(Target -> bit_col_inic == Target -> Qcols - DISPLAY_COLS) 
            {                                                                                                                                 
              if(!trigger)
              {
                //Serial.println(F("trigger!!!"));
                trigger = true;
                reps++;
              }
              if(reps == loops_show)
              {
               //Serial.println(F("final..."));
               reps = 0;
               state = 2;
               Target ->delta_col = 0;
               return true;
              }
            }
            else
             trigger = false;
            if(((unsigned long int)(t - t_last)) > Tscrll)
            {
              //Serial.println("Scrolling...");
              t_last = t;
              scrolling = true;
              /*
              if(Target -> bit_col_inic == Target -> Qcols)
              {
              
              }
              */  //Mejor en refresh()
            }
            break;
    case 2: //Estado Final
            if(((unsigned long int)(t - t_last)) > TexpF)
            {
              t_last = t;
              state = 3; // state++;
              Target ->delta_col = 0;
              return false;
            }
            break;
    case 3: //Empty State // o Ninguno
            return false;
            break;  //Esta demas pero bueno....
   }//switch
   return true;
  }//if(Target != NULL)
  return false;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------
/*********************************************
 *  Display Efect Dual (Dual Scroll)  ********
 * *******************************************
 */
class DisplayEfectDual : public DisplayEfect
{
  protected:
  int Tprv_rev;  //nuevo campo para este efecto
  int Tprv_fwd;  //nuevo campo para este efecto
  public:
  //DisplayEfectDual(TCartel *MiCartel, int tI, int tF, int tS, int Nloops, int tIb, int tFb);
  DisplayEfectDual(TCartel *MiCartel, unsigned long int tI, unsigned long int tF, int tS, int Nloops, int tIb, int tFb);  //v1.3
  virtual boolean Update(unsigned long int t); 
};
//---------------------------------------------------------------------------------------------------------------------------------------------
DisplayEfectDual::DisplayEfectDual(TCartel *MiCartel,unsigned long int tI, unsigned long int tF, int tS, int Nloops, int tIb, int tFb)
:DisplayEfect(MiCartel, tI, tF, tS, Nloops)
{
  //Aparte de las inicializaciones en el constructor de la clase base
  Tprv_rev = tFb;
  Tprv_fwd = tIb;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
//v1.2c: Realmente no es necesario el uso de trigger aqui
//Por otro lado ya no se quedaria estancado si el cartel tiene menos de DISPLAY_COLS columnas
boolean DisplayEfectDual::Update(unsigned long int t)
{
  if(Target != NULL)
  {
   switch(state)
   {
    case 0: //Estado inicial
            if(((unsigned long int)(t - t_last)) > TexpI)
            {
              t_last = t;
              state = 1; // state++;
              Target ->delta_col = 1;
              scrolling = true; //variable global
              //Serial.println(F("Initial Show Time Elpased"));
            }
            break;
    case 1: //Scrolling Fwd
            //if(Target -> bit_col_inic == Target -> Qcols - 40)
            if(Target -> bit_col_inic == Target -> actualQcols - DISPLAY_COLS)  //v1.1d rev plus
            {
              /*
              if(!trigger)
              {
                trigger = true;
                reps++;
                Serial.println(F("trigger!!!"));
              }
              */ //v1.2c
              reps++;  //v1.2c
              if(reps == loops_show)
              {
               //Serial.println(F("final..."));
               reps = 0;
               state = 2;  //Estado Final
               Target ->delta_col = 0;
               return true;
              }
              else
              {
                state = 5;  //prev rev
                t_last = t;
                Target ->delta_col = 0;
                //state = 4;
                //Target ->delta_col = -1;
              }
            }//if(Target -> bit_col_inic == Target -> actualQcols - DISPLAY_COLS)
            //else
             //trigger = false;
            if(((unsigned long int)(t - t_last)) > Tscrll)
            {
              //Serial.println(F("Scrolling..."));
              t_last = t;
              scrolling = true;
            }
            break;
    case 2: //Estado Final
            if(((unsigned long int)(t - t_last)) > TexpF)
            {
              t_last = t;
              state = 3; // state++;
              Target ->delta_col = 0;
              return false;
            }
            break;
    case 3: //Empty State // o Ninguno
            return false;
            break;  //Esta demas pero bueno....
    case 4: //Scrolling Rev  
            if(Target -> bit_col_inic == 0)
            {
               t_last = t;
               state = 6; //prev fwd
               Target ->delta_col = 0;
               //state = 1;
               //Target ->delta_col = 1;
            }
            if(((unsigned long int)(t - t_last)) > Tscrll)
            {
              //Serial.println("Scrolling...");
              t_last = t;
              scrolling = true;
            }
            break;
    case 5: //prev_rev
            if(((unsigned long int)(t - t_last)) > Tprv_rev)
            {
              t_last = t;
              state = 4;  //Scrolling Rev 
              Target ->delta_col = -1;
            }
            break;
     case 6: //prev_fwd
            if(((unsigned long int)(t - t_last)) > Tprv_fwd)
            {
              t_last = t;
              state = 1; //Scrolling Fwd
              Target ->delta_col = 1;
            }
            break;
   }//switch
   return true;
  }//if(Target != NULL)
  return false;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
/*********************************************
 *  Display Efect Static (Not Scroll)  *******
 * *******************************************
 */
class DisplayEfectStatic : public DisplayEfect
{
  public:
   DisplayEfectStatic(TCartel *MiCartel,unsigned long int tI)
  :DisplayEfect(MiCartel, tI)
  {}; //Todas las inicializaciones en la clase base
  virtual boolean Update(unsigned long int t);
};
//-------------------------------------------------------------------------------------------------------------------------------------------------------
boolean DisplayEfectStatic::Update(unsigned long int t)
{
  if(Target != NULL)
  {
   switch(state)
   {
    case 0: //Estado inicial
            if(((unsigned long int)(t - t_last)) > TexpI)
            {
              state = 3; 
              //Serial.println("Time is over...");
              return false;
            }
            break;
    case 3: //Empty State // o Ninguno
            return false;
            break;  //Esta demas pero bueno....
   }//switch
   return true;
  }//if(Target != NULL)
  return false;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------
/************************************************************************************************
 *  Display Efect Animation (Imagenes animadas)(cada imagen es un frame de la animacion)  *******
 * **********************************************************************************************
 */
//Efecto pensado para las animaciones
class DisplayEfectAnimation : public DisplayEfect
{
  public:
  DisplayEfectAnimation(TCartel *MiCartel,unsigned long int tI, unsigned long int tF, int tS, int Nloops );
  virtual boolean Update(unsigned long int t);
};
//----------------------------------------------------------------------------------------------------------------------------------------------
DisplayEfectAnimation::DisplayEfectAnimation(TCartel *MiCartel,unsigned long int tI, unsigned long int tF, int tS, int Nloops)
:DisplayEfect(MiCartel, tI, tF, tS, Nloops)
{
  if(Target != NULL)
   Target -> marquesina = true;
} 
//----------------------------------------------------------------------------------------------------------------------------------------------------
boolean DisplayEfectAnimation::Update(unsigned long int t)
{
  if(Target != NULL)
  {
   switch(state)
   {
    case 0: //Estado inicial
            if(((unsigned long int)(t - t_last)) > TexpI)
            {
              t_last = t;
              state = 1; // state++;
              Target ->delta_col = DISPLAY_COLS; //Este efecto es una copia de EfectLeft colo que aquí es delta_col = 40 y allá es delta_col = 1
              scrolling = true; //variable global
              //Serial.println("Initial Show Time Elpased");
            }
            break;
    case 1: //Estado Normal //Scrolling
            if(Target -> bit_col_inic == Target -> Qcols - DISPLAY_COLS)
            {
              if(!trigger)
              {
                trigger = true;
                reps++;
              }
              if(reps == loops_show)
              {
               //Serial.println("final...");
               reps = 0;
               state = 2;
               Target ->delta_col = 0;
               return true;
              }
            }
            else
             trigger = false;
            if(((unsigned long int)(t - t_last)) > Tscrll)
            {
              //Serial.println("Scrolling...");
              t_last = t;
              scrolling = true;
            }
            break;
    case 2: //Estado Final
            if(((unsigned long int)(t - t_last)) > TexpF)
            {
              t_last = t;
              state = 3; // state++;
              Target ->delta_col = 0;
              return false;
            }
            break;
    case 3: //Empty State // o Ninguno
            return false;
            break;  //Esta demas pero bueno....
   }//switch
   return true;
  }//if(Target != NULL)
  return false;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
/*********************************************
 *  Display Efect Blink  *********************
 * *******************************************
 */
class DisplayEfectBlink : public DisplayEfect
{
  public:
   DisplayEfectBlink(TCartel *MiCartel, unsigned long int tI, unsigned long int tF, int tS, int Nloops )
  :DisplayEfect(MiCartel, tI, tF, tS, Nloops)
  {}; //Todas las inicializaciones en la clase base
  virtual boolean Update(unsigned long int t);
};
//-------------------------------------------------------------------------------------------------------------------------------------------------------
boolean DisplayEfectBlink::Update(unsigned long int t)
{
  if(Target != NULL)
  {
   switch(state)
   {
    case 0: //Estado inicial
            if(((unsigned long int)(t - t_last)) > Tscrll)
            {
              reps++;
              if(reps == loops_show) //Empty State // o Ninguno
              {
               state = 3; //Empty State 
               return false;
              }
              else
              {
                t_last = t;
                Target -> ToggleInverse = true;
              } 
            }
            break;
    case 3: //Empty State // o Ninguno
            return false;
            break;  //Esta demas pero bueno....
   }//switch
   return true;
  }//if(Target != NULL)
  return false;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------
 
