//Utilities.h
#define CTD_DIGS 3 //Ctd max de digitos
//------------------------------------------------------------------------
//debe devolver unsigned int
//int tenpow(short int power)
unsigned long int tenpow(short int power)
{
  //int N = 1;
  unsigned long int N = 1;
  for(int i = 0; i < power; i++)
   N = N * 10;
  return N;
}
//-------------------------------------------------------------------------
boolean ConvertToInt(char *s_num, int &N)
{
  int dig = 0;  //digitos
  N = 0;
  
  while(s_num[dig] != 'x' && dig < CTD_DIGS ) {dig++;}
 
  for(int i = 0, k = dig -1; i < dig; i++, k--) 
  {
    if(s_num[i] >= '0' && s_num[i] <= '9')
     N += tenpow(k) * (s_num[i] - '0');
    else
     return false;
  }
  return true;
}//boolean ConvertToInt(char *s_num, int &N)
//---------------------------------------------------------------------------
//!FetchNumber(str, i, num)) //i, num son pasados por referencia
boolean FetchNumber(char *str, int &inx, int &num)
{
  char array_tmp[] = {'x', 'x', 'x'};
  //char array_tmp[ctd_digs ];
  inx++;   // ´:´
  int i = 0;
  while(str[inx] != '\0' && i <= CTD_DIGS)
  {
    if(str[inx] != ':' && i < CTD_DIGS)
      array_tmp[i++] = str[inx++];
    else
      return ConvertToInt(array_tmp, num);
    //inx++;
  }//while 
  return false;
}//boolean FetchNumber(char *str, int &inx, int &num)
//----------------------------------------------------------------------
//devuelve la ctd de caracteres antes del terminador null, sino encuentra el terminador null antes de llegar a 100, devuelve 100
int GetQtyCh (char *s)
{
  int i = 0;
  while(s[i] != '\0'  && i < 100)
   i++;

 return i;
}
//---------------------------------------------------------------------------
//.. Swaping significants bits , MSB to LSB, the whole byte ir reversed
byte SwapSB (byte supplier)
{
  byte aux = 0;
  
  for(int j = 7; j >= 0; j--)
   supplier & (1 << j) ? aux = aux | (1 << (7 - j)) : aux = aux &  ~ (1 << (7 - j));

  return aux;
}
//----------------------------------------------------------------------------
void SwappingImages(byte IMG_info[])
{
   int ctd_bytes = pgm_read_byte(&IMG_info[0]);//el 1er byte nos dira la ctd de bytes de la imagen
    int i = 1;
    while(ctd_bytes > 0)
    {
      Serial.print("org: ");
      Serial.print(pgm_read_byte(&IMG_info[i]), HEX);
      Serial.print(" ---> rlt: ");
      Serial.println(SwapSB(pgm_read_byte(&IMG_info[i])), HEX);
      i++;
     ctd_bytes--;
    }
}
//-------------------------------------------------------------------------------
int StrIndexOf(char c, int index_i, char *str)
{
  while(str[index_i] != '\0')
  {
    if(str[index_i] == c)
     return index_i;
    index_i++;
  }
  return -1; //No se encontro
}
//--------------------------------------------------------------------------------
//Revisar, debe devolver un unsigned int
unsigned long int StrToInt(int index_i, int index_f, char *s_num)
//int StrToInt(int index_i, int index_f, char *s_num)
{
  //Serial.println(F("f:StrToInt()"));  //deb
  //Serial.print(F("index inicial: "));  //deb
  //Serial.println(index_i);
  //Serial.print(F("index final: "));  //deb
  //Serial.println(index_f);
  //int N = 0, k = 0;
  unsigned long int N = 0, k = 0;
  while(index_f >= index_i)
  {
    if(s_num[index_f] >= '0' && s_num[index_f] <= '9')
     N += tenpow(k) * (s_num[index_f] - '0');
    else
     return -1;
   index_f--; 
   k++;
  }
  //Serial.println(N); //deb
  return N;
}
//--------------------------------------------------------------------------------------
void CopyStrs(char *org, char *dest, int idx_i, int idx_f)
{
  int i = 0;
  char c =  org[idx_i++];
  while((idx_i - 1) <= idx_f && c != '\0')
  {
    //Serial.println(org[idx_i]); //deb
    //dest[i++] = org[idx_i++];
    dest[i++] = c;
    c = org[idx_i++];
  }
  dest[i] = '\0';  //Agregando fin de cadena...
}
//--------------------------------------------------------------------------------------
/*
  //testing:
 String testnum = ":12:425:3:";
 String testnum2 = ":a2:i:34:231:56t";
 int Num = 0;
 int index_char = 0;
 if(FetchNumber(testnum.c_str(), index_char, Num))
 {
  Serial.print(F("Expected number: 12 / Result: "));
  Serial.println(Num);
 }
 else
  Serial.println(F("1: wrong string"));

 //index_char = 3;
 if(FetchNumber(testnum.c_str(), index_char, Num))
 {
  Serial.print(F("Expected number: 425 / Result: "));
  Serial.println(Num);
 }
 else
  Serial.println(F("2: wrong string"));

  //index_char = 7;
 if(FetchNumber(testnum.c_str(), index_char, Num))
 {
  Serial.print(F("Expected number: 3 / Result: "));
  Serial.println(Num);
 }
 else
  Serial.println("3: wrong string");

 index_char = 0;
 if(FetchNumber(testnum2.c_str(), index_char, Num))
 {
  Serial.print(F("Expected number: a2 / Result: "));
  Serial.println(Num);
 }
 else
  Serial.println(F("4: wrong string"));

 //index_char = 3;
 if(FetchNumber(testnum2.c_str(), index_char, Num))
 {
  Serial.print(F("Expected number: i / Result: "));
  Serial.println(Num);
 }
 else
  Serial.println(F("5: wrong string"));

 //index_char = 5;
 if(FetchNumber(testnum2.c_str(), index_char, Num))
 {
  Serial.print(F("Expected number: 34 / Result: "));
  Serial.println(Num);
 }
 else
  Serial.println(F("6: wrong string"));

//index_char = 8;
 if(FetchNumber(testnum2.c_str(), index_char, Num))
 {
  Serial.print(F("Expected number: 231 / Result: "));
  Serial.println(Num);
 }
 else
  Serial.println(F("7: wrong string"));

 if(FetchNumber(testnum2.c_str(), index_char, Num))
 {
  Serial.print(F("Expected number: 56t / Result: "));
  Serial.println(Num);
 }
 else
  Serial.println(F("8: wrong string"));
 */
