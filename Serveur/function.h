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
#include<time.h>

#define ERROR 	(-1)
#define PORT 5555
#define TAILLEMESSAGE 256
#define NBRECON 10
#define CODEADM "420"
#define CODEUSER "007"


void menuserver(){

    printf("\n---------------------------------------------------------");
    printf("\n\t\t\t>>-Menu Serveur-<<\n");
    printf("\n**  TC  ** Afficher les contacts.");
    printf("\n**  NC  ** Filtrer les contacts par leurs noms.");
    printf("\n**  VC  ** Filtrer les contacts par leurs villes.");
    printf("\n**  P   ** Mettre en pause le serveur.");
    printf("\n** EXIT ** Quitter le programme.");
    printf("\n---------------------------------------------------------");
    printf("\n>> Votre choix : ");

    }

/*Fonction Menu Admin*/
void menuA(int sClient, char MSG[700]){
    strcpy(MSG,"\n---------------------------------------------------------\n\t\t\t>>-Menu Administrateur-<<\n\n**   1  ** pour rechercher un contact.\n**   2  ** pour   un contact.\n**   3  ** pour modifier un contact.\n**   4  ** pour supprimer un contact.\n**   M  ** pour le manuel d'utilisation.\n**   S  ** pour changer de statut.\n** EXIT ** pour quitter le programme.\n---------------------------------------------------------\n>> Votre choix : ");
    write(sClient,MSG,strlen(MSG)+1);
}

/*Fonction Menu Utilisateur*/
void menuU(int sClient,char MSG[700]){
    strcpy(MSG,"\n---------------------------------------------------------\n\t\t\t>>-Menu Utilisateur-<<\n\n**   1  ** pour rechercher un contact.\n**   S  ** pour changer de statut.\n** EXIT ** pour quitter le programme.\n---------------------------------------------------------\n>> Votre choix : ");
    write(sClient,MSG,strlen(MSG)+1);
}


/*Fonction affiche l'adrresse du client connecte*/
void aff_addr(struct sockaddr_in addr){
   printf("\nL'adresse IP : %s dans le port : %d",(char *)inet_ntoa(addr.sin_addr.s_addr),ntohs(addr.sin_port));
}
/*Fonction affiche l'erreur de sortie*/
void afficheErrorSortie(char *nomFct) {
     printf("\nErreur dans %s -> %s ", nomFct, strerror(errno));
     exit(1);
}


    //////////////////////////////////////
    // Fonction liter tout les contacts //
    //////////////////////////////////////

       char * consulter_contacts_serv(void){
     FILE* f1;

//    char ligne[255];
    char * msg= calloc(1000,sizeof(char)); // pour que le variable soit reinitialise a 0 a chaque fois

   f1 = fopen("contacts.txt", "r");
    if (f1 == NULL)
    {
        printf("!! Erreur au niveau de l'ouverture du fichier des contacts !! \n");
    }
        else
           {
             while(!feof(f1))
             {
             fread(msg,1000,30,f1); //lire tout le fichier
              }
          }
            fclose(f1);

            printf("\n%s",msg);

  return msg;
}


    char * consulter_contacts(void){
     FILE* f1;

//    char ligne[255];
    char * msg= calloc(1000,sizeof(char)); // pour que le variable soit reinitialise a 0 a chaque fois

   f1 = fopen("contacts.txt", "r");
    if (f1 == NULL)
    {
        printf("!! Erreur au niveau de l'ouverture du fichier des contacts !! \n");
    }
        else
           {
             while(!feof(f1))
             {
             fread(msg,1000,30,f1); //lire tout le fichier
              }
          }
            fclose(f1);

  return msg;
}

     /////////////////////////////////////////////
     // Fonction Rechercher par premiere lettre //
    /////////////////////////////////////////////


  char *premierlettre(char first);
  char *par_ville(char ville[]);
