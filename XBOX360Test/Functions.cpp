#include <windows.h>
#include <iostream>	

#include "Functions.h"

void SetColor(ConsoleColorCOUT text)
{
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, (WORD)(text));
}

int intlen(int a)					//������� ����������� ������ �����
{
	int len = 0;

	if (a < 0)
	{
		a *= (-1);
		len++;
	}

	while (a > 0)
	{
		a /= 10;
		len++;
	}
	return len;
}

void cls()												//������� ������� ������
{
	COORD position;                                     // ���������� ����������� ���������
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);  // ��������� ����������� ���������� ������������ ������

	position.X = 0;										// ��������� ���������� X
	position.Y = 0;										// ��������� ���������� Y

	Sleep(100);											//������������� ���������� ��������� �� 100�� ��� ����������� ������� �������� �� ������

	SetConsoleCursorPosition(hConsole, position);		//������������� ������ � ������ �������

														//������� ������ �������:
	std::cout << "                                          " << std::endl
		<< "                                          " << std::endl
		<< "                                          " << std::endl
		<< "                                          " << std::endl;

	SetConsoleCursorPosition(hConsole, position);		//������������� ������ � ������ �������
}

void delayAndCls()
{
	system("pause");
	system("cls");
}