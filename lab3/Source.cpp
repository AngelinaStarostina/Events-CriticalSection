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

	cout << "Количество четных элементов: " << count;

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
	cout << "Введите временной интервал, требуемый для отдыха после подготовки одного элемента в массиве: ";
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
	event2 = CreateEvent(NULL, FALSE, FALSE, NULL);	//Инициализировать необходимые события и критические секции.
	InitializeCriticalSection(&critSection);

	int n;
	char* arr = NULL;
	cout << "Введите размерность и элементы массива: ";			//создать массив, размерность и элементы которого вводятся пользователем с консоли;
	cin >> n;
	arr = new char[n+1];
	arr[0] = n;
	for (int i = 1; i < n + 1; i++)
		cin >> arr[i];

	int k;
	cout << "Введите k: ";
	cin >> k;


	HANDLE worker;
	worker = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Work, (void*)arr, 0, NULL);		//запустить поток work; запустить поток CountElementм;
	if (worker == 0)
		return GetLastError();

	HANDLE countEl;
	countEl = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CountElement, (void*)arr, 0, NULL);
	if (countEl == 0)
		return GetLastError();


	WaitForSingleObject(event1, INFINITE);		//Получить от потока work сигнал о начале суммирования 
	for (int i = 0; i < strlen(sortedArr); i++)
	{
		cout << sortedArr[i];					//Выводить на экран элементы массива
	}

	SetEvent(event2);			//известить поток CountElement о начале суммирования


	for (int i = 0; i < strlen(sortedArr); i++)
	{
		cout << sortedArr[i];				// Выводить на экран элементы массива
	}

	EnterCriticalSection(&critSection);
	for (int i = 0; i < strlen(sortedArr); i++)
	{
		cout << sortedArr[i];				// Выводить на экран элементы массива, Дождаться сигнала потока CountElement, Вывести на экран результат работы потока CountElement
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