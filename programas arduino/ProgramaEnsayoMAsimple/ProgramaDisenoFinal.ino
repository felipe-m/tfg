#include <LiquidCrystal.h>
LiquidCrystal lcd(16,17,23,25,27,29);
//pins de la shield
      #define BEEPER 37           // Beeper conectado a GADGETS3D shield MEGA_18BEEPER
      #define LCD_PINS_RS 16      // LCD control conectado a GADGETS3D  shield LCDRS
      #define LCD_PINS_ENABLE 17  // LCD enable pin conectado a GADGETS3D shield LCDE
      #define LCD_PINS_D4 23      // LCD signal pin, conectado a GADGETS3D shield LCD4
      #define LCD_PINS_D5 25      // LCD signal pin, conectado a GADGETS3D shield LCD5
      #define LCD_PINS_D6 27      // LCD signal pin, conectado a GADGETS3D shield LCD6
      #define LCD_PINS_D7 29      // LCD signal pin, conectado a GADGETS3D shield LCD7
      #define BTN_EN1 31          // Encoder, conectado a GADGETS3D shield S_E1
      #define BTN_EN2 33          // Encoder, cconectado a GADGETS3D shield S_E2
      #define BTN_ENC 35          // Encoder Click, connected to Gadgets3D shield S_EC
      #define X_STEP_PIN 54       // PIN de pulsos para avanzar el motor
      #define X_DIR_PIN 55        // PIN para indicar la direccion en la que debe avanzar
      #define X_MIN_PIN 3         // PIN para el fin de carrera colocado al inicio del recorrido
      #define X_MAX_PIN 2         // PIN para el fin de carrera colocado al final del recorrido
// signos
       byte empty[8] = 
       {          
          B00000,
          B00100,
          B01010,
          B10001,
          B10001,
          B01010,
          B00100,          
       };   
        
       byte full[8] = 
       {
          B00000,
          B00100,
          B01110,
          B11111,
          B11111,
          B01110,
          B00100,      
        };

       byte micro[8] = 
       {
          B00000,
          B10001,
          B10001,
          B11011,
          B10101,
          B10000,
          B10000,
       };
//variables internas
      bool btn_en1,btn_en2,btn_enc,btn_en1_prev,btn_en2_prev ;               //variables de lectura directa del encoder
      bool fc_inic_X,fc_fin_X = false;                                       //variables de lectura de los fin de carrera
      bool pulso_X,dirreccion_X = false;                                     //variables para escribir los motores
      bool derecha, izquierda, pulsador = false;                             // variables de lectura del encoder interpretadas
      int volatile estado,estado_ant  = 0;                                   // Variables del estado (valores de 0 a 5)
      int posicion = 0;                                                      // Variable de posicion en la pantalla lcd
      int i,j,k,t = 0 ;                                                      // diversos contadores
      int volatile di,df,v= 0;                                               // variables del ensayo (distancia inicial, distancia final, velocidad)
      long x, x_mm= 0 ;                                                      // variable de la distancia puntual (en micras)
      bool activado = true;                                                 // variable que indica la señal que envia un fin de carrera al activarse
