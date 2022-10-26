#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>

#define ERROR 	(-1)
#define PORT 5555
#define TAILLEMESSAGE 256
#define IPADRESSE "127.0.0.1"

void aff_addr(struct sockaddr_in addr){
   printf("\nVotre adresse IP : %s connectee au port : %d\n",(char *)inet_ntoa(addr.sin_addr.s_addr),ntohs(addr.sin_port));
}

void afficheErrorSortie(char *nomFct) {
     printf("\nErreur dans %s -> %s ", nomFct, strerror(errno));
     exit(1);
}
int main(int nbrarg,char *arg[]){
     char msg[1000]="";
     int sClient,ret_fct;
	 struct sockaddr_in  addrServeur;
	 int longMessage, flg;
	 char msg1[1000]=""  ;



     /* Création du socket client */
     sClient=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
     if(sClient==ERROR){ afficheErrorSortie("socket()");  exit(1);}

	 /* Infos serveur */
	 addrServeur.sin_family = AF_INET;
     addrServeur.sin_port   = htons(PORT);

	 /* Pour tester l'application avec un serveur sur une machine en réseau
	    il faut mettre l'adresse ip du serveur ici */
     addrServeur.sin_addr.s_addr=inet_addr(IPADRESSE);

     /* Connection +  gestion d'erreur */
     ret_fct=connect(sClient,(struct sockaddr*)&addrServeur,sizeof(struct sockaddr));
     if(ret_fct==ERROR){ afficheErrorSortie("connect()");  exit(1);}

     printf("\nConnexion ...");
     aff_addr(addrServeur);

	/* Rendre le socket non bloquant */
    flg = fcntl(sClient,F_GETFL,0);
    fcntl(sClient,F_SETFL,flg | O_NONBLOCK);

 	/* rendre le clavier non bloquant */
    flg = fcntl(STDIN_FILENO,F_GETFL,0);
    fcntl(STDIN_FILENO,F_SETFL,flg | O_NONBLOCK);

    printf("\n\t\t\t-------------------------\n\t\t\t|\tBIENVENUE       |\n\t\t\t|      CHER CLIENT      |\n\t\t\t-------------------------\n\n\n");
    sleep(1);
    printf("\n>>Veuillez entrer le code d'acces:\n$> ");
    scanf("%s",msg);
	 while(1){


		if(gets(msg)>0){
		    strcpy(msg1," ");
            write(sClient,msg,strlen(msg)+1);
            strcpy(msg," ");
            longMessage = read(sClient,msg1,1000);
            printf("%s",msg1);
            strcpy(msg1," ");
		}

        longMessage=0;
		longMessage=read(sClient,msg,1000);
		if(longMessage>0){
		   printf("%s",msg);
		   if(!strcmp(msg,"exit") || !strcmp(msg,"EXIT"))break;
		}
	}

    /* libération  de la connection  */
    close(sClient);

return 0;
}
// main