void fonctionmenu(){


    char msgmenu[30];
    char msgfirst;
    char msgfirstv1[0];

    char ville[20];

    int i;
    char lsd[1000];
    char p[10];



          while(gets(msgmenu)){
            i=0;
             if(strcmp(msgmenu,"M")==0 ||strcmp(msgmenu,"m")==0){

                menuserver();continue;

            } else if(strcmp(msgmenu,"TC")==0 ||strcmp(msgmenu,"tc")==0){

               consulter_contacts_serv();
               printf("---------------------------------------------------------\n> ");continue;
            }

             else if(strcmp(msgmenu,"NC")==0 ||strcmp(msgmenu,"nc")==0){
                     i=0;
                    printf("\n>>Entrez la premiere lettre des contacts a filtrer :\n$> ");

                    while(i!=1){
                        if(gets(msgfirstv1)>0){
                            gets(msgfirstv1);
                            i++;
                        }else{
                            continue;
                        }

                    }
                    msgfirst=msgfirstv1[0];
                    strcpy(lsd,premierlettre(msgfirst));
                    printf("\n%s",lsd);
                    printf("\n---------------------------------------------------------\n> ");
                    break;

                }else if(strcmp(msgmenu,"vc")==0||strcmp(msgmenu,"VC")==0){
                        printf("\n>>Entrez la ville des contacts a filtrer :\n$> ");
                        strcpy(ville,"");
                        while(i!=1){
                            if(gets(ville)>0){
                                gets(ville);
                                i++;
                            }else{
                                continue;
                            }

                        }
                        strcpy(lsd,par_ville(ville));
                        printf("\n%s",lsd);
                        printf("---------------------------------------------------------\n> ");

                        break;


                }else if(strcmp(msgmenu,"p")==0 ||strcmp(msgmenu,"P")==0){

                    puts("Serveur en pause...");
                    printf("Tapez ** P ** pour reprendre le service...\n");
                    strcpy(p,"i");
                    do{
                        sleep(1);
                        scanf("%s",p);
                        }while (strcmp(p,"p") != 0 && strcmp(p,"P") !=0);
                      printf("Serveur en marche.\n");
                      printf("---------------------------------------------------------\n> ");break;


                }else if(strcmp(msgmenu,"d")==0 ||strcmp(msgmenu,"D")==0){

                    puts("Serveur en ecoute...");break;

                }else if (( !strcmp(msgmenu,"exit")) || ( !strcmp(msgmenu,"EXIT")) ){
                    exit(1);


                }else{

                    printf("\nChoix non disponible. Donnez un choix valide : ");
                    continue;}

        }

  }

   char * premierlettre(char first)
{
     char * msg= calloc(1000,sizeof(char));
     char tmp[120]="";
     FILE* f1 ;
     char ligne[100];
     char c;
     long b=0;
     int k=0;
    f1 = fopen("contacts.txt", "r");
    if (f1 == NULL)
    {
        printf("error au niveau d'ouvrire le fichier de contact !! \n");
    }else{
             while(!feof(f1)){
                c=fgetc(f1); //va lire le premier caractere de chaque ligne
                             if(c==first ){

                                           k++;
                                           b=fseek(f1,-1, SEEK_CUR  );
                                           fgets(ligne,120,f1);
                                           strcat(msg,ligne);
				         	       }else {
				         	            fgets(tmp,120,f1);  strcpy(tmp,"") ; //pour perdre le reste de ligne  d'un caractere qui nous interesse pas
				         	               continue;  }
				                    }
        if (k==0) printf("\nAucun nom de contact commence par cette lettre.");  }
        fclose(f1);
    return msg;
		   }



 char * par_ville(char ville[])
{
     FILE* f1 ;

     char * msg= calloc(800,sizeof(char));
     char * tmp= calloc(800,sizeof(char));


    char ligne[100];
    char tmp1[100];
//    long b;

    int i,k=0;


    f1 = fopen("contacts.txt", "r");
    if (f1 == NULL)
    {
        printf("error au niveau d'ouvrire le fichier de contact !! \n");
    }else{
             while(!feof(f1)){

                        fscanf(f1,"%s",ligne);           // on lit le nom
                        strcpy(tmp,ligne);
                        for( i=strlen(tmp);i<17;i++) strcat(tmp," "); // tabulation du nom

                        fscanf(f1,"%s",ligne);         // on lit le prenom
                        strcat(tmp,ligne);
                        for( i=strlen(tmp);i<32;i++) strcat(tmp," "); //tabulation du prenom

                        fscanf(f1,"%s",ligne);     // on lit le telephone
                        strcat(tmp,ligne);
                        for( i=strlen(tmp);i<44;i++) strcat(tmp," ");  // tabulation du telephone
                        fscanf(f1,"%s",ligne);      // on lit la ville

    if(strcmp(ligne,ville)==0 ){

                                    k++;

                                    strcat(tmp,ligne);
                                    for( i=strlen(tmp);i<56;i++) strcat(tmp," "); // tabulation du ville

                                    fgets(ligne,90,f1);   // lire le reste de la ligne
                                    strcat(tmp,ligne);
                                    strcat(msg,tmp);





                              }else {

                                             fgets(tmp1,90,f1); // faire perdre le reste de la ligne d'une ville qu'on cherche pas
                                             strcpy(tmp,"");
                                             continue;  }








				 }
				    if (k==0){ strcpy(msg,"Aucun contact dans cette ville\n");  }

		   }


                    fclose(f1);


 return msg;
}



    ////////////////////////////////////
    // Fonction Rechercher un contact //
    ////////////////////////////////////


