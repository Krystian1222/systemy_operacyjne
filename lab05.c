#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

DWORD WINAPI tekst_watku(LPVOID dane);

float suma_z_watkami = 0;

typedef struct
{
	float *miejsce;
	int rozmiar;
} struktura;

HANDLE muteks;

/***************************************************************/
int main(int argc, char **argv)
{
	int liczba_danych = atoi(argv[1]);
	int liczba_watkow = atoi(argv[2]);
	if((argc != 3) || ((liczba_danych < 0 || liczba_danych > 1000000) || (liczba_watkow < 0 || liczba_watkow > liczba_danych)))
	{
		printf("Nieprawidlowa liczba argumentow i / lub niewlasciwe argumenty.\n");
		return 1;
	}
	struktura *wsk_str;
	HANDLE *uchwyty_watkow;
	DWORD *TIDy;
	TIDy = (DWORD*)malloc(sizeof(DWORD) * liczba_watkow);
	uchwyty_watkow = (HANDLE*)malloc(sizeof(HANDLE) * liczba_watkow);
	wsk_str = (struktura*)malloc(sizeof(struktura) * liczba_watkow);

	muteks = CreateMutex(NULL, FALSE, NULL);
	float *t;
	t = (float*)malloc(sizeof(float) * liczba_danych);

	for(int i = 0; i < liczba_danych; i++)
	{
		t[i] = 0;
	}
	srand(0);
	for(int i = 0; i < liczba_danych; i++)
	{
		t[i] = 1000. * rand() / RAND_MAX;
	}

	int reszta = liczba_danych % liczba_watkow;
	int indeks = 0;

	clock_t t1 = clock();
	if(reszta != 0)
	{
		for(int i = 0; i < liczba_watkow - 1; i++)
		{
			wsk_str[i].miejsce = t + indeks;
			wsk_str[i].rozmiar = liczba_danych / liczba_watkow;
			//HANDLE WINAPI(&uchwyty_watkow[i], NULL, tekst_watku, &wsk_str[i]);
			uchwyty_watkow[i] = CreateThread(NULL, 0, tekst_watku, &wsk_str[i], 0, TIDy + i);
			indeks = indeks + liczba_danych / liczba_watkow;
		}

		wsk_str[liczba_watkow - 1].rozmiar = liczba_danych / liczba_watkow + reszta;
		wsk_str[liczba_watkow - 1].miejsce = t + indeks;
		uchwyty_watkow[liczba_watkow - 1] = CreateThread(NULL, 0, tekst_watku, &wsk_str[liczba_watkow - 1], 0, TIDy + (liczba_watkow - 1));
		for(int i = 0; i < liczba_watkow; i++)
		{
			//void* suma = 0;
			WaitForSingleObject(uchwyty_watkow[i], INFINITE);
			CloseHandle(uchwyty_watkow[i]);
			//pthread_join(uchwyty_watkow[i], (void *)&suma);
			//printf("Thread %f sum=%f\n", uchwyty_watkow[i], (float)suma);
			//printf("Thread %lu sum=%f\n", uchwyty_watkow[i], (float)suma);
		}
	}
	else
	{
		for(int i = 0; i < liczba_watkow; i++)
		{
			wsk_str[i].miejsce = t + indeks;
			wsk_str[i].rozmiar = liczba_danych / liczba_watkow;
			uchwyty_watkow[i] = CreateThread(NULL, 0, tekst_watku, &wsk_str[i], 0, TIDy + i);
			indeks = indeks + liczba_danych / liczba_watkow;
		}

		for(int i = 0; i < liczba_watkow; i++)
		{
			//void* suma = 0;
			WaitForSingleObject(uchwyty_watkow[i], INFINITE);
			CloseHandle(uchwyty_watkow[i]);
			//printf("Thread %lu\n", uchwyty_watkow[i]);
		}
	}

	clock_t t2 = clock();
	double czas_z_watkami = (double)(t2 - t1) / CLOCKS_PER_SEC;
	printf("w/Threads : sum=%f, time=%lfsec\n", suma_z_watkami, czas_z_watkami);

/**************************************************************/
	float suma_bez_watkow = 0;
	clock_t t3 = clock();
	for(int i = 0; i < liczba_danych; i++)
	{
		suma_bez_watkow = suma_bez_watkow + t[i];
	}
	clock_t t4 = clock();
	double czas_sumy_bez_watkow = (t4 - t3) / (double)CLOCKS_PER_SEC;
	printf("wo/Threads: sum=%f, time=%lfsec\n", suma_bez_watkow, czas_sumy_bez_watkow);

	return 0;
}
/********************************************************************/
DWORD WINAPI tekst_watku(LPVOID dane)
{
	float suma = 0;
	DWORD obecny_watek;
	obecny_watek = GetCurrentThreadId();
	struktura *wsk_str = (struktura *) dane;
	if(wsk_str->rozmiar <= 1)
	{
		suma = wsk_str->miejsce[0];
	}
	else
	{
		for(int i = 0; i < (wsk_str->rozmiar); i++)
		{
			suma = suma + wsk_str->miejsce[i];
		}
	}

	printf("Thread #%d size=%d\n", obecny_watek, wsk_str->rozmiar);
	printf("Thread #%d sum=%f\n", obecny_watek, suma);
	WaitForSingleObject(muteks, INFINITE);
	suma_z_watkami = suma_z_watkami + suma;
	ReleaseMutex(muteks);

	//return (void *)suma;
}
