/* *** modulo de la version full nano + nodeMCU v1.6 ***
 *  //!!!!!!!OJO: FAVOR DE ACTUALIZAR NUMERO DE VERSION EN RESPUESTA A CMND <V
 *  v1.6b
 *  - Agregado parametro boolean pls a TCartel, como parche en Refresh() para que funcione bien la entrada por la izq
 *  - Terminando de ajustar ToMakeRAMwithAlphabet y la nueva funcion FetchByteInAlphabet, con las nuevas optimizaciones en el tamaño del código
 *  - Mejora en la implementación de las intensidades (trabajando al 25, 50, 75 y 100), Se trata de subdividir el tiempo de la IT
 *  - Agregado comando para saber el nuemro de version y otros datos como ctd de columnas del panel.
 *  v1.6
 *  - Tratando de crear efecto de entrada por la izquierda, faltaria probarlo
 *  - Borrando algunos comentarios en Refresh(), ver versiones anteriores para cualquier duda..., falta probar
 *  - Tratando de escribir un poco mas corta funcion ToMakeRAMwithAlphabet, para que sea mas facil su mtto e incorporacion de nuevos caracteres, ya esta,falta probar
 *  v1.5
 *  
 *  - Eliminando DualScrollRin y LeftScrollRin, creando nueva clase entrada, que permitira enrar por la derecha cualquier cartel y combinarlo con cualquier otro efecto
 *     * nueva clase: EntranceScroll
 *  
 *  v1.4
 *  
 *  - Arreglando bug con toggle y demas, en algunas ocasiones on toggleIndex = 0 se realizaba esta operacion toggleArray[indexToggle - 1] = ..., por lo que traia problemas indexar -1 aqui...
 *  - Preparando el firmware para trabajar con distintas intensidades (25, 50, 75 y 100), -- ok
 *  - Probado 25% de Intensidad y 128 cols -- ok
 *  
 *  v1.3
 *  - cambiada expreion de tipo: if((usigned long int)(tB - tA) >= CTE) a if(((usigned long int)(tB - tA)) >= CTE)
 *  - Arreglos a funciones utilities, tenpower y StrToInt y tI y tF en efectos convertidos a unsigned long int
 *  - En algun momento se creo el efecto Blink, no se habia documentado
 *  v1.2d
 *  - Ligeros cambios en la implementacion de DualScroll y DualScrollRin
 *  - Vuelta atras en el uso e implementacion de ClearRAM()
 *  v1.2c
 *  - Mejorado el uso de ClearRAM(), en estos momentos ya no hace falta usarlo con tanta frecuencia como antes. Y tambien optimizada su implementacion, ahora
 *    no es necesario borrar todos los bytes de la RAM, solo los "DISPLAY_COLS / 8" primeros. Para DIPSLAY_COLS = 40, serian solo los primeros 5 bytes de cada fila.
 *  - Añadido efecto Blink
 *  - Cambiada implementación de efecto DualScrollRin
 *  - Añadido parametro ToggleInverse en clase TCartel, y usado en Refresh() para crear nuevo efecto Blink.
 *  - Creada cte DISPLAY_COLS (actualmente es igual a 40 cols) para que el escalado hacia diplays mas grandes (con mas columnas) sea mas facil.
 *  - Aumentada cte LIM_COLS, ya no es necesario reservar 5 bytes (40 bits = 40 cols) en la RAM para efectos de salida...
 *  - Optimizando RAM, faltaba Exit Left, agregado parametro IndexLastByte en TCartel
 *  v1.2b
 *  - Agregado caracter "ü"
 *  - Arreglada comilla simple que no salia
 *  - Uso de Buffer con arreglos de bytes para recibir via serie, y no strings, ganando estabilidad con la memoria dinamica
 *   (En un futuro para ganar mas memoria se pudiera combinar el buffer de recepcion con el del msj)
 *  - Nuevas funciones:  CopyStrs,StrToInt
 *  - BAUDIOS en 19200
 *  v1.2
 *  - Cambiado BAUDIOS de 9600 a 19200 en Comunicacion Serie, finalmente se volvio a 9600,  para luego dejarlo en 4800, se queria aumentar, pero mas lento es mas estable
 *  - Agregadas imagenes de logos del taller de celular Master Movil (Arley)
 *  - Algunas imagenes guardadas con 40 bytes, algunos byes en blanco, en 0, no estaban bien centradas, fueron guardadas quitando los bytes en blanco.
 *  v1.1d2
 *  - Añadido efecto de salida Right Scroll 2, este sacara el cartel completo y no solo lo que hay en la pantalla... (ver mas en implementacion)
 *  v1.1d 
 *   plus: Añadido parametro actualQcols en clase TCartel, para ser usado en efect dual scroll
 *   normal: --- no recuerdo...
 *  v1.1c:
 *  - Trabajando con imágenes de nuevo
 *  --------------------------------------------------------------
 * - Arreglado autocentrado cdo spaces era mayor que 1
 * - Agregado setTimeout a serial, con el fin de descartar msj q no  cumplan con el protocolo
 * - Aumentado a 15 ls ctd de carteles
 * - Arreglado comilla en tipo0, tipo1 y tipo2, era cierre de comilla, se sustituyo por una general, se puidera pensar en implementar abre y cierre comillas
 * - queda pendiente revizar la optimizacion del espacio para el tipo 3(Ahora se llama SingleChar_II)
 */