char * consulter_one_contact(char nom_chercher[],char prenom_chercher[])
{    char * msg= calloc(700,sizeof(char));
     FILE* f1 ;


    char ligne[100]="";
    int k=0;

    f1 = fopen("contacts.txt", "r");
    if (f1 == NULL)
    {
        printf("\n !! Erreur au niveau de l'ouverture du fichier des contacts !! \n");
    }else{
             while(!feof(f1)){

			            fscanf(f1,"%s ",ligne);   //va lire mots par motss

    if(strcmp(ligne,nom_chercher)==0 ){
      		                         fscanf(f1,"%s",ligne);

			         		     if(strcmp(ligne,  prenom_chercher )==0){ k=3;
			         		     strcpy(msg,"\n");
			         		  	 strcat(msg,nom_chercher);
			         		  	 strcat(msg,"\t\t");
			         		  	 strcat(msg,prenom_chercher);
			         		  	 strcat(msg,"\t");
			         			 fgets(ligne,90,f1); //va lire jusqu'a la fin de la ligne
			         			 strcat(msg,ligne);
			         			 strcat(msg,"\n---------------------------------------------------------\n**   R  ** pour repeter la commande.\n**   0  ** pour revenir au Menu Principal.\n** EXIT ** pour quitter le programme.\n---------------------------------------------------------\n>>Votre choix : ");
                                   break;

				         	      }else {
				         	          continue;
                                  }

                                        }
				 }

		   }


                    fclose(f1);
        if(k==0) {  strcpy(msg,"\n Ce contact n'existe pas!\n---------------------------------------------------------\n**   R  ** pour repeter la commande.\n**   0  ** pour revenir au Menu Principal.\n** EXIT ** pour quitter le programme.\n---------------------------------------------------------\n>>Votre choix : ") ;

                  }


 return msg;
}



char * cat_word(char msg[]){
     char * str= calloc(30,sizeof(char));
     if(strlen(msg)>0 && strcmp(msg," ")!=0 ){

     /*Premier mot doit pointer sur le message et le reste sur Null*/
      char * first = strtok(msg," ");
      char * second =strtok(NULL," ");
      char * third =strtok(NULL," ");

      strcat( str ,first);
     if(second!=NULL){ strcat(str,"_"); strcat(str,second);
         if(third!=NULL){ strcat(str,"_"); strcat(str,third);
            }
      }
      return str;
      } else return msg;





 }

    ////////////////////////////////////
    // Fonction Enregitrer un contact //
    ////////////////////////////////////




