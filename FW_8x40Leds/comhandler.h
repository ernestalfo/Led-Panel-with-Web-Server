#include "timeraux.h"
//void Imprimir();

unsigned long int tstart = 0;
//String Rec;
//String Aux;
#define MAXREC 200
byte BufferRx[MAXREC];

void HandleCom()
{
  //cli();//stop interrupts
  TIMSK1 &= ~(1 << OCIE1A);
  if(currCartel != NULL)
   currCartel -> ApagarFila();

  //Rec = Serial.readStringUntil(0x0D);  //No incluye a 0x0D //?? no estoy muy seguro de eso, ver ESP_WebConfig_LedPanel donde se tuvo q eliminar el caracter nueva linea...
  int ctd_rec = Serial.readBytesUntil(0x0D, BufferRx, MAXREC) ;
  
  //Serial.println(F("deb: Receiving..."));

  if(ctd_rec > 1)
  {
    if(BufferRx[0] != '<') 
    {
     //Serial.println(F("Wrong protocol..."));
     InicializarTimer();
     return;
    }
    if(BufferRx[1] == 'S')  //Cmnd de iniciar
    {
      int f = StrIndexOf(':', 3, BufferRx);
      CTDuseful = StrToInt(3, f - 1, BufferRx); 
      if(CTDuseful > 0)
      {
       Serial.print("<R");
       Serial.write(0x0D);
       PanelState = 1; //requesting new message... //or waiting for a new message
       //for(int i = 0; i < CTDCARTELES; i++) //parte de las acciones del reseteo
         //toggleArray[i] = false;
       tstart = millis();
      }
      else
       PanelState = 0;  //en v1.4 de la interfaz web y el ESP, se puede haber recibido en cero para indicar apagar el  panel de leds...
      //-- reseting... :
      for(int i = 0; i < CTDCARTELES; i++) //parte de las acciones del reseteo
       toggleArray[i] = false;
      indexToggle = 0; //added //sincronizando con esp8266...
      //---
    }//if(ptrStr[1] != 'S')
    else if(BufferRx[1] == 'M' || BufferRx[1] == 'C')
    {
      //Serial.println(F("deb: Recibiendo msj(<M)"));
      if(BufferRx[1] == 'M')
      {
       indexToggle++;
       //if(indexToggle == CTDuseful)
       if(indexToggle >= CTDuseful)
        indexToggle = 0;
      }
      int f = StrIndexOf(':', 3, BufferRx);
     
      //Serial.println(f);  //deb

      int nchars = StrToInt(3, f - 1, BufferRx); 
      //Serial.println(Aux); //deb
      //Serial.println(nchars); //deb
      int fincad = f + nchars + 1;
      if(nchars > 0)
      {
       //ConfigMSG.msg = Rec.substring(f + 1, fincad); 
       //(Rec.substring(f + 1, fincad)).getBytes(ConfigMSG.msg, CHMAX); //changed in v1.2
       CopyStrs(BufferRx, ConfigMSG.msg, f + 1, fincad - 1);
       //Serial.println(ConfigMSG.msg);   //deb...
       f = fincad  + 1;
       ConfigMSG.ABC = StrToInt(f, f, BufferRx);  ////Asumiendo un solo digito
       f = f + 2;
       ConfigMSG.E = StrToInt(f, f, BufferRx);
       f = f + 2;
       ConfigMSG.IN = StrToInt(f, f, BufferRx); //IN ---> Entrada //new in v1.5
       f = f + 2;
       ConfigMSG.Exit = StrToInt(f, f, BufferRx);
       f = f + 2;
       ConfigMSG.spaces = StrToInt(f, f, BufferRx);
       f = f + 2;
       ConfigMSG.offset =  StrToInt(f, f, BufferRx);
       f = f + 2;
       ConfigMSG.inverse = StrToInt(f, f, BufferRx);
       f = f + 2;
       fincad = StrIndexOf(':', f, BufferRx);
       ConfigMSG.Nloops =  StrToInt(f, fincad - 1, BufferRx);
       f = fincad + 1;
       fincad = StrIndexOf(':', f, BufferRx);
       ConfigMSG.TScrollExit = StrToInt(f, fincad - 1, BufferRx);
       f = fincad + 1;
       fincad = StrIndexOf(':', f, BufferRx);
       ConfigMSG.TScrollIn = StrToInt(f, fincad - 1, BufferRx);
       f = fincad + 1;
       fincad = StrIndexOf(':', f, BufferRx);
       ConfigMSG.TScrollE = StrToInt(f, fincad - 1, BufferRx);
       f = fincad + 1;
       fincad = StrIndexOf(':', f, BufferRx);
       ConfigMSG.TInitial = StrToInt(f, fincad - 1, BufferRx);
       //Serial.println(ConfigMSG.TInitial);
       f = fincad + 1;
       fincad = StrIndexOf(':', f, BufferRx);
       ConfigMSG.TEnd = StrToInt(f, fincad - 1, BufferRx);   
       f = fincad + 1;
       fincad = StrIndexOf(':', f, BufferRx);
       ConfigMSG.Tprv_rev = StrToInt(f, fincad - 1, BufferRx);   
       f = fincad + 1;
       fincad = StrIndexOf(':', f, BufferRx);
       ConfigMSG.Tprv_fwd = StrToInt(f, fincad - 1, BufferRx);  
       //Imprimir(); //Comprobacion //debuggeo //etc
       PanelState = 2;  //Se ha recibido un msj con info, es decir el campo del msj no esta vacio
      }//if(nchars > 0)
     
    }//else if(BufferRx[1] == 'M')
    else if(BufferRx[1] == 'V')
    {//Han solicitado la version del firmware.
      //Serial.println(F("Firmware version: 1.6b"));
      Serial.print(F("\nFirmware version: "));
      Serial.println(FW_VER);
      Serial.println(F("ROWS: 8"));
      Serial.print(F("COLUMNS: "));
      Serial.println(DISPLAY_COLS);
      switch(DesiredPulse_k)
      {
        case 1: Serial.println(F("Intensity: 25%"));
                break;
        case 2: Serial.println(F("Intensity: 50%"));
                break;
        case 3: Serial.println(F("Intensity: 75%"));
                break;
        case 4: Serial.println(F("Intensity: 100%"));
                break;  
      } 
      Serial.write(0x0D);
    }// else if(BufferRx[1] == 'V')
  }// if(Rec.length() > 1)
 //else  //deb...
 //{
   // Serial.println(F("nothing"));
  //}
  //InicializarTimer();  //Ahora se inicializa en otro sitio
 // sei();//allow interrupts
 ///*
 while(Serial.available()) //Clearing buffer
  Serial.read();
  //*/
}//void HandleCom()

