#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <wait.h>
#include <time.h>

#define MUNKARA_FEL SIGRTMIN
#define SIKERES_MINTAVETEL SIGRTMIN

struct Patient {
    char name[30];
    char address[30];
    int taj;
    char test[20];
    char type;
};

struct Address {
    char address[30];
};

void makePlace() {
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
}

void handler(int signumber){
    //printf("Signal with number %i has arrived\n",signumber);
}

int main(int argc, char *argv[]) {

    int pipefd1[2], pipefd2[2];
    pid_t child1, child2;

    if(pipe(pipefd1) == -1) {
        perror("Hiba a pipe nyitaskor!");
        exit(EXIT_FAILURE);
    }

    if(pipe(pipefd2) == -1) {
        perror("Hiba a pipe nyitaskor!");
        exit(EXIT_FAILURE);
    }

    child1 = fork();

    if(child1 == -1) {
        perror("Fork hiba");
        exit(EXIT_FAILURE);
    }

    if(child1 > 0) {
        child2 = fork();

        if(child2 == -1) {
            perror("Fork hiba");
            exit(EXIT_FAILURE);
        }

        if(child2 > 0) {
            /* Parent code */
            makePlace();

            char n1[15], n2[15];
            int quantity;

            int c1; //Modositas tipusa
            int c2; //Express
            char c3[20]; //Valasztott teszt
            int id, exist, found;

            FILE * fp = fopen ("data.txt", "r");

            fscanf(fp, "%d", &quantity);

            struct Patient* temp = malloc(quantity * sizeof(struct Patient));
            struct Address* addresses = malloc(sizeof(struct Address));
            int numofadd = 1; //számolja a különböző címek számát
            bool filter; //szűri az ismétlődő címeket

            for(int i = 0; i < quantity; i++) {
                fscanf(fp, "%s %s %s %d %s %c", &n1, &n2, &temp[i].address , &temp[i].taj, &temp[i].test, &temp[i].type);
                strcat(n1, " ");
                strcat(n1, n2);
                strcpy(temp[i].name, n1);
                if(i == 0) {
                    strcpy(addresses[0].address, temp[i].address);
                }
                else {
                    filter = true;
                    for(int j = 0; j < numofadd; j++) {
                        if(strcmp(temp[i].address, addresses[j].address) == 0) {
                            filter = false;
                            break;
                        }
                    }
                    if(filter) {
                        ++numofadd;
                        addresses = (struct Address *) realloc(addresses, (numofadd) * sizeof(struct Address));
                        strcpy(addresses[numofadd - 1].address, temp[i].address);
                    }
                }
            }

            fclose(fp);

            //-----------------------------------------------------------------------------------------------

            struct Patient* ptr0 = malloc(quantity * sizeof(struct Patient));

            //tömb rendezés az addresses segítségével

            int fill = 0;
            for(int i = 0; i < numofadd; i++) {
                for(int j = 0; j < quantity; j++) {
                    if(strcmp(temp[j].address, addresses[i].address) == 0) {
                        ++fill;
                        strcpy(ptr0[fill - 1].name, temp[j].name);
                        strcpy(ptr0[fill - 1].address, temp[j].address);
                        ptr0[fill - 1].taj = temp[j].taj;
                        strcpy(ptr0[fill - 1].test, temp[j].test);
                        ptr0[fill - 1].type = temp[j].type;
                    }
                }
            }

            free(addresses);
            free(temp);

            int ready_vehicles = 0;

            signal(MUNKARA_FEL, handler);
			
			sigset_t mask, oldmask;
			sigemptyset(&mask);
			sigaddset(&mask, MUNKARA_FEL);
			sigprocmask(SIG_BLOCK, &mask, &oldmask);

			while(ready_vehicles < 2) {
				sigsuspend(&oldmask);
				++ready_vehicles;
			}
            
			sigprocmask(SIG_UNBLOCK, &mask, NULL);

            close(pipefd1[0]);
            close(pipefd2[0]);

            int places = 0; //meghatározza hányadik különböző helyre megy a mentőautó
            const int keep = 1;
            const int stop = 0;
            int tasks = 0;

            for(int i = 0; i <= quantity; i++) {
                if(i == quantity) {
                    printf("\nKiosztott feladatok: %d\n", tasks);
                    write(pipefd1[1], &stop, sizeof(int));
                    write(pipefd2[1], &stop, sizeof(int));
                    break;
                }
                else {
                    if(i == 0) {
                        ++places;
                        ++tasks;
                        write(pipefd2[1], &keep, sizeof(int));
                        write(pipefd2[1], &ptr0[i], sizeof(struct Patient)); 
                    }
                    else {
                        if(strcmp(ptr0[i - 1].address, ptr0[i].address) != 0) {
                            ++places;
                        }

                        if(places <= 3) {
                            ++tasks;
                            write(pipefd2[1], &keep, sizeof(int));
                            write(pipefd2[1], &ptr0[i], sizeof(struct Patient));                    
                        }
                        else if(places <= 6) {
                            ++tasks;
                            write(pipefd1[1], &keep, sizeof(int));
                            write(pipefd1[1], &ptr0[i], sizeof(struct Patient));
                        }
                        else {
                            write(pipefd1[1], &stop, sizeof(int));
                            write(pipefd2[1], &stop, sizeof(int));
                            printf("\nKiosztott feladatok: %d\n", tasks);
                            printf("Nincs eleg kapacitas!\n");
                            break;
                        }
                    }
                }
            }

            close(pipefd1[1]);
            close(pipefd2[1]);

            int done_vehicles = 0;

            signal(SIKERES_MINTAVETEL, handler);
			
			sigemptyset(&mask);
			sigaddset(&mask, SIKERES_MINTAVETEL);
			sigprocmask(SIG_BLOCK, &mask, &oldmask);

			while(done_vehicles < 2) {
				sigsuspend(&oldmask);
				++done_vehicles;
			}

            FILE * ff = fopen ("data.txt", "w");
            fprintf(ff, "%d\n", (quantity - tasks));
            
            for(int i = tasks; i < quantity; i++) {
                fprintf(ff, "%s %s %d %s %c\n", ptr0[i].name, ptr0[i].address ,ptr0[i].taj, ptr0[i].test, ptr0[i].type);
            }

            fclose(ff);
            free(ptr0);
            
            struct Patient* ptr = malloc(quantity * sizeof(struct Patient));

            FILE * fn = fopen ("data.txt", "r");
            fscanf(fn, "%d", &quantity);
            for(int i = 0; i < quantity; i++) {
                fscanf(fn, "%s %s %s %d %s %c", &n1, &n2, &ptr[i].address , &ptr[i].taj, &ptr[i].test, &ptr[i].type);
                strcat(n1, " ");
                strcat(n1, n2);
                strcpy(ptr[i].name, n1);
            }

            fclose(fn);

            //-----------------------------------------------------------------------------------------------

            do {
                printf("\nModositasi lehetosegek:\n1. Regisztracio vizsgalatra\n2. Vizsgalat lemondasa\n3. Vizsgalat szerkesztese\n4. Teljes adatbazis listazasa\n5. Szures vizsgalat alapjan\n6. Kilepes\nSorszam: ");
                scanf("%d", &c1);

                if(c1 > 6 || c1 < 1) {
                    makePlace();
                    printf("Ilyen opcio nem letezik!\n\n");
                }
                else if(c1 < 4){
                    makePlace();
                    printf("TAJ szam: ");
                    scanf("%d", &id);

                    exist = 0;
                    for(int i = 0; i < quantity; i++) {
                        if(ptr[i].taj == id) {
                            exist = 1;
                        }
                    }

                    if((c1 == 2 || c1 == 3) && exist == 0) {
                        makePlace();
                        printf("A paciens nincs regisztralva!\n\n");
                    }
                    else if(c1 == 1) { //Regisztracio
                        makePlace();
                        ++quantity;
                        ptr = (struct Patient *) realloc(ptr, (quantity) * sizeof(struct Patient));
                        ptr[quantity - 1].taj = id;
                        printf("Nev: ");
                        scanf("%s %s", &n1, &n2);
                        strcat(n1, " ");
                        strcat(n1, n2);
                        strcpy(ptr[quantity - 1].name, n1);
                        makePlace();
                        printf("Lakcim (ne hasznaljon szokozt): ");
                        scanf("%s", &ptr[quantity - 1].address);
                        makePlace();
                        printf("Vizsgalat tipusa: ");
                        scanf("%s", &ptr[quantity - 1].test);
                        do {
                            makePlace();
                            printf("Express?\n1. Igen\n2. Nem\nSorszam: ");
                            scanf("%d", &c2);
                            if(c2 == 1) {
                                ptr[quantity - 1].type = 'E';
                                break;
                            }
                            else if(c2 == 2) {
                                ptr[quantity - 1].type = 'H';
                                break;
                            }
                        } while(1);
                    }
                    else if(c1 == 2 && exist) { //Torles

                        makePlace();
                        for(int i = 0; i < quantity; i++) {
                            if(ptr[i].taj == id) {
                                printf("%s %c\n", ptr[i].test, ptr[i].type);
                            }
                        }
                        printf("Torlendo vizsgalat neve: ");
                        scanf("%s", &c3);
                        makePlace();

                        found = 0;
                        for(int i = 0; i < quantity; i++) {
                            if(strcmp(ptr[i].test, c3) == 0 && ptr[i].taj == id) {
                                found = 1;
                            }
                            if(found) {
                                strcpy(ptr[i].name, ptr[i + 1].name);
                                strcpy(ptr[i].address, ptr[i + 1].address);
                                ptr[i].taj = ptr[i + 1].taj;
                                strcpy(ptr[i].test, ptr[i + 1].test);
                                ptr[i].type = ptr[i + 1].type;
                            }
                        }
                        if(found) {
                            --quantity;
                            ptr = (struct Patient *) realloc(ptr, (quantity) * sizeof(struct Patient));
                            makePlace();
                        }
                        else {
                            printf("Ilyen vizsgalatra nem regisztralt!\n\n");
                        }
                    }
                    else if(c1 == 3 && exist) { //Modositas

                        makePlace();
                        for(int i = 0; i < quantity; i++) {
                            if(ptr[i].taj == id) {
                                printf("%s %c\n", ptr[i].test, ptr[i].type);
                            }
                        }
                        printf("Modositando vizsgalat neve: ");
                        scanf("%s", &c3);

                        found = 0;
                        for(int i = 0; i < quantity; i++) {
                            if(strcmp(ptr[i].test, c3) == 0 && ptr[i].taj == id) {
                                found = 1;
                                makePlace();
                                printf("Vizsgalat tipusa: ");
                                scanf("%s", &ptr[i].test);
                                do {
                                    makePlace();
                                    printf("Express?\n1. Igen\n2. Nem\nSorszam: ");
                                    scanf("%d", &c2);
                                    if(c2 == 1) {
                                        ptr[i].type = 'E';
                                        break;
                                    }
                                    else if(c2 == 2) {
                                        ptr[i].type = 'H';
                                        break;
                                    }
                                } while(1);
                            }
                            if(found) {
                                break;
                            }
                        }
                        makePlace();
                        if(found == 0) {
                            printf("Ilyen vizsgalatra nem regisztralt!\n\n");
                        }
                    }
                }
                else if(c1 == 4) {
                    makePlace();
                    for(int i = 0; i < quantity; i++) {
                        printf("%s %s %d %s %c\n", ptr[i].name, ptr[i].address ,ptr[i].taj, ptr[i].test, ptr[i].type);
                    }
                    printf("\n");
                }
                else if(c1 == 5) { //Szurt listazas
                    makePlace();
                    printf("Vizsgalat tipusa: ");
                    scanf("%s", &c3);
                    makePlace();
                    printf("Akik %s vizsgalatra jelentkeztek:\n\n", c3);
                    for(int i = 0; i < quantity; i++) {
                        if(strcmp(ptr[i].test, c3) == 0)
                        printf("%s %d\n", ptr[i].name ,ptr[i].taj);
                    }
                    printf("\n\n");
                }
                else { //c1 == 6 (kilepes)
                    break;
                }
            } while(1);

            //-----------------------------------------------------------------------------------------------

            FILE * fo = fopen ("data.txt", "w");
    
            fprintf(fo, "%d\n", quantity);
            
            for(int i = 0; i < quantity; i++) {
                fprintf(fo, "%s %s %d %s %c\n", ptr[i].name, ptr[i].address ,ptr[i].taj, ptr[i].test, ptr[i].type);
            }

            fclose(fo);  

            free(ptr);

            exit(EXIT_SUCCESS);
        }
        else {
            /* Child 2 code */
            sleep(1);
			kill(getppid(), MUNKARA_FEL);
            close(pipefd2[1]);
            struct Patient ptr;
            int notEnd;
            read(pipefd2[0], &notEnd, sizeof(int));
            while(notEnd) {
                read(pipefd2[0], &ptr, sizeof(struct Patient));
                printf("2. Mentoauto: %s %s %d %s %c\n", ptr.name, ptr.address ,ptr.taj, ptr.test, ptr.type);
                sleep(1); 
                read(pipefd2[0], &notEnd, sizeof(int));
            }
            close(pipefd2[0]);
            kill(getppid(), SIKERES_MINTAVETEL);
            exit(EXIT_SUCCESS);
        }
    }
    else {
        /* Child 1 code */
        sleep(1);
		kill(getppid(), MUNKARA_FEL);
        close(pipefd1[1]);
        struct Patient ptr;
        int notEnd;
        read(pipefd1[0], &notEnd, sizeof(int));
        while(notEnd) {
            read(pipefd1[0], &ptr, sizeof(struct Patient));
            printf("1. Mentoauto: %s %s %d %s %c\n", ptr.name, ptr.address ,ptr.taj, ptr.test, ptr.type);
            sleep(1); 
            read(pipefd1[0], &notEnd, sizeof(int));
        }
        close(pipefd1[0]);
        kill(getppid(), SIKERES_MINTAVETEL);
        exit(EXIT_SUCCESS);
    }
    return 0;
}