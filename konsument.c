#include <stdio.h>
#include <io.h>
#include <fcntl.h>

int main(void)
{
    char znak;
    do
    {
        int dp_o;
        do
        {
            int dp_otworz = open("SO_LAB12.txt", O_RDONLY);
            dp_o = dp_otworz;
        }while(dp_o < 0);
        int dp_czytaj = read(dp_o, &znak, 1);
        if(dp_czytaj > 0)
        {
            write(STDOUT_FILENO, &znak, 1);
            close(dp_o);
            unlink("SO_LAB12.txt");
        }
        else
        {
            close(dp_o);
        }
    } while(znak != '*');
    return 0;
}
