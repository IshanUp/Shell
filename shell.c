#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <grp.h>
#include <pwd.h>
#include <time.h>
#include <sys/wait.h>
char directory[1000000] = "~";
char path[100][1000]; //stores the current path
char currprocname[1000];
int currprocid = -1;
int counts = 0;
int pipenumber = 0;
int pd[2];
int in;
int x1, x2;
int x, bit;

int tekken(char *str, char arg[][100]);

void leftandrightspace(char *str)
{

    char space[] = " ";
    int alen = 0;
    char arg[100][100];
    int i = 0;
    char v[1000] = "";
    alen = tekken(str, arg);

    while (i < alen)
    {
        strcat(v, arg[i]);
        if (i != alen - 1)
        {
            strcat(v, space);
        }
        i++;
    }
    strcpy(str, v);
}

int ifnull(char *arr)
{
    if (arr == NULL)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int tekken(char *str, char arg[][100])
{
    char del[] = " \n";
    int alen = 0;
    char *ptr = strtok(str, del);
    //tokenizing on new line
    while (ptr != NULL)
    {
        strcpy(arg[alen], ptr);
        alen++;
        ptr = strtok(NULL, del);
    }
    return alen;
}

void pipenow(char string[])
{
    pipenumber = 0;
    int i = 0;
    int size = strlen(string);
    while (i < size)
        if (string[i++] == '|')
            pipenumber++;
}
void semaparse(char *userin, char **arr)
{
    int i;
    i = 0;
    while (i < 10)
    {
        //splitting
        arr[i] = strsep(&userin, " ");
        if (ifnull(arr[i]))
        {
            break;
        }
        i++;
    }
}

void pipetoken(char res[][100], char inp[])
{
    int x3 = strlen(inp);
    x1 = 0;
    int i = 0;
    x2 = 0;
    char delim[] = "|";
    while (i < x3)
    {
        if (inp[i] != delim[0])
        {
            res[x1][x2++] = inp[i];
        }
        else
        {   int temp = x2;
            x2 = 0;
            res[x1++][temp] = '\0';
        }
        i++;
    }
    int j = 0;
    int sum = x1 + 1;
    res[x1][x2] = '\0';

    while (j < sum)
    {
        leftandrightspace(res[j]);
        j++;
    }
}

int braccount(char string[])
{
    counts = 0;
    int x = strlen(string);
    for (int i = 0; i < x; i++)
    {
        if (string[i] == '<')
            counts++;
        if (string[i] == '>')
            counts = counts + 2;
    }
    return counts;
}
void redirect(char inp[])
{

    int turnchange = 0;
    int count = 0;
    char carr[strlen(inp)];
    strcpy(carr, inp);
    if (counts == 2)
    {
        count = 0;
        char *givenin = strtok(inp, ">");
        char fexec[strlen(carr)];
        int i = 0;
        int lencarr= strlen(carr);
        while(i < lencarr)
        {
            if (turnchange == 1)
                fexec[count++] = carr[i];
            if (carr[i] == '>')
                turnchange = 1;
            i++;
        }
        fexec[count] = '\0';
        leftandrightspace(fexec);
        leftandrightspace(givenin);
        int fd = open(fexec, O_WRONLY | O_CREAT | O_TRUNC);
        if (fork() == 0)
        {
            char *execution[1000];
            dup2(fd, 1);
            semaparse(givenin, execution);
            execvp(execution[0], execution);
        }
    }
    else if (counts == 1)
    {
        char *givenin = strtok(inp, "<");
        char fexec[strlen(carr)];
        count = 0;
        turnchange = 0;
        int i =0;
        int lencarr = strlen(carr);
        while(i < lencarr)
        {
            if (turnchange == 1)
                fexec[count++] = carr[i];
            if (carr[i] == '<')
                turnchange = 1;
            i++;
        }
        fexec[count] = '\0';
        leftandrightspace(fexec);
        leftandrightspace(givenin);
        int fd = open(fexec, O_RDONLY);
        if (fork() == 0)
        {
            char *execution[1000];
            dup2(fd, 0);
            semaparse(givenin, execution);
            dup2(fd, 0);
            execvp(execution[0], execution);
        }
    }
    else if (counts == 3)
    {

        char *givenin = strtok(inp, "<");
        char jk[strlen(carr)];
        char jk1[strlen(carr)];
        x = 0;
        bit = 0;
        int size = strlen(carr);
        int i = 0;
        for (i = 0; i < size;)
        {
            if (carr[i] == '>')
                break;
            if (bit == 1)
                jk[x++] = carr[i];
            if (carr[i] == '<')
                bit = 1;
            i++;
        }
        jk[x] = '\0';
        x = bit = 0;
        i = 0;
        for (i = 0; i < size;)
        {
            if (bit == 1)
                jk1[x++] = carr[i];
            if (carr[i] == '>')
                bit = 1;
            i++;
        }
        jk1[x] = '\0';
        leftandrightspace(givenin);
        leftandrightspace(jk);
        leftandrightspace(jk1);
        int lk1 = open(jk1, O_WRONLY  | O_TRUNC | O_CREAT);
        int lk = open(jk, O_RDONLY);
        if (fork() == 0)
        {
            char *execution[100];
            semaparse(givenin, execution);
            dup2(lk, 0);
            dup2(lk1, 1);
            execvp(execution[0], execution);
        }
    }
}

struct bginfo
{
    int status; // 1 for bg , 0 for stopped
    char bgname[1000];
    int id;
};
typedef struct bginfo bginfo;
int backnum = 0; //size of array of background process

bginfo bg[1000];

unsigned int pathCount = 1; //length of path array
int isDirectory(const char *path)
{
    struct stat statbuf;
    if (stat(path, &statbuf) != 0)
        return 0;
    return S_ISDIR(statbuf.st_mode);
}
void pwds(char directory[])
{
    //printf("here");
    printf("%s", directory);
}

void pinfos(char words[100][10000], int wordSize)
{
    if (wordSize - 1 != 0)
    {
        // if pid is given
        char procPath[1000] = "/proc/";
        char statPath[1000], execPath[1000];
        char pid[1000];
        strcpy(pid, words[1]);
        strcat(procPath, pid);
        strcpy(statPath, procPath);
        strcat(statPath, "/status");
        strcpy(execPath, procPath);
        strcat(execPath, "/exe");

        int f = open(statPath, O_RDONLY);

        char statInfo[10000];
        char statwords[1000][1000];
        int statwordsize = 0;
        read(f, statInfo, 1355);

        char del[] = " \a\r\t\n";
        char *ptr = strtok(statInfo, del);
        while (ptr != NULL)
        {
            strcpy(statwords[statwordsize], ptr);
            statwordsize++;
            ptr = strtok(NULL, del);
        }
        char readlinked[200] = "";
        readlink(execPath, readlinked, sizeof(readlinked));

        printf("pid -- %s\n", statwords[12]);
        printf("Process Status -- %s\n", statwords[5]);
        printf("memory -- %s kB\n", statwords[50]);
        printf("Executable Path -- %s\n", readlinked);
    }
    else
    {
        // if pid is not given ie. pid of current process is taken
        char procPath[1000] = "/proc/";
        char statPath[1000], execPath[1000];
        char pid[1000];
        sprintf(pid, "%d", getpid());
        strcat(procPath, pid);
        strcpy(statPath, procPath);
        strcat(statPath, "/status");
        strcpy(execPath, procPath);
        strcat(execPath, "/exe");

        int f = open(statPath, O_RDONLY);

        char statInfo[10000];
        char statwords[1000][1000];
        int statwordsize = 0;
        read(f, statInfo, 1355);

        char del[] = " \a\r\t\n";
        char *ptr = strtok(statInfo, del);
        while (ptr != NULL)
        {
            strcpy(statwords[statwordsize], ptr);
            statwordsize++;
            ptr = strtok(NULL, del);
        }
        char readlinked[200] = "";
        readlink(execPath, readlinked, sizeof(readlinked));

        printf("pid -- %s\n", statwords[12]);
        printf("Process Status -- %s\n", statwords[5]);
        printf("memory -- %s kB\n", statwords[50]);
        printf("Executable Path -- %s", readlinked);
    }
}

void execfun(char words[100][10000], int wordSize)
{

    char *arg[1000];
    int status;
    for (int i = 0; i < wordSize; i++)
    {
        arg[i] = words[i];
        //printf("%s\n",arg[i]);
    }

    //if & is not there
    if (!(strcmp(words[wordSize - 1], "&") == 0))
    {
        //printf("and is not there!");
        int pid = fork();
        if (pid == 0)
        {
            int temp = execvp(arg[0], arg);
            if (temp == -1)
            {
                printf("ERROR: Command not found!");
            }
            exit(1);
        }

        sprintf(currprocname, "%s", arg[0]);
        currprocid = pid;
        waitpid(pid, &status, WUNTRACED);
        currprocid = -1;
        sprintf(currprocname, "");
    }
    else
    {

        int pid = fork();

        if (pid == 0)
        {
            arg[wordSize - 1] = NULL;
            int temp[2];
            temp[0] = execvp(arg[0], arg);

            if (temp[0] == -1)
                printf("ERROR: Command not found!");
            exit(1);
        }
        else
            printf("%d\n", pid);
        bg[backnum].id = (int)pid;
        sprintf(bg[backnum].bgname, "%s", arg[0]);
        backnum++;
        waitpid(pid, &status, WNOHANG);
    }
}

void echos(char words[100][10000], int wordSize)
{

    ;
    for (int i = 1; i < wordSize; i++)
    {
        printf("%s ", words[i]);
    }
}

void lss(char words[100][10000], int wordSize)
{
    struct dirent *sd;
    char dot[] = ".";

    if (strcmp(words[1], "-l") == 0)
    {
        int dosomething = 0;
        DIR *dir;
        dir = opendir(".");
        if (dir == NULL)
        {
            printf("ERROR : Unable to open directory");
        }

        while ((sd = readdir(dir)) != NULL)
        {
            char dirname[1000];
            strcpy(dirname, sd->d_name);

            if (dirname[0] == dot[0])
            {
                continue;
            }
            struct stat buf;
            char *path;
            path = dirname;

            int num = buf.st_mode;
            stat(path, &buf);
            if (isDirectory(path) == 1)
            {
                printf("d");
            }
            else
            {
                printf("-");
            }
            if (buf.st_mode & S_IRUSR)
            {
                printf("r");
            }
            else
            {
                printf("-");
            }
            if (buf.st_mode & S_IWUSR)
            {
                printf("w");
            }
            else
            {
                printf("-");
            }
            if (buf.st_mode & S_IXUSR)
            {
                printf("x");
            }
            else
            {
                printf("-");
            }
            //
            if (buf.st_mode & S_IRGRP)
            {
                printf("r");
            }
            else
            {
                printf("-");
            }
            if (buf.st_mode & S_IWGRP)
            {
                printf("w");
            }
            else
            {
                printf("-");
            }
            if (buf.st_mode & S_IXGRP)
            {
                printf("x");
            }
            else
            {
                printf("-");
            }
            //
            if (buf.st_mode & S_IROTH)
            {
                printf("r");
            }
            else
            {
                printf("-");
            }
            if (buf.st_mode & S_IWOTH)
            {
                printf("w");
            }
            else
            {
                printf("-");
            }
            if (buf.st_mode & S_IXOTH)
            {
                printf("x");
            }
            else
            {
                printf("-");
            }
            printf(" ");

            printf(" %ld ", buf.st_nlink);

            //printf("%d ",buf.st_uid);
            //printf("%d ",buf.st_gid);
            struct passwd *pw = getpwuid(buf.st_uid);
            struct group *gr = getgrgid(buf.st_gid);
            printf("%s %s ", pw->pw_name, gr->gr_name);
            printf("%ld", buf.st_size);
            char strnumber[50];
            int spaces = 8;

            sprintf(strnumber, "%ld", buf.st_size);
            int numlen;
            numlen = strlen(strnumber);
            spaces = spaces - numlen;
            //   printf("spaces %d ",spaces );
            for (int i = 0; i < spaces + 1; i++)
            {
                printf(" ");
            }
            if (spaces < 0)
            {
                printf("              ");
            }

            char *dtime = ctime(&buf.st_mtime);
            int len = strlen(dtime);
            dtime[len - 1] = '\0';
            printf("%s ", dtime);

            printf("%s\n", dirname);
        }

        closedir(dir);
    }
    else if (strcmp(words[1], "-a") == 0)
    {
        //   int dosomethingelse=0;
        DIR *dir;

        dir = opendir(".");
        if (dir == NULL)
        {
            printf("ERROR : Unable to open directory");
        }
        while ((sd = readdir(dir)) != NULL)
        {
            printf("%s\n", sd->d_name);
        }

        closedir(dir);
    }
    else if (strcmp(words[1], "-la") == 0 || strcmp(words[1], "-al") == 0)
    {
        DIR *dir;
        dir = opendir(".");
        if (dir == NULL)
        {
            printf("ERROR : Unable to open directory");
        }

        while ((sd = readdir(dir)) != NULL)
        {
            char dirname[1000];
            strcpy(dirname, sd->d_name);

            struct stat buf;
            char *path;
            path = dirname;

            int num = buf.st_mode;
            stat(path, &buf);
            if (isDirectory(path) == 1)
            {
                printf("d");
            }
            else
            {
                printf("-");
            }
            if (buf.st_mode & S_IRUSR)
            {
                printf("r");
            }
            else
            {
                printf("-");
            }
            if (buf.st_mode & S_IWUSR)
            {
                printf("w");
            }
            else
            {
                printf("-");
            }
            if (buf.st_mode & S_IXUSR)
            {
                printf("x");
            }
            else
            {
                printf("-");
            }
            //
            if (buf.st_mode & S_IRGRP)
            {
                printf("r");
            }
            else
            {
                printf("-");
            }
            if (buf.st_mode & S_IWGRP)
            {
                printf("w");
            }
            else
            {
                printf("-");
            }
            if (buf.st_mode & S_IXGRP)
            {
                printf("x");
            }
            else
            {
                printf("-");
            }
            //
            if (buf.st_mode & S_IROTH)
            {
                printf("r");
            }
            else
            {
                printf("-");
            }
            if (buf.st_mode & S_IWOTH)
            {
                printf("w");
            }
            else
            {
                printf("-");
            }
            if (buf.st_mode & S_IXOTH)
            {
                printf("x");
            }
            else
            {
                printf("-");
            }
            printf(" ");

            printf(" %ld ", buf.st_nlink);

            //printf("%d ",buf.st_uid);
            //printf("%d ",buf.st_gid);
            struct passwd *pw = getpwuid(buf.st_uid);
            struct group *gr = getgrgid(buf.st_gid);
            printf("%s %s ", pw->pw_name, gr->gr_name);
            printf("%ld", buf.st_size);
            char strnumber[50];
            int spaces = 8;

            sprintf(strnumber, "%ld", buf.st_size);
            int numlen;
            numlen = strlen(strnumber);
            spaces = spaces - numlen;
            //   printf("spaces %d ",spaces );
            for (int i = 0; i < spaces + 1; i++)
            {
                printf(" ");
            }
            if (spaces < 0)
            {
                printf("              ");
            }

            char *dtime = ctime(&buf.st_mtime);
            int len = strlen(dtime);
            dtime[len - 1] = '\0';
            printf("%s ", dtime);

            printf("%s\n", dirname);
        }

        closedir(dir);
    }

    else
    {
        DIR *dir;
        dir = opendir(".");
        if (dir == NULL)
        {
            printf("ERROR : Unable to open directory");
        }
        while ((sd = readdir(dir)) != NULL)
        {
            char dirname[1000];
            strcpy(dirname, sd->d_name);

            if (dirname[0] == dot[0])
            {
                continue;
            }
            printf("%s\n", dirname);
        }

        closedir(dir);
    }
}

void cds(char words[100][10000], int wordSize, char *cwd)
{

    int cdStat;

    char cds[] = "~";
    if (strcmp(words[1], "~") == 0)
    {
        // printf("here in tilda\n");
        cdStat = chdir(cwd);
        // printf("%d\n",cdStat);
        pathCount = 1;
        memset(directory, 0, sizeof(directory));
        for (int i = 0; i < pathCount; i++)
        {
            if (i != 0)
            {
                strcat(directory, "/");
            }
            strcat(directory, path[i]);
            // printf("path i %s",path[i]);
        }
        cdStat = 2;
    }

    else if (words[1][0] == cds[0])
    {
        //printf("here!succe\n");
        //directory= words[1];
        strcpy(directory, words[1]);
        char temparray[10000];
        int tempcount = 0;
        for (int i = 1; i < strlen(words[1]); i++)
        {
            temparray[tempcount] = words[1][i];
            tempcount++;
        }
        // printf("temparr %s\n",temparray);

        pathCount = 0;
        char delim[] = "/";
        char *ptr = strtok(words[1], delim);
        char tempcwd[10000];
        strcpy(tempcwd, cwd);

        while (ptr != NULL)
        {
            strcpy(path[pathCount], ptr);
            //  printf("path[%d] = %s\n",pathCount,ptr);
            pathCount++;
            ptr = strtok(NULL, delim);
        }
        strcat(tempcwd, temparray);
        cdStat = chdir(tempcwd);
        // printf("cd stat is %d\n",cdStat);
        if (cdStat != 0)
        {
            printf("ERROR Directory does not exist!");
        }
        cdStat = 2;
    }
    else
    {
        cdStat = chdir(words[1]);
        //printf("here assigning cdstat\n");
    }

    if (cdStat == 0)
    {

        if (strcmp(words[1], "..") == 0)
        {
            if (pathCount == 1)
            {
                printf("Can't go above home directory!");
            }
            else
            {
                pathCount--;
                memset(directory, 0, sizeof(directory));

                for (int i = 0; i < pathCount; i++)
                {
                    if (i != 0)
                    {
                        strcat(directory, "/");
                    }
                    strcat(directory, path[i]);
                }
            }
        }

        else if ((strcmp(words[1], ".") == 0))
        {
            int x = 0;
            //       printf("here in. \n");
        }

        else
        {
            //         printf("here in else\n");
            strcat(directory, "/");
            strcat(directory, words[1]);
            //printf("%s\n",words[1]);
            char delim[] = "/";
            char *ptr = strtok(words[1], delim);
            while (ptr != NULL)
            {
                strcpy(path[pathCount], ptr);
                //  printf("path[%d] = %s\n",pathCount,ptr);
                pathCount++;
                ptr = strtok(NULL, delim);
            }
        }
    }
    else if (cdStat == 2)
    {
        int qfas = 1;
        //       printf("here in cd stat 2\n");
    }
    else
    {
        printf("ERROR : Directory does not exist!");
    }
}

void seten(char words[100][10000], int wordSize)
{
    if (wordSize > 3)
    {
        printf("ERROR : More than two arguments");
    }
    else if (wordSize < 1)
    {
        printf("ERROR : Not enough arguments");
    }
    else
    {
        if (wordSize == 2)
        {
            setenv(words[1], "", 1);
            printf("Success! Environment variable %s set up as empty string since no input given\n", words[1]);
        }
        else
        {
            setenv(words[1], words[2], 1);
            printf("Success ! Environment variable %s set up as %s", words[1], words[2]);
        }
    }
}

void unseten(char words[100][10000], int wordSize)
{
    if (wordSize != 2)
    {
        printf("ERROR : incorrect amount of arguments");
    }
    else
    {
        int stat = unsetenv(words[1]);
        if (stat == -1)
        {
            perror("unsetenv error");
        }
        else
        {
            printf("Environment Variable %s is removed", words[1]);
        }
    }
}

void jobs(char words[100][10000], int wordSize)
{
    //printf("%d",backnum);
    int valid = 0;
    int count = 1;
    for (int i = 0; i < backnum; i++)
    {
        if (bg[i].id != -1)
        {
            valid = 1;
            char pname[100] = "/proc/";
            char path2[200], temp2[100];
            sprintf(temp2, "%d", bg[i].id);
            strcat(pname, temp2);
            strcat(pname, "/stat");
            char result[1000] = "";
            int handle = open(pname, O_RDONLY);
            char info[5000];
            read(handle, info, 5000);
            int j = 0;
            char *token = (char *)malloc(5000 * sizeof(char));
            char **tokenset = (char **)malloc(5000 * sizeof(char *));

            token = strtok(info, " ");
            while (token != NULL)
            {
                tokenset[j] = token;
                ++j;
                token = strtok(NULL, " ");
            }
            if (strcmp(tokenset[2], "T") == 0)
            {
                printf("[%d] %s %s [%d]\n", count, "Stopped", bg[i].bgname, bg[i].id);
            }
            else
            {
                printf("[%d] %s %s [%d]\n", count, "Running", bg[i].bgname, bg[i].id);
            }
            count++;
        }
    }
    if (valid == 0)
    {
        printf("No background jobs\n");
    }
}

void kjob(char words[100][10000], int wordSize)
{
    int id = atoi(words[1]);
    int signal = atoi(words[2]);
    int valid = 0;
    int count = 0;
    for (int i = 0; i < backnum; i++)
    {
        if (bg[i].id != -1)
        {
            count++;
            if (id == count)
            {
                int temp = kill(bg[i].id, signal);
                if (temp == -1)
                {
                    perror("ERROR : kjob failed!");
                    break;
                }

                int stat;
                valid = 1;
                while (waitpid(-1, &stat, WNOHANG) > 0)
                    ;
                fprintf(stderr, "Process %d killed with signal %d", bg[i].id, signal);
                if (signal == 9)
                {
                    bg[i].id = -1;
                }
                break;
            }
        }
    }
    if (valid == 0)
    {
        printf("ERROR : process not found !\n");
    }
}

void fg(char words[100][10000], int wordSize)
{
    int arg1 = atoi(words[1]);
    int enter = 0;
    int cid = 0;
    for (int i = 0; i < backnum; i++)
    {
        if (bg[i].id != -1)
        {
            cid++;
            if (arg1 == cid)
            {
                int temp = kill(bg[i].id, SIGCONT);
                printf("Process %d moved to foreground!\n", bg[i].id);
                int stat;
                currprocid = bg[i].id;
                bg[i].id = -1;
                strcpy(currprocname, bg[i].bgname);
                enter = 1;
                waitpid(currprocid, &stat, WUNTRACED);
                currprocid = -1;
                sprintf(currprocname, "");
                break;
            }
        }
    }
    if (enter = 0)
    {
        printf("Process not found\n");
    }
}

void bground(char words[100][10000], int wordSize)
{
    int userin = atoi(words[1]);
    int enter = 0;
    int cid = 0;

    for (int i = 0; i < backnum; i++)
    {
        if (bg[i].id != -1)
        {
            cid++;
            if (cid == userin)
            {
                enter = 1;
                int temp = kill(bg[i].id, SIGCONT);
                if (temp == -1)
                {
                    printf("ERROR: Kill failed , check if process exits\n");
                }
                printf("Process %d moved to background\n", bg[i].id);
                break;
            }
        }
    }
    if (enter = 0)
    {
        printf("Process not found\n");
    }
}

void overkill()
{
    int enter = 0;
    int cid = 0;
    for (int i = 0; i < backnum; i++)
    {
        if (bg[i].id != -1)
        {
            cid++;
            fprintf(stderr, "Process %d killed !\n", bg[i].id);
            kill(bg[i].id, 9);
            bg[i].id = -1;
        }
    }
}

void handler(int signl)
{
    if (signl == SIGINT && currprocid >= 0)
    {
        kill(currprocid, 9);
        printf("Ctrl + C entered . Process %d terminated!", currprocid);
        strcpy(currprocname, "");
        currprocid = -1;
        signal(SIGINT, SIG_IGN);
    }
    else if (signl == SIGTSTP && currprocid >= 0)
    {
        printf("ctrl z entered!\n");
        int invalid = 0;
        for (int i = 0; i < backnum; i++)
        {
            if (bg[i].id == currprocid)
            {
                invalid = 1;
                printf("breaking!\n");
                break;
            }
        }
        if (invalid == 0)
        {
            bg[backnum].id = currprocid;
            sprintf(bg[backnum].bgname, "%s", currprocname);
            backnum++;
            printf("now in bg!\n");
        }

        currprocid = -1;
        kill(currprocid, SIGSTOP);
        strcpy(currprocname, "");
        signal(SIGSTOP, SIG_IGN);
    }
    fflush(stdout);
}

void lstrip(char *s)
{
    for (; *s != '\0'; ++s)
    {
        if (*s == 'x')
            *s = *(s + 1);
        else
            break;
    }
}

int main()
{
    strcpy(path[0], "~"); // starting from home directory ~
    int executed = 0;
    char *buf;
    buf = (char *)malloc(10 * sizeof(char));
    buf = getlogin();

    int run = 1;
    char hostname[1024];
    char input[1000000];
    hostname[1023] = '\0';
    gethostname(hostname, 1023);
    char words[100][10000]; // will hold each individual word in the input (array of strings)
    char commands[100][10000];
    unsigned int comSize = 0;
    unsigned int wordSize = 0; // will tell us the length of words array

    char buff[PATH_MAX]; //current working directory
    char *cwd;
    cwd = getcwd(buff, PATH_MAX);

    while (run == 1)
    {
        printf("\n");
        // resetting the words and commands array
        executed = 0;
        wordSize = 0;
        comSize = 0;
        memset(words, 0, sizeof(words));
        memset(commands, 0, sizeof(commands));
        //checking signals
        signal(SIGSTOP, handler);
        signal(SIGTSTP, handler);
        signal(SIGINT, handler);
        signal(SIGABRT, handler);

        //checking signal if any process has exited
        int statuses;
        int pidstat = waitpid(-1, &statuses, WNOHANG);
        while (pidstat > 0)
        {
            for (int i = 0; i < backnum; i++)
            {
                //if a background processes has exited
                if (bg[i].id == pidstat)
                {
                    bg[i].id = -1;
                    fprintf(stderr, "Process %d exited normally\n", pidstat);
                    break;
                }
            }
            pidstat = waitpid(-1, &statuses, WNOHANG);
        }

        printf("<%s@%s:%s>", buf, hostname, directory);
        fflush(stdin);
        scanf("%[^\n]%*c", input);

        size_t num;
        //getline(&input,&num,stdin);
        // we are tokenizing on smemi colons and storing each toekn in commands array

        char *com = strtok(input, ";");
        while (com != NULL)
        {
            strcpy(commands[comSize], com);
            comSize++;
            com = strtok(NULL, ";");
        }

        // we iterate over the command array and run each command one by one

        for (int iterator = 0; iterator < comSize; iterator++)
        {
            char presComm[10000]; // present command
            strcpy(presComm, commands[iterator]);
            char *inp = strtok(presComm, " ");
            while (inp != NULL)
            {
                if (strcmp(inp, " ") == 0)
                {
                    continue;
                }
                else
                {
                    strcpy(words[wordSize], inp);
                    wordSize++;
                }
                inp = strtok(NULL, " ");
            }
            strcpy(presComm, commands[iterator]);
            pipenow(presComm);
            braccount(presComm);
            if (pipenumber > 0)
            {
                int i;
                int countpipe = pipenumber;
                char commands[countpipe + 1][100];
                pipetoken(commands, presComm);
                pipe(pd);
                char *execution[1000];
                char *execution1[1000];
                 semaparse(commands[0], execution1);
                if (fork() == 0)
                {
                    dup2(pd[1], 1);
                    execvp(execution1[0], execution1);
                }
                else
                {
                    wait(NULL);
                }
                close(pd[1]);
                i = 0;
                while (i < pipenumber - 1)
                {
                    int in = pd[0];
                    pipe(pd);
                    semaparse(commands[i + 1], execution);
                    if (fork() == 0)
                    {
                     
                        dup2(in, 0);
                        close(in);
                        dup2(pd[1], 1);
                        execvp(execution[0], execution);
                    }
                    close(pd[1]);
                    i++;
                }
                in = pd[0];
                pipe(pd);
                char *somethingelse[1000];
                semaparse(commands[countpipe],somethingelse);
                if (fork() == 0)
                {
                    
                    dup2(in, 0);
                    close(in);
                    execvp(somethingelse[0], somethingelse);
                }
                close(pd[1]);
                close(pd[0]);
            }
            else if (braccount(presComm) > 0)
            {
                char copy[1000];
                strcpy(copy, presComm);
                redirect(copy);
            }
            // cd command

            else if (strcmp(words[0], "cd") == 0)
            {
                executed = 1;
                cds(words, wordSize, cwd);
            }

            else if (strcmp(words[0], "pwd") == 0)
            {
                executed = 1;
                // printf("%s",directory);
                pwds(directory);
            }

            else if (strcmp(words[0], "echo") == 0)
            {
                executed = 1;
                //printf("herenow");
                echos(words, wordSize);
            }

            else if (strcmp(words[0], "ls") == 0)
            {
                executed = 1;
                lss(words, wordSize);
            }

            else if (strcmp(words[0], "pinfo") == 0)
            {
                executed = 1;
                pinfos(words, wordSize);
            }

            else if (strcmp(words[0], "setenv") == 0)
            {
                executed = 1;
                seten(words, wordSize);
            }

            else if (strcmp(words[0], "unsetenv") == 0)
            {
                executed = 1;
                unseten(words, wordSize);
            }
            else if (strcmp(words[0], "jobs") == 0)
            {
                executed = 1;
                if (wordSize != 1)
                {
                    printf("ERROR : Invalid number of arguments!");
                }
                else
                {
                    jobs(words, wordSize);
                }
            }

            else if (strcmp(words[0], "kjob") == 0)
            {
                executed = 1;
                if (wordSize != 3)
                {
                    printf("ERROR: Invalid number of arguments !");
                    continue;
                }
                kjob(words, wordSize);
            }

            else if (strcmp(words[0], "fg") == 0)
            {
                executed = 1;
                if (wordSize != 2)
                {
                    printf("ERROR: Invalid number of arguments!");
                    continue;
                }
                fg(words, wordSize);
            }

            else if (strcmp(words[0], "bg") == 0)
            {
                executed = 1;
                if (wordSize != 2)
                {
                    printf("ERROR: Invalid number of arguments!");
                    continue;
                }
                bground(words, wordSize);
            }

            else if (strcmp(words[0], "overkill") == 0)
            {
                executed = 1;
                if (wordSize != 1)
                {
                    printf("ERROR: Overkill dosen't take arguments!");
                    continue;
                }
                overkill();
            }

            else if (strcmp(words[0], "quit") == 0)
            {
                if (wordSize != 1)
                {
                    printf("quit does not take any arguments!\n");
                    continue;
                }
                overkill();
                printf("Exiting shell!\n");
                exit(0);
            }
            else if (executed == 0)
            {

                execfun(words, wordSize);
            }
        }
    }
}