void enregistrer_contact(char msg[])
{

    FILE *f1;
	   f1=fopen("contacts.txt","a"); //a pour n'est pas ecraser et lire ?
	    if (f1 == NULL)
    {
        printf("\n!! Erreur au niveau de l'ouverture du fichier des contacts !! \n");
    }
        else
           {
	          while(!feof(f1)){

	                       if(getc(f1)== EOF){
		                 	fprintf(f1,"%s\n",msg);
                            break;
	               }else{  printf("\n le contact n'est pas enregistré !!"); break; 	   }
	             }
	        }
	   fclose(f1);
}



    /////////////////////////////////
    // Fonction Ajouter un contact //
    /////////////////////////////////


char* ajouter_contact(int sClient){

    int longMessage;
    int i,j=0;
    //int n=0;
     char msgn1[100]="",msgp1[100]="",msgt1[100]="",msga1[100]="",msge1[100]="";
     char msger[300];

     char tmp[75];
     char nom[20],prenom[20];


char * msg= calloc(1000,sizeof(char));
char * msgf= calloc(1000,sizeof(char));

    strcpy(msgn1,"\n>>Entrez le nom du nouveau contact :\n$> ");
    strcpy(msgp1,">>Entrez le prenom du nouveau contact :\n$> ");
    strcpy(msgt1,">>Entrez le numero de telephone du nouveau contact :\n$> ");
    strcpy(msga1,">>Entrez la ville du nouveau contact :\n$> ");
    strcpy(msge1,">>Entrez l'e-mail du nouveau contact :\n$> ");
    strcpy(msger,"Ce contact existe deja!\n---------------------------------------------------------\n**   R  ** pour repeter la commande.\n**   0  ** pour revenir au Menu Principal.\n** EXIT ** pour quitter le programme.\n---------------------------------------------------------\n>>Votre choix : ");



strcpy(msg,"");


write(sClient,msgn1,200);

    while(j!=1){
            fonctionmenu();
            j=0;
            longMessage=0;
            longMessage=read(sClient,tmp,50);
            if(longMessage>0){          strcpy(nom,tmp);

                        strcpy(tmp,cat_word(tmp));  // si le nom est compose on le colle avec "_"
                for(i=strlen(tmp);i<17;i++){
                strcat(tmp," ");
                }j++;   } }




strcat(msg,tmp);


write(sClient,msgp1,200);

    while(j!=2){
        fonctionmenu();
            j=1;
            longMessage=0;
            longMessage=read(sClient,tmp,50);
            if(longMessage>0){                strcpy(prenom,tmp);
                 strcpy(tmp, cat_word(tmp));     // si le prenom est compose on le colle avec "_"
                for(i=strlen(tmp)+17;i<32;i++){
                strcat(tmp," ");
                }j++;   } }


strcat(msg,tmp);



write(sClient,msgt1,200);

    while(j!=3){
        fonctionmenu();
            j=2;
            longMessage=0;
            longMessage=read(sClient,tmp,50);
            if(longMessage>0){
                for(i=strlen(tmp);i<12;i++){
                strcat(tmp," ");
                }j++;   } }


strcat(msg,tmp);


write(sClient,msga1,200);

     while(j!=4){
         fonctionmenu();
            j=3;
            longMessage=0;
            longMessage=read(sClient,tmp,50);
            if(longMessage>0){
                strcpy(tmp, cat_word(tmp)); // si la ville est compose on le colle avec "_"
                for(i=strlen(tmp);i<13;i++){
                strcat(tmp," ");
                }j++;   } }


strcat(msg,tmp);


write(sClient,msge1,200);

    while(j!=5){
        fonctionmenu();
            j=4;
            longMessage=0;
            longMessage=read(sClient,tmp,50);
            if(longMessage>0){
                for(i=strlen(tmp);i<26;i++){
                strcat(tmp," ");
                }j++;   } }


strcat(msg,tmp);

if(strcmp (consulter_one_contact(nom,prenom),"\n Ce contact n'existe pas!\n---------------------------------------------------------\n**   R  ** pour repeter la commande.\n**   0  ** pour revenir au Menu Principal.\n** EXIT ** pour quitter le programme.\n---------------------------------------------------------\n>>Votre choix : ")==0)
 {
    enregistrer_contact(msg);
    strcat(msg,"\n Le contact a ete enregistre avec succes!\n---------------------------------------------------------\n**   R  ** pour repeter la commande.\n**   0  ** pour revenir au Menu Principal.\n** EXIT ** pour quitter le programme.\n---------------------------------------------------------\n>>Votre choix : ");
    write(sClient,msg,500);}
 else{ write(sClient,msger,500);}

 strcat(msgf,nom);
 strcat(msgf,"  ");
 strcat(msgf,prenom);

 return msgf;


}



    ////////////////////////////////////
    // Fonction Supprimer un contact //
    ////////////////////////////////////



