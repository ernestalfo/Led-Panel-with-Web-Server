/*     v1.6:
 *     -----
 *    - Agregada opcion de entrada por la izq, la q estaba puesta decia izq pero realmente era derecha
 *      Left: Entra por la izq y camina pa la derecha
 *      Right: Entra por la derecha y camina pa la izq
 *      v1.5:
 *     -----
 *   - Reordenando btns en settings
 *   - Quitado btn network, pudiera aparecer en advance settings junto a intensidad
 *   - Anadiendo tipo de entrada y tiempo de scroll para la entrada a la interfaz
 *     v1.4:
 *    -----
 *   - Arreglado hipervinculo al lado del titulo de Profiles que llevaba a index.html cuando debe ser settings.html
 *   - Protegiendo ficheros del uso indiscriminado, protegiendo ciclos de escritura en la flash.
 *   - Agregado checkbox para formato de 24 horas del reloj, agregada ccorrespondiente implementacion en getAMPM();
 *   - Agregado btn "Test" en Settings
 *   - Agregado btn "Leds Off" en Settings:
 *      * Eliminando antigua variable interval_trigger, la mayoria de las veces comentareada, y no sinedo usada despues de asignarle valores
 *      * En esta version apagar los leds seguira con el reloj corriendo si estaba activado, creo que esta bien asi
 *   - Agregado btn "Load & Save" en pagina profiles, tratando de hacer un poco mas facil la interaccion con la interfaz cuando se quiere switchear el perfil, ej: uno para
 *     Abierto y otro para Cerrado, tratando de que sean menos clicks, o pasos...
 *     
 *    v1.3:
 *    -----
 *    - cambiada expreion de tipo: if((usigned long int)(tB - tA) >= CTE) a if(((usigned long int)(tB - tA)) >= CTE)
 *    - Nuevo btn images en settings, y nueva pagina con tabla de imagenes
 *    - Se volvio a poner el Blink en la interfaz web, no se porque se habia perdido, lo habia puesto en alguna version 1.2x pro no se...
 *    
 *   v1.2
 *    - Mejorando desempeño e  interaccion en pagina web profiles
 *    - Agregados nuevos estados, y nuevas variables glbales para insertar el clock al resto del sistema que ya estaba fucionando
 *    - Agegado clock.html, y clock.h
 *    - Agregado btn settings en index.html, pasado btn profiles a settings
 *    - Ya no se enciende Led built-in cdo se activa el modo Ap, porque la luz se reflejaba en los leds (ya se armo una cajita)
 *    - Cambiado BUADIOS de 9600 a 19200 (Serial Comuncation), al final se quedo en 9600, para luego ponerlo a 4800, finalmente: 19200
 *    
 *   v1.1d2
 *   - Añadida posibilidad de elegir efecto de salida Right Scroll B / Right Scroll II
 *   - Cambiado msj de inicio y tiempo de exposicion a "Wifi AP"
 *   v1.d
 *  ------------------------------------------------- 
 *   modulo de la version full nano + nodeMCU v1.1c
 *  Aumentado a 15 la CTDCARTELES
 *   v1.1c:
 *   
 *  - Trabajando con imagenes de nuevo
 *   -----------------------------------------------------------------
 *  - Agregado setTimeout a serial
 *  - No se termino de implementar switch entre Ap y STATION, se trabajara en AP solamente...
 *  - Agregado Switching entre AP o STATION, aunque solo se realizara al despertar por ahora
 */
/* 
  ESP_WebConfig 
*/
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "global.h"
#include <FS.h>
//#include <ArduinoJson.h>

/*
Include the HTML, STYLE and Script "Pages"
*/
#include "Page_Index.h"
#include "Page_Textos.h"
#include "Page_ABC.h"
#include "Page_Efectos.h"
#include "Page_Profiles.h"
//#include "Page_Network.h"
#include "Page_Script.js.h"
#include "Page_Style.css.h"
#include "Page_Settings.h"
#include "Page_Clock.h"
#include "Page_Images.h"

//Como Access Point: dir: 192.168.4.1/...

#define ACCESS_POINT_NAME  "LEDMatrix"				
#define ACCESS_POINT_PASSWORD  "Enter123456" 
boolean APmode = true;

unsigned long int t = 0;

