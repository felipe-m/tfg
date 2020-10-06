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
      #define X_DIR_PIN 55        // PIN para indicar la di_Xreccion en la que debe avanzar
      #define X_MIN_PIN 3         // PIN para el fin de carrera colocado al inicio del recorrido
      #define X_MAX_PIN 2         // PIN para el fin de carrera colocado al final del recorrido
      #define Y_STEP_PIN 60
      #define Y_DIR_PIN 61
      #define Y_ENABLE_PIN 56
      #define Y_MIN_PIN 14
      #define Y_MAX_PIN 15
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
      bool btn_en1,btn_en2,btn_enc,btn_en1_prev,btn_en2_prev ;           //variables de lectura directa del encoder
      bool fc_inic_X,fc_fin_X,fc_inic_Y,fc_fin_Y = false;                //variables de lectura de los fin de carrera
      bool pulso,dirreccion = false;                                     //variables para escribir los motores
      bool derecha, izquierda, pulsador = false;                         // variables de lectura del encoder interpretadas
      int volatile estado,estado_ant, v   = 0;                           // Variables del estado (valores de 0 a 5)
      int posicion = 0;                                                  // Variable de posicion en la pantalla lcd
      int i,j,k,w,c= 0 ;                                                 // diversos contadores
      long di_X,di_Y,df_X,df_Y = 0L;                                     // variables del ensayo (di_Xstancia inicial, di_Xstancia final, velocidad)
      int pagina = 1;                                                    // variable de la pagina de seleccion de variables
      long x,y = 0L ;                                                    // variable de la distancia puntual (en micras)
      int t, tiempo;                                                     // variable de apoyo para definir la velocidad
      bool microsegundo, microsegundo_ant= false;                       // variable para indicar si ha pasado un microsegundo
void setup() {
      //INTERRUPCION CADA 50 MICROSEGUNDOS, 100 Khz
        cli();
        TCCR1A = 0;
        TCCR1B = 0;
        TCNT1  = 0;
        OCR1A = 199;
        TCCR1B |= (1 << WGM12);
        TCCR1B |= (0 << CS00)|(1 << CS01)|( 0 << CS02);
        TIMSK1 |= (1 << OCIE1A);
        sei();
  
      pinMode(BTN_EN1,INPUT_PULLUP);    // Encoder 1
      pinMode(BTN_EN2,INPUT_PULLUP);    // Encoder 2
      pinMode(BTN_ENC,INPUT_PULLUP);    // Encoder Swith
      pinMode(BEEPER,OUTPUT);           // BEEPER
      pinMode(X_STEP_PIN,OUTPUT);       // Pasos del motor X
      pinMode(X_DIR_PIN,OUTPUT);        // di_Xrreccion del motor X
      pinMode(X_MIN_PIN,INPUT);         // Fin de carrera inicio X
      pinMode(X_MAX_PIN,INPUT);         // Fin de carrera terminal X
      pinMode(Y_STEP_PIN,OUTPUT);       // Pasos del motor Y
      pinMode(Y_DIR_PIN,OUTPUT);        // di_Xrreccion del motor Y
      pinMode(Y_MIN_PIN,INPUT);         // Fin de carrera inicio Y
      pinMode(Y_MAX_PIN,INPUT);         // Fin de carrera terminal Y
      
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
      lcd.print("ANALOGO doble");
      lcd.setCursor(1,3);
      lcd.print("iniciar experimento");
}
ISR(TIMER1_COMPA_vect)
{
 
 w++;  
 if (w>tiempo)

    {
      w=0;
      j++;
      
    } 
 pulso =~ pulso;
 if (estado== 2 or estado== 3 or estado== 5 )
 {
 digitalWrite(Y_STEP_PIN,pulso);
 digitalWrite(X_STEP_PIN,pulso);
  
 }

}
/////////////////ESTADOS//////////////////////////////////////////////////

