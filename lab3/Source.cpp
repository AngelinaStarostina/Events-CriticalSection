#include <iostream>
#include <windows.h>
#include <process.h>
using namespace std;

HANDLE event1, event2;
CRITICAL_SECTION critSection;
char* sortedArr = NULL;

void WINAPI CountElement(LPVOID* arr)
{
	WaitForSingleObject(event2, INFINITE);
	int count = 0;
	int n = (char)arr[0];
	for (int i = 1; i <= n; i++)
	{
		if ((int)arr[i] % 2 == 0 && (int)arr[i] != 0)
			count++;
	}

	cout << "���������� ������ ���������: " << count;

	EnterCriticalSection(&critSection);
	for (int i = 0; i < strlen(sortedArr); i++)
	{
		cout << sortedArr[i] << endl;
	}
	LeaveCriticalSection(&critSection);

}

UINT WINAPI Work(LPVOID* arr)
{
	int time;
	cout << "������� ��������� ��������, ��������� ��� ������ ����� ���������� ������ �������� � �������: ";
	cin >> time;
	int n = (char)arr[0];
	sortedArr = new char[n];
	

	SetEvent(event1);
	return 0;
}

int main()
{
	setlocale(LC_ALL, "rus");

	event1 = CreateEvent(NULL, FALSE, FALSE, NULL);
	event2 = CreateEvent(NULL, FALSE, FALSE, NULL);	//���������������� ����������� ������� � ����������� ������.
	InitializeCriticalSection(&critSection);

	int n;
	char* arr = NULL;
	cout << "������� ����������� � �������� �������: ";			//������� ������, ����������� � �������� �������� �������� ������������� � �������;
	cin >> n;
	arr = new char[n+1];
	arr[0] = n;
	for (int i = 1; i < n + 1; i++)
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
	for (int i = 0; i < strlen(sortedArr); i++)
	{
		cout << sortedArr[i];					//�������� �� ����� �������� �������
	}

	SetEvent(event2);			//��������� ����� CountElement � ������ ������������


	for (int i = 0; i < strlen(sortedArr); i++)
	{
		cout << sortedArr[i];				// �������� �� ����� �������� �������
	}

	EnterCriticalSection(&critSection);
	for (int i = 0; i < strlen(sortedArr); i++)
	{
		cout << sortedArr[i];				// �������� �� ����� �������� �������, ��������� ������� ������ CountElement, ������� �� ����� ��������� ������ ������ CountElement
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