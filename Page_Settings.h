//
//  HTML PAGE
//
const char PAGE_GeneralSettings[] PROGMEM = R"=====(
<meta name="viewport" content="width=device-width, initial-scale=1" />
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<link rel="stylesheet" href="style.css" type="text/css" />
<script src="microajax.js"></script> 
<a href="index.html"  class="btn btn--s"><</a>&nbsp;&nbsp;<strong>General Settings </strong>
<hr>
<a href="clock.html" style="width:150px" class="btn btn--m btn--blue">Clock</a><br>
<a href="profiles.html" style="width:150px" class="btn btn--m btn--blue">Profiles</a><br>
<a href="javascript:LedsONOFF()"  style="width:150px" class="btn btn--m btn--blue">Leds ON/OFF</a><br>
<a href="javascript:Test()"  style="width:150px" class="btn btn--m btn--blue">Test</a><br>
<a href="images.html"  style="width:150px" class="btn btn--m btn--blue">Images Index</a><br>
<a href="network.html"  style="width:150px" class="btn btn--m btn--blue">Advanced</a><br>
<hr>
<div id="ledsstatus">Leds: ON</div>
<hr>
<div id="infolabel">N/A</div>
<script>


function Test()
{
   setValues("settings/test");
}

function LedsONOFF()
{
   setValues("/settings/ledsonoff");
}

window.onload = function ()
{
	load("style.css","css", function() 
	{
		load("microajax.js","js", function() 
		{
				// Do something after load...
       setValues("/settings/values");
		});
	});
}
function load(e,t,n){if("js"==t){var a=document.createElement("script");a.src=e,a.type="text/javascript",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}else if("css"==t){var a=document.createElement("link");a.href=e,a.rel="stylesheet",a.type="text/css",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}}

</script>
)=====";
void settings_values()
{
  String values ="";
  if(state == 6)
    values += "ledsstatus|" + String("Leds: OFF") +  "|div\n";
  else
    values += "ledsstatus|" + String("Leds: ON") +  "|div\n";
  server.send ( 200, "text/plain", values);
}
//Con estas dos funciones aqui abajo no creo que haya problema con que no se mande info al servidor, y se pudiera quedar frizado etc, imagino que no, de todas formas chequear!!!
//Preferi mandar informacion, trabaja mas rapido asi
void settings_test()
{
     String values ="";
     if(state != 6)
     {
      //Espacio en blanco, invertido
      //efecto estacionario con 30 segundos en Tinitial, nigun efecto de salida
      String Env = "<C:1: :0:1:0:0:1:0:1:0:0:0:0:30000:0:0:0:";  //Clock  //En este caso no sera usado como clock, pero surtirá el efecto necesario, pues no se incrementara la ctd de msjs
                                                                  //en el Arduino Nano...
  
      Env += (char)0x0D;
      Serial.print(Env);
      values += "infolabel|" + String("Done!!!") +  "|div\n";
     }
     else
      values += "infolabel|" + String("Please Turn ON Led Panel first") +  "|div\n";
     server.send ( 200, "text/plain", values);
}
void settings_ledsonoff()
{
  String values ="";
  if(state != 6)
  {
   Serial.print("<S:0:");  //Poniendo al Arduino Nano en estado cero, analizar efecto del btn Next, cuando el ArduinoNano no conoce la ctd de msjs
                           //Realmente cuando único es necesario que el ArduinoNano sepa la ctd de msjs, es cuando se esta usando la opcion de inverse en Toggle
                           //lo que significa que una vez que el msj x es recibido se mostrara invertido y la proxima vez no y asi, es una opcion que esta en la interfaz, pero no creo que se 
                           //use mucho: Toggle: false/true/toggle, Realmente se obtienen mejores resultados con el efecto Blink
                           //de todas formas repensar todo el asunto y probar el efecto de next, valorar cambiar algunas implementaciones
                          
   Serial.write(0x0D);
   prv_state = state;
   state = 6;  //Leds OFF
   values += "infolabel|" + String("Done!!!") +  "|div\n";
   values += "ledsstatus|" + String("Leds: OFF") +  "|div\n";
  }
  else
  { 
    state = prv_state;
    PanelSync();  
    values += "ledsstatus|" + String("Leds: ON") +  "|div\n";
    values += "infolabel|" + String("Done!!!") +  "|div\n";
   //values += "infolabel|" + String("Led Panel is already OFF") +  "|div\n";
  }
  server.send ( 200, "text/plain", values);
}