/*
void Imprimir()
{
  Serial.println(F("msg:"));
  Serial.print(ConfigMSG.msg);
  Serial.println(F("|fin de la cadena|"));
  Serial.println(F("ABC:"));
  Serial.println(ConfigMSG.ABC);
  Serial.println(F("E:"));
  Serial.println(ConfigMSG.E);
  Serial.println(F("IN:"));             //new in v1.5
  Serial.println(ConfigMSG.IN);         //new in v1.5
  Serial.println(F("Exit:"));
  Serial.println(ConfigMSG.Exit);
  Serial.println(F("spaces:"));
  Serial.println(ConfigMSG.spaces);
  Serial.println(F("offset:"));
  Serial.println(ConfigMSG.offset);
  Serial.println(F("inverse:"));
  Serial.println(ConfigMSG.inverse);
  Serial.println(F("Nloops:"));
  Serial.println(ConfigMSG.Nloops);
  Serial.println(F("TScrollExit:"));
  Serial.println(ConfigMSG.TScrollExit);
  Serial.println(F("TScrollIN:"));        //new in v1.5
  Serial.println(ConfigMSG.TScrollIn);    //new in v1.5
  Serial.println(F("TScrollE:"));
  Serial.println(ConfigMSG.TScrollE);
  Serial.println(F("TInitial:"));
  Serial.println(ConfigMSG.TInitial);
  Serial.println(F("TEnd:"));
  Serial.println(ConfigMSG.TEnd);
  Serial.println(F("Tprv_rev:"));
  Serial.println(ConfigMSG.Tprv_rev);
  Serial.println(F("Tprv_fwd:"));
  Serial.println(ConfigMSG.Tprv_fwd);
}
*/