void inicio()////////INICIO, estado 0/////////////////////////////////////
{     
 
 if (pulsador == true)
      {
        
        estado=1;
        estado_ant = 0;

        j,k,posicion=0;
        di_X=0L;
        di_Y =0L;
        df_X=0L;
        df_Y = 0L;
        t = 5;
        tiempo = 5;
        v=50;
        x=0L;
        y =0L;
        pagina =1 ;
        
                                                        lcd.clear();
                                                        lcd.setCursor(1,0);
                                                        lcd.print("dist.iniX");
                                                        lcd.setCursor(12,0);
                                                        lcd.print(di_X);
                                                        lcd.setCursor(18,0);
                                                        lcd.print("mm");
                                                        lcd.setCursor(1,1);
                                                        lcd.print("dist.finX");
                                                        lcd.setCursor(12,1);
                                                        lcd.print(df_X);
                                                        lcd.setCursor(18,1);
                                                        lcd.print("mm");
                                                        lcd.setCursor(1,2);      
                                                        lcd.print("velocidad");
                                                        lcd.setCursor(12,2);
                                                        lcd.print(v);
                                                        lcd.setCursor(16,2);
                                                        lcd.write(byte(2));
                                                        lcd.setCursor(17,2);
                                                        lcd.print("m/s");
                                                        lcd.setCursor(1,3);     
                                                        lcd.print("   pagina -1-");
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
      }
}
void DefinicionDeVariables()////DEFINICION DE VARIABLES , estado 1//////////
{
  
if (pagina == 1)
{  

                                  
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
                                      if (df_X< di_X)
                                                {
                                                  df_X = di_X;
                                                  lcd.setCursor(12,1);
                                                  lcd.print("---");
                                                  lcd.setCursor(12,1);
                                                  lcd.print(df_X);
                                                }
                                      
                                          
                                          switch (k)
                                          {
                                            case 1://posicion =1, k=1
                                                  if (derecha == true and df_X+100<400)
                                                  {
                                                    df_X = df_X + 100;
                                                    lcd.setCursor(12,1);
                                                    lcd.print("---");
                                                    lcd.setCursor(12,1);
                                                    lcd.print(df_X);
                                                  }
                                                  else if (izquierda == true and df_X-100 >= 0)
                                                  {
                                                    df_X = df_X-100;
                                                    lcd.setCursor(12,1);
                                                    lcd.print("---");
                                                    lcd.setCursor(12,1);
                                                    lcd.print(df_X);
                                                  }
                                            break;
                                            case 2: //posicion = 1, k=2
                                                  if (derecha == true and df_X+10<400)
                                                  {
                                                    df_X = df_X + 10;
                                                    lcd.setCursor(12,1);
                                                    lcd.print("---");
                                                    lcd.setCursor(12,1);
                                                    lcd.print(df_X);
                                                  }
                                                  else if (izquierda == true and df_X-10 >= 0)
                                                  {
                                                    df_X = df_X-10;
                                                    lcd.setCursor(12,1);
                                                    lcd.print("---");
                                                    lcd.setCursor(12,1);
                                                    lcd.print(df_X);
                                                  }
                                             break;
                                             case 3: //posicion = 1, k =3
                                                  if (derecha == true and df_X+1<400)
                                                  {
                                                    df_X = df_X + 1;
                                                    lcd.setCursor(12,1);
                                                    lcd.print("---");
                                                    lcd.setCursor(12,1);
                                                    lcd.print(df_X);
                                                  }
                                                  else if (izquierda == true and df_X-1 >= 0)
                                                  {
                                                    df_X = df_X-1;
                                                    lcd.setCursor(12,1);
                                                    lcd.print("---");
                                                    lcd.setCursor(12,1);
                                                    lcd.print(df_X);
                                                  }
                                             break;
                                             default:
                                                  if (derecha == true )
                                                  {
                                                    posicion = 2;
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
                                                  }
                                                  else if (izquierda == true)
                                                  {
                                                    posicion = 0;
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
                                                        }
                                                     else if (izquierda == true)
                                                        {
                                                          posicion = 1;
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
                                                        }
                                             }
                                             else 
                                             {
                                                     if (izquierda == true and v>50)
                                                        {
                                                         v=v-5;
                                                         t= 1150/v ;
                                                         lcd.setCursor(12,2);
                                                         lcd.print("---");
                                                         lcd.setCursor(12,2);
                                                         lcd.print(v);
                                                        }
                                                     else if (derecha == true and v<200)
                                                        {
                                                         v=v+5;
                                                         t=  230000/(100*v*2) ;
                                                         lcd.setCursor(12,2);
                                                         lcd.print("---");
                                                         lcd.setCursor(12,2);
                                                         lcd.print(v);
                                                        }
                                                     
                                             }
                                             
                                        
                                                
                                            break;
                                            case 3:
                                         
                                                      if( pulsador == true)
                                                      {
                                                     
                                                        j,k = 0;
                                                        posicion = 0;
                                                        pagina = 2;
                                                        
                                                        lcd.clear();
                                                        lcd.setCursor(1,0);
                                                        lcd.print("dist.iniY");
                                                        lcd.setCursor(12,0);
                                                        lcd.print(di_Y);
                                                        lcd.setCursor(18,0);
                                                        lcd.print("mm");
                                                        lcd.setCursor(1,1);
                                                        lcd.print("dist.finY");
                                                        lcd.setCursor(12,1);
                                                        lcd.print(df_Y);;
                                                        lcd.setCursor(18,1);
                                                        lcd.print("mm");
                                                        lcd.setCursor(1,2);      
                                                        lcd.print("empezar experimento");
                                                        lcd.setCursor(1,3);     
                                                        lcd.print("   pagina -2-");
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
                                                      }
                                            
                                            
                                                      else if (derecha == true )
                                                        {
                                                          posicion = 0;
                                                          k=0;
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
                                                        }
                                                      else if (izquierda == true)
                                                        {
                                                          posicion = 2;
                                                          k=0;
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
                                                        }
                                        
                                      break;
                                      default: 
                                          if (df_X< di_X)
                                                {
                                                  df_X = di_X;
                                                  lcd.setCursor(12,1);
                                                  lcd.print("---");
                                                  lcd.setCursor(12,1);
                                                  lcd.print(df_X);
                                                }
                                      
                                          switch (k)
                                              {
                                                case 1://posicion =0, k=1
                                                      if (derecha == true and di_X+100<400)
                                                      {
                                                        di_X = di_X + 100;
                                                        lcd.setCursor(12,0);
                                                        lcd.print("---");
                                                        lcd.setCursor(12,0);
                                                        lcd.print(di_X);
                                                      }
                                                      else if (izquierda == true and di_X-100 >= 0)
                                                      {
                                                        di_X = di_X-100;
                                                        lcd.setCursor(12,0);
                                                        lcd.print("---");
                                                        lcd.setCursor(12,0);
                                                        lcd.print(di_X);
                                                        
                                                      }
                                                break;
                                                case 2: //posicion = 0, k=2
                                                      if (derecha == true and di_X+10<400)
                                                      {
                                                        di_X = di_X + 10;
                                                        lcd.setCursor(12,0);
                                                        lcd.print("---");
                                                        lcd.setCursor(12,0);
                                                        lcd.print(di_X);
                                                      }
                                                      else if (izquierda == true and di_X-10 >= 0)
                                                      {
                                                        di_X = di_X-10;
                                                        lcd.setCursor(12,0);
                                                        lcd.print("---");
                                                        lcd.setCursor(12,0);
                                                        lcd.print(di_X);
                                                      }
                                                 break;
                                                 case 3: //posicion = 0, k =3
                                                      if (derecha == true and di_X+1<400)
                                                      {
                                                        di_X = di_X + 1;
                                                        lcd.setCursor(12,0);
                                                        lcd.print("---");
                                                        lcd.setCursor(12,0);
                                                        lcd.print(di_X);
                                                        
                                                      }
                                                      else if (izquierda == true and di_X-1 >= 0)
                                                      {
                                                        di_X = di_X-1;
                                                        lcd.setCursor(12,0);
                                                        lcd.print("---");
                                                        lcd.setCursor(12,0);
                                                        lcd.print(di_X);
                                                      }
                                                 break;
                                                 default:
                                                      if (derecha == true )
                                                      {
                                                        posicion = 1;
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
                                                      }
                                                      else if (izquierda == true)
                                                      {
                                                        posicion = 3;
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
                                                      }
                                                  break;
                                              }
                                  
                                      break;
                                  
                                    }
}
else
{  

                                  
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
                                      if (df_Y< di_Y)
                                                {
                                                  df_Y = di_Y;
                                                  lcd.setCursor(12,1);
                                                  lcd.print("---");
                                                  lcd.setCursor(12,1);
                                                  lcd.print(df_Y);
                                                }
                                      
                                          
                                          switch (k)
                                          {
                                            case 1://posicion =1, k=1
                                                  if (derecha == true and df_Y+100<400)
                                                  {
                                                    df_Y = df_Y + 100;
                                                    lcd.setCursor(12,1);
                                                    lcd.print("---");
                                                    lcd.setCursor(12,1);
                                                    lcd.print(df_Y);
                                                  }
                                                  else if (izquierda == true and df_Y-100 >= 0)
                                                  {
                                                    df_Y = df_Y-100;
                                                    lcd.setCursor(12,1);
                                                    lcd.print("---");
                                                    lcd.setCursor(12,1);
                                                    lcd.print(df_Y);
                                                  }
                                            break;
                                            case 2: //posicion = 1, k=2
                                                  if (derecha == true and df_Y+10<400)
                                                  {
                                                    df_Y = df_Y + 10;
                                                    lcd.setCursor(12,1);
                                                    lcd.print("---");
                                                    lcd.setCursor(12,1);
                                                    lcd.print(df_Y);
                                                  }
                                                  else if (izquierda == true and df_Y-10 >= 0)
                                                  {
                                                    df_Y = df_Y-10;
                                                    lcd.setCursor(12,1);
                                                    lcd.print("---");
                                                    lcd.setCursor(12,1);
                                                    lcd.print(df_Y);
                                                  }
                                             break;
                                             case 3: //posicion = 1, k =3
                                                  if (derecha == true and df_Y+1<400)
                                                  {
                                                    df_Y = df_Y + 1;
                                                    lcd.setCursor(12,1);
                                                    lcd.print("---");
                                                    lcd.setCursor(12,1);
                                                    lcd.print(df_Y);
                                                  }
                                                  else if (izquierda == true and df_Y-1 >= 0)
                                                  {
                                                    df_Y = df_Y-1;
                                                    lcd.setCursor(12,1);
                                                    lcd.print("---");
                                                    lcd.setCursor(12,1);
                                                    lcd.print(df_Y);
                                                  }
                                             break;
                                             default:
                                                  if (derecha == true )
                                                  {
                                                    posicion = 2;
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
                                                  }
                                                  else if (izquierda == true)
                                                  {
                                                    posicion = 0;
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
                                                  }
                                              break;
                                          }
                                  
                                      break;
                                      case 2:
                                  
                                           
 
                                                      if( pulsador == true)
                                                      {
                                                        estado=2;
                                                        estado_ant=1;
                                                        di_X=di_X*1000L;
                                                        df_X=df_X*1000L;
                                                        di_Y=di_Y*1000L;
                                                        df_Y=df_Y*1000L;
                                                        
                                                        lcd.clear();
                                                        lcd.setCursor(1,0);
                                                        lcd.print("PREPARANDO");
                                                        lcd.setCursor(1,1);
                                                        lcd.print("EXPERIMENTO...");
                                                        tiempo=20;
                                                        j,k = 0;
                                                      }
                                            
                                            
                                                      if (derecha == true )
                                                        {
                                                          posicion = 3;
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
                                                          k=0;
                                                        }
                                                      else if (izquierda == true)
                                                        {
                                                          posicion = 1;
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
                                                          k=0;
                                                        }
                                        
                                                
                                            break;
                                            case 3:
                                         
                                                      if( pulsador == true)
                                                      {
                                                        j,k = 0;
                                                        posicion = 0;
                                                        pagina = 1;
                                                        
                                                        lcd.clear();
                                                        lcd.setCursor(1,0);
                                                        lcd.print("dist.iniX");
                                                        lcd.setCursor(12,0);
                                                        lcd.print(di_X);
                                                        lcd.setCursor(18,0);
                                                        lcd.print("mm");
                                                        lcd.setCursor(1,1);
                                                        lcd.print("dist.finX");
                                                        lcd.setCursor(12,1);
                                                        lcd.print(df_X);
                                                        lcd.setCursor(18,1);
                                                        lcd.print("mm");
                                                        lcd.setCursor(1,2);      
                                                        lcd.print("velocidad");
                                                        lcd.setCursor(12,2);
                                                        lcd.print(v);
                                                        lcd.setCursor(16,2);
                                                        lcd.write(byte(2));
                                                        lcd.setCursor(17,2);
                                                        lcd.print("m/s");
                                                        lcd.setCursor(1,3);     
                                                        lcd.print("   pagina -1-");
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
                                                        
                                                      }
                                            
                                            
                                                      else if (derecha == true )
                                                        {
                                                          posicion = 0;
                                                          k=0;
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
                                                        }
                                                      else if (izquierda == true)
                                                        {
                                                          posicion = 2;
                                                          k=0;
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
                                                        }
                                        
                                      break;
                                      default: 
                                          if (df_Y< di_Y)
                                                {
                                                  df_Y = di_Y;
                                                  lcd.setCursor(12,1);
                                                  lcd.print("---");
                                                  lcd.setCursor(12,1);
                                                  lcd.print(df_Y);
                                                }
                                      
                                          switch (k)
                                              {
                                                case 1://posicion =0, k=1
                                                      if (derecha == true and di_Y+100<400)
                                                      {
                                                        di_Y = di_Y + 100;
                                                        lcd.setCursor(12,0);
                                                        lcd.print("---");
                                                        lcd.setCursor(12,0);
                                                        lcd.print(di_Y);
                                                      }
                                                      else if (izquierda == true and di_Y-100 >= 0)
                                                      {
                                                        di_Y = di_Y-100;
                                                        lcd.setCursor(12,0);
                                                        lcd.print("---");
                                                        lcd.setCursor(12,0);
                                                        lcd.print(di_Y);
                                                        
                                                      }
                                                break;
                                                case 2: //posicion = 0, k=2
                                                      if (derecha == true and di_Y+10<400)
                                                      {
                                                        di_Y = di_Y + 10;
                                                        lcd.setCursor(12,0);
                                                        lcd.print("---");
                                                        lcd.setCursor(12,0);
                                                        lcd.print(di_Y);
                                                      }
                                                      else if (izquierda == true and di_Y-10 >= 0)
                                                      {
                                                        di_Y = di_Y-10;
                                                        lcd.setCursor(12,0);
                                                        lcd.print("---");
                                                        lcd.setCursor(12,0);
                                                        lcd.print(di_Y);
                                                      }
                                                 break;
                                                 case 3: //posicion = 0, k =3
                                                      if (derecha == true and di_Y+1<400)
                                                      {
                                                        di_Y = di_Y + 1;
                                                        lcd.setCursor(12,0);
                                                        lcd.print("---");
                                                        lcd.setCursor(12,0);
                                                        lcd.print(di_Y);
                                                        
                                                      }
                                                      else if (izquierda == true and di_Y-1 >= 0)
                                                      {
                                                        di_Y = di_Y-1;
                                                        lcd.setCursor(12,0);
                                                        lcd.print("---");
                                                        lcd.setCursor(12,0);
                                                        lcd.print(di_Y);
                                                      }
                                                 break;
                                                 default:
                                                      if (derecha == true )
                                                      {
                                                        posicion = 1;
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
                                                      }
                                                      else if (izquierda == true)
                                                      {
                                                        posicion = 3;
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
                                                      }
                                                  break;
                                              }
                                  
                                      break;
                                  
                                    }
}
}
void preparacion()/////////////PREPARACION, estado 2////////////////////////
{ 
  dirreccion=true;                   
  if ( x >= di_X and y >= di_Y)
  {
                                                              estado = 4;
                                                              estado_ant = 3;
                                                              lcd.clear();
                                                              lcd.setCursor(1,0);
                                                              lcd.print("PAUSA");
                                                              lcd.setCursor(0,1);
                                                              lcd.print("X");
                                                              lcd.setCursor(0,2);
                                                              lcd.print("Y");
                                                              lcd.setCursor(0,3);
                                                              lcd.print(" CONTINUAR");
                                                              lcd.setCursor(12,3);
                                                              lcd.print("terminar");
                                                              lcd.setCursor(2,1);
                                                              lcd.print(x);
                                                              lcd.print("/");
                                                              lcd.print(di_X);
                                                              lcd.print(" ");
                                                              lcd.write(byte(2));
                                                              lcd.print("m"); 
                                                              lcd.setCursor(2,2);
                                                              lcd.print(y);
                                                              lcd.print("/");
                                                              lcd.print(di_Y);
                                                              lcd.print(" ");
                                                              lcd.write(byte(2));
                                                              lcd.print("m"); 
                                                              posicion = 1; 
  }
  else
  {
    if (pulsador == true)
    {
      estado = 4;
      estado_ant = 2 ;
                                                              lcd.clear();
                                                              lcd.setCursor(1,0);
                                                              lcd.print("PAUSA");
                                                              lcd.setCursor(0,1);
                                                              lcd.print("X");
                                                              lcd.setCursor(0,2);
                                                              lcd.print("Y");
                                                              lcd.setCursor(0,3);
                                                              lcd.print(" CONTINUAR");
                                                              lcd.setCursor(12,3);
                                                              lcd.print("terminar");
                                                              lcd.setCursor(2,1);
                                                              lcd.print(x);
                                                              lcd.print("/");
                                                              lcd.print(di_X);
                                                              lcd.print(" ");
                                                              lcd.write(byte(2));
                                                              lcd.print("m"); 
                                                              lcd.setCursor(2,2);
                                                              lcd.print(y);
                                                              lcd.print("/");
                                                              lcd.print(di_Y);
                                                              lcd.print(" ");
                                                              lcd.write(byte(2));
                                                              lcd.print("m"); 
                                                              posicion = 1; 
      
    }
    else
    {
    
                          if (x < di_X)
                          {
                                                            if( j > 63)
                                                            {
                                                             
                                                             lcd.setCursor(2,2);
                                                                    x = x+15L  ;
                                                             lcd.print(x);
                                                             lcd.print("/");
                                                             lcd.print(di_X);
                                                             lcd.print(" ");
                                                             lcd.setCursor(17,2);
                                                             lcd.write(byte(2));
                                                             lcd.print("m"); 
                                                            }


                          }
                          if (y < di_Y)
                          {
                                                            if( j > 63)
                                                            {
                                                             lcd.setCursor(2,3);
                                                                    y = y+15L ;
                                                             lcd.print(y);
                                                             lcd.print("/");
                                                             lcd.print(di_Y);
                                                             lcd.print(" ");
                                                             lcd.setCursor(17,3);
                                                             lcd.write(byte(2));
                                                             lcd.print("m"); 
                                                            }


                        }
  }
  }
  if( j > 63)
  {
    j=0;
  }

}
void accion()/////////////////////ACCION estado (3)///////////////////////////////////////
{
  
 
  dirreccion=true;
  
 
   
                                                       

  if ( x >= df_X and y >= df_Y)
  {
                                                              estado = 4;
                                                              estado_ant = 5;
                                                              lcd.clear();
                                                              lcd.setCursor(1,0);
                                                              lcd.print("PAUSA");
                                                              lcd.setCursor(0,1);
                                                              lcd.print("X");
                                                              lcd.setCursor(0,2);
                                                              lcd.print("Y");
                                                              
                                                              lcd.setCursor(12,3);
                                                              lcd.print("TERMINAR");
                                                              lcd.setCursor(2,1);
                                                              lcd.print(x);
                                                              lcd.print("/");
                                                              lcd.print(df_X);
                                                              lcd.print(" ");
                                                              lcd.write(byte(2));
                                                              lcd.print("m"); 
                                                              lcd.setCursor(2,2);
                                                              lcd.print(y);
                                                              lcd.print("/");
                                                              lcd.print(df_Y);
                                                              lcd.print(" ");
                                                              lcd.write(byte(2));
                                                              lcd.print("m"); 
                                                              posicion = 1; 
  }
  else
  {
    if (pulsador == true)
    {
      estado = 4;
      estado_ant = 3 ;
                                                              lcd.clear();
                                                              lcd.setCursor(1,0);
                                                              lcd.print("PAUSA");
                                                              lcd.setCursor(0,1);
                                                              lcd.print("X");
                                                              lcd.setCursor(0,2);
                                                              lcd.print("Y");
                                                              lcd.setCursor(0,3);
                                                              lcd.print(" CONTINUAR");
                                                              lcd.setCursor(12,3);
                                                              lcd.print("terminar");
                                                              lcd.setCursor(2,1);
                                                              lcd.print(x);
                                                              lcd.print("/");
                                                              lcd.print(df_X);
                                                              lcd.print(" ");
                                                              lcd.write(byte(2));
                                                              lcd.print("m"); 
                                                              lcd.setCursor(2,2);
                                                              lcd.print(y);
                                                              lcd.print("/");
                                                              lcd.print(df_Y);
                                                              lcd.print(" ");
                                                              lcd.write(byte(2));
                                                              lcd.print("m"); 
                                                              posicion = 1; 
      
    }
    else
    {
    
                          if (x < df_X)
                          {
                                                           
                                                              
                                                            if( j > 63)
                                                            {
                                                              lcd.setCursor(2,2);
                                                                    x = x+15L ;
                                                               lcd.print(x);
                                                               lcd.print("/");
                                                               lcd.print(df_X);
                                                               lcd.print(" ");
                                                               lcd.setCursor(17,2);
                                                               lcd.write(byte(2));
                                                               lcd.print("m"); 
                                                            }


                          }
                          if (y < df_Y)
                          {
                                                           
                                                            if( j > 63)
                                                            {
                                                              lcd.setCursor(2,3);
                                                                    y = y+15L ;
                                                               lcd.print(y);
                                                               lcd.print("/");
                                                               lcd.print(df_Y);
                                                               lcd.print(" ");
                                                               lcd.setCursor(17,3);
                                                               lcd.write(byte(2));
                                                               lcd.print("m"); 
                                                            }


                        }
  }
  }
 if( j > 63)
  {
    j=0;
  }
}

