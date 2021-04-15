/////////////////   Zadanie 6 ///////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
 
double pi_leibniz(int n){
        if(n % 2 == 0){
                return (1)/(2*(double)n+1);;
        }
        else{
                return (-1)/(2*(double)n+1);
        }
}
int main(){
        int n,k,i,j, jj;
        int potok[2];
        double tmp;
        printf("rodzic o id %d \n",getpid()); 
        printf("Podaj liczbe elementow: \n");
        scanf("%d", &n);
        printf("Ile procesow chcesz utworzyc? :\n");
        scanf("%d", &k); 
        pid_t pids;     
        int nPerThread = n/k;
        double main_result = 0;
        pipe(potok);
        // Tworzenie odpowiedniej liczby procesow
        for(i=0;i<k;i++){     
                if((pids = fork())<0){
                        perror("fork"),exit(1);
                }
                if(pids==0){
                        printf("dziecko %d o id %d \n", i ,getpid());   
                        for(j=0;j<nPerThread;j++){      
                                tmp = pi_leibniz(i*nPerThread + j);
                                write(potok[1],&tmp,sizeof(tmp));  
                                //printf("Proces numer %d dla N %d = %f  \n",i, i*nPerThread + j, tmp);
                        }
                        exit(0);
                }
                
        } 
        //Rodzic odczytujacy na biezaco dane z potoku   
        //Nie mozna czekac do momentu ukonczenia wsyzstkich zadan wykonywanych przez dzieci
        //poniewaz dla duzej liczby N brakuje miejsce w potoku (4kB) 
        for(jj = n;jj > 0;jj--){                          
                close(potok[1]);
                double part_result=0;
                read(potok[0],&part_result, sizeof(part_result));
                //printf("Pobranie z potoku liczby %f\n" ,part_result);
                main_result += part_result;                         
        }                
        printf("Liczba Pi jest rowna  %f\n", (4*main_result));
        return 0;
}