void setup ( void ) {
  
	//Serial.begin(115200);
 //Serial.begin(9600);
 Serial.begin(BAUDIOS);
 Serial.setTimeout(50);  //50 ms  
 delay(500);
 Serial.println("Starting ESP8266");

 SPIFFS.begin();

 server.on ( "/index.html", []()  { server.send ( 200, "text/html", PAGE_IndexMainPage ); } ); //index.html
 server.on ( "/index/clear", send_msg_clr_html);
 server.on ( "/index/send", index_send_f);
 server.on ( "/index/save", index_save_f);
 server.on ( "/index/load", index_load_f);
 server.on ( "/index/next", index_next_f);
  
 server.on ( "/textos.html", send_textos_configuration_html); //textos.html
 server.on ( "/textos/prev", send_prev_html);
 server.on ( "/textos/next", send_next_html);
 server.on ( "/textos/values", send_textos_configuration_values);

 //server.on ( "/profiles.html", send_profiles_configuration_html); //profiles.html
 server.on ( "/profiles.html", []()  { server.send ( 200, "text/html", PAGE_Profiles); } );//profiles.html
 server.on ( "/profiles/values", send_profiles_configuration_values);
 server.on ( "/profiles/default", profiles_default_f);
 server.on ( "/profiles/p1/values", profiles_p1);
 server.on ( "/profiles/p2/values", profiles_p2);
 server.on ( "/profiles/p3/values", profiles_p3);
 server.on ( "/profiles/p4/values", profiles_p4);
 server.on ( "/profiles/p5/values", profiles_p5);
 server.on ( "/profiles/loadandsend", profiles_loadandsend_f);
 
 //server.on ( "/settings.html", send_settings_configuration_html); //settings.html
 server.on ( "/settings.html",[]()  { server.send ( 200, "text/html", PAGE_GeneralSettings ); } );
 server.on ( "/settings/ledsonoff", settings_ledsonoff);
 server.on ( "/settings/test", settings_test);
 server.on ("/settings/values", settings_values);

 server.on ( "/clock.html", send_clock_configuration_html); //textos.html
 server.on ( "/clock/values", send_clock_configuration_values);
 server.on("/clockU/values",send_clockrefresh_configuration_values);
  
 server.on ("/abc.html", send_abc_configuration_html);   //abc.html
 server.on ("/abc/values", send_abc_configuration_values);
 
 server.on ("/efectos.html", send_efectos_configuration_html);  //efectos.html
 server.on ("/efectos/values", send_efectos_configuration_values);

 server.on ("/images.html",[]() { server.send ( 200, "text/html", PAGE_ImagesIndex); } );   //images.html
 //server.on ( "/network.html", send_netkwork_configuration_html); //network.html
  
 server.on ( "/style.css", []() {server.send ( 200, "text/plain", PAGE_Style_css );  } );
 server.on ( "/microajax.js", []() {server.send ( 200, "text/plain", PAGE_microajax_js );  } );
 
 server.onNotFound ( []() {server.send ( 400, "text/html", "Page not Found" );   }  );

 pinMode(LED_BUILTIN, OUTPUT); //ON = AP Mode / OFF = STATION Mode
 digitalWrite(LED_BUILTIN, HIGH); //Turnig Off led

 wifiConnect();
 t = millis();
 
 LoadDefaults();  //moved here in the last revision  //Cargando nombre de perfil por defecto
}

 
void loop ( void ) 
{
  //t_x = millis();
	server.handleClient();

  t_x = millis();
  MyClock.Update(t_x);

  String values ="";
  int i = 0, ctduseful = 0;
  switch(state)
  {
    case 0: //Dando chance al Arduino Nano a que despierte...
           //if((unsigned long int)(millis() - t) > Twait) 
           if(((unsigned long int)(t_x - t)) > Twait) 
           {
            //if(WiFi.status() == WL_CONNECTED)
            if(!APmode)
            {
             ConfigRXTX[0].msg = String(WiFi.localIP()[0]) + "." + String(WiFi.localIP()[1]) + "." + String(WiFi.localIP()[2]) + "." + String(WiFi.localIP()[3]);
             //Serial.println("STATION mode");
            }
            else
            {
             //ConfigRXTX[0].msg = "AP Mode";
             ConfigRXTX[0].msg = "Wifi AP";  //v1.1d
            //Serial.println(ConfigRXTX[0].msg); //debugging...
             //Serial.println("AP mode");
            }
            ConfigRXTX[0].E = "1"; //Static... / Estacionario...
            ConfigRXTX[0].TInitial = 2500; //2500 ms -> 2.5 seg de exposicion
            server.begin();
            //Serial.println(F("HTTP server started")); //Esta linea en este preciso lugar no viene bien para q el ArduinoNano entienda el msj en el contexto del protocolo establecido
            Serial.print(F("<S:1:"));
            Serial.write(0x0D);
            t = millis();
            state = 1;
           }//if((unsigned long int)(millis() - t) > Twait)
           break;
    
    case 1: //Dando chance a q se muestre la dir IP el tiempo necesario
           //if((unsigned long int)(millis() - t) > TwaitIPshowing) 
           if(((unsigned long int)(t_x - t)) > TwaitIPshowing) 
           {            
            //LoadDefaults();  //pasando a setup
            LoadingConfig(ConfigRXTX, FileNameDefault.c_str());
            LoadingConfig(ConfigWS, FileNameDefault.c_str());
            LoadClockSettings();  //v1.2

            if(TimeMode != 1)
             PanelSync();

            //Luego de cargar los valores por defecto se decide a que estado pasar
            if(TimeMode == 0)
             state = 2;  //Clock Disabled
            else if(TimeMode == 1)
            {
             state = 3;   //Clock Always ON
             SendTime(t_x, false);
            }
            else if(TimeMode == 2)
              state = 4; //Clock at ...MSGCycle
            else if(TimeMode == 3)
            {
              state = 5; //Clock ByInterval
              t_interval = t_x;
            }
           }//if((unsigned long int)(millis() - t) > Twait)
           //if(Serial.available())
            //HandleCom();
           break;
   //case 2: //Clock Disabled  //pero puede ser que todavia no se haya mandado un msg diferente y se a necesario actualizar la pantalla con la hora actual
           //if(enviarTime && KeepTimeUpdated)  
           // SendTime(t_x);
   case 3: //Clock Always ON
           if(enviarTime)
           {
            SendTime(t_x, false);
            //enviarTime = false;  //enviarTime es un aviso del timer, de que se actualizaron los mins
           }
           break;
   case 4: //Clock at ...MSGCycle
           if(enviarTime && KeepTimeUpdated)  //KeepTimeUpdated se pone en true en SendTime() y se pone en false en SendMSG()
           {
            SendTime(t_x);
            //enviarTime = false;  //Ya se pone en falso en SendTime()
           }
           break;
  case 5: //Clock ByInterval
          //Al parecer dentro del intervalo esta eltiempo que es mostrado el cartel con la info del reloj
           if((unsigned long)(t_x - t_interval) >= Interval)
           {
            SendTime(t_x); 
            t_interval = t_x;
           }
          if(enviarTime && KeepTimeUpdated)
           SendTime(t_x);
  }//switch(state)
 if(Serial.available())
  HandleCom();
}//looop

