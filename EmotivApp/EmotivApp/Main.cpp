/**
  *
  * Aplicativo desenvolvido em C/C++ utilizando o headset
  * Emotiv Epoc em conjunto com Arduino para manipular
  * dois servo motores (mini braço articulado).
  *
  * Data: 19/10/2010
  *
  * 22/10/2010 - Corrigido bug de comunicação serial com os dois motores
  *              e implementada movimentação vertical.
  *
  * Autor: Renato Peterman
  *
  * Epoc(TM) é marca registrada da Emotiv(R) Systems, Inc.
  *
  */

//Bibliotecas C e Windows
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <Commctrl.h> // Necessária para manipular o controle IPADDRESS

//Bibliotecas STL (C++)
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

//Headers Emotiv
#include "EmoStateDLL.h"
#include "edk.h"
#include "edkErrorCode.h"

//Recursos
#include "resource.h"

//Classe para comunicação serial.
#include "SerialClass.h"

//Header com a função para definir as cores
#include "cores.h"

//Constantes (Portas das aplicações)
const unsigned int CONTROL_PANEL_PORT	= 3008;
const unsigned int EMO_COMPOSER_PORT	= 1726;

//Biblioteca Emotiv
#pragma comment(lib, "../lib/edk.lib")

//Namespace STL (C++)
using namespace std;

//Declaracao das funcoes criadas
void ConectaEmoEngine(HWND hwnd);
void ConectaEmoComposer(HWND hwnd);
void ConectaControlPanel(HWND hwnd);
void DesconectaEmotiv(HWND hwnd);

Serial serial ("COM3");
EmoEngineEventHandle eEvent		= EE_EmoEngineEventCreate();
EmoStateHandle eState			= EE_EmoStateCreate();
int state						= 0;
unsigned int userID				= 0;
int pos1;
int pos2;

