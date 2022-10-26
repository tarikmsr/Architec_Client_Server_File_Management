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
#define NBRECON 10

#define CODEADM "420" //Code de l'administrateur
#define CODEUSER "007" //Code de l'utilisateur

#include "function.h" //La libraire des fonctions




int main(int argc,char *argv[]){

	struct sockaddr_in addrServeur,addrSocket;
	int sClient,sServeur;
	int longAddr,ret_fct,longMessage=TAILLEMESSAGE;
	char msg[1000]="";
	int flg, i=0;

    char choix[10];
    char p[10];
	char msg1[200];

	char msga[1000]="";
	char msgu[600]="";

	char manuel[1000]="";

	char  lsd[1000]="";

	char nom_chercher[50];
    char prenom_chercher[50];

    char ville[20];

    char operation[3]="";
    char newelement[50]="";
    char * msgf= malloc(sizeof(char)*1000);
    char * msgs= malloc(sizeof(char)*1000);

    char msgmenu[30];

    char msgfirst;
    char msgfirstv1[0];






    pid_t childpid;


    //Creation du socket Serveur
	sServeur = socket(AF_INET,SOCK_STREAM, IPPROTO_TCP);
	if(sServeur==ERROR){
	    afficheErrorSortie("socket()");
	    exit(1);}

	addrServeur.sin_family = AF_INET;
	addrServeur.sin_port = htons(PORT);
	addrServeur.sin_addr.s_addr=0;

    //Bind
	ret_fct  = bind(sServeur,(struct sockaddr*)&addrServeur,sizeof(struct sockaddr_in));
	if(ret_fct==ERROR){
	    afficheErrorSortie("bind()");
	    exit(1);}


    //Ouverture du Serveur//

printf("\n\t\t\t\t-------------------------\n\t\t\t\t|\tBIENVENUE       |\n\t\t\t\t|      AU SERVEUR       |\n\t\t\t\t-------------------------\n\n\n");


        printf("\n**   D  ** Demarrer le service.");
		printf("\n**   M  ** Afficher le menu des commandes.");
		printf("\n** EXIT ** Quitter le serveur.");
        printf("\n---------------------------------------------------------");
		printf("\nVotre Choix : ");

// Boucle pour le serveur
while(1){
    fonctionmenu();
    break;
}




    //Connection avec le client
	ret_fct=listen(sServeur,NBRECON);
	if(ret_fct==ERROR){
	    afficheErrorSortie("listen()");
	    exit(1);}

	longAddr=sizeof(struct sockaddr_in);


	//BOUCLE INFINIE//

	while(1){

	sClient=accept(sServeur,(struct sockaddr *)&addrSocket,&longAddr);
	if(sClient==ERROR){
	    afficheErrorSortie("accept()");
	    exit(1);}

	printf("\nConnexion ...");
	aff_addr(addrSocket);
	printf("\n");

	/* rendre le socket non bloquant */
    flg = fcntl(sClient,F_GETFL,0);
    fcntl(sClient,F_SETFL,flg | O_NONBLOCK);

 	/* rendre le clavier non bloquant */
    flg = fcntl(STDIN_FILENO,F_GETFL,0);
   fcntl(STDIN_FILENO,F_SETFL,flg | O_NONBLOCK);


    if ((childpid = fork()) == 0 ) {  // On a utiliser fork() pour avoir un multi-client
		close(sServeur);
    sleep(1);


	printf("\n\t\tTapez ** M ** a n'importe quel moment pour afficher le menu.\n");



	/* boucle infinie */
    while(1){

        fonctionmenu();

        first:

		longMessage=read(sClient,msg,TAILLEMESSAGE+1);
		if(longMessage>0){

		    while(1){
		    fonctionmenu();

		        //////////////////////////////////
		        //       LE CAS D'UN ADMIN      //
		        //////////////////////////////////

		        if(strcmp(msg,CODEADM) == 0){

		        printf("\nL'administrateur vient de se connecter.\n");
		        saveoper("L'administrateur vient de se connecter.\n",addrSocket);
                menuA(sClient,msga);


		        while(1){
		            fonctionmenu();

                    longMessage=0;
                    longMessage=read(sClient,msg,TAILLEMESSAGE+1);
                    if(longMessage>0){

                        if (strcmp(msg,"1")==0){


                            repeata1:

                            strcpy(msg1,"\n\n>>Entrez le nom du contact que vous voulez rechercher:\n$> ");
                            write(sClient,msg1,200);

                                while(i!=1){
                                    fonctionmenu();

                                    i=0;
                                    longMessage=0;
                                    longMessage=read(sClient,msg,200);
                                  //  strcat()
                                    if(longMessage>0){

                                        strcpy(nom_chercher,msg);
                                        i++;
                                        }
                                }
                            strcpy(msg1,"\n>>Entrez le prenom du contact que vous voulez rechercher:\n$> ");
                            write(sClient,msg1,TAILLEMESSAGE);

                                while(i!=2){
                                    fonctionmenu();

                                    longMessage=0;
                                    longMessage=read(sClient,msg,TAILLEMESSAGE+1);
                                    if(longMessage>0){

                                        strcpy(prenom_chercher,msg);
                                         i++;
                                        }
                                }

                            strcpy(msgf,">420/1//L'administrateur a choisi de rechercher un contact: ");
                            strcat(msgf,nom_chercher);
                            strcat(msgf," ");
                            strcat(msgf,prenom_chercher);
                            strcat(msgf,"\n");
                            printf("%s",msgf);
                            saveoper(msgf,addrSocket);

                            strcpy(nom_chercher,cat_word(nom_chercher));
                            strcpy(prenom_chercher,cat_word(prenom_chercher));

                            strcpy(msg,consulter_one_contact(nom_chercher,prenom_chercher));
                            write (sClient,msg,600);




                            ///////////////////////
                            //Menu Apres Fonction//
                            ///////////////////////


                            while(1){
                             fonctionmenu();
                                longMessage=0;
                                longMessage=read(sClient,msga,TAILLEMESSAGE+1);
                                if(longMessage>0){

                                    if ( (strcmp(msga,"R")==0) || (strcmp(msga,"r")==0) ){

                                        goto repeata1;
                                        continue;
                                    }else if (strcmp(msga,"0")==0){

                                        menuA(sClient,msga);
                                        break;
                                    }else if ( (!strcmp(msga,"exit")) || (!strcmp(msga,"EXIT")) ){

                                        write(sClient,msga,strlen(msga)+1);
                                        exit(1);
                                    }else{

                                        (strcpy(msga,"\nChoix non disponible! Donnez un choix valide : "));
                                        write(sClient,msga,strlen(msga)+1);
                                        continue;
                                        }
                                    }
                                }



                        }else if (strcmp(msg,"2")==0){

                            repeata2:


                            msgf=ajouter_contact(sClient);

                            strcpy(msgs,">420/2//L'administrateur a choisi d'ajouter un contact: ");
                            strcat(msgs,msgf);
                            strcat(msgs,"\n");
                            printf("%s",msgs);
                            saveoper(msgs,addrSocket);



                            ///////////////////////
                            //Menu Apres Fonction//
                            ///////////////////////


                            while(1){
                                fonctionmenu();

                            longMessage=0;
                            longMessage=read(sClient,msga,TAILLEMESSAGE+1);
                            if(longMessage>0){

                                if ( (strcmp(msga,"R")==0) || (strcmp(msga,"r")==0) ){

                                    goto repeata2;
                                    continue;
                                }else if (strcmp(msga,"0")==0){

                                    menuA(sClient,msga);
                                    break;
                                }else if ( (!strcmp(msga,"exit")) || (!strcmp(msga,"EXIT")) ){

                                    write(sClient,msga,strlen(msga)+1);
                                    exit(1);
                                }else{

                                    (strcpy(msga,"\nChoix non disponible! Donnez un choix valide : "));
                                    write(sClient,msga,strlen(msga)+1);
                                    continue;
                                    }
                                }
                            }






                    }else if (strcmp(msg,"3")==0){

                        repeata3:

                        strcpy(msg1,"\n\n>>Entrez le nom du contact que vous voulez modifier:\n$> ");
                        write(sClient,msg1,200);

                        while(i!=1){
                            fonctionmenu();

                            i=0;
                            longMessage=0;
                            longMessage=read(sClient,msg,200);
                            if(longMessage>0){

                                strcpy(nom_chercher,msg);
                                i++;
                            }
                        }

                            strcpy(msg1,"\n>>Entrez le prenom du contact que vous voulez modifier:\n$> ");
                            write(sClient,msg1,TAILLEMESSAGE);

                        while(i!=2){
                            fonctionmenu();

                            longMessage=0;
                            longMessage=read(sClient,msg,TAILLEMESSAGE+1);
                            if(longMessage>0){

                                strcpy(prenom_chercher,msg);

                                i++;
                            }
                        }
                        strcpy(msgf,">420/3//L'administrateur a choisi de modifier un contact :");
                        strcat(msgf,nom_chercher);
                        strcat(msgf," ");
                        strcat(msgf,prenom_chercher);
                        strcat(msgf,"\n");
                        printf("%s",msgf);
                        saveoper(msgf,addrSocket);

                           strcpy(nom_chercher,cat_word(nom_chercher));
                           strcpy(prenom_chercher,cat_word(prenom_chercher));








                        if( strcmp( consulter_one_contact( nom_chercher,prenom_chercher ),"\n Ce contact n'existe pas!\n---------------------------------------------------------\n**   R  ** pour repeter la commande.\n**   0  ** pour revenir au Menu Principal.\n** EXIT ** pour quitter le programme.\n---------------------------------------------------------\n>>Votre choix : "  ) !=0 ){

                        strcpy(msg,"\n\n**   1  ** pour modifier le nom du contact.\n**   2  ** pour modifier le prenom du contact.\n**   3  ** pour modifier le numero de telephone du contact.\n**   4  ** pour modifier la ville du contact.\n**   5  ** pour modifier l'e-mail du contact.\n**   6  ** pour modifier tout les elements du contact.\n>> Votre choix: ");
                        write(sClient,msg,350);
                        while(1){
                            fonctionmenu();

                            longMessage=0;
                            longMessage=read(sClient,operation,TAILLEMESSAGE+1);
                            if(longMessage>0){

                                if(strcmp(operation,"1")== 0){

                                    strcpy(msg1,"\n\n>>Entrez le nouveau nom du contact :\n$> ");
                                    write(sClient,msg1,200);

                                    while(1){
                                        fonctionmenu();

                                        longMessage=0;
                                        longMessage=read(sClient,msg,TAILLEMESSAGE+1);
                                        if(longMessage>0){
                                            strcpy(msg,cat_word(msg));
                                            strcpy(newelement,msg);
                                            break;
                                            }
                                    }
                                    strcpy(msg,modifier_element_contact(nom_chercher, prenom_chercher, newelement, operation));
                                    strcat(msg,"\n---------------------------------------------------------\n**   R  ** pour repeter la commande.\n**   0  ** pour revenir au Menu Principal.\n** EXIT ** pour quitter le programme.\n---------------------------------------------------------\n>>Votre choix : ");
                                    write(sClient,msg,400);break;


                                }else if(strcmp(operation,"2")== 0){

                                    strcpy(msg1,"\n\n>>Entrez le nouveau prenom du contact :\n$> ");
                                    write(sClient,msg1,200);

                                    while(1){
                                        fonctionmenu();

                                        longMessage=0;
                                        longMessage=read(sClient,msg,TAILLEMESSAGE+1);
                                        if(longMessage>0){
                                            strcpy(msg,cat_word(msg));
                                            strcpy(newelement,msg);
                                            break;
                                            }
                                    }
                                    strcpy(msg,modifier_element_contact(nom_chercher, prenom_chercher, newelement, operation));
                                    strcat(msg,"\n---------------------------------------------------------\n**   R  ** pour repeter la commande.\n**   0  ** pour revenir au Menu Principal.\n** EXIT ** pour quitter le programme.\n---------------------------------------------------------\n>>Votre choix : ");
                                    write(sClient,msg,400);break;

                                }else if(strcmp(operation,"3")== 0){

                                    strcpy(msg1,"\n\n>>Entrez le nouveau numero de telephone du contact :\n$> ");
                                    write(sClient,msg1,200);

                                    while(1){
                                        fonctionmenu();

                                        longMessage=0;
                                        longMessage=read(sClient,msg,TAILLEMESSAGE+1);
                                        if(longMessage>0){
                                            strcpy(newelement,msg);
                                            break;
                                            }
                                        }
                                    strcpy(msg,modifier_element_contact(nom_chercher, prenom_chercher, newelement, operation));
                                    strcat(msg,"\n---------------------------------------------------------\n**   R  ** pour repeter la commande.\n**   0  ** pour revenir au Menu Principal.\n** EXIT ** pour quitter le programme.\n---------------------------------------------------------\n>>Votre choix : ");
                                    write(sClient,msg,400);break;

                                }else if(strcmp(operation,"4")== 0){

                                    strcpy(msg1,"\n\n>>Entrez la nouvelle ville  du contact :\n$> ");
                                    write(sClient,msg1,200);

                                    while(1){
                                        fonctionmenu();

                                        longMessage=0;
                                        longMessage=read(sClient,msg,TAILLEMESSAGE+1);
                                        if(longMessage>0){
                                            strcpy(newelement,msg);
                                            break;
                                            }
                                    }
                                    strcpy(msg,modifier_element_contact(nom_chercher, prenom_chercher, newelement, operation));
                                    strcat(msg,"\n---------------------------------------------------------\n**   R  ** pour repeter la commande.\n**   0  ** pour revenir au Menu Principal.\n** EXIT ** pour quitter le programme.\n---------------------------------------------------------\n>>Votre choix : ");
                                    write(sClient,msg,400);break;


                                }else if(strcmp(operation,"5")== 0){

                                    strcpy(msg1,"\n\n>>Entrez le nouveau e-mail du contact :\n$> ");
                                    write(sClient,msg1,200);

                                    while(1){
                                        fonctionmenu();

                                        longMessage=0;
                                        longMessage=read(sClient,msg,TAILLEMESSAGE+1);
                                        if(longMessage>0){
                                            strcpy(newelement,msg);
                                            break;
                                            }
                                    }
                                    strcpy(msg,modifier_element_contact(nom_chercher, prenom_chercher, newelement, operation));
                                    strcat(msg,"\n---------------------------------------------------------\n**   R  ** pour repeter la commande.\n**   0  ** pour revenir au Menu Principal.\n** EXIT ** pour quitter le programme.\n---------------------------------------------------------\n>>Votre choix : ");
                                    write(sClient,msg,400);break;



                                }else if(strcmp(operation,"6")== 0){

                                    while(1){
                                        fonctionmenu();
                                        i=0;
                                        strcpy(msg, supprimer_contact(nom_chercher,prenom_chercher));
                                        if (strlen(msg)== strlen( "\nLa suppression est terminee.\n---------------------------------------------------------\n**   R  ** pour repeter la commande.\n**   0  ** pour revenir au Menu Principal.\n** EXIT ** pour quitter le programme.\n---------------------------------------------------------\n>>Votre choix : " )){
                                            strcpy(msg,"");
                                            ajouter_contact(sClient);
                                            break ;
                                        }else{
                                            continue;
                                            i++;	if(i==1) break;
                                            }
                                         }

                                    write(sClient,msg,400);break;

                                    }else {
                                            strcpy(msg,"Choix non disponible. Donnez un choix valide : ");
                                            write(sClient,msg,200);
                                            continue;
                                        }
                                }
                            }


                         }else{
                             strcpy(msg,"\n!! Le contact n'existe pas !!\n---------------------------------------------------------\n**   R  ** pour repeter la commande.\n**   0  ** pour revenir au Menu Principal.\n** EXIT ** pour quitter le programme.\n---------------------------------------------------------\n>>Votre choix : ");
                             write(sClient,msg,400);
                         }



                            ///////////////////////
                            //Menu Apres Fonction//
                            ///////////////////////


                            while(1){
                                fonctionmenu();

                            longMessage=0;
                            longMessage=read(sClient,msga,TAILLEMESSAGE+1);
                            if(longMessage>0){

                                if ( (strcmp(msga,"R")==0) || (strcmp(msga,"r")==0) ){

                                    goto repeata3;
                                    continue;
                                }else if (strcmp(msga,"0")==0){

                                    menuA(sClient,msga);
                                    break;
                                }else if ( (!strcmp(msga,"exit")) || (!strcmp(msga,"EXIT")) ){

                                    write(sClient,msga,strlen(msga)+1);
                                    exit(1);
                                }else{

                                    (strcpy(msga,"\nChoix non disponible! Donnez un choix valide : "));
                                    write(sClient,msga,strlen(msga)+1);
                                    continue;
                                    }
                                }
                            }




                    }else if(strcmp(msg,"4")==0){

                        repeata4:

                        strcpy(msg1,"\n\n>> Entrez le nom du contact que vous voulez supprimer :\n$> ");
                        write(sClient,msg1,200);

                        while(i!=1){
                            fonctionmenu();

                            i=0;
                            longMessage=0;
                            longMessage=read(sClient,msg,100);
                            if(longMessage>0){

                                strcpy(nom_chercher,msg);
                                i++;
                            }
                        }

                        strcpy(msg1,"\n>> Entrez le prenom du contact que vous voulez supprimer :\n$> ");
                        write(sClient,msg1,TAILLEMESSAGE);

                        while(i!=2){
                            fonctionmenu();

                            longMessage=0;
                            longMessage=read(sClient,msg,TAILLEMESSAGE+1);
                            if(longMessage>0){

                                strcpy(prenom_chercher,msg);

                                i++;
                            }
                        }

                        strcpy(msgf,">420/4//L'administrateur a choisi de supprmier un contact :");
                        strcat(msgf,nom_chercher);
                        strcat(msgf," ");
                        strcat(msgf,prenom_chercher);
                        strcat(msgf,"\n");
                        printf("%s",
                               msgf);
                        saveoper(msgf,addrSocket);
                        strcpy(nom_chercher, cat_word(nom_chercher));// si nom compose le coller avec _
                        strcpy(prenom_chercher, cat_word(prenom_chercher));// si prenom composer le coller avec _



                        strcpy(msg,supprimer_contact(nom_chercher,prenom_chercher));
                        write (sClient,msg,500);



                            ///////////////////////
                            //Menu Apres Fonction//
                            ///////////////////////


                            while(1){
                                fonctionmenu();

                            longMessage=0;
                            longMessage=read(sClient,msga,TAILLEMESSAGE+1);
                            if(longMessage>0){

                                if ( (strcmp(msga,"R")==0) || (strcmp(msga,"r")==0) ){

                                    goto repeata4;
                                    continue;
                                }else if (strcmp(msga,"0")==0){

                                    menuA(sClient,msga);
                                    break;
                                }else if ( (!strcmp(msga,"exit")) || (!strcmp(msga,"EXIT")) ){

                                    write(sClient,msga,strlen(msga)+1);
                                    exit(1);
                                }else{

                                    (strcpy(msga,"\nChoix non disponible! Donnez un choix valide : "));
                                    write(sClient,msga,strlen(msga)+1);
                                    continue;
                                    }
                                }
                            }




                    }else if ( (strcmp(msg,"M")==0) || (strcmp(msg,"m")==0) ){

                        puts(">420/M//L'administrateur a choisi d'afficher le manuel d'utilisation.");
                        saveoper(">420/M//L'administrateur a choisi d'afficher le manuel d'utilisation.\n",addrSocket);
                        strcpy(manuel,"\n------------------------------> Manuel d'utilisation <-----------------------------\n|\t\t\t\t\t\t\t\t\t\t  |\n|* Faites attention au majuscules lors de rechercher/supprimer/ajouter un contact.|\n|* Lors de l'ajout d'un contact. Il est preferable de ne pas depasser :\t\t  |\n|\t- 17 caracteres pour le nom.\t\t\t\t\t\t  |\n|\t- 15 caracteres pour le prenom.\t\t\t\t\t\t  |\n|\t- 12 caracteres pour le numero de telepone.\t\t\t\t  |\n|\t- 13 caracteres pour la ville.\t\t\t\t\t\t  |\n|\t- 26 caracteres pour l'adresse e-mail.\t\t\t\t\t  |\n| Cela pour eviter des problemes de tabulation.\t\t\t\t\t  |\n| Ce programme a ete cree par Amine Zeggaf, Tarik El Msaouri, Yousra Berdey      |\n| et Amine Medaghri Alaoui.\t\t\t\t\t\t\t  |\n|\t\t\t\t\t\t\tEncarde par M. Ait Kbir   |\n-----------------------------------------------------------------------------------\nPour revenir au menu, appuyez sur la touche * ENTRER *.\n");
                        write(sClient,manuel,strlen(manuel)+1);

                        while(1){
                            fonctionmenu();

                            longMessage=0;
                            longMessage=read(sClient,msg1,TAILLEMESSAGE+1);
                            if(longMessage>0){
                                menuA(sClient, msga);
                                break;
                            }

                        }

                    }else if ( (strcmp(msg,"S")==0) || (strcmp(msg,"s")==0) ){

                        puts(">420/S//L'administrateur a choisi de changer de statut.");
                        saveoper(">420/S//L'administrateur a choisi de changer de statut.\n",addrSocket);
                        strcpy(msg,"\n>>Donnez le code d'acces :\n$>  ");
                        write(sClient,msg,200);
                        goto first;


                    }else if ( (!strcmp(msg,"exit")) || (!strcmp(msg,"EXIT")) ){

                        write(sClient,msg,strlen(msg)+1);
                        exit(1);

                    }else {(strcpy(msg1,"Choix non disponible. Donnez un choix valide : "));
                        write(sClient,msg1,strlen(msg1)+1);
                        continue;
                    }
                }
            }





		        ////////////////////////////////////////
		        //       LE CAS D'UN UTILISATEUR      //
		        ////////////////////////////////////////






            }else if(strcmp(msg,CODEUSER) == 0){
                printf("\nL'utilisateur vient de se connecter.\n");
                saveoper("L'utilisateur vient de se connecter.\n",addrSocket);
                menuU(sClient,msgu);

		        while(1){
		            fonctionmenu();

                longMessage=0;
		        longMessage=read(sClient,msg,TAILLEMESSAGE+1);
		        if(longMessage>0){
                    if (strcmp(msg,"1")==0){

                        repeatu1:

                        strcpy(msg1,"\n\n>>Entrez le nom du contact que vous voulez rechercher:\n$> ");
                        write(sClient,msg1,200);

                        while(i!=1){
                         fonctionmenu();
                            i=0;
                            longMessage=0;
                            longMessage=read(sClient,msg,200);
                            if(longMessage>0){

                                strcpy(nom_chercher,msg);
                                i++;
                            }
                        }
                        strcpy(msg1,"\n>>Entrez le prenom du contact que vous voulez rechercher:\n$> ");
                        write(sClient,msg1,TAILLEMESSAGE);

                        while(i!=2){
                            fonctionmenu();

                            longMessage=0;
                            longMessage=read(sClient,msg,TAILLEMESSAGE+1);
                            if(longMessage>0){

                                strcpy(prenom_chercher,msg);
                                i++;

                            }
                        }

                           strcpy(msgf,">007/1//L'utilisateur a choisi de rechercher un contact : ");
                           strcat(msgf,nom_chercher);
                           strcat(msgf," ");
                           strcat(msgf,prenom_chercher);
                           strcat(msgf,"\n");

                           printf("%s",msgf);
                           saveoper(msgf,addrSocket);

                           strcpy(nom_chercher,cat_word(nom_chercher));
                           strcpy(prenom_chercher,cat_word(prenom_chercher));

                        strcpy(msg,consulter_one_contact(nom_chercher,prenom_chercher));
                       // strcat(msg,"\n---------------------------------------------------------\n**   R  ** pour repeter la commande.\n**   0  ** pour revenir au Menu Principal.\n** EXIT ** pour quitter le programme.\n---------------------------------------------------------\n>>Votre choix : ");
                        write (sClient,msg,600);


                            ///////////////////////
                            //Menu Apres Fonction//
                            ///////////////////////

                        while(1){
                        fonctionmenu();
                        longMessage=0;
                        longMessage=read(sClient,msgu,TAILLEMESSAGE+1);
                        if(longMessage>0){

                            if ( (strcmp(msgu,"R")==0) || (strcmp(msgu,"r")==0) ){

                                goto repeatu1;
                                continue;
                                }else if (strcmp(msgu,"0")==0){

                                menuU(sClient,msgu);
                                break;
                                }else if ( (!strcmp(msgu,"exit")) || (!strcmp(msgu,"EXIT")) ){

                                    write(sClient,msgu,strlen(msgu)+1);
                                    exit(1);
                                }else{

                                    (strcpy(msgu,"\nChoix non disponible! Donnez un choix valide: "));
                                    write(sClient,msgu,strlen(msgu)+1);
                                    continue;
                                    }
                                }
                            }



                    }else if ( (strcmp(msg,"S")==0) || (strcmp(msg,"s")==0) ){

                        puts(">420/S//L'utilisateur a choisi de changer de statut.");
                        saveoper(">420/S//L'utilisateur a choisi de changer de statut.\n",addrSocket);
                        strcpy(msg,"\n>>Donnez le code d'acces : \n$> ");
                        write(sClient,msg,200);
                        goto first;




                    }else if ( (!strcmp(msg,"exit")) || (!strcmp(msg,"EXIT")) ){
                        write(sClient,msg,strlen(msg)+1);
                        exit(1);

                    }else{
                        (strcpy(msg1,"Choix non disponible. Donnez un choix valide : "));
                        write(sClient,msg1,strlen(msg1)+1);
                        menuU(sClient,msgu);continue;
                    }
                 }
                }
		        }else{
                strcpy(msg1,"\nCode Invalide. Veulliez ressayer :\n$> ");
                write(sClient,msg1,strlen(msg1)+1);}
                break;
                    }

 		   printf("\nLe client a tape : %s  \n",msg);

		}

    }



    }

	}




	close(sServeur);
	close(sClient);
	return 0;
}
