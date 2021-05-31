#include <stdio.h>
#include <io.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void)
{
    char znak;
    do
    {
        int desk = 0;
        do
        {
            int deskryptor = open("SO_LAB12.txt", O_WRONLY | O_EXCL | O_CREAT, S_IREAD | S_IWRITE);
            desk = deskryptor;
        } while(desk < 0);
        read(STDIN_FILENO, &znak, 1);
        write(desk, &znak, 1);
        close(desk);
    } while(znak != '*');
    return 0;
}
