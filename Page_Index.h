//
//  HTML PAGE
//
const char PAGE_IndexMainPage[] PROGMEM = R"=====(
<meta name="viewport" content="width=device-width, initial-scale=1" />
<strong>LED Matrix</strong> by ErnestoAP
<hr>
<a href="textos.html" style="width:150px" class="btn btn--m btn--blue">Texts</a><br>
<a href="settings.html" style="width:150px" class="btn btn--m btn--blue">Settings</a><br>
<a href="javascript:Save()"  style="width:150px" class="btn btn--m btn--blue">Save</a><br>
<a href="javascript:Load()"  style="width:150px"  class="btn btn--m btn--blue">Load</a><br>
<a href="javascript:Send()"  style="width:150px"  class="btn btn--m btn--blue">Send</a><br>
<a href="javascript:Clear()" style="width:150px"  class="btn btn--m btn--blue">Clear</a><br>
<a href="javascript:Next()" style="width:150px"  class="btn btn--m btn--blue">Next</a><br>
<hr>
<div id="infolabel">N/A</div>


<script>

function Clear()
{
   setValues("index/clear");
}

function Send()
{
   setValues("index/send");
}

function Save()
{
  if(confirm("Save Config to Flash in current profile.."))
  {
   document.getElementById('infolabel').innerHTML = "Saving Config...";
   setValues("index/save");
  }
}

function Load()
{
  document.getElementById('infolabel').innerHTML = "Loading Config...";
  setValues("index/load");
}

function Next()
{
  document.getElementById('infolabel').innerHTML = "Next Message...";
  setValues("index/next");
}

window.onload = function ()
{
	load("style.css","css", function() 
	{
		load("microajax.js","js", function() 
		{
				// Do something after load...
		});
	});
}
function load(e,t,n){if("js"==t){var a=document.createElement("script");a.src=e,a.type="text/javascript",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}else if("css"==t){var a=document.createElement("link");a.href=e,a.rel="stylesheet",a.type="text/css",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}}

</script>
)=====";

void send_msg_clr_html()
{
  //Serial.println("Clearing data...."); 
  ClearConfig();
  //Clearing data....
  String values ="";
  
  values += "infolabel|" + String("Empty") +  "|div\n";
  server.send ( 200, "text/plain", values);
  //Serial.println(__FUNCTION__); 
}

void SendToRamRXTX()
{
  for(int i = 0; i < CTDCARTELES; i++)
   ConfigRXTX[i] = ConfigWS[i];
}

void  index_send_f()
{
  //Originalmente de aqui se pasaba para un estado transitorio donde se hacian las acciones antes de pasar al otro, no hace falta...
  String values ="";
  if(state == 3)
   values += "infolabel|" + String("Clock: Always ON, MSG no sended") +  "|div\n";
  else
  {
    if(state == 6)
    {
     //state = prv_state;  //Retrieving last state....
      values += "infolabel|" + String("Leds Status: OFF") +  "|div\n";
    }
    else
    {
     SendToRamRXTX();
     PanelSync();
     values += "infolabel|" + String("Start Sended") +  "|div\n";
    }
  }

  server.send( 200, "text/plain", values);
}

void index_save_f()
{
  //String values ="";
  //values += "infolabel|" + String("Saving Config...") +  "|div\n";
  //server.send ( 200, "text/plain", values);
  
  //Serial.println("Saving Config in FS");            
  //File configFile = SPIFFS.open("CONFIG_MSGS_FILE", "w");
  File configFile = SPIFFS.open(FileName, "w");
  if(!configFile)
      Serial.println("file open failed");
  else
  {
    //Write data to file
    //Serial.println("Writing Data to File");
    for(int i = 0; i < CTDCARTELES; i++)
   {
    configFile.println(ConfigWS[i].msg);
    configFile.println(ConfigWS[i].ABC);
    configFile.println(ConfigWS[i].E);
    configFile.println(ConfigWS[i].Exit); 
    configFile.println(ConfigWS[i].TScrollExit); 
    configFile.println(ConfigWS[i].spaces); 
    configFile.println(ConfigWS[i].offset); 
    configFile.println(ConfigWS[i].inverse);
    configFile.println(ConfigWS[i].Nloops);
    configFile.println(ConfigWS[i].TScrollE);  
    configFile.println(ConfigWS[i].TInitial); 
    configFile.println(ConfigWS[i].TEnd); 
    configFile.println(ConfigWS[i].Tprv_rev); 
    configFile.println(ConfigWS[i].Tprv_fwd);
    //new in v1.5:
    configFile.println(ConfigWS[i].IN);  
    configFile.println(ConfigWS[i].TScrollIN); 
   }
   configFile.close();
  }
  String values ="";
  values += "infolabel|" + String("Done!!!") +  "|div\n";
  server.send ( 200, "text/plain", values);
}//void index_save_f()