void pausa()////////////////PAUSA estado(4)////////////////////////////////////////
{
  switch (estado_ant)
  {
    case 2:
      if (posicion == 1)
      {


        if (derecha == true or izquierda == true)
        {
          posicion =2;
          lcd.setCursor(0,3);
          lcd.print(" continuar");
          lcd.setCursor(12,3);
          lcd.print("TERMINAR");
        }
        if (pulsador == true)
        {
          estado = 2;
          estado_ant=1;
                                                        
                                                        lcd.clear();
                                                        lcd.setCursor(1,0);
                                                        lcd.print("PREPARANDO");
                                                        lcd.setCursor(1,1);
                                                        lcd.print("EXPERIMENTO...");
                                                        lcd.setCursor(0,2);
                                                        lcd.print("X");
                                                        lcd.setCursor(0,3);
                                                        lcd.print("Y");
                                                        j,k = 0;
                                                        tiempo = 5;
          
          
        }
      }
      else
      {
        
        if (derecha == true or izquierda == true)
        {
          posicion =1;
          
          lcd.setCursor(0,3);
          lcd.print(" CONTINUAR");
          lcd.setCursor(12,3);
          lcd.print("terminar");
        }
        if (pulsador == true)
        {
          estado = 5;
          estado_ant=1;
                                                        
                                                        lcd.clear();
                                                        lcd.setCursor(1,0);
                                                        lcd.print("TERMINANDO");
                                                        lcd.setCursor(1,1);
                                                        lcd.print("EXPERIMENTO...");
                                                        lcd.setCursor(0,2);
                                                        lcd.print("X");
                                                        lcd.setCursor(0,3);
                                                        lcd.print("Y");
                                                        j,k = 0;
                                                        tiempo =  5;
          
          
        }
      }
    break;
    case 3:
    if (posicion == 1)
      {


        if (derecha == true or izquierda == true)
        {
          posicion =2;
          lcd.setCursor(0,3);
          lcd.print(" continuar");
          lcd.setCursor(12,3);
          lcd.print("TERMINAR");
        }
        if (pulsador == true)
        {
          estado = 3;
          estado_ant=1;
                                                        
                                                        lcd.clear();
                                                        lcd.setCursor(1,0);
                                                        lcd.print("REALIZANDO");
                                                        lcd.setCursor(1,1);
                                                        lcd.print("EXPERIMENTO...");
                                                        lcd.setCursor(0,2);
                                                        lcd.print("X");
                                                        lcd.setCursor(0,3);
                                                        lcd.print("Y");
                                                        j,k = 0;
                                                        
                                                        tiempo = t;
          
          
        }
      }
      else
      {
        
        if (derecha == true or izquierda == true)
        {
          posicion =1;
          
          lcd.setCursor(0,3);
          lcd.print(" CONTINUAR");
          lcd.setCursor(12,3);
          lcd.print("terminar");
        }
        if (pulsador == true)
        {
          estado = 5;
          estado_ant=1;
                                                        
                                                        lcd.clear();
                                                        lcd.setCursor(1,0);
                                                        lcd.print("TERMINANDO");
                                                        lcd.setCursor(1,1);
                                                        lcd.print("EXPERIMENTO...");
                                                        lcd.setCursor(0,2);
                                                        lcd.print("X");
                                                        lcd.setCursor(0,3);
                                                        lcd.print("Y");
                                                        j,k = 0; 
                                                        tiempo =  5;
          
          
        }
      }
    break;
    default:
    
    


        
        if (pulsador == true)
        {
          estado = 5;
          estado_ant=1;
                                                        
                                                        lcd.clear();
                                                        lcd.setCursor(1,0);
                                                        lcd.print("TERMINANDO");
                                                        lcd.setCursor(1,1);
                                                        lcd.print("EXPERIMENTO...");
                                                        j,k = 0;
                                                        tiempo =  5;
          
          
        }
     
    break;
  }
  
}