void setup() {
  
      pinMode(BTN_EN1,INPUT_PULLUP);    // Encoder 1
      pinMode(BTN_EN2,INPUT_PULLUP);    // Encoder 2
      pinMode(BTN_ENC,INPUT_PULLUP);    // Encoder Swith
      pinMode(BEEPER,OUTPUT);           // BEEPER
      pinMode(X_STEP_PIN,OUTPUT);       // Pasos del motor X
      pinMode(X_DIR_PIN,OUTPUT);        // Dirreccion del motor X
      pinMode(X_MIN_PIN,INPUT);         // Fin de carrera inicio
      pinMode(X_MAX_PIN,INPUT);         // Fin de carrera terminal
      
      btn_en1 , btn_en1_prev = digitalRead(BTN_EN1);
      btn_en2 , btn_en2_prev = digitalRead(BTN_EN2);
      btn_enc = digitalRead(BTN_ENC);
      

      lcd.begin(20,4);
      lcd.createChar(0, empty);
      lcd.createChar(1,  full);
      lcd.createChar(2, micro);

      lcd.clear();
      lcd.setCursor(1,0);
      lcd.print("ENSAYO DE MODELO");
      lcd.setCursor(1,1);
      lcd.print("ANALOGO");
      lcd.setCursor(1,3);
      lcd.print("iniciar experimento");
}
/////////////////ESTADOS//////////////////////////////////////////////////
void inicio()////////INICIO, estado 0/////////////////////////////////////
{     
 
 if (pulsador == true)
      {
        
        estado=1;
        estado_ant = 0;

        j,k,posicion=0;
        di=0;
        df= 0;
        v = 0;
        x=0;
        x_mm= 0;
        
        lcd.clear();
        lcd.setCursor(1,0);
        lcd.print("dist. inic");
        lcd.setCursor(18,0);
        lcd.print("mm");
        lcd.setCursor(1,1);
        lcd.print("dist.final");
        lcd.setCursor(18,1);
        lcd.print("mm");
        lcd.setCursor(1,2);      
        lcd.print("velocidad");
        lcd.setCursor(16,2);
        lcd.write(byte(2));
        lcd.setCursor(17,2);
        lcd.print("m/s");
        lcd.setCursor(1,3);     
        lcd.print("iniciar experimento");
        
      }
}
void DefinicionDeVariables()////DEFINICION DE VARIABLES , estado 1//////////
{
  lcd.setCursor(0,0);
  lcd.write(byte(0));
  lcd.setCursor(0,1);
  lcd.write(byte(0));
  lcd.setCursor(0,2);
  lcd.write(byte(0));
  lcd.setCursor(0,3);
  lcd.write(byte(0));
  lcd.setCursor(0,posicion);
  lcd.write(byte(1));
  lcd.setCursor(12,0);
  lcd.print("---");
  lcd.setCursor(12,0);
  lcd.print(di);
  lcd.setCursor(12,1);
  lcd.print("---");
  lcd.setCursor(12,1);
  lcd.print(df);
  lcd.setCursor(12,2);
  lcd.print("---");
  lcd.setCursor(12,2);
  lcd.print(v);
  if (df< di)
  {
    df=di;
  }

  if (pulsador == true  and k<3) //k es una variable que avanza en horizontal por la pantalla, cuando vale 0 podemos cambia en vertical con derecha o izquierda, cuando no aumentamos la variable con la que estemos trabajando
  {
    k++;
  }
  else if (pulsador == true and k==3)
  {
    k=0;
  }
  switch (posicion) 
  {
    case 1: 
        switch (k)
        {
          case 1://posicion =1, k=1
                if (derecha == true and df+100<400)
                {
                  df = df + 100;
                }
                else if (izquierda == true and df-100 >= 0)
                {
                  df = df-100;
                }
          break;
          case 2: //posicion = 1, k=2
                if (derecha == true and df+10<400)
                {
                  df = df + 10;
                }
                else if (izquierda == true and df-10 >= 0)
                {
                  df = df-10;
                }
           break;
           case 3: //posicion = 1, k =3
                if (derecha == true and df+1<400)
                {
                  df = df + 1;
                }
                else if (izquierda == true and df-1 >= 0)
                {
                  df = df-1;
                }
           break;
           default:
                if (derecha == true )
                {
                  posicion = 2;
                }
                else if (izquierda == true)
                {
                  posicion = 0;
                }
            break;
        }

    break;
    case 2:
     if ( k>1 or k== 0 )
     {
      k==0;
             if (derecha == true )
                {
                  posicion = 3;
                }
             else if (izquierda == true)
                {
                  posicion = 1;
                }
     }
     else 
     {
             if (derecha == true and t<15 )
                {
                  t++;
                }
             else if (izquierda == true and t>0)
                {
                  t=t-1;
                }
             v= 234.35 / t ;
     }
     

        
    break;
    case 3:
 
              if( pulsador == true)
              {
                estado=2;
                estado_ant=1;
                
                lcd.clear();
                lcd.setCursor(1,0);
                lcd.print("PREPARANDO");
                lcd.setCursor(1,1);
                lcd.print("EXPERIMENTO...");
                lcd.setCursor(9,2);
                lcd.print("/");
                lcd.print(di);
                lcd.print("000");
                lcd.print(" ");
                lcd.write(byte(2));
                lcd.print("m"); 
                j,k = 0;
              }
    
    
              if (derecha == true )
                {
                  posicion = 0;
                  k=0;
                }
              else if (izquierda == true)
                {
                  posicion = 2;
                  k=0;
                }

    break;
    default:
    switch (k)
        {
          case 1://posicion =0, k=1
                if (derecha == true and di+100<400)
                {
                  di = di + 100;
                }
                else if (izquierda == true and di-100 >= 0)
                {
                  di = di-100;
                }
          break;
          case 2: //posicion = 0, k=2
                if (derecha == true and di+10<400)
                {
                  di = di + 10;
                }
                else if (izquierda == true and di-10 >= 0)
                {
                  di = di-10;
                }
           break;
           case 3: //posicion = 0, k =3
                if (derecha == true and di+1<400)
                {
                  di = di + 1;
                }
                else if (izquierda == true and di-1 >= 0)
                {
                  di = di-1;
                }
           break;
           default:
                if (derecha == true )
                {
                  posicion = 2;
                }
                else if (izquierda == true)
                {
                  posicion = 0;
                }
            break;
        }

    break;

  }
  
 }
