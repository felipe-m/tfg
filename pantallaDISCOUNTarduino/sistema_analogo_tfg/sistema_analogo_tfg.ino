#include <LiquidCrystal.h>
LiquidCrystal lcd(16,17,23,25,27,29);
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
      
      int i,j = 0;
      bool btn_en1;
      bool btn_en2;
      bool btn_enc=true;
      bool btn_en1_prev ;
      bool btn_en2_prev ;
      bool btn_enc_prev = false;
      bool derecha,izquierda = false;
      bool eleccion, pulso = false; 
      int volatile estado,posicion = 0; 
      int volatile distancia, Xo , Xf , cuentapulso= 0;
      int volatile v= 5;
      int volatile tiempo, to, tr, tinic ,tpausa= 0;
      bool paso,dir,fc_inic,fc_fin = false;
      bool positivo = true; //lectura que hay que enviar para que la maquina avance
               
      byte empty[8] = {          
          B00000,
          B00100,
          B01010,
          B10001,
          B10001,
          B01010,
          B00100,          
      };    
      byte full[8] = {
          B00000,
          B00100,
          B01110,
          B11111,
          B11111,
          B01110,
          B00100,      
      };

       byte micro[8] = {
          B00000,
          B10001,
          B10001,
          B11011,
          B10101,
          B10000,
          B10000,
       };