/* 
 *  Asi escanea, y asi todo lo q este puesto en la RAM saldra de forma correcta en el display
 *  Scan Rows
 *  Top to Bottom
 *  Left to right
 *  LSB at right
 *  
 *  *** FW_8x40LEDS_v0.1BETA_6 ***
 *  
 *  
 *  *** FW_8x40LEDS_v0.1BETA_5e ***
 * - Efectos de salir por la izq y por la derecha en version beta  
 * - todas las letras de 8 filas
 * - Se descubrió que el Arduino no procesa las vocales con tilde tambien como lo hace con la recepcion serie, 
 *   es decir no se puede crear una cdena char cartel[] = ... conteniendo estas vocales porq no trabaja tambien ...
 *   se codifican con dos bytes.
 *  
 *  *** FW_8x40LEDS_v0.1BETA_5d ***
 *  - Agregando la letra 'ñ' y las vocales con tilde a los distintos abecedarios/alfabetos/tipos_de_letras
 *  - Mejorar vocales con tilde, tal vez poniendo todas  las letras en 8 filas...
 *  - Averiguando como agregar salida con display en limpio tanto para la derecha como para la izquierda
 *   una var seria con la RAM con cols en impio
 *   otra var seria haciendo algnas operaciones en refresh
 *  - Autocentrado en display
 *  
 *  *** FW_8x40LEDS_v0.1BETA_5b ***
 *  - Agregado DisplayEfectA y DisplayefectB
 *  *** FW_8x40LEDS_v0.1BETA_4 ***
 *  - Agregando un nuevo Alfabeto(tipo de letra) sencillo, que proviene de una fuente para scan por columnas.
 *  
 *  -> Ver otros cambios en cambios.txt
 *   *** FW_8x40LEDS_v0.1BETA ***
 * - Recordar que las columnas que se lleve un cartel bsado en un texto dependera del alfaeto del cual se formo el cartel, normalmente si se basa en ALpha2 tendra mas columnas 
 *   que si se basa en Alpha0, o Alpha1, siendo Alpha0 el de menos columnas
 *   
 *   *** FW_8x40LEDS_v0.1BETA_2 ***
 * - Working : Agregando efecto de unir el ppo con el final
 * 
 *   *** FW_8x40LEDS_v0.1BETA_3 ***
 *  - Se cammbio de 2000000 baudios a 9600, mejoro la recepcion de los datos serie
 *  - Si se quitan las its se observaron problemas con la recepion serie, pero si fue necesario quitar la it del timer1 encargado del refresh()
 */
               
#define DEBUG

//#include "matrixaux.h"
#include "comhandler.h"
#include "TEfect.h"
#include "TExit.h"
#include "TEntrance.h"  //new in v1.5


unsigned long int t0 = 0, t1 = 0;



//char msgTest[] = "cüartel";  //debugging -- carcater 'ü'

