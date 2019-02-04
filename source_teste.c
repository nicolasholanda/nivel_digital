/*
	Autor: Nicolas Holanda, Raphael Otaviano e Luiz Henrique
	Data: 29/06/2018
	
					========Projeto N�vel Digital========
	Este projeto serve para medir quantidades de l�quidos em recipientes, atrav�s
	de pulsos ultrass�nicos e utilizando a capacidade de processamento de um PIC
	18F4520.
*/


#include <p18f4520.h>
#include <delays.h>
#include <timers.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "lcd.h"

#define echo PORTBbits.RB3
#define trigger PORTBbits.RB1

/* Definindo constantes */
#define raio_maior 6.55
#define raio_menor 5.45
#define altura_const 771.0

#pragma config MCLRE = OFF
#pragma config WDT = OFF

/*
* C�LCULO DO DELAY:
* 
* Ciclos = (Tempo_Delay * Fosc) / 4
* 
* Nesse caso, temos um PIC com 8MHz e queremos delay
* de 10us. Logo:
*
* Ciclos = (10u * 8M) / 4
* Ciclos = 80 / 4 = 20
*
* Logo a fun��o Delay10TCYx(2) satisfar�.
* ==========================================================
* C�LCULO DO PRESCALE DO TIMER:
*
* Se a frequ�ncia � de 8MHz, o tempo de cada ciclo �:
* 1/(8/4) = 0,5uS.
*
* Se quero contar a quantidade de uS no pino de ccp, ent�o o prescale
* ser� de 1:2. Mas como o timer possui mem�ria suficiente para at�
* 2^16 = 65536. S� contar�amos at� 65536us e queremos medir at� 4m, que equivale
* a 58*4000 = 232000. Portanto 232000/65536 = 3.5, logo o prescale 1:4 fica melhor.
* Vai contar at� 58000
*/


/* Fun��o que configura o PIC */
void set_pin(){

	/* Clock interno, de 8MHz */
	OSCCON = 0b01110010;

	/* RB3 configurado como entrada e
	 * o resto como sa�da*/
	TRISB = 0b00001000;
	TRISD=0;
	
	/* Todas os pinos digitais */
	ADCON1 = 0x7F;
	
	/* Inicializa vari�veis e o LCD */
	Inicializa_LCD();
	trigger = 0;
	echo = 0;
}

/* Inverte uma string */
void reverse(char *str, int len)
{
    int i=0, j=len-1, temp;
    while (i<j)
    {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++; j--;
    }
}

/* Fun��o que converte de inteiro para string */
int intToStr(int x, char str[], int d)
{
    int i = 0;
    while (x)
    {
        str[i++] = (x%10) + '0';
        x = x/10;
    }

    while (i < d)
        str[i++] = '0';
 
    reverse(str, i);
    str[i] = '\0';
    return i;
}
 
/* Fun��o que converte de float para char */
void ftoa(float n, char *res, int afterpoint)
{
    int ipart = (int)n;
    float fpart = n - (float)ipart;
    int i = intToStr(ipart, res, 0);

    if (afterpoint != 0)
    {
        res[i] = '.';
        fpart = fpart * pow(10, afterpoint);
        intToStr((int)fpart, res + i + 1, afterpoint);
    }
}
 

void main(){
	float result_float=0.0, cap=0.0, dist=0.0;
	int result=0, result_resto=0;
	char str[8], str_resto[], str_cap[8];
	set_pin();
	while(1){

		/* CALCULANDO A DIST�NCIA */
		trigger = 1;
		Delay10TCYx(2);
		trigger = 0;
		while(echo==0);
		OpenTimer0( TIMER_INT_OFF & T0_16BIT & T0_SOURCE_INT & T0_PS_1_4 );
		while(echo==1);
		dist = (float) (ReadTimer0()*2)/58;
		dist -=1.16;
		if(dist<=0.5){dist=0.0;}
		WriteTimer0(0);

		/* CALCULANDO O VOLUME */
		result_float = (float)altura_const -( (3.14*dist)/3.0 )*(pow(raio_maior,2.0)+raio_maior*raio_menor + pow(raio_menor,2.0));

		/* CALCULANDO A CAPACIDADE */
		cap = (result_float/altura_const)*100.0;
		if(cap<=0.0){
			cap=0.0;
		}

		
		/* MOSTRANDO NO LCD */

		ftoa(result_float, str, 2);
		/* Transformando os n�meros em string */

		ultoa(cap, str_cap);
		
		/* Verificando se o ressultado d� entre 0cm e a altura com uma margem de erro de 9cm*/
		if(result_float <= altura_const + 9 && result_float>=0.0){
			Posiciona_LCD(1,1);
			Escreve_LCD("Vol: ");
			Escreve_str_LCD(str);
			Escreve_LCD("mL");
			Posiciona_LCD(2,1);
			Escreve_LCD("Cap(%): ");
			Escreve_str_LCD(str_cap);
			Escreve_LCD("%");
		}
		
		/* Caso d� um valor negativo ou maior que a altura da caixa d'�gua, mostra 'inv�lido' */
		else{
			Posiciona_LCD(1,4);
			Escreve_LCD("Invalido!");
		}
		
		/* O resultado fica dispon�vel durante um tempo e logo depois � recalculado */
		Delay10KTCYx(200);
		Limpa_LCD();
		
	}
}