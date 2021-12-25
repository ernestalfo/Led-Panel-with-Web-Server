//
//  HTML PAGE
//
const char PAGE_EfectosSettings[] PROGMEM = R"=====(
<meta name="viewport" content="width=device-width, initial-scale=1" />
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<link rel="stylesheet" href="style.css" type="text/css" />
<script src="microajax.js"></script> 
<a href="textos.html"  class="btn btn--s"><</a>&nbsp;&nbsp;<strong>Efecto  </strong><span id="index_texts">ukn/15</span>
<hr>
<form action="" method="post">
<table border="0"  cellspacing="0" cellpadding="3" style="width:310px" >
<tr>
<td align="left">N Loops: </td><td align="right"><input type="text" id="Nreps" name="Nreps" size="5" value="00000"></td>
</tr>
<tr>
<td align="left">T Scroll (ms): </td><td align = "right"><input type="text" id="Tscroll" name="Tscroll" size="5" value="00000"></td>
</tr>
<tr>
<td align="left">T Initial (ms): </td><td align = "right"><input type="text" id="Tinitial" name="Tinitial" size="5" value="00000"></td>
</tr>
<tr>
<td align="left">T End (ms): </td><td align = "right"><input type="text" id="Tend" name="Tend" size="5" value="00000"></td>
</tr>
<tr>
<td align="left">T prev rev (ms): </td><td align = "right"><input type="text" id="Tprevrev" name="Tprevrev" size="5" value="00000"></td>
</tr>
<tr>
<td align="left">T prev fwd (ms): </td><td align = "right"><input type="text" id="Tprevfwd" name="Tprevfwd" size="5" value="00000"></td>
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
				setValues("/efectos/values");
		});
	});
}
function load(e,t,n){if("js"==t){var a=document.createElement("script");a.src=e,a.type="text/javascript",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}else if("css"==t){var a=document.createElement("link");a.href=e,a.rel="stylesheet",a.type="text/css",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}}



</script>
)=====";

void send_efectos_configuration_html()  //boton submit
{
  if (server.args() > 0 )  // Save Settings
  {
    for ( uint8_t i = 0; i < server.args(); i++ )
    {
      if (server.argName(i) == "Nreps") ConfigWS[index_cartel - 1].Nloops = server.arg(i); 
      if (server.argName(i) == "Tscroll")  ConfigWS[index_cartel - 1].TScrollE =  server.arg(i); 
      if (server.argName(i) == "Tinitial") ConfigWS[index_cartel - 1].TInitial =  server.arg(i); 
      if (server.argName(i) == "Tend") ConfigWS[index_cartel - 1].TEnd = server.arg(i); 
      if (server.argName(i) == "Tprevrev")  ConfigWS[index_cartel - 1].Tprv_rev =  server.arg(i); 
      if (server.argName(i) == "Tprevfwd") ConfigWS[index_cartel - 1].Tprv_fwd =  server.arg(i); 
    }
  }
  server.send ( 200, "text/html", PAGE_EfectosSettings); 
}

void send_efectos_configuration_values() 
{
  String values ="";
  
  values += "index_texts|" + (String)index_cartel + "/15" +  "|div\n";
  values += "Nreps|" + ConfigWS[index_cartel - 1].Nloops + "|input\n";
  values += "Tscroll|" + ConfigWS[index_cartel - 1].TScrollE + "|input\n";
  values += "Tinitial|" + ConfigWS[index_cartel - 1].TInitial + "|input\n";
  values += "Tend|" + ConfigWS[index_cartel - 1].TEnd + "|input\n";
  values += "Tprevrev|" + ConfigWS[index_cartel - 1].Tprv_rev + "|input\n";
  values += "Tprevfwd|" + ConfigWS[index_cartel - 1].Tprv_fwd + "|input\n";

  server.send ( 200, "text/plain", values);
}