char * supprimer_contact(char nom_chercher[],char prenom_chercher[])
   {
         FILE* f1;
        FILE* f2;

    char * mots= calloc(500,sizeof(char));
        char resteligne[100]="";
        char nom [90]="",tmp[50]="";
        int i; //tmp :variable temporaire
       //char * ligne[90]= malloc(sizeof(char)*700);
        int k=0;
       f1 = fopen("contacts.txt","r");
        if (f1 == NULL)
        {
            printf("\n Erreur au niveau de l'ouverture du fichier des contacts origine \n");
        }
       else {
           f2 = fopen("copie.txt", "w+");
            if (f2 == NULL)
            {
                printf("Erreur au niveau de l'ouverture du fichier copie");
            }
            else
               {
                 while(!feof(f1))
                 {
                     fscanf(f1,"%s ",mots); //va lire mots par motss
                       strcpy(nom,mots);   // il va stocker le nom dans un variable temporaire : nom
                         fscanf(f1,"%s ",mots)  ;

                         if(strcmp(nom,nom_chercher)!=0 || strcmp(mots,prenom_chercher )!=0){

                         if( strcmp(mots,nom )!=0 ){
                        for(i=strlen(nom);i<17;i++) { strcat(nom," ");   }

                         strcat(nom,mots);
                        for(i=strlen(nom);i<32;i++) {strcat(nom," ");  }


                             }  else {
                        fgets(tmp,90,f1)  ;
                         strcpy(nom,"");    }

                        strcpy(resteligne,"");

                         fgets(resteligne,90,f1);       	 //on va lire le reste de ligne
                        // strcat(nom,"\t ");
                         strcat(nom,resteligne);
                         strcat(nom,"");

                                           fprintf(f2,nom);

                       }else{   k=1; fgets(tmp,100,f1);  } //on va stocker le reste de ligne dans un var temporaire pour seulement bouger le curseur
                  }
                   fclose(f2);
              }
                fclose(f1);
           }
                remove("contacts.txt");
                rename("copie.txt","contacts.txt");

             if(k!=0){strcpy(mots,"\nLa suppression est terminee.\n---------------------------------------------------------\n**   R  ** pour repeter la commande.\n**   0  ** pour revenir au Menu Principal.\n** EXIT ** pour quitter le programme.\n---------------------------------------------------------\n>>Votre choix : ");
             }
             else { strcpy(mots,"\nLe contact n'existe pas - Ou il a deja ete supprime.\n---------------------------------------------------------\n**   R  ** pour repeter la commande.\n**   0  ** pour revenir au Menu Principal.\n** EXIT ** pour quitter le programme.\n---------------------------------------------------------\n>>Votre choix : "); }



     return mots ;
}




    ////////////////////////////////////
    //  Fonction Modifier un contact  //
    ////////////////////////////////////



