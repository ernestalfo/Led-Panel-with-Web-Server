//
//  HTML PAGE
//
const char PAGE_NetworkSettings[] PROGMEM = R"=====(
<meta name="viewport" content="width=device-width, initial-scale=1" />
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<link rel="stylesheet" href="style.css" type="text/css" />
<script src="microajax.js"></script> 
<a href="index.html"  class="btn btn--s"><</a>&nbsp;&nbsp;<strong>NETWORK Settings</strong><span id="index_texts">ukn/15</span>
<hr>
<form action="" method="post">
<table border="0"  cellspacing="0" cellpadding="3" style="width:310px" >
<tr>
<td align="left">SSID: </td><td align="right"><input type="text" style="width:200px" id="ssid" name="ssid" placeholder="SSID"></td>
</tr>
<tr>
<td align="left">password: </td><td align="right"><input type="password" style="width:200px" id="pass" name="pass" placeholder="password"></td>
</tr>
</table>
<hr>
<table border="0"  cellspacing="3" style="width:310px" >
<tr><td align="center"><input type="submit" style="width:150px" class="btn btn--m btn--blue" value="Save"></td></tr>
</table>
</form>
<script>



window.onload = function ()
{
	load("style.css","css", function() 
	{
		load("microajax.js","js", function() 
		{
				setValues("/network/values");
		});
	});
}
function load(e,t,n){if("js"==t){var a=document.createElement("script");a.src=e,a.type="text/javascript",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}else if("css"==t){var a=document.createElement("link");a.href=e,a.rel="stylesheet",a.type="text/css",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}}



</script>
)=====";

String pass = "", ssid = "";

void send_network_configuration_html()  //boton submit
{
  
  if (server.args() > 0 )  // Save Settings
  {
    for ( uint8_t i = 0; i < server.args(); i++ )
    {
      if (server.argName(i) == "ssid")ssid = server.arg(i); 
      if (server.argName(i) == "pass") pass =  server.arg(i);  
    }
    File configFile = SPIFFS.open("networksettings.dat", "w");
    if(!configFile)
      Serial.println("file open failed");
     else
     {
       //Write data to file
       //Serial.println("Writing Data to File");
       configFile.println(ssid);
       configFile.println(pass);
       configFile.close();
     }
  }
  server.send ( 200, "text/html", PAGE_NetworkSettings); 
}

void  send_network_configuration_values()
{
  String values ="";


  values += "SbC|" + ConfigWS[index_cartel - 1].spaces + "|input\n";
  values += "off|" + ConfigWS[index_cartel - 1].offset + "|input\n";
  values += "inv|" + ConfigWS[index_cartel - 1].inverse + "|input\n";

  server.send ( 200, "text/plain", values);
}