void setup() {
  // put your setup code here, to run once:
 #ifdef DEBUG
  //Serial.begin(250000); //En el IDE 1.6.11 no existe 2 000 000 baudios
  //Serial.begin(2000000);   //No viene bien tanta velocidad para la recepcion de los datos 
  //Serial.begin(9600); 
  Serial.begin(BAUDIOS); //v1.2
  //Serial.setTimeout(50);  //50 ms
  //Serial.setTimeout(100);  //50 ms //...It defaults to 1000 milliseconds. //Vamos a dejarlo por defecto v1.2 
  //Serial.setTimeout(3000);  //tirando piedras  //tampoco
  //Rec.reserve(300);
  //Aux.reserve(300);
 #endif
 //noInterrupts()
 cli();//stop interrupts
 InicializarPanel();

 //debugginng...  ñ, á, é, í, ó, ú, ü
/*
 Serial.println(msgTest);
 CopyStrs(msgTest, ConfigMSG.msg, 0, 15); 
 //currCartel = new TText(ConfigMSG.msg.c_str(), ConfigMSG.ABC, ConfigMSG.spaces, 0, ConfigMSG.offset); //debuggeo...
 currCartel = new TText(ConfigMSG.msg, ConfigMSG.ABC, ConfigMSG.spaces, 0, ConfigMSG.offset); //debuggeo...
 currCartel -> Display(); //Aqui es donde se conforma la RAM...  //debuggeo
 InicializarTimer(); //debuggeo
 */
 //InicializarTimer();
 
 SPI.begin();
 SPI.beginTransaction(SPISettings(16000000, LSBFIRST, SPI_MODE0));//Start SPI transaction
 sei();//allow interrupts
 t1 = t0 = millis();
 /*
 String imgINFO = ":0"; 
 currCartel = new TImage(imgINFO.c_str(), false);
 currCartel -> Display(); //Aqui es donde se conforma la RAM...  //debuggeo
 Serial.println(currCartel -> Qcols);
 //delay(1000);
 //DisplayRAM();
 InicializarTimer(); //debuggeo
 */
 /*
 Serial.println("IM_CopaB_1");
 SwappingImages(IM_CopaB_1);
 Serial.println("IM_CopaB_2");
 SwappingImages(IM_CopaB_2);
 Serial.println("IM_CopaB_3");
 SwappingImages(IM_CopaB_3);
 Serial.println("IM_HappyFace2");
 SwappingImages(IM_HappyFace2);
 Serial.println("IM_Heart_0");
 SwappingImages(IM_Heart_0);
 Serial.println("IM_Heart_1");
 SwappingImages(IM_Heart_1);
 Serial.println("IM_Heart_2");
 SwappingImages(IM_Heart_2);
 */
}//setup()


