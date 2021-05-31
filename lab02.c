#include <stdio.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>


int main(int argc, char **argv[])
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

			int pid_rodzica = getpid();

			int dziecko1 = fork();
			if(dziecko1 == 0)
			{
				execlp(argv[0], argv[0], lewy_arg, NULL);
			}

			int dziecko2 = fork();
			if(dziecko2 == 0)
			{
				execlp(argv[0], argv[0], prawy_arg, NULL);
			}

			int s1;
			int pid_dziecka_1 = waitpid(dziecko1, &s1, 0);
			printf("%10d  %10d        %15s  %c\n", pid_rodzica, pid_dziecka_1, lewy_arg, WEXITSTATUS(s1));

			int s2;
			int pid_dziecka_2 = waitpid(dziecko2, &s2, 0);
			printf("%10d  %10d        %15s  %c\n", pid_rodzica, pid_dziecka_2, prawy_arg, WEXITSTATUS(s2));

			if(WEXITSTATUS(s1) > WEXITSTATUS(s2))
			{
				return WEXITSTATUS(s1);
			}
			else
			{
				return WEXITSTATUS(s2);
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
