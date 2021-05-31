#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <process.h>
int main(int argc, char *argv[])
{
	if(argc == 2)
	{
		char *argument = argv[1];
		unsigned int dlugosc_argumentu = strlen(argument);
		if(dlugosc_argumentu > 20)
		{
			fprintf(stderr, "Podany argument jest dluzszy niz 20 znakow.\n");
			return 1;
		}
		else if(dlugosc_argumentu == 1)
		{
			unsigned int znak = (int) *argument;
			return znak;
		}
		else
		{
			const int lewa_dlugosc = dlugosc_argumentu / 2;
			const int prawa_dlugosc = dlugosc_argumentu - lewa_dlugosc;
			char lewy_arg[11];
			char prawy_arg[11];

			for(int i = 0; i < 11; i++)
			{
				lewy_arg[i] = '\0';
			}

			for(int i = 0; i < 11; i++)
			{
				prawy_arg[i] = '\0';
			}

			int indeks = 0;
			for(int i = lewa_dlugosc; i < dlugosc_argumentu; i++)
			{
				prawy_arg[indeks] = argument[i];
				indeks = indeks + 1;
			}

			for(int i = 0; i < lewa_dlugosc; i++)
			{
				lewy_arg[i] = argument[i];
			}

			STARTUPINFO si;
			PROCESS_INFORMATION pi[2];
			memset(&si, 0, sizeof(si));
			memset(&pi, 0, sizeof(pi));
			char s_1[100] = "win_procesy.exe ";
			char s_2[100] = "win_procesy.exe ";
			strcat(s_1, lewy_arg);
			strcat(s_2, prawy_arg);


			si.cb = sizeof(si);
			int dziecko_1 = CreateProcessA(NULL, s_1, NULL, NULL,
                                  0, 0, NULL, NULL, &si, pi + 0);

            int dziecko_2 = CreateProcessA(NULL, s_2, NULL, NULL,
                                  0, 0, NULL, NULL, &si, pi + 1);

            int id_rodzica = GetCurrentProcessId();

            DWORD s1;
            WaitForSingleObject(pi[0].hProcess, INFINITE);
            GetExitCodeProcess(pi[0].hProcess, &s1);
            printf("%10d  %10d        %15s   %c\n", id_rodzica, pi[0].dwProcessId, lewy_arg, s1);
            CloseHandle(pi[0].hProcess);
            CloseHandle(pi[0].hThread);

            DWORD s2;
            WaitForSingleObject(pi[1].hProcess, INFINITE);
            GetExitCodeProcess(pi[1].hProcess, &s2);
            printf("%10d  %10d        %15s   %c\n", id_rodzica, pi[1].dwProcessId, prawy_arg, s2);
            CloseHandle(pi[1].hProcess);
            CloseHandle(pi[1].hThread);


			if(s1 > s2)
            {
                return s1;
            }
            else
            {
                return s2;
            }
		}
	}
	else
	{
		fprintf(stderr, "Liczba argumentow jest rozna od 1.\n");
		return 1;
	}

	return 0;
}
