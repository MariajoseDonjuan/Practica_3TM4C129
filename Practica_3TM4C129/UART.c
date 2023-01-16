#include "lib/include.h"

extern void Configurar_UART2(void)
{
    SYSCTL->RCGCUART  = (1<<2);   //Paso 1 (RCGCUART) pag.344 UART/modulo0 0->Disable 1->Enable
    //                    A      B     C     D      E     F 
    SYSCTL->RCGCGPIO |= (0<<0)|(0<<1)|(0<<2)|(1<<3)|(0<<4)|(0<<5);     //Paso 2 (RCGCGPIO) pag.340 Enable clock port A
    //(GPIOAFSEL) pag.671 Enable alternate function
    GPIOD_AHB->DIR = (0<<4) | (1<<5);//PD4 PD5
    GPIOD_AHB->AFSEL = (1<<4) | (1<<5);

    //GPIO Port Control (GPIOPCTL) PA0-> U0Rx PA1-> U0Tx pag.688
    GPIOD_AHB->PCTL = (GPIOD_AHB->PCTL&0xFF00FFFF) | 0x00110000;// (1<<0) | (1<<4);//0x00000011
    // GPIO Digital Enable (GPIODEN) pag.682
    GPIOD_AHB->DEN = (1<<4) | (1<<5);//PD4 PD5
    //UART0 UART Control (UARTCTL) pag.918 DISABLE!!
    UART2->CTL = (0<<9) | (0<<8) | (0<<0)| (0<<4);

   
    /*
    BRD = 20,000,000 / (16 * 57600) = 21.70
    UARTFBRD[DIVFRAC] = integer(0.7 * 64 + 0.5) = 45
    */
    UART2->IBRD = 21;
    // UART Fractional Baud-Rate Divisor (UARTFBRD) pag.915
    UART2->FBRD = 45;
    //  UART Line Control (UARTLCRH) pag.916
    UART2->LCRH = (0x3<<5)|(1<<4);
    //  UART Clock Configuration(UARTCC) pag.939
    UART2->CC =(0<<0);
    //Disable UART0 UART Control (UARTCTL) pag.918
    UART2->CTL = (1<<0) | (1<<8) | (1<<9)| (1<<4);



}

extern char readChar(void)
{
   
    int v;
    char c;
    while((UART2->FR & (1<<4)) != 0 );
    v = UART2->DR & 0xFF;
    c = v;
    return c;
}
extern void printChar(char c)
{
    while((UART2->FR & (1<<5)) != 0 );
    UART2->DR = c;
}
extern void printString(char* string2)
{
    while(*string2)
    {
        printChar(*(string2++));
    }
}

extern int readString(char delimitadorF,char *string,char *string2)
{
    int s=0;
int j=0;
   int i=0;
   char c = readChar();
   while(c != delimitadorF)
   {
       string[i]=c;
       i++;
       c=readChar();
   }
   s=i*2;
   int h=0;
   for(j=0;j<s;j++)
   {
       
       string2[h]=string[i-1];
       h++;
       string2[h]=j+1;
       h++;
       i--;
   }

   return i;

}
extern char invertirString(char *string)
{
    int i=0;
int J=0;
    int longitud = sizeof(string);
  char temp;
  for (int i= 0, j = longitud - 1; i < (longitud / 2); i++, J--) 
  {
    temp= string[i];
    string[i] = string[J];
    string[J] = temp;
  }
  return string;
}


