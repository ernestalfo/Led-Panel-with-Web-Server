#ifndef GLOBAL_H
#define GLOBAL_H

ESP8266WebServer server(80);									//The Webserver
//boolean firstStart = true;								  //On firststart = true, NTP will try to get a valid time

//#define CTDCARTELES 10
#define CTDCARTELES 15
//#define Twait 1000
#define Twait 150 //v1.2
//#define TwaitIPshowing 30000 //
#define TwaitIPshowing 2400   //v1.1d

//#define BAUDIOS 4800 //v1.2
#define BAUDIOS 19200 //v1.2

int index_cartel = 1;  //usado para el sitio web 1/15  

int state = 0;
int prv_state = 0; //v1.4

int index_cartelRXTX = 0;  

//Profiles:
int index_profile = 0;
//String ProfileNames[5];
int profileByDefault = 0;

String FileNameDefault = "CONFIG_MSGS_FILE_P1";  //por defecto perfil 1... 
String FileName = "CONFIG_MSGS_FILE_P1"; //por defecto perfil 1...

//String NamesProfiles[5]={"guayaba","platano","mango","fresa","melocoton"};


//-------------------------------------------------------------------------------
//Clock:
String TimeStr = "--:--:--";
//TClock MyClock;
String IntervalStr = "0";
String ShowTStr = "0";
//String TimeModeStr = "0";
int TimeMode = 0;
unsigned long int Interval = 0;
unsigned long int ShowT = 30000;
boolean enviarTime = false;
unsigned long int t_x = 0, t_interval = 0;
int CTDmsgs_utiles = 0;
int idx_utiles = 0;
boolean KeepTimeUpdated = false;
boolean CycleCompleted = false;
//boolean interval_trigger = false;  //v1.4 fue eliminada del resto del codigo en esta version
unsigned long int t_last_timesended = 0;
unsigned long int actualShowT;
//v1.4:
boolean MilitaryStd = false;
boolean MilitaryStd_PRV = false;
int TimeMode_PRV = 0;
unsigned long int Interval_PRV = 0;
unsigned long int ShowT_PRV = 0;
//-------------------------------------------------------------------------------


/*
struct TMsg_Config
{
  String msg = "";
  byte ABC = 0; //tipo de letra
  byte E = 0; //Efecto: Ninguno/Estacionario/Left Scroll/Dual Scroll
  byte Exit = 0; //Tipo de salida, ninguna, left, right
  int TScrollExit = 0; //scrolling time en la salida
  byte spaces = 0; // espacios entre caracteres
  byte offset = 0; // columnas de offset, deben ser menor de 8, si hiciera falta mas usar espacios en blanco
  byte inverse = 0; //true, false, toggle  //inverse true significa q las letras se conformaran apagando los leds  encendiendo los otros
  byte Nloops = 0; //veces que se mostrara el mensaje
  int TScrollE = 0;  //tiempo de desplazamento de la marquesina
  int TInitial = 0; //tiempo inicial antes de empezar a desplazarse
  int TEnd = 0; //tiemp en el que el cartel se queda fijo en la ultima repeticion antes de salir por la izq por la derecha, o simplemente se muestre el q viene a continuacion
  int Tprv_rev = 0; //en el efecto "dual scroll" el cartel se desplaza para la izq y para la der hasta llegar a los extremos, este es el tiempo una vez llegado al extremo antes de
               //cambiar de direccion
  int Tprv_fwd = 0;
};
*/
struct TMsg_Config_web
{
  String msg = "";
  String ABC = "0"; //tipo de letra  //
  String E = "2"; //Efecto: Ninguno/Estacionario/Left Scroll/Dual Scroll/Animation/Blink
  String IN = "0"; //Tipo de entrada: ninguna, left  ------------------------------  //new in v1.5
  String Exit = "0"; //Tipo de salida, ninguna, left, right, rightII
  String TScrollExit = "100"; //scrolling time en la salida
  String TScrollIN = "130"; //srolling time en la entrada -------------------------  //new in v1.5
  String spaces = "1"; // espacios entre caracteres  //un espacio entre caracteres por defecto
  String offset = "0"; // columnas de offset, deben ser menor de 8, si hiciera falta mas usar espacios en blanco
  String inverse = "0"; //false, true, toggle  //inverse true significa q las letras se conformaran apagando los leds  encendiendo los otros
  String Nloops = "1"; //veces que se mostrara el mensaje
  String TScrollE = "130";  //tiempo de desplazamento de la marquesina, efectos
  String TInitial = "1500"; //tiempo inicial antes de empezar a desplazarse
  String TEnd = "1500"; //tiemp en el que el cartel se queda fijo en la ultima repeticion antes de salir por la izq por la derecha, o simplemente se muestre el q viene a continuacion
  String Tprv_rev = "400"; //en el efecto "dual scroll" el cartel se desplaza para la izq y para la der hasta llegar a los extremos, este es el tiempo una vez llegado al extremo antes de
               //cambiar de direccion
  String Tprv_fwd = "400";
};

//TMsg_Config ConfigWS[CTDCARTELES];
TMsg_Config_web ConfigWS[CTDCARTELES];

TMsg_Config_web ConfigRXTX[CTDCARTELES];

void ClearConfig() //Se limpia la config de la RAM del Web Server
{
  //poniendo valores por defecto...
  index_cartel = 1;
  for(int i = 0; i < CTDCARTELES; i++)
  {
   ConfigWS[i].msg = "";
   ConfigWS[i].ABC = "0";
   ConfigWS[i].E = "2";
   ConfigWS[i].IN= "0";
   ConfigWS[i].Exit = "0"; 
   ConfigWS[i].TScrollExit = "100"; 
   ConfigWS[i].TScrollIN = "130"; 
   ConfigWS[i].spaces = "1"; 
   ConfigWS[i].offset = "0"; 
   ConfigWS[i].inverse = "0";
   ConfigWS[i].Nloops = "1";
   ConfigWS[i].TScrollE = "130";  
   ConfigWS[i].TInitial = "1500"; 
   ConfigWS[i].TEnd = "1500"; 
   ConfigWS[i].Tprv_rev = "400"; 
   ConfigWS[i].Tprv_fwd = "400";
  }
}//void ClearConfig()
//-----------------------------------------------
void PanelSync()
{
  int i = 0, ctduseful = 0;
  
  while(i < CTDCARTELES)
  {
    if(ConfigRXTX[i].msg.length() > 0)
     ctduseful++;
    i++;
  }
  CTDmsgs_utiles = ctduseful;
  idx_utiles = 0;
            
  Serial.print("<S:" + String(ctduseful) + ":");
  Serial.write(0x0D);
            
  index_cartelRXTX = 0;
  CycleCompleted = false;
}
//-----------------------------------------------
//En un futuro el estado del panel llevado ahora en las variables state y prv_state, pudiera ser implementado en una clase
void ChangePanelState(int newstate)
{
  if(state != 6)
  {
    prv_state = state;
    state = newstate;
  }
  else
   prv_state = newstate;
}
//----------------------------------------------------
//La funcion RetrieveState sera usada para recuperar el estado anterior o deseado cuando se encienda el panel
//por ejemplo al cambiar los settings del reloj, se esta poniendo un estado deseado...
void RetrieveState()
{
  state = prv_state;
}

#endif
