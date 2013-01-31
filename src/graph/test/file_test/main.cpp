

#include <stdio.h>
#include <QDebug>
char mybuffer[80];
/* 该例子在ubuntu 下是运行不出预想的效果的，原因
 * 就是ubuntu自己的引起的。正确的应该要多调用一次
 * fseek*/

int main()
{
    FILE * pFile;
    pFile = fopen ("example.txt","r+");
    if (pFile == NULL) perror ("Error opening file");
    else {
        fputs ("test",pFile);
        fflush (pFile);    // flushing or repositioning required
        fseek(pFile, 0, SEEK_SET); // 关键语句，没有的话会出错。
        fgets (mybuffer,80,pFile);
        puts (mybuffer);
        fclose (pFile);
        return 0;
    }
}
