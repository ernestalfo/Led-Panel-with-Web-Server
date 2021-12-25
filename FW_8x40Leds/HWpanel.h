/*
 * Por ahora solo definiciones y pocas funciones, a lo mejor despues coge mas peso este fichero
 * De forma tal que a lo mejor sea facil cambiar la configuracion actual de HW
 */

//*** Columnas desde la 0 a la 39, y empienzan con la col 0 siendo la primera ,siendo D0 ,si nos imaginamos los registros en serie como uno solo y sus salidas D0 a D39
//Si se cambiara la ubicacion fisica del cartel; esto se arreglaria en la parte donde se lee de la RAM y se empieza a tirar al reg de desplazaiento, actualmente se 
//lee primero al final de la RAM, porq D0 corresponde con C0 (col 0) y D39(ultimo pin del reg de desp) con c39, y D39 se debe escribir siempre de primero en el registro
//, si D39 correspondiera con C0 habria que leer primero el inicio de la RAM. Siempre que mantengamos la imagen en la RAM en ese oreden dese C0 hasta C39, q lo mejor es dejarlo asi
//, que la RAM siempre corresponda a una represantacion directa de lo que se vera fisicamente en el Display, sin q este invertida u otras... **** 

//5 bytes en cada fila 5 * 8 ---> 40 leds
//8 filas ---> 8 * 40 ---> 320 leds

#include <SPI.h>

int filas_pins[8] = {2, 3, 4, 5, 6, 7, 8, 9}; //2 ---> Top ... 9 ---> Bottom

#define SCK 13     // 
#define RCK 10     // 
#define SER 11     //

#define DISPLAY_COLS 40  //Supuestamente si se tiene un panel con mas columnas solo se debe poner ese numero aqui, que sea multiplo de 8 please...
//#define DISPLAY_COLS 128  //Probando...  //OK
#define TimeOutCom 2500
//#define CTDCARTELES 10  //maxima cantidad
#define CTDCARTELES 15  //maxima cantidad4
#define FW_VER "v1.6b"

//unsigned long int precisionCounter = 0;   //Usado para ver si con 128 cols, da tiempo a subdividir el tiempo de refresh...

int CTDuseful = 0; //la ctd q esta activa, los q tienen info en el msg, el campo msg no esta vacío...
int PanelState  = 0; //0 estado inicial

#define BAUDIOS 19200
#define CHMAX 200 //added v1.2

boolean toggleArray[CTDCARTELES];
int indexToggle = 0;

struct TMsg_Config
{
  //String msg = "";
  //String msg = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_ `abcdefghijklmnopqrstuvwxyz{|}~"; //debuggeo
                 //Esta es una prueba a ver cuantas letras se pueden mandar, y que el panel de leds funcione bien como se espera. 123
  //String msg = "ñáéíóú"; ///debuggeo
  char msg[CHMAX];  //v1.2 Se aboga por el uso de este arreglo y no por objectos de tipo string
  byte ABC = 0; //tipo de letra   0, 1 , 2 , 3 y 4 para imagenes
  byte E = 0; //Efecto: Ninguno, Estacionario, Left Scroll, Dual Scroll, Animation, Blink
  byte IN = 0; //Entrada: Ninguna , Left
  byte Exit = 0; //Tipo de salida, ninguna, left, right, rightB
  byte spaces = 1; // espacios entre caracteres  //puesto a 1 por defecto en v1.2b
  byte offset = 0; // columnas de offset, deben ser menor de 8, si hiciera falta mas usar espacios en blanco
  byte inverse = 0; //true, false, toggle  //inverse true significa q las letras se conformaran apagando los leds  encendiendo los otros
  byte Nloops = 0; //veces que se mostrara el mensaje
  int TScrollExit = 0; //scrolling time en la salida
  int TScrollIn = 0; //scrolling time en la entrada
  int TScrollE = 0;  //tiempo de desplazamento de la marquesina
  unsigned long int TInitial = 0; //tiempo inicial antes de empezar a desplazarse  //tambien el tiempo total en efecto estatico
  int TEnd = 0; //tiemp en el que el cartel se queda fijo en la ultima repeticion antes de salir por la izq por la deRecha, o simplemente se muestre el q viene a continuacion
  int Tprv_rev = 0; //en el efecto "dual scroll" el cartel se desplaza para la izq y para la der hasta llegar a los extremos, este es el tiempo una vez llegado al extremo antes de
                   //cambiar de diReccion
  int Tprv_fwd = 0;
}ConfigMSG;

void InicializarPanel()
{
 for(int i = 2; i < 10; i++)
 {
  pinMode(i, OUTPUT);
  digitalWrite(i, LOW); //Turning OFF BD682 through ULN2803
 }
 pinMode(SCK, OUTPUT);
 pinMode(RCK, OUTPUT);
 pinMode(SER, OUTPUT);
 digitalWrite(SCK, LOW);
 digitalWrite(RCK, LOW);
 
 for(int i = 0; i < CTDCARTELES; i++)
  toggleArray[i] == false;  


}//void InicializarPanel()
//----------------------------------------------------------------------------------
