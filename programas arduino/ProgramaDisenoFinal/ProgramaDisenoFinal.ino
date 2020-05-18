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
      int i,j,k = 0 ;                                                        // diversos contadores
      int volatile di,df,v= 0;                                               // variables del ensayo (distancia inicial, distancia final, velocidad)
      long x, x_ant= 0 ;                                                            // variable de la distancia puntual (en micras)
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
}
/////////////////ESTADOS//////////////////////////////////////////////////
void inicio()////////INICIO, estado 0/////////////////////////////////////
{
      
      if (pulsador == true)
      {
        
        estado=1;
        estado_ant = 0;

        j,k=0;
        
        lcd.clear();
        lcd.setCursor(1,0);
        lcd.print("dist. inicial");
        lcd.setCursor(18,0);
        lcd.print("mm");
        lcd.setCursor(1,1);
        lcd.print("dist.final");
        lcd.setCursor(18,0);
        lcd.print("mm");
        lcd.setCursor(1,2);      
        lcd.print("velocidad");
        lcd.setCursor(18,2);
        lcd.write(byte(2));
        lcd.print("m");
        lcd.setCursor(1,3);     
        lcd.print("iniciar experimento");
        
      }
}
void DefinicionDeVariables()////DEFINICION DE VARIABLES , estado 1//////////
{
  lcd.setCursor(0,0);
  lcd.print(byte(1));
  lcd.setCursor(0,1);
  lcd.print(byte(1));
  lcd.setCursor(0,2);
  lcd.print(byte(1));
  lcd.setCursor(0,3);
  lcd.print(byte(1));
  lcd.setCursor(0,posicion);
  lcd.print(byte(2));
  lcd.setCursor(14,0);
  lcd.print("   ");
  lcd.setCursor(14,0);
  lcd.print(di);
  lcd.setCursor(14,1);
  lcd.print("  ");
  lcd.setCursor(14,1);
  lcd.print(df);
  lcd.setCursor(14,2);
  lcd.print("  ");
  lcd.setCursor(14,2);
  lcd.print(v);
  
  switch (posicion) 
  {
   case 0://posicion 0
        
       switch (k)
      {
          case 1: //posicion 0, k1
            if (pulsador == true)
            {
              k++;
            }
            
             if (derecha == true and di+100<400)
             {
              di=di+100;
             }
             else if (izquierda == true and di-100>0)
             {
              di=di-100;
             }
            
          break;
          case 2: //posicion 0 , k2
            if (pulsador == true)
            {
              k++;
            }  
              if (derecha == true and di+10 < 400)
              {
               di=di+10;
              }
              else if (izquierda == true and di-100>0)
              {
               di=di-10;
              }
            
          break;
          case 3: //posicion 0 , k3
            if (pulsador == true and k==3)
              {
                k=0;
              }      
             if (derecha == true and di+1<400)
             {
              di=di+1;
             }
             else if (izquierda == true and di-1 > 0)
             {
              di=di-1;
             }
        
          break;
          default: //posicion 0, k0
            if (derecha == true)
            {
              posicion++;
            }
            else if( izquierda == true)
            {
              posicion=posicion-1;
            }
          break;
      break;
      }
    case 1: //posicion 1
      switch (k)
      {
          case 1: //posicion 1 ,k1
            if (pulsador == true )
            {
              k++;
            }
            if (derecha == true and df+100<400)
            {
              df=df+100;
            }
            else if (izquierda == true and df-100>di)
            {
              df=df-100;
            }
              
          break;
          case 2: //posicion 1 , k2
            if (pulsador == true )
            {
              k++;
            }    
            if (derecha == true and df+10<400)
            {
               df=df+10;
            }
            else if (izquierda == true and df-10 > di)
            {
               df=df-10;
            }
                  
          break;
          case 3: //posicion 1, k3
            if (pulsador == true and k==3)
              {
                k=0;
              }
                     
            if (derecha == true and df+1 <400)
            {
              df=df+1;
            }
            else if (izquierda == true and df-1>di)
            {
              df=df-1;
            }
              
          break;
          default: //posicion 1, k0 
            if (derecha == true)
            {
              posicion++;
            }
            else if( izquierda == true)
            {
              posicion=posicion-1;
            }
          break;
      break;
      }
    case 2://posicion 2
      switch (k)
      {
        
          case 1 :// posicion 2, k1
            if (pulsador == true )
            {
              k++;
            }
             if (derecha == true and v+100 <150)
             {
              v=v+100;
             }
             else if (izquierda == true and v-100 > 0)
             {
              v=v-100;
             }
            
          break;
          case 2 :// posicion 2, k2
            if (pulsador == true )
            {
              k++;
            }
               
             if (derecha == true and v+10 <150)
             {
              v=v+10;
             }
             else if (izquierda == true  and v-10 >0)
             {
              v=v-10;
             }
            
          break;
          case 3 :// posicion 2, k3
                       
             if (derecha == true and v+1<150)
             {
                v=v+1;
             }
             else if (izquierda == true and v-1>0)
             {
                v=v-1;
             }
          
          break;
          default: //posicion 3, k0
            if (derecha == true)
            {
              posicion++;
            }
            else if( izquierda == true)
            {
              posicion=posicion-1;
            }
          break;   
      break;
      }
    default: //posicion 3
      if(pulsador == true)
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
              lcd.print(di*1000);
              lcd.print(" ");
              lcd.write(byte(2));
              lcd.print("m"); 

              j,k= 0;
      }
     break;
        
        
        }
  }
