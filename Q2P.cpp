//reference https://stackoverflow.com/questions/36493974/calculating-prime-numbers-pthread
//g++ -std=c++11 -lpthread Q2P.cpp -o Q2P
#include <iostream>
#include <pthread.h>
#include <list>
#include <algorithm>
#include <iterator>
#include <sys/time.h>

using namespace std;

list<int> result;
int NUM;
int NUMOFTHREADS;

/* Prototypes */
void *printPrimeThreadCalculation(void *threadid);
void primeCheckerThread(int num, int numOfThreads);

int main() {
    
    cout << "PLZ input threads number : " ;
    cin >> NUMOFTHREADS;
    cout << "\n";
    cout << "PLZ input uplevel : ";
    cin  >> NUM;
    cout << "\n";
    
    long long L1,L2;
    timeval tv;
    gettimeofday(&tv,NULL);
    L1 = tv.tv_sec*1000*1000 + tv.tv_usec;// start time
    
    /* Thread Prime Checker */
    primeCheckerThread(NUM, NUMOFTHREADS);
    /*sort and print out result*/
    result.remove(1);//1 is not prime number
    result.sort();//multiple threads makes elements out of order
    result.unique();//delete possible duplicate elements
    ostream_iterator<int> output{cout, " "};
    cout << "Prime number: ";
    copy(result.cbegin(), result.cend(), output);
    
    gettimeofday(&tv,NULL);
    L2 = tv.tv_sec*1000*1000+tv.tv_usec;//end time
    printf("\n running time is %lld us\n",L2-L1);//running time
    /* Exits the threads */
    pthread_exit(NULL);
    return 0;
}

void *printPrimeThreadCalculation(void *threadid) {
    long tid = (long) threadid;
    int interval = (NUM-1)/NUMOFTHREADS;//divide list on several threads
    
    int number1 = 1 + tid * interval;//1st number in on this thread
    int number2 = NUM - (NUMOFTHREADS - 1 - tid) * interval;//last
    int isPrime, i;
    
    /* Calculations */
    while (number1 <= number2) {
        /* Assume isPrime is true */
        isPrime = 1;
        
        for (i = 2; i < number1 && isPrime; i++)
        {
            if (number1 % i == 0)
            {
                isPrime = 0;
                break;
            }
        }
        if (isPrime == 1)
        {
            result.push_back(number1);//add to result list
        }
        number1++;
    }
    printf("\n");
}

void primeCheckerThread(int num, int numOfThreads) {
    pthread_t threads[100];
    //int id[100];
    long tmp;
    
    //for(int i = 0; i < NUMOFTHREADS; i++) {
    //    id[i] = i;
    // }
    //thread create
    for (int i = 0; i < numOfThreads; i++) {
        /* Creates threads */
        tmp = pthread_create(&threads[i], NULL, printPrimeThreadCalculation, (void *)i);
        if (tmp) {
            printf("ERROR; return code from pthread_create() is %d\n", tmp);
        }
        //thread wait---> until every prime number is found out and added into the list
        for(int i = 0; i < numOfThreads; i++) {
            pthread_join(threads[i],NULL);
        }
    }
}
