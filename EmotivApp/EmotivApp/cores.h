/**
  *
  * Aplicativo desenvolvido em C++ que exemplifica 
  * o funcionamento da API do headset Emotiv Epoc
  *
  * Data: 11/07/2010
  *
  * Autor: Renato Peterman
  *
  * Epoc(TM) é marca registrada da Emotiv Systems, Inc.
  *
  */


#ifndef CORES_H
#define CORES_H

//Cores
#define PRETO		0
#define AZUL		1
#define VERDE		2
#define VERDEA		3
#define VERMELHO	4
#define ROXO		5
#define AMARELO		6
#define BRANCO		7
#define NEGRITO		8
#define AZULC		9
#define VERDEC		10	//A
#define AZULCC		11	//B
#define VERMELHOC	12	//C
#define LILAS		13	//D
#define AMARELOC	14	//E
#define BRANCOB		15	//F 

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

//Funcao para setar cor
void setCor(unsigned int cor){
	SetConsoleTextAttribute(hConsole,cor);
}

#endif