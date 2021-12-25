
#include "clock.h"
TClock MyClock; //instancia de TClock

void SaveClockSettings();
void SendTime(unsigned long int, boolean);
//
//   The HTML PAGE
//
const char PAGE_ClockSettings[] PROGMEM = R"=====(
<meta name="viewport" content="width=device-width, initial-scale=1" />
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<link rel="stylesheet" href="style.css" type="text/css" />
<script src="microajax.js"></script> 
<a href="settings.html"  class="btn btn--s"><</a>&nbsp;&nbsp;<strong>Clock</strong>
<hr>
<form action="" method="post">
<table border="0"  cellspacing="0" cellpadding="3" style="width:310px" >
<tr><td align="left"><div id="time_label">Time: </div></td><td align="center"><input type="text" size="8" id="time_value" name="time_value" value="--:--:--"></td>
<td align="right">
<select  id="time_mode" name="time_mode">
  <option value="0">Disabled</option>
  <option value="1">AlwaysON</option>
  <option value="2">EveryMSGCycle</option>
  <option value="3">ByInterval</option>
</select>
</td>
</tr>
</table>
<table border="0"  cellspacing="3" cellpadding="3" style="width:310px" >
<tr>
<td align="center">
<a href="javascript:GetTime()"  style="width:75px" class="btn btn--m btn--blue">Get</a><br>
</td>
<td align="center">
<a href="javascript:RefreshClockPage()"  style="width:75px"  class="btn btn--m btn--blue">Refresh</a><br>
</td>
</tr>
</table>
<table border="0"  cellspacing="3" cellpadding="3" style="width:310px" >
<tr><td align="left"><div id="STD_label">24-Hour Clock</div></td><td align="right"><input type="checkbox"  id="STD" name="STD"></td></tr>
<tr><td align="left"><div id="interval_label">Interval (ms): </div></td><td align="right"><input type="text"  id="interval" name="interval" size="5" value="00000"></td></tr>
<tr><td align="left"><div id="showtime_label">ShowTime (ms): </div></td><td align="right"><input type="text"  id="showtime" name="showtime" size="5" value="00000"></td></tr>
</table>
<hr>
<table border="0"  cellspacing="3" style="width:310px">
<tr><td align="center"><input type="submit" style="width:150px" class="btn btn--m btn--blue" value="Save"></td></tr>
</table>
</form>
<script>




function GetTime()
{  
 var today = new Date();
 var time = today.getHours() + ":" + today.getMinutes() + ":" + today.getSeconds();
 document.getElementById("time_value").value = time;

}




function RefreshClockPage()
{
	setValues("/clockU/values");
}




window.onload = function ()
{
	load("style.css","css", function() 
	{
		load("microajax.js","js", function() 
		{
				setValues("/clock/values");
		});
	});
}

function load(e,t,n){if("js"==t){var a=document.createElement("script");a.src=e,a.type="text/javascript",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}else if("css"==t){var a=document.createElement("link");a.href=e,a.rel="stylesheet",a.type="text/css",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}}



</script>
)=====";



/*
const char PAGE_WaitAndReload[] PROGMEM = R"=====(
<meta http-equiv="refresh" content="5; URL=textos.html">
Please Wait....Saving Data...
)=====";
*/

void send_clock_configuration_values()  
{
  String values ="";

  values += "time_value|" + MyClock.Get()  + "|input\n";
  values += "time_mode|" + (String)TimeMode  + "|input\n";
  //values += "interval|" + IntervalStr  + "|input\n";
  values += "interval|" + (String)Interval  + "|input\n";
  //values += "showtime|" + ShowTStr  + "|input\n";
  values += "showtime|" + (String)ShowT  + "|input\n";
  values += "STD|" +  (String) (MilitaryStd ? "checked" : "") + "|chk\n";  //v1.4
  server.send ( 200, "text/plain", values);
  //Serial.println(__FUNCTION__);
}

void send_clockrefresh_configuration_values() //setValues("/clockU/values");   .../clockU/values...
{
  String values ="";

  values += "time_value|" + MyClock.Get()  + "|input\n";
  server.send ( 200, "text/plain", values);
  //Serial.println(__FUNCTION__);
}