//void LoadingFile(TMsg_Config_web *A,const char *);
void LoadingConfig(TMsg_Config_web *A,const char *);

void index_load_f()
{
  //LoadingFile(ConfigWS, "CONFIG_MSGS_FILE"); 
  //LoadingConfig(ConfigWS, "CONFIG_MSGS_FILE");
  LoadingConfig(ConfigWS, FileName.c_str());
  String values ="";
  values += "infolabel|" + String("Done!!!") +  "|div\n";
  server.send ( 200, "text/plain", values);
}//index_load_f()

void SendCurrentMSG();

void index_next_f()
{
  String values ="";
  if(state == 3)
   values += "infolabel|" + String("Clock: Always ON, MSG no sended") +  "|div\n";
  else if(state == 6) //Leds OFF
   values += "infolabel|" + String("Leds Status: OFF") +  "|div\n";
  else
  {
    values += "infolabel|" + String("Done!!!") +  "|div\n";
    SendCurrentMSG();
  }
  
  server.send ( 200, "text/plain", values);
}//index_next_f()

//void LoadingFile(TMsg_Config_web *A, const char *filename)
void LoadingConfig(TMsg_Config_web *A, const char *filename)
{
  //File configFile = SPIFFS.open("CONFIG_MSGS_FILE", "r");
  File configFile = SPIFFS.open(filename, "r");
  if(!configFile)
      Serial.println("file open failed"); //pudiera confundir al arduino nano... aunque en este se valida el protocolo de cierta forma
  else
  {
    //Write data to file
    //Serial.print("Reading Data from File: ");
    //Serial.println(FileName);
   for(int i = 0; i < CTDCARTELES; i++) //se deberia preguntar por el fin del archivo tambien...
   {
    A[i].msg = configFile.readStringUntil('\n');
    A[i].msg = A[i].msg.substring(0, A[i].msg.length() - 1); //Quitando el fin de linea q no interesa... //o el caracter de terminacion i dont know very well... but it is necesary...
    
    A[i].ABC = configFile.readStringUntil('\n');
    A[i].ABC = A[i].ABC.substring(0, A[i].ABC.length() - 1); //Quitando el fin de linea q no interesa...
    
    A[i].E = configFile.readStringUntil('\n');
    A[i].E = A[i].E.substring(0, A[i].E.length() - 1); //Quitando el fin de linea q no interesa...
    
    A[i].Exit = configFile.readStringUntil('\n'); 
    A[i].Exit = A[i].Exit.substring(0, A[i].Exit.length() - 1);
    
    A[i].TScrollExit = configFile.readStringUntil('\n');
    A[i].TScrollExit = A[i].TScrollExit.substring(0, A[i].TScrollExit.length() - 1);
    
    A[i].spaces = configFile.readStringUntil('\n');
    A[i].spaces = A[i].spaces.substring(0, A[i].spaces.length() - 1);
    
    A[i].offset = configFile.readStringUntil('\n');
    A[i].offset = A[i].offset.substring(0, A[i].offset.length() - 1);
    
    A[i].inverse = configFile.readStringUntil('\n');
    A[i].inverse = A[i].inverse.substring(0, A[i].inverse.length() - 1);
    
    A[i].Nloops = configFile.readStringUntil('\n');
    A[i].Nloops = A[i].Nloops.substring(0, A[i].Nloops.length() - 1);
    
    A[i].TScrollE = configFile.readStringUntil('\n');  
    A[i].TScrollE = A[i].TScrollE.substring(0, A[i].TScrollE.length() - 1);
    
    A[i].TInitial = configFile.readStringUntil('\n'); 
    A[i].TInitial = A[i].TInitial.substring(0, A[i].TInitial.length() - 1);

    A[i].TEnd = configFile.readStringUntil('\n');
    A[i].TEnd = A[i].TEnd.substring(0, A[i].TEnd.length() - 1);
    
    A[i].Tprv_rev = configFile.readStringUntil('\n');
    A[i].Tprv_rev = A[i].Tprv_rev.substring(0, A[i].Tprv_rev.length() - 1);
    
    A[i].Tprv_fwd = configFile.readStringUntil('\n');
    A[i].Tprv_fwd = A[i].Tprv_fwd.substring(0, A[i].Tprv_fwd.length() - 1);

    //new in v1.5:
    //Salvar primero con el formato nuevo para que no de problema al cargar
    ///*
    A[i].IN = configFile.readStringUntil('\n');
    A[i].IN = A[i].IN.substring(0, A[i].IN.length() - 1);
    
    A[i].TScrollIN = configFile.readStringUntil('\n');
    A[i].TScrollIN = A[i].TScrollIN.substring(0, A[i].TScrollIN.length() - 1); 
    //*/
   }
   configFile.close();
  }//else   
}//void LoadingFile()
//-------------------------------------------------------------------------------------------------------------------------
void SendCurrentMSG()
{
      //Aqui hay que rspetar el protocolo
      int i_unseful = 0;
      while(ConfigRXTX[index_cartelRXTX].msg.length() == 0 && i_unseful < CTDCARTELES)
      {
       index_cartelRXTX++;
       
       //if(index_cartelRXTX == CTDCARTELES)
       if(index_cartelRXTX >= CTDCARTELES)  //Podria trabajar mejor q la linea de arriba en algunas ocasiones...
        index_cartelRXTX = 0;
       
       i_unseful++;
      }
  
      if(i_unseful < CTDCARTELES)
      {
       //Serial.println("Responding Request of next message...");
       String Env = "<M:";
       Env += ConfigRXTX[index_cartelRXTX].msg.length();
       Env += ':';
       Env += ConfigRXTX[index_cartelRXTX].msg;
       Env += ':';
       Env += ConfigRXTX[index_cartelRXTX].ABC;
       Env += ':';
       Env += ConfigRXTX[index_cartelRXTX].E;
       Env += ':';
       Env += ConfigRXTX[index_cartelRXTX].IN; //new in v1.5
       Env += ':';
       Env += ConfigRXTX[index_cartelRXTX].Exit;
       Env += ':';
       Env += ConfigRXTX[index_cartelRXTX].spaces;
       Env += ':';
       Env += ConfigRXTX[index_cartelRXTX].offset;
       Env += ':';
       Env += ConfigRXTX[index_cartelRXTX].inverse;
       Env += ':';
       Env += ConfigRXTX[index_cartelRXTX].Nloops ;
       Env += ':';
       Env += ConfigRXTX[index_cartelRXTX].TScrollExit;
       Env += ':';
       Env += ConfigRXTX[index_cartelRXTX].TScrollIN;  //new in v1.5
       Env += ':';
       Env += ConfigRXTX[index_cartelRXTX].TScrollE;
       Env += ':';
       Env += ConfigRXTX[index_cartelRXTX].TInitial;
       Env += ':';
       Env += ConfigRXTX[index_cartelRXTX].TEnd;
       Env += ':';
       Env += ConfigRXTX[index_cartelRXTX].Tprv_rev;
       Env += ':';
       Env += ConfigRXTX[index_cartelRXTX].Tprv_fwd;
       Env += ':';
       Env += (char)0x0D;
       Serial.print(Env);
       index_cartelRXTX++;
       //if(index_cartelRXTX == CTDCARTELES)
       if(index_cartelRXTX >= CTDCARTELES)
        index_cartelRXTX = 0;
       //added v1.2:
       idx_utiles++;
       if(idx_utiles >= CTDmsgs_utiles)
       {
        CycleCompleted = true;
        idx_utiles = 0;
        //Serial.println("deb: Cycle Completed!!!");
       }
       else
        CycleCompleted = false;

       KeepTimeUpdated = false; //Ya no hay que mantener el tiempo actualizado, pues ya no esta en pantalla, lo que hay en pantalla es otro msg
       
       //return true; //Habia msj para mandar y se mandó
      }//(i_unseful < CTDCARTELES)
     // else
      // return false; ningun msj para mandar
}
