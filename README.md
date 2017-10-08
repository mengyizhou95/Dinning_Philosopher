# Dinning_Philosopher
//reference : http://blog.csdn.net/helloworld10086/article/details/39971529
//g++ -lpthread test.c -o test
#include <iostream>
#include <cstdio>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#define LEFT i
#define RIGHT (i+1) % N
using namespace std;

class Semaphore {
private:
    sem_t sem;
public:
    Semaphore(int value = 1) {
        sem_init(&sem,0,value);
        //init signal & signal is shared among threads(parameter "0")
    }
    void Pick() {
        sem_wait(&sem); //wait for signal, pick up forks
    }
    void Put() {
        sem_post(&sem); //release signal, put down forks
    }
};
int N = 0;
Semaphore mutex[100];
pthread_t thread[100];

int id[100];// thread id
void* solve(void* param) {
    int i = *((int*)param);
    while(1) {
        if(i % 2 == 0) {
            mutex[LEFT].Pick();
            mutex[RIGHT].Pick();
            printf("PHI NO.%d is eating \n",i+1);
            sleep(3);//eating 3s
            mutex[RIGHT].Put();
            mutex[LEFT].Put();printf("NO.%d finishes eating, start thinking \n", i+1);
        }else {
            mutex[RIGHT].Pick();
            mutex[LEFT].Pick();
            printf("PHI NO.%d is eating \n",i+1);
            sleep(3);//eating 3s
            mutex[LEFT].Put();
            mutex[RIGHT].Put();printf("NO.%d finishes eating, start thinking\n", i+1);
        }
        
        sleep(1); //waiting 1s for others to pick, prevent starvation
        
    }
    pthread_exit(NULL); //exit thread
}

int main() {
    printf("Please input number of philosophers\n");
    printf("We suggest a number compatible with your hardware\n");
    printf("number:");
    scanf("%d", &N);
    printf("\nTotal number is %d\n", N);
    
    for(int i = 0; i < N; i++) {
        id[i] = i;
    }
    //thread create
    int tmp;
    for(int i = 0; i < N; i++) {
        tmp = pthread_create(&thread[i],NULL,solve,&id[i]);
        if(tmp != 0) {
            printf("thread%d creating fails\n",i);
        }
    }
    //thread wait
    for(int i = 0; i < N; i++) {
        pthread_join(thread[i],NULL);
        printf("thread%d finishes\n",i);
    }
    
    return 0;
}