void reinicio()//////////REINICIO estado (5)////////////////////////////////////////////////

{
  dirreccion=false;
     
  if ( x<20 and y < 20)
  { 
      estado = 0;
      lcd.clear();
      lcd.setCursor(1,0);
      lcd.print("ENSAYO DE MODELO");
      lcd.setCursor(1,1);
      lcd.print("ANALOGO doble");
      lcd.setCursor(1,3);
      lcd.print("iniciar experimento");
  }
  else
  {
    if (pulsador == true)
    {
      estado = 4;
      estado_ant = 5 ;
                                                              lcd.clear();
                                                              lcd.setCursor(1,0);
                                                              lcd.print("PAUSA");
                                                              lcd.setCursor(0,1);
                                                              lcd.print("X");
                                                              lcd.setCursor(0,2);
                                                              lcd.print("Y");
                                                              lcd.setCursor(12,3);
                                                              lcd.print("TERMINAR");
                                                              lcd.setCursor(2,1);
                                                              lcd.print(x);
                                                              lcd.write(byte(2));
                                                              lcd.print("m");
                                                              
                                                              lcd.setCursor(2,2);
                                                              lcd.print(y);
                                                              lcd.write(byte(2));
                                                              lcd.print("m"); 
                                                              posicion = 1; 
      
    }
    else
    {
    
                          if ( x>20)
                          {
                                                            
                                                              lcd.setCursor(0,2);
                                                              lcd.print("X");
                                                              
                                                            if( j > 63)
                                                            {
                                                              lcd.setCursor(2,2);
                                                              lcd.print("     ");
                                                              
                                                                    x = x-15L ;
                                                               lcd.setCursor(2,2);
                                                               lcd.print(x);
                                                               lcd.setCursor(17,2);
                                                               lcd.write(byte(2));
                                                               lcd.print("m"); 
                                                               
                                                            }


                          }
                          if (y >20)
                          {
                                                            
                                                              lcd.setCursor(0,3);
                                                              lcd.print("Y");
                                                            if( j > 63)
                                                            {
                                                              lcd.setCursor(2,3);
                                                              lcd.print("     ");
                                                              
                                                                    y = y-15L ;
                                                               lcd.setCursor(2,3);
                                                               lcd.print(y);
                                                               lcd.setCursor(17,3);
                                                               lcd.write(byte(2));
                                                               lcd.print("m"); 
                                                            }


                        }
  }
  }
if( j > 63)
  {
    j=0;
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
      fc_inic_Y = digitalRead(Y_MIN_PIN);
      fc_fin_Y  = digitalRead(Y_MAX_PIN);
      digitalWrite(X_DIR_PIN,dirreccion);
      digitalWrite(Y_DIR_PIN,dirreccion);
  
  
    if (btn_enc == false)//detector de flanco del pulsador/////////////////
      {
        i++;         
      }
      if (i >= 78)
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
      }//////////////////////////////////////////////////////////////////
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