void HandleCom()
{
  String Rec = "";
  Rec = Serial.readStringUntil(0x0D);//No incluye a 0x0D
  //Serial.println("receiving...");
  //Serial.println(Rec.length());
  if(Rec.length() > 1)
  {
    const char *ptrStr; 
    ptrStr = Rec.c_str();
    if(ptrStr[0] != '<') //Added v0.03b
    {
     //Serial.println("Wrong protocol...");
     return;
    }
    if(ptrStr[1] == 'R')
    {
     if(state < 3)
      SendCurrentMSG();
     else 
     {
      switch(state)
      {
        case 3: //Clock AlwaysON
                //SendTime(false);  //true means without efect / ningun efecto /* mal el comentario y mal la linea , fue aceptada en compilacion pq se convirtio false a unsigned long int */
                SendTime(t_x, false);   //false means without efect
                break;
        case 4: //Clock at ...MSGCycle
                if(CycleCompleted)  
                {
                  SendTime(t_x); //por defecto efecto en true
                  CycleCompleted = false;
                }
                else
                 SendCurrentMSG();
                break;
        case 5: //Clock ByInterval
                SendCurrentMSG();       
                break;
      }
     }
    }//if(ptrStr[1] != 'R')
  }// if(Rec.length() > 2)
}//void HandleCom()
//--------------------------------------------------------------------------------------------------------------
void wifiConnect()
{
  //reset networking
  WiFi.softAPdisconnect(true);
  WiFi.disconnect();          
  delay(1000);
  //check for stored credentials
  /*
  if(SPIFFS.exists("networksettings.dat")){
    //const char * _ssid = "", *_pass = "";
    String ssid = "", pass = "";
    File configFile = SPIFFS.open("networksettings.dat", "r");
    if(configFile){

        ssid = configFile.readStringUntil('\n');
        ssid = ssid.substring(0, ssid.length() - 1);
        pass = configFile.readStringUntil('\n');
        pass = pass.substring(0, pass.length() - 1);
        
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid, pass);
       
        int waits = 0;
        while (WiFi.status() != WL_CONNECTED && waits < 100) 
        {
          delay(500);
          Serial.print(".");
          digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));
          waits++;
        }
    }
  }
  */
  if (WiFi.status() == WL_CONNECTED)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    APmode = false;
  } 
  else 
  {
    WiFi.mode(WIFI_AP);
    //WiFi.softAPConfig(local_ip, gateway, netmask);
    WiFi.softAP(ACCESS_POINT_NAME, ACCESS_POINT_PASSWORD); 
    //digitalWrite(LED_BUILTIN, LOW);   //v1.2 //la luz del led azul se ve reflejada en los leds, ahora el sistema está montado en una caja     
  }
  //Serial.println("");
  //WiFi.printDiag(Serial);
}