char*  modifier_element_contact(char nom_chercher[],char prenom_chercher[],char newelement[],char operation[])
   {  FILE* f1 ;      int i;  	int k=0;   long b=0;
    char * msg=calloc(200,sizeof(char)) ;

    char mots[30],nom[30]="",tmp[100]="";

    f1 = fopen("contacts.txt", "r+");
    if (f1 == NULL)
    {
        printf("Error au niveau d'ouvrire le fichier de contact !! \n");
    }else{
             while(!feof(f1)){    char c;   strcpy(nom,"");
                                   c=fgetc(f1);  b=fseek(f1,-1, SEEK_CUR  );
                                     fscanf(f1,"%s",mots);
                                     strcat(nom,mots);
                                    c=fgetc(f1);   fscanf(f1,"%s",mots);
            if(strcmp(nom,nom_chercher)==0 ){
            if(strcmp(mots, prenom_chercher )==0){

                           if(strcmp(operation,"1") == 0){ //modifie nom
                                     b= fseek(f1,-strlen(mots) ,SEEK_CUR  );
                                     b= fseek(f1,-17 ,SEEK_CUR  );
                                    for( i=strlen(newelement);i<17;i++) strcat(newelement," ");//pour tabulation
                                    fprintf(f1,"%s",newelement);
			                           k=1; break;
                                  }else {

                           if(strcmp(operation,"2") == 0){ //modifie prenom
                                     b= fseek(f1,-17 ,SEEK_CUR  );
                                    b= fseek(f1,-strlen(mots) ,SEEK_CUR  );
                                    b= fseek(f1,+17 ,SEEK_CUR  );
			         	    		for( i=strlen(newelement);i<15;i++) strcat(newelement," ");
                                    fprintf(f1,"%s",newelement);
                                    k=1; break;

									 }else {

                            if(strcmp(operation,"3") == 0){ //modifie Number
                                     b= fseek(f1,-17 ,SEEK_CUR  );
                                    b= fseek(f1,-strlen(mots) ,SEEK_CUR  );
                                    b= fseek(f1,+32 ,SEEK_CUR  );
			         	    		for( i=strlen(newelement);i<12;i++) strcat(newelement," ");
                                    fprintf(f1,"%s",newelement);
                                    k=1; break;

									 }else {

                            if(strcmp(operation,"4") == 0){ //modifie ville
                                     b= fseek(f1,-17 ,SEEK_CUR  );
                                    b= fseek(f1,-strlen(mots) ,SEEK_CUR  );
                                    b= fseek(f1,+44 ,SEEK_CUR  );
			         	    		for( i=strlen(newelement);i<13;i++) strcat(newelement," ");
                                    fprintf(f1,"%s",newelement);
                                    k=1; break;

									 }else {

                            if(strcmp(operation,"5") == 0){ //modifie Email
                                    b= fseek(f1,-17 ,SEEK_CUR  );
                                    b= fseek(f1,-strlen(mots) ,SEEK_CUR  );
                                    b= fseek(f1,+57 ,SEEK_CUR  );
			         	    		for( i=strlen(newelement);i<26;i++) strcat(newelement," ");
                                    fprintf(f1,"%s",newelement);
                                    k=1; break;

									 }else {
                                            k=2;
                                                }

                                        }
                                        }
                                        }
                                        }
                                    }
                                 }else {  fgets(tmp,90,f1); continue; //on va stocker le reste de la ligne dans tmp pour ne le lire pas autre fois
                                 }
                            }
                  }
            fclose(f1);

           if(k==1){strcpy(msg,"La modification est terminee.\n");
		    }else{
		        if(k==0){ strcpy(msg,"le contact n'est pas modifier - Ou il n'existe pas . \n");
		          }else{  strcpy(msg,"Le choix est invalide.\n");  //goto modifier
		            }
		          }
    return msg;
}
void   saveoper(char msg[],struct sockaddr_in addr){

     time_t t;
     char msg0[100];


     FILE *f3;
     f3=fopen("Operations.txt","a");
     if (f3 == NULL)
    {
        printf("Error au niveau d'ouvrire le fichier d'operations !! \n");
    }
        else
           {
               time(&t);
               while(!feof(f3)){

	                       if(getc(f3)== EOF){
		                 	fprintf(f3,"\n%s",ctime(&t));
                            strcpy(msg0,"IP:");
                            strcpy(msg0,(char *)inet_ntoa(addr.sin_addr.s_addr));
                            fprintf(f3,"%s",msg0);
		                 	fprintf(f3,"\t%s",msg);

	                    	break;
	               }else{
	                         break; 	   }
	             }
	        }
	   fclose(f3);





 }







