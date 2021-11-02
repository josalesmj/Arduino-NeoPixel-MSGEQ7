/*
ARDUINO   PORT 
  PIN     NAME
-------   ----  
   0      PD0
   1      PD1
   2      PD2
   3      PD3
   4      PD4
   5      PD5
   6      PD6
   7      PD7
   8      PB0
   9      PB1
  10      PB2
  11      PB3
  12      PB4
  13      PB5
  A0      PC0
  A1      PC1
  A2      PC2
  A3      PC3
  A4      PC4
  A5      PC5 
*/

//PORTD &= ~bit(PORTD2);                    // set digital pin 2 as LOW
//PORTD |= bit(PORTD2);                     // set digital pin 2 as HIGH

/*
 * Histórico de otimização do código
 * Versão 1.0
 *    5500 bytes (17%) de espaço de armazenamento para programas
 *    258 bytes (12%) de memória dinâmica
 *    
 * Versão 1.1
 *    4860 bytes (15%) de espaço de armazenamento para programas
 *    249 bytes (12%) de memória dinâmica
 *    Mudanças:
 *        Remoção de variáveis globais desnecessárias.
 *        
 * Versão 1.2
 *    4844 bytes (15%) de espaço de armazenamento para programas
 *    249 bytes (12%) de memória dinâmica
 *    Mudanças:
  *      Alteração das funções pinMode e digitalWrite para manipulação das portas nativamente.
 *    
 * Versão 1.3
 *    3864 bytes (12%) de espaço de armazenamento para programas
 *    74 bytes (3%) de memória dinâmica
 *    Mudanças:
 *        Remoção da linha inicialização da comunicação serial.
 *        
 *        
 * Versão 1.4
 *    3858 bytes (12%) de espaço de armazenamento para programas
 *    74 bytes (3%) de memória dinâmica
 *    Mudanças:
 *        Variável FreqVal movida de global para local.
 *        
 * Versão 1.5 (Removida: A inicialização inline causou um travamento do código)
 *    3912 bytes (12%) de espaço de armazenamento para programas
 *    60 bytes (2%) de memória dinâmica
 *    Mudanças:
 *        Remoção da variável FreqVal para uma inicialização inline.
 *        
 * Versão 1.6 (Removida: A rotina contida no "while (true)" não era executada)
 *    3708 bytes (12%) de espaço de armazenamento para programas
 *    74 bytes (3%) de memória dinâmica
 *    Mudanças:
 *        Remoção das funções setup() e loop().
 *        
 *        
 *        
 *        
 * Versão atual:
 *    1.5
 *       
 */

#include <Adafruit_NeoPixel.h>
#include <AudioAnalyzer.h>

#define PIN 11 //Registrador B
//#define msgRST 3 //Registrador D
//#define msgStrobe 2 //Registrador D
#define NUMLED 256
#define msgAnalog 0

Adafruit_NeoPixel pixels(NUMLED, PIN, NEO_GRB + NEO_KHZ800);

Analyzer Audio = Analyzer(2, 3, 0); //Strobe pin ->10  RST pin ->11 Analog Pin ->0

void setup() {
  //Serial.begin(115200);
  pixels.begin();
  pixels.setBrightness(15);
  Audio.Init();         //Init module
  //pinMode(msgRST, OUTPUT);
  //pinMode(msgStrobe, OUTPUT);
  DDRD = DDRD | B00001100; //Coloca pin 2 e 3 como OUTPUT
}

void loop() {
  //Serial.println("Alive...");
  pixels.clear();
  uint32_t myColor = pixels.Color(random(1, 256), random(1, 256), random(1, 256));
  boolean flag = false;
  int FreqVal[7] = {63, 160, 400, 1000, 2500, 6250, 16000};
  Audio.ReadFreq(FreqVal);      //return 7 value of 7 bands pass filter
  //Audio.ReadFreq(new int[7] {63, 160, 400, 1000, 2500, 6250, 16000});

  for (int i = 0; i < 7; i++) {
    //digitalWrite(msgStrobe, LOW);
    PORTD &= ~bit(PORTD2);  //Coloca pino 2 como LOW
    delayMicroseconds(20);
    int spectrumRead = analogRead(0);
    int PWMvalue = map(spectrumRead, 0, 1024, 0, 16);
    //digitalWrite(msgStrobe, HIGH);
    PORTD |= bit(PORTD2); //Coloca pino 2 como HIGH
    if(PWMvalue > 0) {
      if(flag == false) {
        pixels.fill(myColor, ((i+1)*16) - PWMvalue, PWMvalue);
      }
      else {
        pixels.fill(myColor, 16*i, PWMvalue);
      }
    }
    flag = !flag;
    pixels.show();
  }
}

/*int main () {
  Serial.begin(115200);
  pixels.begin();
  pixels.setBrightness(15);
  Audio.Init();         //Init module
  //pinMode(msgRST, OUTPUT);
  //pinMode(msgStrobe, OUTPUT);
  DDRD = DDRD | B00001100; //Coloca pin 2 e 3 como OUTPUT

  while (true) {
    Serial.println("1");
    pixels.clear();
    uint32_t myColor = pixels.Color(random(1, 256), random(1, 256), random(1, 256));
    boolean flag = false;
    int FreqVal[7] = {63, 160, 400, 1000, 2500, 6250, 16000};
    Audio.ReadFreq(FreqVal);      //return 7 value of 7 bands pass filter
    //Audio.ReadFreq(new int[7] {63, 160, 400, 1000, 2500, 6250, 16000});
    Serial.println("2");
    for (int i = 0; i < 7; i++) {
      Serial.println("3");
      //digitalWrite(msgStrobe, LOW);
      PORTD &= ~bit(PORTD2);  //Coloca pino 2 como LOW
      //delayMicroseconds(20);
      int spectrumRead = analogRead(0);
      int PWMvalue = map(spectrumRead, 0, 1024, 0, 16);
      //digitalWrite(msgStrobe, HIGH);
      PORTD |= bit(PORTD2); //Coloca pino 2 como HIGH
      if(PWMvalue > 0) {
        if(flag == false) {
          pixels.fill(myColor, ((i+1)*16) - PWMvalue, PWMvalue);
        }
        else {
          pixels.fill(myColor, 16*i, PWMvalue);
        }
        Serial.println("4");
      }
      Serial.println("5");
      flag = !flag;
      pixels.show();
    }
    Serial.println("6");
  }
  Serial.println("7");
}*/