BOOL CALLBACK DlgProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case WM_INITDIALOG:

			//Timer: necessário para criar um loop constante de atualização.
			UINT ret;
			ret = SetTimer(hwnd, 1, 50, NULL);
			if(ret == 0)
				MessageBox(hwnd, "Erro ao criar o timer!", "Erro", MB_OK | MB_ICONEXCLAMATION);

			/*
			if(serial.IsConnected()){
				serial.WriteData("090a",5);
				serial.WriteData("090b",5);
			}
			*/
			

			SetDlgItemText(hwnd, IDC_EDIT1, "127.0.0.1");
			//SendDlgItemMessage(hwnd, IDC_IPADDRESS1, IPM_SETADDRESS, 0, MAKEIPADDRESS(127,0,0,1));

			//MessageBox(hwnd, "Este aplicativo utiliza as portas padrão para se conectar aos aplicativos Emotiv:\n\nEmoComposer: 1726\n\nControl Panel: 3008", "Aviso", MB_ICONINFORMATION | MB_OK);

		break;
		case WM_COMMAND:

			switch(LOWORD(wParam))
			{
				case IDC_BT1:
					ConectaEmoComposer(hwnd);
				break;

				case IDC_BT2:
					ConectaControlPanel(hwnd);
				break;

				case IDC_BT3:
					ConectaEmoEngine(hwnd);
				break;

				case IDC_BTDESCONECTA:
					DesconectaEmotiv(hwnd);
				break;
			}
		break;

		case WM_CLOSE:
			EE_EngineDisconnect();
			EndDialog(hwnd,0);
		break;

		case WM_DESTROY:
			KillTimer(hwnd, 1);
			EE_EmoStateFree(eState);
			EE_EmoEngineEventFree(eEvent);
			PostQuitMessage(0);
		break;

		default:

			state = EE_EngineGetNextEvent(eEvent);

			if(state == EDK_OK)
			{
				EE_Event_t eventType = EE_EmoEngineEventGetType(eEvent);
				EE_EmoEngineEventGetUserId(eEvent,&userID);

				if(eventType == EE_EmoStateUpdated)
				{
					EE_EmoEngineEventGetEmoState(eEvent, eState);
					const float timestamp = ES_GetTimeFromStart(eState);

					//Convertendo de const float para char
					ostringstream os;
					os << timestamp;
					string str = os.str();
					
					//Bateria
					int bateria;
					int max;
					ES_GetBatteryChargeLevel(eState,&bateria,&max);

					//Informações
					SetDlgItemInt(hwnd, IDC_STATICSINAL, static_cast<int>(ES_GetWirelessSignalStatus(eState)),FALSE);
					SetDlgItemText(hwnd, IDC_STATICTEMPO2, str.c_str());
					SetDlgItemInt(hwnd, IDC_STATICUSER2, userID, FALSE);
					SetDlgItemInt(hwnd, IDC_STATICBATERIA, bateria, FALSE);

					//Expressiv
					map<EE_ExpressivAlgo_t, float> estados;

					EE_ExpressivAlgo_t upperFaceAction = ES_ExpressivGetUpperFaceAction(eState);
					float			   upperFacePower  = ES_ExpressivGetUpperFaceActionPower(eState);

					EE_ExpressivAlgo_t lowerFaceAction = ES_ExpressivGetLowerFaceAction(eState);
					float			   lowerFacePower  = ES_ExpressivGetLowerFaceActionPower(eState);
					
					estados[ upperFaceAction ] = upperFacePower;
					estados[ lowerFaceAction ] = lowerFacePower;
					
					ostringstream os2;
					os2 << estados[ EXP_SMILE ];
					string strEstados = os2.str().substr(0,5);

					//Cognitiv
					/*
					EE_CognitivAction_t actionType	= ES_CognitivGetCurrentAction(eState);
					float				actionPower = ES_CognitivGetCurrentActionPower(eState);
					
					map<EE_CognitivAction_t, float> cogEstados;

					cogEstados[ actionType ] = actionPower;

					ostringstream os3;
					os3 << cogEstados[ COG_RIGHT ];
					string strCog = os3.str().substr(0,5);
					*/
					
					//Manipulando os motores

					
					if(ES_ExpressivIsLookingLeft(eState) == 1 && ES_ExpressivIsLookingRight(eState) == 0)
					{
						serial.WriteData("180a",5);
						pos1 = 180;
					}
					if(ES_ExpressivIsLookingRight(eState) == 1 && ES_ExpressivIsLookingLeft(eState) == 0)
					{
						serial.WriteData("000a",5);
						pos1 = 0;
					}
					if(ES_ExpressivIsLookingLeft(eState) == 0 && ES_ExpressivIsLookingRight(eState) == 0)
					{
						if(pos1 != 90)
						{
							serial.WriteData("090a",5);
							pos1 = 90;
						}
					}
					
					if(estados[ EXP_EYEBROW ] > 0.0)
					{
						serial.WriteData("180b",5);
						pos2 = 180;
					}
					
					/*
					if(estados[ EXP_FURROW ] > 0.0)
					{
						serial.WriteData("060b",5);
						pos2 = 60;
					}
					*/
					if(estados[EXP_EYEBROW] == 0.0 /*&& estados[EXP_FURROW] == 0.0*/)
					{
						if(pos2 != 90)
						{
							serial.WriteData("090b",5);
							pos2 = 90;
						}
					}
					
					//Fim do codigo dos motores.


					//Comunicação com o Dialog
					SetDlgItemText(hwnd, IDC_STATICSORRIR, strEstados.c_str());
					SetDlgItemInt(hwnd, IDC_STATICFECHAROLHOS, ES_ExpressivIsBlink(eState),FALSE);
					SetDlgItemInt(hwnd, IDC_STATICOLHODIREITO, ES_ExpressivIsRightWink(eState),FALSE);
					SetDlgItemInt(hwnd, IDC_STATICOLHOESQUERDO, ES_ExpressivIsLeftWink(eState),FALSE);

					SetDlgItemInt(hwnd, IDC_STATICOLHARCIMA, ES_ExpressivIsLookingUp(eState),FALSE);
					SetDlgItemInt(hwnd, IDC_STATICOLHARBAIXO, ES_ExpressivIsLookingDown(eState),FALSE);
					SetDlgItemInt(hwnd, IDC_STATICOLHARDIREITA, ES_ExpressivIsLookingRight(eState),FALSE);
					SetDlgItemInt(hwnd, IDC_STATICOLHARESQUERDA, ES_ExpressivIsLookingLeft(eState),FALSE);
				}
			}

			return FALSE;
	}

	return FALSE;
}