void loop() {
 t1 = millis();
 switch(PanelState)
 {
  case 1: //Se ha recibido un start(<S) y se ha mandado la primera solicitud de msj o se ha terminado con el msj anterior y se solicitó otro
          if(((unsigned long int)(t1 - tstart)) > TimeOutCom) 
          {
            tstart = t1;
            Serial.print("<R");  //Se sigue mandando la solicitud si se llega al TimeOUT sin recibir un nuevo msj
            Serial.write(0x0D);
          }
          break;
  case 2: //Ya se ha recibido un msj con info...
          //currCartel -> ApagarFila();  //ya se apaga en la recepcion del msg...
          ClearRAM();   //Me parece que ya no hace falta borrar la RAM en esta version: v1.2c
                          //Al menos aqui...
          boolean toogle_tmp;
          toogle_tmp = false;  //No se porq no funciona con la linea de abajo
          //boolean toogle_tmp = false; //v1.4
          if(ConfigMSG.inverse == 2) //2 --> toggle
          {
            //Serial.println("toggle...");
            //Serial.println(CTDuseful);
            if(indexToggle == 0 && CTDuseful > 0)
            {
             toogle_tmp = toggleArray[CTDuseful - 1];
             toggleArray[CTDuseful - 1] = !toogle_tmp;
             //Serial.println(toogle_tmp);
            }
            //else
            else if(indexToggle > 0) //v1.4, Fixing important bug...
            {
             toogle_tmp = toggleArray[indexToggle - 1];
             toggleArray[indexToggle - 1] = !toogle_tmp;
            }
          }
          else
           toogle_tmp = ConfigMSG.inverse;

          if(currCartel != NULL)
          {
            delete currCartel;
            currCartel = NULL;
          } 
          if(ConfigMSG.ABC == 4)  //si se trata de una imagen
          {
            if(ConfigMSG.E == 4) //Animacion
             currCartel = new TImage(ConfigMSG.msg, toogle_tmp, true);  //Le dice que la imagen sera un frame de una animacion
            else 
             currCartel = new TImage(ConfigMSG.msg, toogle_tmp);
          }
          else
            currCartel = new TText(ConfigMSG.msg, ConfigMSG.ABC, ConfigMSG.spaces, toogle_tmp, ConfigMSG.offset); //ConfigMSG.msg.c_str()
            
          currCartel -> Display(); //Aqui es donde se conforma la RAM...
          //DisplayRAM(); //Debuggeando...
          //InicializarTimer();  //v1.1d2  //Comentareado aqui en esta version //Ahora se encargaran los efectos de Inicializar el timer...
          PanelState = 3; //Entrada...
          if(MyEntrance != NULL)
          {
            delete MyEntrance;
            MyEntrance = NULL;
           }
          switch(ConfigMSG.IN)
          {
            case 0: //Ninguno
                    //Serial.println(F("Ninguna Entrada"));
                    SelectEfect();
                    ///InicializarTimer();
                    break;
            case 1: //Right, entra por la derecha se mueve para la izquierda
                    //Serial.println(F("Entrada por la derecha"));
                    //MyEntrance = new EntranceScroll(currCartel, ConfigMSG.TScrollIn);
                    MyEntrance = new EntranceRight(currCartel, ConfigMSG.TScrollIn); //v1.6
                    break;
            case 2: //Left, entra por la izq se mueve para la derecha
                    MyEntrance = new EntranceLeft(currCartel, ConfigMSG.TScrollIn); //v1.6
                    break;
          }//switch(ConfigMSG.IN)
          currCartel->Refresh(); 
          InicializarTimer(); 
          break;
  case 3: //Entrada...
          if(!MyEntrance -> Update(t1))
          {
           //Serial.println(F("Se acabó la entrada..."));
           SelectEfect();
          }//if(!MyEntrance -> Update(t1))
          break;
  case 4: //Efectos....
         /*
         //codigo para pruebas de costo de la funcion Refresh()
         //Refresh(), sin apagar fila
         TIMSK1 &= ~(1 << OCIE1A);
         Serial.println(F("Inicio de la prueba..."));
         unsigned long int Tcost_inic = micros();
         for(int i = 0; i < 400; i++)
          currCartel->Refresh(); 
         unsigned long int Tcost_final = micros();
         Tcost_inic = Tcost_final - Tcost_inic;
         Serial.print("La funcion toma ejecutarse 400 veces: ");
         Serial.println(Tcost_inic);
         */
         //Las pruebas arrojaron que para 40 cols la demora de Refresh() es de aprox 42 microsegundos
         //y para 128 cols de aprox 110 microsegundos
         //--------------------------------------------------------
         if(!MiEfecto -> Update(t1))
         {
           PanelState = 5;  //Exiting...
           if(MyExit != NULL)  //corrected in FULL v1.1
           {
             delete MyExit;
             MyExit = NULL;
           }
           switch(ConfigMSG.Exit)
           {
             case 0: //Ningun efecto de salida
                     Serial.print("<R");
                     Serial.write(0x0D);
                     PanelState = 1; //requesting new message... //or waiting for a new message
                     tstart = millis();
                     break;
             case 1: //Exit Left Scroll
                     MyExit = new ExitScrollLeft(currCartel,  ConfigMSG.TScrollExit);
                     break;
             case 2: //Exit Right Scroll
                     MyExit = new ExitScrollRight(currCartel,  ConfigMSG.TScrollExit);
                     break;
             case 3: //Exit Right Scroll II
                     MyExit = new ExitScrollRightB(currCartel,  ConfigMSG.TScrollExit);  
                     break;
           }//switch(ConfigMSG.Exit)
          }//if(!MiEfecto -> Update(t1))
         break;
  case 5: //Exiting....
          //if(MyExit != NULL)  //No es necesario si fuera el caso ya es mandado a state = 1.
           if(!MyExit -> Update(t1))
           {
             Serial.print("<R");
             Serial.write(0x0D);
             PanelState = 1; //requesting new message... //or waiting for a new message
             tstart = millis();
           }
          break;
 }//switch(PanelState)

 if(Serial.available())
    HandleCom();
}//void loop()
void SelectEfect()
{
           PanelState = 4;  //Efectos...
           if(MiEfecto != NULL)
           {
             delete MiEfecto;
             MiEfecto = NULL;
            }
           switch(ConfigMSG.E)
           {
             case 0: //Ninguno
                     PanelState = 0; //ningún efecto... //No saldrá de aquí hasta que se reciba un cmnd nuevo
                     ///InicializarTimer();
                     break;
             case 1: //Estacionario
                     //Serial.print(ConfigMSG.TInitial);
                     //Serial.println(" ...waiting...");
                     MiEfecto = new DisplayEfectStatic(currCartel, ConfigMSG.TInitial);
                     break;
             case 2: //...Left
                     MiEfecto = new DisplayEfectLeft(currCartel, ConfigMSG.TInitial, ConfigMSG.TEnd, ConfigMSG.TScrollE,  ConfigMSG.Nloops);
                     break;
             case 3: //...Dual
                     MiEfecto = new DisplayEfectDual(currCartel, ConfigMSG.TInitial, ConfigMSG.TEnd, ConfigMSG.TScrollE,  ConfigMSG.Nloops, ConfigMSG.Tprv_fwd, ConfigMSG.Tprv_rev);
                     break;
             case 4: //Animation
                     MiEfecto = new DisplayEfectAnimation(currCartel, ConfigMSG.TInitial, ConfigMSG.TEnd, ConfigMSG.TScrollE,  ConfigMSG.Nloops);
                     break;
             case 5: //Blink
                     MiEfecto = new DisplayEfectBlink(currCartel, ConfigMSG.TInitial, ConfigMSG.TEnd, ConfigMSG.TScrollE,  ConfigMSG.Nloops);
                     break;
            } 
}