void preparacion()/////////////PREPARACION, estado 2////////////////////////
{
if(x_mm < di and fc_fin_X != activado)
  {
    pulso_X=!pulso_X;
    dirreccion_X=true;
    if (pulso_X = true);
    {
      j++;
      delay(1);
    }
    if(j>61)
    {
      x=x+15;
      j=0;
      lcd.setCursor(3,2);
      lcd.print("      ");
      lcd.setCursor(3,2);
      lcd.print(x_mm);
       if(x<100)
      {
        if(x<10)
        {
          lcd.print("0");
        }
        lcd.print("0");
      }
      lcd.print(x);
      delay(1);
    }
    if (x > 1000)
    {
      x= x-1000;
      x_mm = x_mm+1;
    }
    if (pulsador == true)
    {
    pulso_X=false;
    dirreccion_X=true;
    estado = 4;
    estado_ant = 2;
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("PAUSA");
    lcd.setCursor(2,2);
    lcd.print(" continuar");
    lcd.setCursor(2,3);
    lcd.print(" terminar");
    lcd.setCursor(2,1);
    lcd.print(x_mm);
    if(x<100)
      {
        if(x<10)
        {
          lcd.print("0");
        }
        lcd.print("0");
      }
    lcd.print(x);
    lcd.print("/");
    lcd.print(di);
    lcd.print("000");
    lcd.print(" ");
    lcd.write(byte(2));
    lcd.print("m"); 
    posicion = 1; 
    }
      
        
    
  }
  else if (fc_fin_X == activado)
  {
    pulso_X=false;
    dirreccion_X=true;
    estado = 4;
    estado_ant = 5;
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("PAUSA");
    lcd.setCursor(2,2);
    lcd.print(" terminar");
    lcd.setCursor(2,3);
    lcd.print(" terminar");
    lcd.setCursor(2,1);
    lcd.print(x_mm);
     if(x<100)
      {
        if(x<10)
        {
          lcd.print("0");
        }
        lcd.print("0");
      }
    lcd.print(x);
    lcd.print("/");
    lcd.print(df);
    lcd.print("000");
    lcd.print(" ");
    lcd.write(byte(2));
    lcd.print("m");
    posicion = 1; 
  }
  
  else
  {
    pulso_X=false;
    dirreccion_X=true;
    estado = 4;
    estado_ant = 3;
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("PAUSA");
    lcd.setCursor(2,2);
    lcd.print(" continuar");
    lcd.setCursor(2,3);
    lcd.print(" terminar");
    lcd.setCursor(2,1);
    lcd.print(x_mm);
     if(x<100)
      {
        if(x<10)
        {
          lcd.print("0");
        }
        lcd.print("0");
      }
    lcd.print(x);
    lcd.print("/");
    lcd.print(di);
    lcd.print("000");
    lcd.print(" ");
    lcd.write(byte(2));
    lcd.print("m"); 
    posicion = 1; 
  }
}
void accion()/////////////////////ACCION estado (3)///////////////////////////////////////
{
if(x_mm < df and fc_fin_X != activado)
  {
    pulso_X=!pulso_X;
    dirreccion_X=true;
    if (pulso_X = true);
    {
      j++;
      delay(t);
    }
    if(j>61)
    {
      x=x+15;
      j=0;
      lcd.setCursor(3,2);
      lcd.print("      ");
      lcd.setCursor(3,2);
      lcd.print(x_mm);
      if(x<100)
      {
        if(x<10)
        {
          lcd.print("0");
        }
        lcd.print("0");
      }
      lcd.print(x);
      delay(t);
    }
    if (x > 1000)
    {
      x= x-1000;
      x_mm = x_mm+1;
    }
    if (pulsador == true)
    {
    pulso_X=false;
    dirreccion_X=true;
    
    estado = 4;
    estado_ant = 3;
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("PAUSA");
    lcd.setCursor(2,2);
    lcd.print(" continuar");
    lcd.setCursor(2,3);
    lcd.print(" terminar");
    lcd.setCursor(2,1);
    lcd.print(x_mm);
     if(x<100)
      {
        if(x<10)
        {
          lcd.print("0");
        }
        lcd.print("0");
      }
    lcd.print(x);
    lcd.print("/");
    lcd.print(df);
    lcd.print("000");
    lcd.print(" ");
    lcd.write(byte(2));
    lcd.print("m"); 
    posicion = 1; 
    }
      
        
    
  }
  else
  {
    pulso_X=false;
    dirreccion_X=true;
    estado = 4;
    estado_ant = 5;
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("PAUSA");
    lcd.setCursor(2,2);
    lcd.print(" terminar");
    lcd.setCursor(2,3);
    lcd.print(" terminar");
    lcd.setCursor(2,1);
    lcd.print(x_mm);
     if(x<100)
      {
        if(x<10)
        {
          lcd.print("0");
        }
        lcd.print("0");
      }
    lcd.print(x);
    lcd.print("/");
    lcd.print(df);
    lcd.print("000");
    lcd.print(" ");
    lcd.write(byte(2));
    lcd.print("m");
    posicion = 1; 
  }
}
void pausa()////////////////PAUSA estado(4)////////////////////////////////////////
{

if (derecha == true or izquierda == true  )
{
  if (posicion == 1)
  {
    posicion = 2;
  }
  else
  {
    posicion = 1;
  }
}
if (posicion == 1)
{
lcd.setCursor(1,2);
lcd.write(byte(1));
lcd.setCursor(1,3);
lcd.write(byte(0));
           if (pulsador == true and estado_ant ==2)
           {
                          estado=2;
                          estado_ant=1;
                          
                          lcd.clear();
                          lcd.setCursor(1,0);
                          lcd.print("PREPARANDO");
                          lcd.setCursor(1,1);
                          lcd.print("EXPERIMENTO...");
                          lcd.setCursor(9,2);
                          lcd.print("/");
                          lcd.print(di);
                          lcd.print("000");
                          lcd.print(" ");
                          lcd.write(byte(2));
                          lcd.print("m"); 
                          j,k = 0;
           }
           else if (pulsador == true and estado_ant == 3)
           {
                          estado=3;
                          estado_ant=1;
                          
                          lcd.clear();
                          lcd.setCursor(1,0);
                          lcd.print("REALIZANDO");
                          lcd.setCursor(1,1);
                          lcd.print("EXPERIMENTO...");
                          lcd.setCursor(9,2);
                          lcd.print("/");
                          lcd.print(df);
                          lcd.print("000");
                          lcd.print(" ");
                          lcd.write(byte(2));
                          lcd.print("m"); 
                          j,k = 0;
            
           }
           else if (pulsador == true and estado_ant ==5)
           {
                          estado=5;
                          estado_ant=1;
                          
                          lcd.clear();
                          lcd.setCursor(1,0);
                          lcd.print("EXPERIMENTO");
                          lcd.setCursor(1,1);
                          lcd.print("TERMINADO");
                          lcd.setCursor(9,2);
                          lcd.print("/");
                          lcd.print("000000");
                          lcd.print(" ");
                          lcd.write(byte(2));
                          lcd.print("m"); 
                          j,k = 0;
            
           }
}
else
{
 lcd.setCursor(1,2);
 lcd.write(byte(0));
 lcd.setCursor(1,3);
 lcd.write(byte(1));
   if (pulsador == true )
           {
                          estado=5;
                          estado_ant=1;
                          
                          lcd.clear();
                          lcd.setCursor(1,0);
                          lcd.print("EXPERIMENTO");
                          lcd.setCursor(1,1);
                          lcd.print("TERMINADO");
                          lcd.setCursor(9,2);
                          lcd.print("/");
                          lcd.print("000000");
                          lcd.print(" ");
                          lcd.write(byte(2));
                          lcd.print("m"); 
                          j,k = 0;
            
           }
}
}
void reinicio()//////////REINICIO estado (5)////////////////////////////////////////////////
{

  if (pulsador  == true)
  {
    
    pulso_X=false;
    dirreccion_X=true;
    estado = 4;
    estado_ant = 5;
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("PAUSA");
    lcd.setCursor(2,2);
    lcd.print(" terminar");
    lcd.setCursor(2,3);
    lcd.print(" terminar");
    lcd.setCursor(2,1);
    lcd.print(x_mm);
     if(x<100)
      {
        if(x<10)
        {
          lcd.print("0");
        }
        lcd.print("0");
      }
    lcd.print(x);
    lcd.print("/");
    lcd.print(df);
    lcd.print("000");
    lcd.print(" ");
    lcd.write(byte(2));
    lcd.print("m");
    posicion = 1; 
  }
  if (x_mm <1 and x < 40 or fc_inic_X != activado )
  
      {
            estado=0;
            estado_ant = 5;
              
            lcd.clear();
            lcd.setCursor(1,0);
            lcd.print("ENSAYO DE MODELO");
            lcd.setCursor(1,1);
            lcd.print("ANALOGO");
            lcd.setCursor(1,3);
            lcd.print("iniciar experimento");

              j,k= 0;
      }
  else 
  {
          pulso_X=!pulso_X;
          dirreccion_X= false;
          if (pulso_X = true);
                {
                  j++;
                  delay(1);
                }
          if(j>61)
                {
                 j=0;
                 if(x-15 > 0)
                 {
                      x=x-15;
                  
                 }
                 else
                 {
                      x_mm = x_mm -1;
                      x= x+985;
                  }
                  lcd.setCursor(3,2);
                  lcd.print("      ");
                  lcd.setCursor(3,2);
                  lcd.print(x_mm);
                  if(x<100)
                  {
                      lcd.print("0");
                  }
                  lcd.print(x);
                  delay(1);
                 if(pulsador == true)
                    {
                    pulso_X=false;
                    dirreccion_X=true;
                    estado = 4;
                    estado_ant = 3;
                    lcd.clear();
                    lcd.setCursor(1,0);
                    lcd.print("PAUSA");
                    lcd.setCursor(2,2);
                    lcd.print(" continuar");
                    lcd.setCursor(2,1);
                    lcd.print(x_mm);
                    if(x<100)
                      {
                        lcd.print("0");
                      }
                    lcd.print(x);
                    lcd.print("/");
                    lcd.print(df);
                    lcd.print("000");
                    lcd.print(" ");
                    lcd.write(byte(2));
                    lcd.print("m"); 
                    posicion = 1;
                    } 
  }
  }

}
///////////////////////////////////////////////////////////////////////////
void loop()  // variables de intermedias de salida (izquierda, derecha , pulsador, fc_inic_X, fx_fin_X)
  {

      btn_en1 = digitalRead(BTN_EN1);
      btn_en2 = digitalRead(BTN_EN2);
      btn_enc = digitalRead(BTN_ENC); 
      fc_inic_X = digitalRead(X_MIN_PIN);
      fc_fin_X  = digitalRead(X_MAX_PIN);
      digitalWrite(X_STEP_PIN,pulso_X);
      digitalWrite(X_DIR_PIN,dirreccion_X);
      
      if (btn_enc == false)//detector de flanco del pulsador/////////////////
      {
        i++;         
      }
      if (i >= 3)
      {
        digitalWrite(BEEPER,true);
         pulsador = true;
         i=0;
         delay(200);
        digitalWrite(BEEPER,false);
      }
      else
      {
        pulsador = false;
      }
      //////////////////////////////////////////////////////////////////////
      if (btn_en1 != btn_en1_prev || btn_en2 != btn_en2_prev)
      {   
          if ( btn_en2 == false & btn_en1 == false & btn_en2_prev == true & btn_en1_prev == false)
            {
              derecha = true;
              izquierda = false;
            } 
         else if( btn_en2 == false & btn_en1 == false & btn_en2_prev == false & btn_en1_prev == true )
            {
              derecha = false;
              izquierda = true;
            }  
         else
            { 
              
              derecha = false;
              izquierda = false;
                 
            }
            
      }
      else
      {
        derecha = false;
        izquierda = false;
             
      }
        btn_en1_prev = btn_en1;
        btn_en2_prev = btn_en2;
       /////////////////////////////////////////////////////////////////////
      switch (estado) // gestion de estados
      {
          case 1: 
            DefinicionDeVariables();
            break;
          case 2:
            preparacion  ();
            break;
          case 3:
            accion  ();
            break;
          case 4:
            pausa();
            break;
          case 5:
            reinicio();
            break;
          default:
            inicio();
            break;
      }//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  } 
