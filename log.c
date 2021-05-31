#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <io.h>
#include <time.h>
#include <sys/stat.h>
#include <errno.h>

struct log
{
    struct tm date; // czas
    char argument[10]; // napis z argumentu wywolania
};

int main(int argc, char *argv[])
{
    if(argc > 3 || argc < 2)
    {
        fprintf(stderr, "Zla liczba argumentow.\n");
        return 1;
    }
	int deskryptor_pliku = 0;
	if (strcmp(argv[1], "-r") == 0)
	{
		intptr_t uchwyt;
		struct _finddata64i32_t FD_najnowszy;
		struct _finddata64i32_t FD_aktualny;
		struct log info;
		if ((uchwyt = _findfirst64i32("*.log", &FD_najnowszy)) != -1L)
		{
			while (_findnext64i32(uchwyt, &FD_aktualny) == 0)
			{
				if (difftime(FD_najnowszy.time_write, FD_aktualny.time_write) < 0)
				{
					FD_najnowszy = FD_aktualny;
				}
			}
			int status_flags = _O_RDWR | _O_BINARY;
			deskryptor_pliku = _open(FD_najnowszy.name, status_flags);
			_lseek(deskryptor_pliku, -sizeof(info), SEEK_END);
			_read(deskryptor_pliku, &info, sizeof(struct log));
			_findclose(deskryptor_pliku);
			printf("%s%s", asctime(&info.date), info.argument);
		}
		else
		{
			printf("Brak plikow z rozszerzeniem *.log.\n");
		}
	}
	else if (strcmp(argv[1], "-w") == 0)
	{
		char nazwa_pliku[25];
		struct log informacja = { 0 };
		time_t czas;
		time(&czas);
		informacja.date = *localtime(&czas);
		int i;
		for (i = 0; i < 9; i++)
		{
			informacja.argument[i] = argv[2][i];
		}
		informacja.argument[9] = '\0';
		//printf("%s", informacja.argument);
		int tryb_otwarcia = _O_CREAT | _O_APPEND | _O_RDWR | _O_BINARY;
		int prawa = _S_IREAD | _S_IWRITE;
		snprintf(nazwa_pliku, sizeof(nazwa_pliku), "%d.%02d.%02d.%02d.%02d.log",
			informacja.date.tm_year + 1900, informacja.date.tm_mon + 1,
			informacja.date.tm_mday, informacja.date.tm_hour, informacja.date.tm_min);
		//printf("Liczba znakow: %d", liczba);
		deskryptor_pliku = _open(nazwa_pliku, tryb_otwarcia, prawa);
		int blad = _write(deskryptor_pliku, &informacja, sizeof(struct log));
		if (blad < 0)
		{
			fprintf(stderr, "Blad zapisu pliku %d.", errno);
		}
		_close(deskryptor_pliku);
    }
    else
    {
        fprintf(stderr, "Nieprawidlowy argument nr 1.\n");
		//printf("%s", argv[1]);
        return 1;
    }
}
