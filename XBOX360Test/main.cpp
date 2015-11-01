/*
������ ���������:
S_1_;_2_;_3_n
_1_ - ����� ����� ��������� � ������������ ��������� ��������� (�.� �� _2_ �� n ������������)
_2_ - ������� ����� �������� �� ��� X
_3_ - ������� ����� �������� �� ��� Y
*/


#include "CXBOXController.h"		//���������� ������ � ���������, � ����� http://www.codeproject.com/Articles/26949/Xbox-Controller-Input-in-C-with-XInput
#include "Functions.h"

#include <iostream>					
#include <iomanip> 

#include <cstdlib>

#include <Windows.h>				//���������� ��� ������ � ��������� �������� ������� �� ������ ������
#include <process.h>				//���������� ��� ������ � ����������� ��������

#include "Serial lib\Serial.h"		//���������� ��� ������ � COM ������

CXBOXController* Player1;			//������ ��� ������ � ���������

Serial serial;						//������ COM �����

//COM ����:
HANDLE hSerial;						//���������� COM �����
wchar_t buffPortName[7];			//��������� ���������� �������� ����� COM �����

bool flgOK = false;					//���� ���������� ������ ������
bool flgStop = false;				//���� ��������� ������ ������

int STICK_MODE;						//���������� ����� ���������� (���� ��� ��� �����)

const int SLEEP_MSEC = 100;			//�������� ����� ���������� ���������

void Thread(void* pParams)			//����� ��� �������� ������ � Arduino
{
	//���������� ��� �������� ���������:
	int *iPosLX = new int;
	int *iPosLY = new int;
	int *iPosRX = new int;
	int *iPosRY = new int;

	int lenLX, lenLY, lenRX, lenRY;	//���������� ��� �������� ������ �����

	while (true)
	{
		if (flgStop) break;			//���� ������ ������ �� ������ ������ �������� �� ��������� �����

		if (flgOK)					//���� ������������� ������ ��������� �� �������� ������������ ������
		{
			int dataLen = 0;

			switch (STICK_MODE)
			{
			case 0:
			{
				*iPosLX = Player1->GetState().Gamepad.sThumbLX;
				*iPosLY = Player1->GetState().Gamepad.sThumbLY;
				break;
			}
			case 1:
			{
				*iPosLX = Player1->GetState().Gamepad.sThumbRX;
				*iPosLY = Player1->GetState().Gamepad.sThumbLY;
				break;
			}
			case 2:
			{
				*iPosLX = Player1->GetState().Gamepad.sThumbLX;
				*iPosLY = Player1->GetState().Gamepad.sThumbRY;
				break;
			}
			case 3:
			{
				*iPosLX = Player1->GetState().Gamepad.sThumbRX;
				*iPosLY = Player1->GetState().Gamepad.sThumbRY;
				break;
			}
			default:
			{
				*iPosLX = Player1->GetState().Gamepad.sThumbLX;
				*iPosLY = Player1->GetState().Gamepad.sThumbLY;
				break;
			}
			}

			lenLX = intlen(*iPosLX);
			lenLY = intlen(*iPosLY);

			char *posLX = new char[lenLX+1];
			char *posLY = new char[lenLY+1];

			itoa(*iPosLX, posLX, 10);
			itoa(*iPosLY, posLY, 10);

			dataLen = lenLX + lenLY + 3;

			char *len = new char[intlen(dataLen)];
			itoa(dataLen, len, 10);

			char *data = new char[dataLen + 2 + intlen(dataLen)+1];
			
			//��������� ���������:
			strcpy(data, "S");
			strcat(data, len);
			strcat(data, ";");
			
			strcat(data, posLX);
			strcat(data, ";");
			
			strcat(data, posLY);
			strcat(data, "n");
			
			//������� ������:
			serial.cSend(data, (sizeof(char)*(dataLen + 2 + intlen(dataLen) + 1)));	//������� � ������ ������������� ���������

			delete[] data;
			delete[] posLX;
			delete[] posLY;

			Sleep(SLEEP_MSEC);
		}
	}

	//�������� ���������� ��� �������� ���������:
	delete iPosLX;
	delete iPosLY;
	delete iPosRX;
	delete iPosRY;

	_endthread();					//��������� �����
}

int main(int argc, char* argv[])
{
	int nmbGmpd;					//���������� ��� �������� ������ ��������

	/*
		To Do: ������� try-catch ���� ������ if ������� �������� �������� ������
	*/

error:								//���� �������� �������� �������������� ������� �� ��� ���� ������ �� �����
	
	std::cout << "Enter number of gamepad(1-4): ";
	std::cin >> nmbGmpd;

	std::cout << "Control mode: " << std::endl;
	std::cout << "0-Left stick" << std::endl;
	std::cout << "1-Left(Y axis) and Right(X axis) stick)" << std::endl;
	std::cout << "2-Left(X axis) and Right(Y axis) stick)" << std::endl;
	std::cout << "3-Right stick" << std::endl;
	std::cout << "Enter Stick mode: ";
	std::cin >> STICK_MODE;

	std::cout << "Enter COM port number(COM_): ";
	std::wcin >> buffPortName;

	//������ � COM ������:
	serial.setCOMname(buffPortName);

	switch (serial.initCOM())
	{
	case 0:
	{
		std::cout << "COM port conected\n";
		delayAndCls();
		break;
	}
	case 1:
	{
		std::cout << "\aSerial port does not exist.\n";
		delayAndCls();

		return 0;
		break;
	}
	case 2:
	{
		std::cout << "\aSome other error occurred.\n";
		delayAndCls();

		return 0;
		break;
	}
	case 3:
	{
		std::cout << "\aGetting state error\n";
		delayAndCls();

		return 0;
		break;
	}
	case 4:
	{
		std::cout << "\aError setting serial port state\n";
		delayAndCls();

		return 0;
		break;
	}
	}

	if (nmbGmpd < 1 || nmbGmpd > 4)
	{
		std::cout << "\aincorrect number\n";
		system("pause");
		system("cls");
		goto error;
	}

	Player1 = new CXBOXController(nmbGmpd);

	_beginthread(Thread, 0, NULL);	//��������� ����� ��� �������� ������ �� Arduino

	std::cout << "Instructions:\n";
	std::cout << "[BACK] Exit\n";
	
	system("cls");

	while(true)
	{
		if(Player1->IsConnected())
		{
			flgOK = true;			//������ ������ ������ ������� ������ � ���������� ������

			cls();					//������� �����
			
			//������� ���������� � ����������� ������ ��������:
			std::cout << "Left stick:\n";
			std::cout << "X pos: " << std::setw(6) << Player1->GetState().Gamepad.sThumbLX << " " << "Y pos: " << std::setw(6) << Player1->GetState().Gamepad.sThumbLY << std::endl;
			std::cout << "Right stick:\n";
			std::cout << "X pos: " << std::setw(6) << Player1->GetState().Gamepad.sThumbRX << " " << "Y pos: " << std::setw(6) << Player1->GetState().Gamepad.sThumbRY << std::endl;
			
			if(Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_BACK)
			{
				flgStop = true;		//��������� ������ �����
				break;
			}
		}
		else
		{
			std::cout << "\t\aERROR! PLAYER " << nmbGmpd << " - XBOX 360 Controller Not Found!\n";
			std::cout << "Press Any Key To Exit.\n";
			flgStop = true;			//��������� ������ �����
			system("pause");
			break;
		}
	}

	delete(Player1);				//������� ������ Player1
	return( 0 );
}