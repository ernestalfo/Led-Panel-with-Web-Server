//
//  HTML PAGE
//
const char PAGE_Profiles[] PROGMEM = R"=====(
<meta name="viewport" content="width=device-width, initial-scale=1" />
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<link rel="stylesheet" href="style.css" type="text/css" />
<script src="microajax.js"></script> 
<a href="settings.html"  class="btn btn--s"><</a>&nbsp;&nbsp;<strong>Current Profile </strong>
<hr>
<form action="" method="post">
<table border="0"  cellspacing="0" cellpadding="3" style="width:310px" >
<tr>
<td align = "left">Profile: </td>
<td align = "right">
<select  id="profile" name="profile" onchange="RefreshValues(this)">
  <option value="0">p1</option>
  <option value="1">p2</option>
  <option value="2">p3</option>
  <option value="3">p4</option>
  <option value="4">p5</option>
</select>
</td>
</tr>
<tr>
<td align = "left">default: </td>
<td align = "right"><div id="defaultlabel">No</div></td>
</tr>
</table>
<table border="0"  cellspacing="3" style="width:310px" >
<tr><td align="center"><a href="javascript:SetAsDefault()" style="width:150px"  class="btn btn--m btn--blue">Set as DEFAULT</a></td></tr>
<tr><td align="center"><a href="javascript:LoadAndSend()" style="width:150px"  class="btn btn--m btn--blue">Load & Send</a></td></tr>
</table>
<hr>
<div id="infolabel">N/A</div>
<script>


function SetAsDefault()
{
   document.getElementById('infolabel').innerHTML = "Saving Defult Choice...";
   setValues("profiles/default");
}

function LoadAndSend()
{
   document.getElementById('infolabel').innerHTML = "Loading and Sendig Messages in current profile...";
   setValues("profiles/loadandsend");
}

function RefreshValues(e)
{
   switch(e.value)
   {
    case '0': setValues("profiles/p1/values");
            break;
    case '1': setValues("profiles/p2/values");
            break;
    case '2': setValues("profiles/p3/values");
            break;
    case '3': setValues("profiles/p4/values");
            break;
    case '4': setValues("profiles/p5/values");
            break;
   }
}

window.onload = function ()
{
  load("style.css","css", function() 
  {
    load("microajax.js","js", function() 
    {
        setValues("/profiles/values");
    }); });
}
function load(e,t,n){if("js"==t){var a=document.createElement("script");a.src=e,a.type="text/javascript",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}else if("css"==t){var a=document.createElement("link");a.href=e,a.rel="stylesheet",a.type="text/css",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}}



</script>
)=====";

//Ya no pinta mucho un btn submit por aqui, pues el perfil actual o current profile es cambiado cte cdo se selecciona otro en el componente select.
/*
void send_profiles_configuration_html()  //boton submit
{
  if (server.args() > 0 )  // Save Settings
  {
    for ( uint8_t i = 0; i < server.args(); i++ )
    {
      //if (server.argName(i) == "profile") index_profile = server.arg(i).toInt(); 
      //if (server.argName(i) == "profilename")  NamesProfiles[index_profile] =  server.arg(i); 
      if (server.argName(i) == "profile")
      {
        index_profile = server.arg(i).toInt(); 
        switch(index_profile)
        {
          case 0: FileName = "CONFIG_MSGS_FILE_P1";
                  break;
          case 1: FileName = "CONFIG_MSGS_FILE_P2";
                  break;
          case 2: FileName = "CONFIG_MSGS_FILE_P3";
                  break;
          case 3: FileName = "CONFIG_MSGS_FILE_P4";
                  break;
          case 4: FileName = "CONFIG_MSGS_FILE_P5";
                  break;
       }
      }
    }
  }
  server.send ( 200, "text/html", PAGE_Profiles); 
}
*/

void  send_profiles_configuration_values()
{
  String values ="";

  values += "profile|" + (String)index_profile+  "|input\n";
  //values += "profilename|" + NamesProfiles[index_profile] + "|input\n";
  if(index_profile == profileByDefault)
   values += "defaultlabel|Yes|div\n";
  else
   values += "defaultlabel|No|div\n";


  server.send ( 200, "text/plain", values);
}

