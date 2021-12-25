//
//  HTML PAGE
//
const char PAGE_ImagesIndex[] PROGMEM = R"=====(
<meta name="viewport" content="width=device-width, initial-scale=1" />
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<link rel="stylesheet" href="style.css" type="text/css" />
<script src="microajax.js"></script> 
<a href="settings.html"  class="btn btn--s"><</a>&nbsp;&nbsp;<strong>Images Index</strong>
<hr>
<table border="2"  cellspacing="3" cellpadding="3" style="width:310px" >
<tr>
<td align="left">:0:</td><td align="right">Happy Face</td>
</tr>
<tr>
<td align="left">:1:</td><td align = "right">Copa_A f1 (empty)</td>
</tr>
<tr>
<td align="left">:2:</td><td align="right">Copa_A f2</td>
</tr>
<tr>
<td align="left">:3:</td><td align = "right">Copa_A f3</td>
</tr>
<td align="left">:4:</td><td align="right">Copa_A f4 (full)</td>
</tr>
<tr>
<td align="left">:5:</td><td align = "right">Vinos</td>
</tr>
<tr>
<td align="left">:6:</td><td align="right">Valois</td>
</tr>
<tr>
<td align="left">:7:</td><td align = "right">Copa_B f1 (empty)</td>
</tr>
<tr>
<td align="left">:8:</td><td align="right">Copa_B f2</td>
</tr>
<tr>
<td align="left">:9:</td><td align = "right">Copa_B f3</td>
</tr>
<tr>
<td align="left">:10:</td><td align="right">Copa_B f4 (full)</td>
</tr>
<tr>
<td align="left">:11:</td><td align = "right">Happy Face v2</td>
</tr>
<td align="left">:12:</td><td align="right">Heart f1 (little)</td>
</tr>
<tr>
<td align="left">:13:</td><td align = "right">Heart f2 (median)</td>
</tr>
<tr>
<td align="left">:14:</td><td align="right">Heart f3 (big)</td>
</tr>
<tr>
<td align="left">:15:</td><td align = "right">Master (org)(Arley)</td>
</tr>
<tr>
<td align="left">:16:</td><td align="right">Master BlackM f1</td>
</tr>
<tr>
<td align="left">:17:</td><td align = "right">Master BlackM f2</td>
</tr>
<tr>
<td align="left">:18:</td><td align="right">Master BlackM f3</td>
</tr>
<tr>
<td align="left">:19:</td><td align = "right">Master BlackM f4</td>
</tr>
<td align="left">:20:</td><td align="right">Master BlackM f5</td>
</tr>
<tr>
<td align="left">:21:</td><td align = "right">Master BlackM f6</td>
</tr>
<tr>
<td align="left">:22:</td><td align="right">Movil (org)(Arley)</td>
</tr>
<tr>
<td align="left">:23:</td><td align = "right">Movil BlackM</td>
</tr>
<tr>
<td align="left">:24:</td><td align="right">Clock OFF(timeless)</td>
</tr>
<tr>
<td align="left">:25:</td><td align = "right">Space (3 cols)</td>
</tr>
<tr>
<td align="left">:26:</td><td align="right">V (Vinos/Valois)</td>
</tr>
<tr>
<td align="left">:27:</td><td align = "right">Vinos f2</td>
</tr>
<td align="left">:28:</td><td align="right">Vinos f3</td>
</tr>
<tr>
<td align="left">:29:</td><td align = "right">Vinos f4</td>
</tr>
<tr>
<td align="left">:30:</td><td align="right">Vinos f5</td>
</tr>
<tr>
<td align="left">:31:</td><td align = "right">Vinos f6</td>
</tr>
<tr>
<td align="left">:32:</td><td align="right">Vinos f7</td>
</tr>
<tr>
<td align="left">:33:</td><td align = "right">Valois f2</td>
</tr>
<td align="left">:34:</td><td align="right">Valois f3</td>
</tr>
<tr>
<td align="left">:35:</td><td align = "right">Valois f4</td>
</tr>
<tr>
<td align="left">:36:</td><td align="right">Valois f5</td>
</tr>
<tr>
<td align="left">:37:</td><td align = "right">Valois f6</td>
</tr>
<tr>
<td align="left">:38:</td><td align = "right">Valois f7</td>
</tr>
</table>
<hr>
<b><i>Animations Example: </i></b>
<br>
<br>
Select ABC: bitmaps and Efect: Animation. <br>
Enter to efect configuration and remember adjust Nloops and T Scroll parameters, and the others parameters as you need.
<br>
<br>
<table border="2"  cellspacing="3" cellpadding="3" style="width:310px" >
<tr>
<td align="left">:12:13:14:13:</td><td align="right">Heart beating</td>
</tr>
<tr>
<td align="left">:1:2:3:4:3:2:</td><td align="right">CopaA filling and emptying</td>
</tr>
</table>


<script>

window.onload = function ()
{
	load("style.css","css", function() 
	{
		load("microajax.js","js", function() 
		{
				//
		});
	});
}
function load(e,t,n){if("js"==t){var a=document.createElement("script");a.src=e,a.type="text/javascript",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}else if("css"==t){var a=document.createElement("link");a.href=e,a.rel="stylesheet",a.type="text/css",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}}



</script>
)=====";
