//
//   The HTML PAGE
//
const char PAGE_TextosMainSettings[] PROGMEM = R"=====(
<meta name="viewport" content="width=device-width, initial-scale=1" />
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<link rel="stylesheet" href="style.css" type="text/css" />
<script src="microajax.js"></script> 
<a href="index.html"  class="btn btn--s"><</a>&nbsp;&nbsp;<strong>Texts</strong>
<hr>
<table border="0"  cellspacing="0" cellpadding="3" style="width:310px" >
<tr><td align="left"><b><a href="javascript:Prev()">Prev</a></b></td><td align="center"><span id="index_texts">ukn/15</span></td><td align="right"><b><a href="javascript:Next()">Next</a></b></td></tr>
</table>
<form action="" method="post">
<table border="0"  cellspacing="0" cellpadding="3" style="width:310px" >
<tr><td align="center"><input type="text" style="width:300px" id="texto" name="texto" value=""></td></tr>
</table>
<table border="0"  cellspacing="0" cellpadding="3" style="width:310px" >
<tr><td align="left">ABC</td>
<td>
<select  id="abc" name="abc">
  <option value="0">SingleChar_I</option>
  <option value="1">DoubleChar_I</option>
  <option value="2">DoubleChar_II</option>
  <option value="3">SingleChar_II</option>
  <option value="4">bitmaps</option>
</select>
</td>
<td colspan="2" align="right"><a href="abc.html" class="btn btn--m btn--blue">Config...</a></td>
</tr>
<tr>
<td align="left">Efect</td>
<td>
<select  id="efect" name="efect">
  <option value="0">-</option>
  <option value="1">Static</option>
  <option value="2">Left Scroll</option>
  <option value="3">Dual Scroll</option> 
  <option value="4">Animation</option> 
  <option value="5">Blink</option>  //En algun momento lo perdi, pero no es nuevo en etsa version, no se donde se perdio, pero bueno aqui ya de nuuevo
</select>
</td>
<td align = "right" colspan="2"><a href="efectos.html"  class="btn btn--m btn--blue">Config...</a></td>
</tr>
<tr>
<td align="left">Entrance</td>
<td>
<select  id="entrance" name="entrance">
  <option value="0">-</option>
  <option value="1">Right</option>
  <option value="2">Left</option>
</select>
</td>
<td align="right">Scroll: (ms)</td><td><input type="text" id="TS_IN" name="TS_IN" size="4" value="0000"></td>
</tr>
<tr>
<td align="left">Exit</td>
<td>
<select  id="exit" name="exit">
  <option value="0">-</option>
  <option value="1">Left</option>
  <option value="2">Right</option>
  <option value="3">Right II</option>
</select>
</td>
<td align="right">Scroll: (ms)</td><td><input type="text" id="TS" name="TS" size="4" value="0000"></td>
</tr>
</table>
<hr>
<table border="0"  cellspacing="3" style="width:310px" >
<tr><td align="center"><input type="submit" style="width:150px" class="btn btn--m btn--blue" value="Save"></td></tr>
</table>
</form>
<script>

function Prev()
{
	setValues("/textos/prev");
}


function Next()
{
	setValues("/textos/next");
}

window.onload = function ()
{
	load("style.css","css", function() 
	{
		load("microajax.js","js", function() 
		{
				setValues("/textos/values");
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

void send_textos_configuration_values()  
{
  String values ="";

  values += "index_texts|" + (String)index_cartel + "/15" +  "|div\n";
  values += "texto|" + ConfigWS[index_cartel - 1].msg + "|input\n";
  values += "abc|" + ConfigWS[index_cartel - 1].ABC + "|input\n";
  values += "efect|" + ConfigWS[index_cartel - 1].E + "|input\n";
  values += "entrance|" + ConfigWS[index_cartel - 1].IN + "|input\n";      //new in v1.5
  values += "TS_IN|" + ConfigWS[index_cartel - 1].TScrollIN + "|input\n";  //new in v1.5
  values += "exit|" + ConfigWS[index_cartel - 1].Exit + "|input\n";
  values += "TS|" + ConfigWS[index_cartel - 1].TScrollExit + "|input\n";
  
  server.send ( 200, "text/plain", values);
  //Serial.println(__FUNCTION__);
}

void send_next_html()
{
  index_cartel++;
  if(index_cartel > CTDCARTELES)
   index_cartel = 1;
   
  send_textos_configuration_values(); 
  //Serial.println(__FUNCTION__); 
}

void send_prev_html()
{
   index_cartel--;
  if(index_cartel == 0)
   //index_cartel = 1;
   index_cartel = CTDCARTELES;
   
  send_textos_configuration_values(); 
  //Serial.println(__FUNCTION__); 
}

void send_textos_configuration_html()  //boton submit
{
  if (server.args() > 0 )  // Save Settings
  {
    for ( uint8_t i = 0; i < server.args(); i++ )
    {
      if (server.argName(i) == "texto") ConfigWS[index_cartel - 1].msg = server.arg(i); 
      if (server.argName(i) == "abc")  ConfigWS[index_cartel - 1].ABC =  server.arg(i); 
      if (server.argName(i) == "efect") ConfigWS[index_cartel - 1].E =  server.arg(i); 
      if (server.argName(i) == "entrance") ConfigWS[index_cartel - 1].IN =  server.arg(i);        //new in v1.5
      if (server.argName(i) == "TS_IN") ConfigWS[index_cartel - 1].TScrollIN =  server.arg(i);    //new in v1.5
      if (server.argName(i) == "exit") ConfigWS[index_cartel - 1].Exit =  server.arg(i);
      if (server.argName(i) == "TS") ConfigWS[index_cartel - 1].TScrollExit =  server.arg(i);
    }
    //server.send ( 200, "text/html", PAGE_WaitAndReload );
  }
  server.send ( 200, "text/html", PAGE_TextosMainSettings);
 //Serial.println(__FUNCTION__); 
}
