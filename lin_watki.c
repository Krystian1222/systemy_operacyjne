#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <stdlib.h>

float suma_z_watkami = 0;

typedef struct
{
	float *miejsce;
	int rozmiar;
} struktura;

pthread_mutex_t muteks = PTHREAD_MUTEX_INITIALIZER;

void *tekst_watku(void *dane)
{
	float suma = 0;
	pthread_t obecny_watek;
	obecny_watek = pthread_self();
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
	printf("Thread #%lu size=%d\n", obecny_watek, wsk_str->rozmiar);
	printf("Thread #%lu sum=%f\n", obecny_watek, suma);
	pthread_mutex_lock(&muteks);
	suma_z_watkami = suma_z_watkami + suma;
	pthread_mutex_unlock(&muteks);
}
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

	struktura *wsk_str;
	wsk_str = (struktura*)malloc(sizeof(struktura) * liczba_watkow);
	pthread_t *TIDy;
	TIDy = (pthread_t*)malloc(sizeof(pthread_t) * liczba_watkow);
	int reszta = liczba_danych % liczba_watkow;
	int indeks = 0;

	clock_t t1 = clock();
	if(reszta != 0)
	{
		for(int i = 0; i < liczba_watkow - 1; i++)
		{
			wsk_str[i].miejsce = t + indeks;
			wsk_str[i].rozmiar = liczba_danych / liczba_watkow;
			pthread_create(&TIDy[i], NULL, tekst_watku, &wsk_str[i]);
			indeks = indeks + liczba_danych / liczba_watkow;
		}

		wsk_str[liczba_watkow - 1].rozmiar = liczba_danych / liczba_watkow + reszta;
		wsk_str[liczba_watkow - 1].miejsce = t + indeks;
		pthread_create(&TIDy[liczba_watkow - 1], NULL, tekst_watku, &wsk_str[liczba_watkow - 1]);
		for(int i = 0; i < liczba_watkow; i++)
		{
			//void* suma = 0;
			pthread_join(TIDy[i], NULL);
			//pthread_join(watki[i], (void *)&suma);
			//printf("Thread %f sum=%f\n", watki[i], (float)suma);
			//printf("Thread %lu sum=%f\n", watki[i], (float)suma);
		}
	}
	else
	{
		for(int i = 0; i < liczba_watkow; i++)
		{
			wsk_str[i].miejsce = t + indeks;
			wsk_str[i].rozmiar = liczba_danych / liczba_watkow;
			pthread_create(&TIDy[i], NULL, tekst_watku, &wsk_str[i]);
			indeks = indeks + liczba_danych / liczba_watkow;
		}

		for(int i = 0; i < liczba_watkow; i++)
		{
			//void* suma = 0;
			pthread_join(TIDy[i], NULL);
			//printf("Thread %lu\n", watki[i]);
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
