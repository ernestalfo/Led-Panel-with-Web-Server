//
//  HTML PAGE
//
const char PAGE_ABCSettings[] PROGMEM = R"=====(
<meta name="viewport" content="width=device-width, initial-scale=1" />
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<link rel="stylesheet" href="style.css" type="text/css" />
<script src="microajax.js"></script> 
<a href="textos.html"  class="btn btn--s"><</a>&nbsp;&nbsp;<strong>ABC  </strong><span id="index_texts">ukn/15</span>
<hr>
<form action="" method="post">
<table border="0"  cellspacing="0" cellpadding="3" style="width:310px" >
<tr>
<td align="left">spaces: </td><td align="right"><input type="text" id="SbC" name="SbC" size="2" value="0"></td>
</tr>
<tr>
<td align="left">offset: </td><td align = "right"><input type="text" id="off" name="off" size="1" value="0"></td>
</tr>
<tr>
<td align = "left">Inverse: </td>
<td align = "right">
<select  id="inv" name="inv">
  <option value="0">false</option>
  <option value="1">true</option>
  <option value="2">toggle</option>
</select>
</td>
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
				setValues("/abc/values");
		});
	});
}
function load(e,t,n){if("js"==t){var a=document.createElement("script");a.src=e,a.type="text/javascript",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}else if("css"==t){var a=document.createElement("link");a.href=e,a.rel="stylesheet",a.type="text/css",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}}



</script>
)=====";

void send_abc_configuration_html()  //boton submit
{
  if (server.args() > 0 )  // Save Settings
  {
    for ( uint8_t i = 0; i < server.args(); i++ )
    {
      if (server.argName(i) == "SbC") ConfigWS[index_cartel - 1].spaces = server.arg(i); 
      if (server.argName(i) == "off")  ConfigWS[index_cartel - 1].offset =  server.arg(i); 
      if (server.argName(i) == "inv") ConfigWS[index_cartel - 1].inverse =  server.arg(i); 
    }
  }
  server.send ( 200, "text/html", PAGE_ABCSettings); 
}

void  send_abc_configuration_values()
{
  String values ="";

  values += "index_texts|" + (String)index_cartel + "/15" +  "|div\n";
  values += "SbC|" + ConfigWS[index_cartel - 1].spaces + "|input\n";
  values += "off|" + ConfigWS[index_cartel - 1].offset + "|input\n";
  values += "inv|" + ConfigWS[index_cartel - 1].inverse + "|input\n";

  server.send ( 200, "text/plain", values);
}
