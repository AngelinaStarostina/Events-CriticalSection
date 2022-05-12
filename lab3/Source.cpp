#include <iostream>
#include <windows.h>
#include <process.h>
using namespace std;

HANDLE event1, event2;
CRITICAL_SECTION critSection;
char* sortedArr = NULL;
int n;

void WINAPI CountElement(char* arr)
{
	WaitForSingleObject(event2, INFINITE);
	int count = 0;
	for (int i = 0; i < n; i++)
	{
		if ((int)arr[i] % 2 == 0 && (int)arr[i] != 0)
			count++;
	}

	cout << "���������� ������ ���������: " << count << endl;

	EnterCriticalSection(&critSection);
	cout << "������ CountElement: ";
	for (int i = 0; i < n; i++)
	{
		cout << sortedArr[i] << endl;
	}
	LeaveCriticalSection(&critSection);
}

UINT WINAPI Work(char* arr)
{
	int time;
	cout << "������� ��������� ��������, ��������� ��� ������ ����� ���������� ������ �������� � �������: ";
	cin >> time;
	sortedArr = new char[n];
	
	int j = 0;
	char H;
	for (int i = 0; i < n; i++)
	{
		H = (char)arr[i];
		if (H < 48)
		{
			sortedArr[j] = H;
			j++;
		}
		else if (H > 57 && H < 65)
		{
			sortedArr[j] = H;
			j++;
		}
		else if (H > 90 && H < 97)
		{
			sortedArr[j] = H;
			j++;
		}
		else if (H > 122)
		{
			sortedArr[j] = H;
			j++;
		}
	}

	if (j != n)
	{
		for (int i = j; i < n; i++)
			sortedArr[i] = ' ';
	}

	int k;
	for (int i = 0; i < n; i++)
	{
		cout << sortedArr[i] << endl;
		Sleep(time);
	}


	cout << endl;
	SetEvent(event1);
	return 0;
}

int main()
{
	setlocale(LC_ALL, "rus");

	event1 = CreateEvent(NULL, FALSE, FALSE, NULL);
	event2 = CreateEvent(NULL, FALSE, FALSE, NULL);	//���������������� ����������� ������� � ����������� ������.
	InitializeCriticalSection(&critSection);

	char* arr = NULL;
	cout << "������� ����������� � �������� �������: ";			//������� ������, ����������� � �������� �������� �������� ������������� � �������;
	cin >> n;
	arr = new char[n];
	for (int i = 0; i < n; i++)
		cin >> arr[i];

	int k;
	cout << "������� k: ";
	cin >> k;


	HANDLE worker;
	worker = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Work, (void*)arr, 0, NULL);		//��������� ����� work; ��������� ����� CountElement�;
	if (worker == 0)
		return GetLastError();

	HANDLE countEl;
	countEl = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CountElement, (void*)arr, 0, NULL);
	if (countEl == 0)
		return GetLastError();


	WaitForSingleObject(event1, INFINITE);		//�������� �� ������ work ������ � ������ ������������ 
	cout << "�������� ������: ";
	for (int i = 0; i < n; i++)
	{
		cout << sortedArr[i] << endl;					//�������� �� ����� �������� �������
	}

	SetEvent(event2);			//��������� ����� CountElement � ������ ������������

	EnterCriticalSection(&critSection);
	cout << "�������� ������ (CriticalSection): ";
	for (int i = 0; i < n; i++)
	{
		cout << sortedArr[i] << endl;				// �������� �� ����� �������� �������, ������� �� ����� ��������� ������ ������ CountElement
	}
	LeaveCriticalSection(&critSection);

	WaitForSingleObject(countEl, INFINITE);
	WaitForSingleObject(worker, INFINITE);

	
	CloseHandle(worker);
	CloseHandle(countEl);
	DeleteCriticalSection(&critSection);
	CloseHandle(event2);
	CloseHandle(event1);

	delete[] sortedArr;
	delete[] arr;

	return 0;
}