//Função principal WinMain
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, DlgProc);
}

void ConectaEmoEngine(HWND hwnd)
{

	//Conecta ao EmoEngine (Headset)
	if(EE_EngineConnect() != EDK_OK){

		//cout << "\nDesculpe, mas nao foi possivel conectar ao EmoEngine" << endl;
		MessageBox(hwnd,"Desculpe, mas nao foi possivel conectar ao Emotiv Epoc Headset.","Erro", MB_ICONERROR | MB_OK);

	}else{

		SetDlgItemText(hwnd, IDC_STATUS, "Status: Conectado ao Headset");

		MessageBox(hwnd,"Conectado ao Emotiv Epoc Headset.","Conectado", MB_ICONINFORMATION | MB_OK);

	}

}

void ConectaControlPanel(HWND hwnd)
{
	//Pega o tamanho do texto contido no controle IDC_EDIT1
	int len = GetWindowTextLength(GetDlgItem(hwnd,IDC_EDIT1));

	int i;
	char* buffer;

	//Se o tamanho for mais que zero executa a condição
	if(len > 0)
	{
		//Aloca a quantidade memória necessária para o buffer
		buffer = (char*) GlobalAlloc(GPTR, len + 1);

		//Pega o texto do controle
		GetDlgItemText(hwnd, IDC_EDIT1, buffer, len + 1);
	}
	
	if(EE_EngineRemoteConnect(buffer,CONTROL_PANEL_PORT) != EDK_OK){
	
		MessageBox(hwnd,"Desculpe, mas nao foi possivel conectar ao Painel de Controle.","Erro", MB_ICONERROR | MB_OK);

	}else{

		SetDlgItemText(hwnd, IDC_STATUS, "Status: Conectado ao Painel de Controle");

		MessageBox(hwnd,"Conectado ao Painel de Controle.","Conectado", MB_ICONINFORMATION | MB_OK);

	}
	
	GlobalFree((HANDLE)buffer);
}

void ConectaEmoComposer(HWND hwnd){

	//Pega o tamanho do texto contido no controle IDC_EDIT1
	int len = GetWindowTextLength(GetDlgItem(hwnd,IDC_EDIT1));

	int i;
	char* buffer;

	//Se o tamanho for mais que zero executa a condição
	if(len > 0)
	{
		//Aloca a quantidade memória necessária para o buffer
		buffer = (char*) GlobalAlloc(GPTR, len + 1);

		//Pega o texto do controle
		GetDlgItemText(hwnd, IDC_EDIT1, buffer, len + 1);
	}
	
	if(EE_EngineRemoteConnect(buffer,EMO_COMPOSER_PORT) != EDK_OK){
	
		MessageBox(hwnd,"Desculpe, mas nao foi possivel conectar ao EmoComposer.","Erro", MB_ICONERROR | MB_OK);

	}else{

		SetDlgItemText(hwnd, IDC_STATUS, "Status: Conectado ao EmoComposer");

		MessageBox(hwnd,"Conectado ao EmoComposer.","Conectado", MB_ICONINFORMATION | MB_OK);

	}
	
	GlobalFree((HANDLE)buffer);
}

void DesconectaEmotiv(HWND hwnd)
{
	if(EE_EngineDisconnect() == EDK_OK)
	{
		SetDlgItemText(hwnd, IDC_STATUS, "Status: Desconectado");
		MessageBox(hwnd,"Desconectado.","Desconectado", MB_ICONINFORMATION | MB_OK);

	}else{
		MessageBox(hwnd,"Erro ao desconectar.","Erro", MB_ICONERROR | MB_OK);
	}
}