#define TDELAY 1350

class TClock{
  public:
  int Hrs;
  int Mins;
  int Segs;
  int offset;
  boolean ValidTime;
  unsigned long int t_last = 0;
  void Update(unsigned long int t);
  void Set(String str_time);
  String Get();
  String GetII();
  String GetAMPM();
  TClock();
  void AddSeconds(int segundos);
};

TClock::TClock()
{
  t_last = 0;
  Hrs = 0;
  Mins = 0;
  Segs = 0;
  ValidTime = false;
  offset = 0;
}
//-------------------------------------------------------------
void TClock::Update(unsigned long int t)
{
 if(ValidTime)
 if((unsigned long)(t - t_last) > 1000) 
 {
  if(++Segs == 60)
  {
    Segs = 0;
    enviarTime = true;
    if(++Mins == 60)
    {
      Mins = 0;
      if(++Hrs == 24)
       Hrs = 0;
    }
  }
  offset = offset + ((unsigned long)(t - t_last) - 1000);
  if(offset >= 1000)
  {
    int e = offset / 1000;
    //int r = offset % 1000;
    offset = offset - e * 1000;
    AddSeconds(e); //Añadir segundos...
  }
  t_last = t;
 }//if((unsigned long)(t - t_last) > 1000)
}
//-----------------------------------------------------------------
void TClock::Set(String str_time)
{
  //Serial.println(F("deb: Setting the clock..."));
  //enviarTime = false;  //No se porq lo tenia en false..no se realmente
  enviarTime = true;
  int i = 0;
  int f = str_time.indexOf(':',i);
  if(f != -1)
  { 
   Hrs = str_time.substring(i, f).toInt();
   if(Hrs >= 24 || Hrs < 0){ValidTime = false; return;}
   //Serial.print("H: ");
   //Serial.println(Hrs);
   i = f + 1;
   f = str_time.indexOf(':', i);
   if(f != -1)
   {
    Mins = str_time.substring(i, f).toInt();
    i = f + 1;
    Segs = str_time.substring(i).toInt(); 
    if(Segs >= 60 || Segs < 0){ValidTime = false;return;}
   }
   else
   {
    Mins = str_time.substring(i, f).toInt();
    Segs = 0;
   }
   if(Mins >= 60 || Mins < 0){ValidTime = false; return;}
   //Serial.print("M: ");
   //Serial.println(Mins);
   //Serial.print("S: ");
   //Serial.println(Segs);
    ValidTime = true;
    //enviarTime = true;
    t_last = millis() - TDELAY;
    offset = 0;
    return;
  }
  ValidTime = false;
  //enviarTime = false;
}
//-------------------------------------------------------------------
String TClock::Get()
{
  String hora;
  if(ValidTime)
  {
    //Serial.println("There is a valid time...");
    hora = String(Hrs) + String(':') + String(Mins) + String(':') + String(Segs);
    //Serial.print("H: ");
    //Serial.println(Hrs);
    //Serial.print("M: ");
    //Serial.println(Mins);
    //Serial.print("S: ");
    //Serial.println(Segs);
    return hora;
  }
  else
  {
   //Serial.println("There is not a valid time...");
   hora = "--:--:--";
   return hora;
  }
}
//-------------------------------------------------------------------
String TClock::GetII()
{
  String hora = "";
  if(ValidTime)
  {
    if(Hrs < 10)
     hora += "0";
     
    hora += String(Hrs) + String(':');
    
    if(Mins < 10)
     hora += "0";
     
    hora +=  String(Mins);
    return hora; 
  }//if(ValidTime)
  else
  {
   hora = "--:--";
   return hora;
  }
}
//-------------------------------------------------------------------
String TClock::GetAMPM()
{
  String hora = "";
  if(ValidTime)
  {
    int hours = Hrs;
    boolean am = true;

    if(hours >= 12 )
    {
      am = false;
      if(hours > 12)
       hours = hours - 12;
    }

    if(hours == 0)
     hours = 12;
     
    if(hours < 10)
     hora += "0";
     
    hora += String(hours) + String(':');
    
    if(Mins < 10)
     hora += "0";
     
    hora +=  String(Mins);

    /*  //Hay que quitarlo por ahora, en el panel de 40 cols no cabe la hora junto con el AM/PM
    if(am)
     hora += " AM";
    else
     hora += " PM";
     */
     
    return hora; 
  }//if(ValidTime)
  else
  {
   hora = "--:--";
   return hora;
  }
}
//-------------------------------------------------------------------
void TClock::AddSeconds(int segundos)
{
  Segs += segundos;
  if(Segs >= 60)
  {
    int offX = Segs / 60;
    Segs = Segs - offX * 60;  //Segs = Segs % 60;
    Mins += offX;
    if(Mins >= 60)
    {
      offX = Mins / 60;
      Mins = Mins - offX * 60;
      Hrs += offX;
      if(Hrs >= 24)
       Hrs = Hrs - 24;
    }
  }
}
//-------------------------------------------------------------------