void preparacion()/////////////PREPARACION, estado 2////////////////////////
{
  if(x<di*1000)
  {
    pulso_X=!pulso_X;
    dirreccion_X=true;
    if (pulso_X = true);
    {
      j++;
    }
    if(j>61)
    {
      x=x+15;
      j=0;
      lcd.setCursor(3,2);
      lcd.print("      ");
      lcd.setCursor(3,2);
      lcd.print(x);
    }
    if (pulsador == true)
    {
    pulso_X=false;
    dirreccion_X=true;
    estado = 4;
    estado_ant = 2;
    
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("¿deseas continuar?");
    lcd.setCursor(2,2);
    lcd.print("si, continuar");
    lcd.setCursor(2,3);
    lcd.print("no, terminar");
    }
    
    
  }
  else
  {
    pulso_X=false;
    dirreccion_X=true;
    estado = 4;
    estado_ant = 3;
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("¿deseas continuar?");
    lcd.setCursor(2,2);
    lcd.print("si, continuar");
    lcd.setCursor(2,3);
    lcd.print("no, terminar");
  }
}
void accion()//////////////////ACCION, estado 3/////////////////////////////
{
    if(x<df*1000 and fc_fin_X== false)
  {
    pulso_X=!pulso_X;
    dirreccion_X=true;
    delay(234/v);
    if (pulso_X = true);
    {
      j++;
     
    }
    if(j>63)
    {
      x=x+15;
      j=0;
      lcd.setCursor(2,2);
      lcd.print("      ");
      lcd.setCursor(2,2);
      lcd.print(x); 
    }
    if (pulsador == true)
    {
    
    dirreccion_X=true;
    estado = 4;
    estado_ant = 3;
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("¿deseas continuar?");
    lcd.setCursor(2,2);
    lcd.print("si, continuar");
    lcd.setCursor(2,3);
    lcd.print("no, terminar");
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
    lcd.print("el ensayo ha");
    lcd.setCursor(1,1);
    lcd.print("terminado");
    lcd.setCursor(1,2);
    lcd.write(byte(1));
    lcd.print(" finalizar");
  }
}

void pausa()///////////////////PAUSA, estado 4 /////////////////////////////
{
  switch (estado_ant)
  {
  case 2:
  
        lcd.setCursor(1,2);
        lcd.write(byte(0));
        lcd.setCursor(1,3);
        lcd.write(byte(0));
        lcd.setCursor(1, posicion);
        lcd.write(byte(1));
    if (posicion= 2)
    {
       if (pulsador == true)
       {
        estado== estado_ant;
        lcd.clear();
        lcd.setCursor(1,0);
        lcd.print("PREPARANDO");
        lcd.setCursor(1,1);
        lcd.print("EXPERIMENTO...");
        lcd.setCursor(9,2);
        lcd.print("/");
        lcd.print(di*1000);
        lcd.print(" ");
        lcd.write(byte(2));
        lcd.print("m"); 

              j,k= 0;
        
       }
       if (derecha == true or izquierda == true)
       {
        posicion = 3;
       }
    }
    else 
    {
       if (derecha == true or izquierda == true)
       {
        posicion = 3;
       }
    }
 break;
 case 3: 
        lcd.setCursor(1,2);
        lcd.write(byte(0));
        lcd.setCursor(1,3);
        lcd.write(byte(0));
        lcd.setCursor(1, posicion);
        lcd.write(byte(1));
    if (posicion= 2)
    {
       if (pulsador == true)
       {
        estado== estado_ant;
        lcd.clear();
        lcd.setCursor(1,0);
        lcd.print("realizando");
        lcd.setCursor(1,1);
        lcd.print("experimento...");
        lcd.setCursor(8,2);
        lcd.print("/");
        lcd.print(df*1000);
        lcd.print(" ");
        lcd.write(byte(2));
        lcd.print("m");
        
       }
       if (derecha == true or izquierda == true)
       {
        posicion = 3;
       }
    }
    else 
    {
       if (derecha == true or izquierda == true)
       {
        posicion = 3;
       }
    }
break;
default:
      lcd.setCursor(1,2);
      lcd.write(byte(1));
      if (pulsador == true)
      {
        estado = 5;
        estado_ant=4;

        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print("reiniciando");
        lcd.setCursor(1,1);
        lcd.print("experimento ...");
        lcd.setCursor(1,3);
        lcd.print(x);
        lcd.setCursor(8,3);
        lcd.write(byte(2));
        lcd.print("m");
      }
break;
  }    
  
}
void reinicio()////////////////REINICIO, estado 5///////////////////////////
{
  pulso_X=!pulso_X;
  dirreccion_X=false;

  if (pulso_X = true);
    {
      j++;
     
    }
    if(j>63)
    {
      x=x-15;
      j=0;
      lcd.setCursor(1,3);
      lcd.print(x);
    }
 
  
  if (fc_inic_X == true or x<0)
  {
      estado=0;
      estado_ant=5;
      
      x,v,di,df,posicion,i,j,k=0;
      pulso_X,dirreccion_X,derecha,izquierda=false;

      lcd.clear();
      lcd.setCursor(1,0);
      lcd.print("ENSAYO DE MODELO");
      lcd.setCursor(1,1);
      lcd.print("ANALOGO");
      lcd.setCursor(1,3);
      lcd.print("iniciar experimento");
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
      digitalWrite(BEEPER,pulsador);
      if (btn_enc == true)//detector de flanco del pulsador/////////////////
      {
        i++;         
      }
      if (i >= 3)
      {
         pulsador = true;
         i=0;
      }
      else
      {
        pulsador = false;
      }
      //////////////////////////////////////////////////////////////////////
      if (btn_en1 != btn_en1_prev || btn_en2 != btn_en2_prev)//discriminacion entre el giro a derecha o izquierda
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
