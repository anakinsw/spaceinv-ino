#include <SPI.h>
#include <Adafruit_GFX.h>
#include <TFT_ILI9163C.h>

#define __CS 10
#define __DC 9
#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0  
#define WHITE   0xFFFF
TFT_ILI9163C display = TFT_ILI9163C(__CS, __DC);
float p = 3.1415926;
int x_nave=40;
bool direccion=true;

int desp=0;

bool disparo=false;
int disparo_x=0;
int disparo_y=0;


bool filas[4][9]={
                    {true,true,true,true,true,true,true,true},
                    {true,true,true,true,true,true,false,true},
                    {true,false,true,true,false,true,true,true},
                    {true,true,true,false,true,true,true,true},
                  };
                  

PROGMEM const unsigned char invader1[]={
B00010000,B01000000,
B00001000,B10000000,
B00011111,B11000000,
B00110111,B01100000,
B01111111,B11110000,
B01011111,B11010000,
B01010000,B01010000,
B00001101,B10000000,
};

PROGMEM const unsigned char invader2[]={
B00001110,B00000000,
B00011111,B00000000,
B00110101,B10000000,
B01111111,B11000000,
B01111111,B11000000,
B00010101,B00000000,
B00101010,B10000000,
B01010101,B01000000,
};

PROGMEM const unsigned char nave[]={
B00011000,
B01111110,
B11000011,
B11000011,
B11111111,
B11111111,
};

PROGMEM const unsigned char bala[]={
B00100000,
B00010000,
B00100000,
B00010000,
};


void setup() {
  // put your setup code here, to run once:
  display.begin(); //iniciamos display
  display.clearScreen();
  display.setCursor(0,0);
  display.setTextColor(RED);  
  display.setTextSize(1);
  display.println("Invaders:32 Lives:3");

  randomSeed(analogRead(5));  //semilla aleatoria
  
  
}

void loop() {
  // put your main code here, to run repeatedly:
  pintar_linea_invaders(0,invader1);
  pintar_linea_invaders(1,invader2);
  pintar_linea_invaders(2,invader1);
  pintar_linea_invaders(3,invader2);

  //pintar_nave();
  piloto_automatico();
  actualizar_estado_disparo();
  disparar();
  detectar_colisiones_invaders();
}


void disparar(){
  /*
    bool disparo=false;
    int disparo_x=0;
    int disparo_y=0;
  */
  if (!disparo){
    disparo_x=x_nave;
    disparo_y=120;
    disparo=true;
 }

  
}

void actualizar_estado_disparo(){
  if (disparo){
    //Esto indica que se ha disparado
    borrar_disparo();
    disparo_y--;
    pintar_disparo();
  }  


  
}

void borrar_disparo(){
  display.drawBitmap(disparo_x, disparo_y, bala, 8,4, BLACK, BLACK);
}

void pintar_disparo(){
  if(disparo_y<5){
    borrar_disparo();
    disparo=false;
  }
  display.drawBitmap(disparo_x, disparo_y, bala, 8,4, WHITE, BLACK);

}


void piloto_automatico(){
  int v=1;
    if (direccion){
      v=abs(v);
    }else{
      v=-v;
    }
    
    if(x_nave==120){
      direccion=false;

    }

    if(x_nave==6){
      direccion=true;
    }


     int r;
     r = random(1, 3);

   
        disparar();
    
    
    mover_nave(v*3);
}

void detectar_colisiones_invaders(){
  int fila;


  for (int b=0;b<5;b++){
    for (int a=0;a<12;a++){
      fila=b+1;
      if(filas[b][a]){ 
        //24
        //(11*a)
        int yfila=20+(11*b);
        if( (disparo_x>(13*a) && disparo_x<((13*a)+13)) && (disparo_y>yfila && disparo_y<(yfila+10))  ){

          filas[b][a]=false;

       //   borrar_disparo();
       //   disparo=false;
         
        }
      }
     
    }
  }
  
  
}


void pintar_linea_invaders(int linea,const uint8_t *malo){
   for (int a=0;a<9;a++){
    if(filas[linea][a]){ 
     pintar_invader(a*13,20+(11*linea),color_aleatorio(),malo);
    }else{
      pintar_invader(a*13,20+(11*linea),BLACK,malo);
    }
   }
}

int color_aleatorio(){
  int r;
  r = random(1, 7);
  switch (r) {
    case 1:
      return BLUE;
      break;
    case 2:
      return RED;
      break;
    case 3:
      return GREEN;
      break;
    case 4:
      return CYAN;
      break;
    case 5:
      return MAGENTA;
      break;
    case 6:
      return YELLOW;
      break;
    case 7:
      return WHITE;
      break;
  }
}


void pintar_invader(int x, int y,int color,const uint8_t *malo){
  display.drawBitmap(x, y, malo, 16,8, color, BLACK);
}

void mover_nave(int vel){
    borrar_nave();
    x_nave=x_nave+vel;
    pintar_nave();
}


void borrar_nave(){
  display.drawBitmap(x_nave, 120, nave, 8,6, BLACK, BLACK);
}

void pintar_nave(){
  if(x_nave<4){
    x_nave=4;
  }
  
   if(x_nave>120){
    x_nave=120;
  }
  
  display.drawBitmap(x_nave, 120, nave, 8,6, WHITE, BLACK);
}