void setup() {


      pinMode(BTN_EN1,INPUT_PULLUP);    // Encoder 1
      pinMode(BTN_EN2,INPUT_PULLUP);    // Encoder 2
      pinMode(BTN_ENC,INPUT_PULLUP);    // Encoder Swith
      pinMode(BEEPER,OUTPUT);           // BEEPER
      pinMode(X_STEP_PIN,OUTPUT);       // Pasos del motor
      pinMode(X_DIR_PIN,OUTPUT);        // Dirreccion del motor
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

void inicio()
{
      i = 0;
      eleccion = false; 
      distancia = 0;
      v = 5;
      Xo=0;
      Xf=1;
      tiempo= 0;
      tpausa= 0;
      
      lcd.setCursor(1,1);
      lcd.print("iniciar");
      lcd.setCursor(1,2);
      lcd.print("experimento");
      
      if (btn_enc == btn_enc_prev)
      {
        digitalWrite(BEEPER,HIGH);
        delay(200);
        digitalWrite(BEEPER,LOW);
        estado = 1;
        posicion = 0;
        lcd.clear();
        lcd.setCursor(1,0);
        lcd.print("dist. inicial");
        lcd.setCursor(1,1);
        lcd.print("dist.final");
        lcd.setCursor(1,2);      
        lcd.print("velocidad");
        lcd.setCursor(1,3);     
        lcd.print("iniciar experimento");
        tinic=0;
        dir = positivo;
      }
}
void ajustes()
{
      
      lcd.setCursor(15,0);        
      if (Xo<10)
      {
        lcd.print("0"); 
      }
      lcd.print(Xo);
      
      lcd.setCursor(15,1);
      if (Xf<10)
      {
        lcd.print("0"); 
      }
      lcd.print(Xf);
      
      lcd.setCursor(11,2);
      if (v<10)
      {
        lcd.print("00");
      }
      else if (v <100)
      {
        lcd.print("0");
      }
      lcd.print(v);
      lcd.setCursor(18,0);
      lcd.print("cm");
      lcd.setCursor(18,1);
      lcd.print("cm");
      lcd.setCursor(16,2);
      lcd.write(byte(2));
      lcd.print("m/s");     
      lcd.write(byte(1));
      
      if (btn_enc == btn_enc_prev)
      {
        digitalWrite(BEEPER,HIGH);
        delay(200);
        digitalWrite(BEEPER,LOW);
        eleccion = !eleccion;
        
      }
      for (i=0;i<4;i++)
      {
        
      lcd.setCursor(0,i);
      if (i  == posicion)
        {
          lcd.write(byte(1));
         
        }
      else
        {
          lcd.write(byte(0));
          
          
        }

      }
      switch (posicion)
      {
                case 0:
                  if (eleccion == true)
                  {
                        if (derecha == true & Xo < 50)
                        {
                          Xo = Xo+1;
                          if (Xo+1 > Xf)
                          {
                            Xf = Xo+1;
                          }
                        }
                        else if (izquierda == true & Xo > 0)
                        {
                          Xo = Xo-1;
                          if (Xo+1 > Xf)
                          {
                            Xf = Xo+1;
                          }
                        }
                  }
                  else
                  {
                        if (derecha == true)
                        {
                          posicion = 1;
                        }
                        else if(izquierda == true)
                        {
                          posicion = 3;
                        }
                  }
                  break;
                  case 1:
                    if (eleccion == true)
                      {
                            if (derecha == true & Xf < 50)
                            {
                              Xf = Xf+1;
                            }
                            else if (izquierda == true & Xf > Xo)
                            {
                              Xf = Xf-1;
                            }
                      }
                      else
                      {
                            if (derecha == true)
                            {
                              posicion = 2;
                            }
                            else if(izquierda == true)
                            {
                              posicion = 0;
                            }
                      }
                    break;
                    case 2:
                      if (eleccion == true)
                      {
                            if (derecha == true & v < 200)
                            {
                              v = v + 5;
                            }
                            else if (izquierda == true & v > 5)
                            {
                              v = v - 5;
                            }
                      }
                      else
                      {
                            if (derecha == true)
                            {
                              posicion = 3;
                            }
                            else if(izquierda == true)
                            {
                              posicion = 1;
                            }
                      }
                     break;
                     default:
                       if (eleccion == true)
                        {
                              estado = 2;
                              lcd.clear();
                              distancia = 0;
                              tiempo = 0;
                                    lcd.setCursor(0,3);
                                    lcd.print(" ");      
                                    lcd.setCursor(1,0);
                                    lcd.print("posicion");
                                    lcd.setCursor(16,0);
                                    lcd.write(byte(2));
                                    lcd.print("m");
                                    lcd.setCursor(1,1);
                                    lcd.print("tiempo");
                                    lcd.setCursor(17,1);
                                    lcd.print("s");
                                    lcd.setCursor(0,2);
                                    lcd.write(byte(1));
                                    lcd.print("detener");
                                    lcd.setCursor(0,3);
                                    lcd.print(" ");
                              tinic = millis();

                        }
                        else
                        {
                              if (derecha == true)
                              {
                                posicion = 0;
                              }
                              else if(izquierda == true)
                              {
                                posicion = 2;
                              }
                        }
                     break;         
              }
    
}
void accion()
{
        
        tiempo= (millis()-tinic-tpausa)/1000 ;
        
        lcd.setCursor(10,0);
              
        lcd.print(distancia);
 
        lcd.setCursor(11,1);
              
        lcd.print(tiempo);
        

        if (distancia <= Xo*1000 )
        {

          pulso = !pulso;
          
        }
        else if (distancia < Xf*1000)
        {
          pulso = !pulso;
          delay(7500/(32*v));
        }
        else
        {
          pulso = false;
        }
        if (pulso == true)
        {
          cuentapulso = cuentapulso +1;
        }
        if (cuentapulso > 31)
        {
          distancia = distancia + 7,5;
          paso= true;
          cuentapulso = 0;
          paso= false;
        }
        if (btn_enc == false || distancia >= Xf*1000)
        {
          estado = 3;
          to = millis();
          digitalWrite(BEEPER,HIGH);
          delay(200);
          digitalWrite(BEEPER,LOW);
          posicion = 2;
          lcd.clear();
                              lcd.setCursor(0,3);
                              lcd.print(" ");      
                              lcd.setCursor(1,0);
                              lcd.print("posicion");
                              lcd.setCursor(16,0);
                              lcd.write(byte(2));
                              lcd.print("m");
                              lcd.setCursor(1,1);
                              lcd.print("tiempo");
                              lcd.setCursor(17,1);
                              lcd.print("s");
                              lcd.setCursor(1,2);
                              lcd.print("continuar");
                              lcd.setCursor(1,3);
                              lcd.print("terminar");                            
          
        }
 
}

void pausa()
{    
        lcd.setCursor(10,0);
              if (distancia < 10)
              {
                lcd.print("0000");
              }
              else if(distancia <100)
              {
                lcd.print("000");
              }
              else if (distancia < 1000)
              {
                lcd.print("00");
              }
              else if ( distancia < 10000)
              {
               lcd.print("0");
              }
        lcd.print(distancia);
        lcd.setCursor(9,1);
              if (tiempo < 10)
              {
                lcd.print("00000");
              }
              else if (tiempo < 100)
              {
                lcd.print("0000");
              }
              else if (tiempo <1000)
              {
                lcd.print("000");
              }
              else if ( tiempo < 1000)
              {
                lcd.print("00");
              }
              else if (tiempo < 10000)
              {
                lcd.print("0");
              }
        lcd.print(tiempo);
        
      for (i=2;i<4;i++)
      {
        
      lcd.setCursor(0,i);
      if (i  == posicion)
        {
          lcd.write(byte(1));
        }
      else
        {
          lcd.write(byte(0));
        }
      }
     switch (posicion)
      {
       case 2:
           if (btn_enc == false)
           {
            if ( distancia < 1000* Xf)
            {
                tpausa = tpausa + millis()- to;
                digitalWrite(BEEPER,HIGH);
                delay(200);
                digitalWrite(BEEPER,LOW);
                posicion =2 ;
                lcd.clear();
                estado=2;

                                    lcd.setCursor(0,3);
                                    lcd.print(" ");      
                                    lcd.setCursor(1,0);
                                    lcd.print("posicion");
                                    lcd.setCursor(16,0);
                                    lcd.write(byte(2));
                                    lcd.print("m");
                                    lcd.setCursor(1,1);
                                    lcd.print("tiempo");
                                    lcd.setCursor(17,1);
                                    lcd.print("s");
                                    lcd.setCursor(0,2);
                                    lcd.write(byte(1));
                                    lcd.print("detener");
                                    lcd.setCursor(0,3);
                                    lcd.print(" ");
            }
           else
            {
              digitalWrite(BEEPER,HIGH);
              delay(500);
              digitalWrite(BEEPER,LOW);
              delay(200);
              digitalWrite(BEEPER,HIGH);
              delay(500);
              digitalWrite(BEEPER,LOW);
              delay(200);
            }
            
           }
           else if( derecha == true || izquierda == true)
           {
            posicion = 3;
           }
           break;
       default:
           if (btn_enc == false)
           {
            tpausa = tpausa + millis()- to;
            digitalWrite(BEEPER,HIGH);
            delay(200);
            digitalWrite(BEEPER,LOW);
            posicion = 3;
            lcd.clear();
            estado = 4;
            lcd.clear();
            
            lcd.setCursor(1,0);
            lcd.print("posicion");
            lcd.setCursor(16,0);
            lcd.write(byte(2));
            lcd.print("m");
            lcd.setCursor(1,2);
            lcd.print("reiniciando");
            dir = !positivo;
            
           }
           else if( derecha == true || izquierda == true)
           {
            posicion = 2;
           }
           break;
      }
         
}
void reinicio()
{
      
      lcd.setCursor(10,0);
              if (distancia < 10)
              {
                lcd.print("0000");
              }
              else if(distancia <100)
              {
                lcd.print("000");
              }
              else if (distancia < 1000)
              {
                lcd.print("00");
              }
              else if ( distancia < 10000)
              {
               lcd.print("0");
              }
        lcd.print(distancia);
        pulso = !pulso;
        if (pulso == true)
        {
          cuentapulso = cuentapulso +1;
        }
        if (cuentapulso > 31)
        {
          distancia = distancia + 7,5;
          paso= true;
          cuentapulso = 0;
          paso= false;
              if (j > 14)
              {
                j = 0;
              }
              else 
              {
                j = j+1;
              }
        }

        
        for (i=0;i<3;i++)
        {
          lcd.setCursor (15+i,2);
          if (i==j/5)
          {
            lcd.write(byte(1));
          }
          else
          {
            lcd.write(byte(0));
          }
        }
        
        

        if(distancia < 1 || btn_enc == false)
        {
          estado=0;
          pulso = false;
          lcd.clear();
        }
        
}
void loop() {
  
      btn_en1 = digitalRead(BTN_EN1);
      btn_en2 = digitalRead(BTN_EN2);
      btn_enc = digitalRead(BTN_ENC); 
      fc_inic = digitalRead(X_MIN_PIN);
      fc_fin  = digitalRead(X_MAX_PIN);
      digitalWrite(X_STEP_PIN,paso);
      digitalWrite(X_DIR_PIN,dir);
                
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
      switch (estado)
      {
          case 1: 
            ajustes ();
            break;
          case 2:
            accion  ();
            break;
          case 3:
            pausa   ();
            break;
          case 4:
            reinicio();
            break;
          default:
            inicio();
            break;
      }
      
}