void send_clock_configuration_html()  //boton submit
{
  if (server.args() > 0 )  // Save Settings
  {
    MilitaryStd = false;  //v1.4
    for ( uint8_t i = 0; i < server.args(); i++ )
    {
      if (server.argName(i) == "time_value") 
      {
        TimeStr = server.arg(i); 
        MyClock.Set(TimeStr);
      }
      //if(server.argName(i) == "time_mode") {TimeModeStr = server.arg(i);}
      if(server.argName(i) == "time_mode") {TimeMode = server.arg(i).toInt();}
      if(server.argName(i) == "interval") {Interval = server.arg(i).toInt();}
      if(server.argName(i) == "showtime") {ShowT = server.arg(i).toInt();}
      if(server.argName(i) == "STD")MilitaryStd = true; //v1.4
    }//for
       if(TimeMode_PRV != TimeMode)
       {
            if(TimeMode != 1)
             PanelSync();
             
            if(TimeMode == 0)
             //state = 2;  //Clock Disabled
             ChangePanelState(2);
            else if(TimeMode == 1)
             //state = 3;   //Clock Always ON
             ChangePanelState(3);
              //SendTime(t_x, false);
              //SendTime(millis(), false); //Se encargara el estado (var state) de enviarlo
              //Serial.println(F("deb: Sending Time from Web Server Clock page submit..."));
             //}
            else if(TimeMode == 2)
              //state = 4; //Clock at ...MSGCycle
              ChangePanelState(4);
            else if(TimeMode == 3)
            {
              //state = 5; //Clock ByInterval
              ChangePanelState(5);
              //t_interval = t_x;
              t_interval = millis();
            }
       }//if(TimeMode_PRV != TimeMode)
            if(TimeMode_PRV != TimeMode || Interval_PRV != Interval || ShowT_PRV != ShowT || MilitaryStd_PRV != MilitaryStd)
            {
              //Solo salvar en la flash si es necesario,  a lo mejor solo se trata de un seteo dela hora y no es necesario salvar nada en ningun file....
              TimeMode_PRV = TimeMode;
              Interval_PRV = Interval;
              ShowT_PRV = ShowT;
              MilitaryStd_PRV = MilitaryStd;
              SaveClockSettings();
            }
    //server.send ( 200, "text/html", PAGE_WaitAndReload );
  }//if (server.args() > 0 )
  server.send ( 200, "text/html", PAGE_ClockSettings); 
 //Serial.println(__FUNCTION__); 
}
//----------------------------------------------------------------------------------------------
/*
void SetClockSettings()
{
  
}
*/
//----------------------------------------------------------------------------------------------
///*
void SendTime(unsigned long int t,boolean efect = true)
{
  //unsigned long int actualShowT;

  //if(KeepTimeUpdated && state != 3)
  if(KeepTimeUpdated)  //Debe funcionar de todas formas aunque no se ponga state != 3
  {
    unsigned long dif = (unsigned long)(t - t_last_timesended);
    if(dif < actualShowT)
     actualShowT -= dif;
    else
     actualShowT = 0;
   //if(TimeMode == 1)
     //actualShowT = 62000;
  }
  else
  {
   //if(efect)
    actualShowT = ShowT;
   //else
    //actualShowT = 62000;
  }
  
  
  t_last_timesended = t;
  boolean notime = false;
  String Aux;
  
  if(MilitaryStd)
   Aux = MyClock.GetII();
  else
    Aux =  MyClock.GetAMPM();
   
  if(Aux == "--:--")
  {
   Aux = ":24:";
   notime = true;
  }
       String Env = "<C:";  //Clock
       Env += Aux.length();
       Env += ':';
       Env += Aux;
       Env += ':';
       if(notime)
        Env += '4';  //Tipo de Letra: bitmap
       else
        Env += '1';  //Tipo de Letra: Double CharI
       Env += ':';
       
       if(efect)
        Env += '1';  //Efecto: Estacionario
       else
        Env += '0';  //Efecto: Ninguno   //Se quedara ahi...

       Env += ':';
       Env += '0';  //Entrada: Ninguno  //new in v1.5
       Env += ':';
       Env += '0';  //Exit: Ninguno
       Env += ':';
       Env += '1';  //Ctd. de espacios entre caracteres: 1
       Env += ':';
       Env += '0';  //offset
       Env += ':';
       Env += '0';  //inverse: false
       Env += ':';
       Env += '1';  //Nloops: 1  //En este caso con efecto estacionario o ninguno(ningun efecto) no importa el valor de Nloops
       Env += ':';
       Env += "0"; //TScrollExit
       Env += ':';
       Env += "0";  //TScrollIN   //new in v1.5
       Env += ':';
       Env += "0";  //TScrollEfect
       Env += ':';

       Env += String(actualShowT);  //TInitial
 
       Env += ':';
       Env += "0"; //TEnd
       Env += ':';
       Env += "0"; //tpr
       Env += ':';
       Env += "0"; //tpf
       Env += ':';
       Env += (char)0x0D;
       Serial.print(Env);
       enviarTime = false;
       KeepTimeUpdated = true; //Ha sido enviado un tiempo, favor de mantenerlo actualizado
       
}
//*/
//-----------------------------------------------------------------------------------------------------------------------
void SaveClockSettings()
{
  
  File MyFile = SPIFFS.open("clocksettings.dat", "w");
  if(!MyFile)
      Serial.println("file open failed");
  else
  {
    //Write data to file
   //Serial.println(F("deb: Writing Data to File"));
   MyFile.println(TimeMode);
   MyFile.println(Interval);
   MyFile.println(ShowT);
   MyFile.println(MilitaryStd);
   MyFile.close();
  }
}
//-----------------------------------------------------------------------------------------------------------------------
void LoadClockSettings()
{
  File MyFile = SPIFFS.open("clocksettings.dat", "r");
  if(!MyFile)
      Serial.println("file open failed"); //pudiera confundir al arduino nano... aunque en este se valida el protocolo de cierta forma
  else
  {
   
    //TimeMode = ((MyFile.readStringUntil('\n')).substring(0, A[i].msg.length() - 1)).toInt();
    TimeMode = (MyFile.readStringUntil('\n')).toInt();
    Interval = (MyFile.readStringUntil('\n')).toInt();
    ShowT = (MyFile.readStringUntil('\n')).toInt();
    MilitaryStd = (MyFile.readStringUntil('\n')).toInt();
    TimeMode_PRV = TimeMode;
    Interval_PRV = Interval;
    ShowT_PRV = ShowT;
    MilitaryStd_PRV = MilitaryStd;
    MyFile.close();
  }//else
}
