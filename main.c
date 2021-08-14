#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
//#include <fcntl.h>
#define MAX_WORD 10
#define MAX_CHAR 100



void AppendInFile (){

   FILE * fPtr;
   fPtr  = fopen("/home/abdalla/Desktop/SimpleShell/termination of childs.log", "a");

   if (fPtr == NULL)
    {
        printf("\nUnable to open file.\n");
        exit(0);
    }

    fprintf(fPtr,"\n");
    fprintf(fPtr,"Child process was terminated");

    fclose(fPtr);
}

void handler(int sig)
{
  AppendInFile();
}

void read_line(char line[]){

char buf[MAX_CHAR];
char *user = getenv("USER");
char hostname[1024];

gethostname(hostname, 1024);


do{

printf("\n");
printf(user);
printf("@");
printf(hostname);
printf(":~");
printf(getcwd(buf, sizeof(buf)));
printf(" $ ");

//printf("\n%s > ",getcwd(buf, sizeof(buf)));
fgets(line,MAX_CHAR,stdin);   // includes \n (enter)

}while (strcmp(line,"\n")==0);

line = strtok(line,"\n"); // remove \n


if (strcmp(line,"exit")==0){

exit(0);
}

}

int process_line(char *args[] , char line[]){

int i=0;
args[i]=strtok(line," ");


if (args[i]==NULL){

return 0;
}

 while(args[i] != NULL){

  i++;
  args[i]=strtok(NULL," ");

}
return i;

}

int read_parse_line(char* args[] , char line[]){

int lenOfargs;
read_line(line);

lenOfargs=process_line(args,line);
return lenOfargs;
}


int main()
{
 char *args[MAX_WORD];
 char line[MAX_CHAR];
 int run = 1;
 int lengthOfargs;
 int background;
 char  *gdir;
 char  *dir;
 char  *to;
 char buf[MAX_CHAR];


  signal(SIGCHLD, handler);


 while(run){

 lengthOfargs=read_parse_line(args,line);
 background = 0;

 if (strcmp(args[0],"exit")==0){
   exit(0);
 }

 if (strcmp(args[lengthOfargs-1], "&") == 0){

      args[lengthOfargs-1] = NULL;
      background =1;
    }

 if (!strcmp(args[0], "cd")){

   if (strcmp(args[1], "/")==0){
      chdir("/");
   }
   else{

    gdir = getcwd(buf, sizeof(buf));
    dir = strcat(gdir, "/");
    to = strcat(dir, args[1]);
    chdir(to);
   }

 }

 pid_t pid = fork();
 if (pid == 0){

 if (execvp(args[0],args) < 0) { // open the called program and exit the current program
        exit(1);
 }

 }
 else{

    if (!background){

      //waitpid(pid,0);
       wait(NULL);
     }

 }
}
    return 0;

}