void  profiles_default_f()
{
  if(profileByDefault != index_profile)//v1.4 //Verificando para no escribir en la flash si no es necesario...
  {
   File configFile = SPIFFS.open("profileboot.dat", "w");
   if(!configFile)
       Serial.println("file open failed");
   else
   {
     //Write data to file
     //Serial.println("Writing Data to File");
     profileByDefault = index_profile;
     switch(profileByDefault)
     {
      case 0: FileNameDefault = "CONFIG_MSGS_FILE_P1";
              break;
      case 1: FileNameDefault = "CONFIG_MSGS_FILE_P2";
              break;
      case 2: FileNameDefault = "CONFIG_MSGS_FILE_P3";
              break;
      case 3: FileNameDefault = "CONFIG_MSGS_FILE_P4";
              break;
      case 4: FileNameDefault = "CONFIG_MSGS_FILE_P5";
              break;
     }
     configFile.println(FileNameDefault);
     configFile.println(profileByDefault);
     configFile.close();
     FileName = FileNameDefault;  //added last rev
   }
  }//if(profileByDefault != index_profile)
   String values ="";
   //---------------
   //values += "profile|" + (String)index_profile+  "|input\n";  //no estorba, pero tampoco hace falta
  if(index_profile == profileByDefault)
   values += "defaultlabel|Yes|div\n";
  else
   values += "defaultlabel|No|div\n";
  //------------------  added v1.2
   values += "infolabel|" + String("Done!!!") +  "|div\n";
   server.send( 200, "text/plain", values);
}

void profiles_change_f()
{
  switch(index_profile)  //v1.4 //movido aqui desde la antigua funcionalidad del btn submit que ya no existe
  {
          case 0: FileName = "CONFIG_MSGS_FILE_P1";
                  break;
          case 1: FileName = "CONFIG_MSGS_FILE_P2";
                  break;
          case 2: FileName = "CONFIG_MSGS_FILE_P3";
                  break;
          case 3: FileName = "CONFIG_MSGS_FILE_P4";
                  break;
          case 4: FileName = "CONFIG_MSGS_FILE_P5";
                  break;
   }
 String values ="";
 if(index_profile == profileByDefault)
   values += "defaultlabel|Yes|div\n";
  else
   values += "defaultlabel|No|div\n";
   
 server.send ( 200, "text/plain", values); 
}

void profiles_p1()
{
  index_profile = 0;
  profiles_change_f();
}

void profiles_p2()
{
  index_profile = 1;
  profiles_change_f();
}

void profiles_p3()
{
  index_profile = 2;
  profiles_change_f();
}

void profiles_p4()
{
  index_profile = 3;
  profiles_change_f();
}

void profiles_p5()
{
  index_profile = 4;
  profiles_change_f();
}

void profiles_loadandsend_f()
{
  LoadingConfig(ConfigWS, FileName.c_str()); //Loading
  String values ="";
  if(state == 3)
   values += "infolabel|" + String("Clock: Always ON, MSG no sended") +  "|div\n";
  else
  {
    if(state == 6)
     state = prv_state;  //Retrieving last state....
    SendToRamRXTX();
    PanelSync();
    values += "infolabel|" + String("Start Sended") +  "|div\n";
  }
  server.send( 200, "text/plain", values);
}
/*
void LoadDefaultName(String &filename)
{
  File configFile = SPIFFS.open(filename, "r");
  if(!configFile)
      Serial.println("file open failed");
  else
  {
     filename = configFile.readStringUntil('\n');
     filename = filename.substring(0, filename.length() - 1);
  }
}
*/

void LoadDefaults()
{
  File configFile = SPIFFS.open("profileboot.dat", "r");
  if(!configFile)
      Serial.println("file open failed");
  else
  {
     FileNameDefault = configFile.readStringUntil('\n');
     FileNameDefault = FileNameDefault.substring(0, FileNameDefault.length() - 1);
     String tmp = configFile.readStringUntil('\n');
     profileByDefault = (tmp.substring(0, FileNameDefault.length() - 1)).toInt();
     index_profile = profileByDefault;
     configFile.close();
     FileName = FileNameDefault;  //added last rev
  }
}
