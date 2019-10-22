#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>

#include <pthread.h>

#define INIT_BALANCE 1000
#define NUM_TRANS 400

int balance = INIT_BALANCE;

int credits = 0;
int debits = 0;

pthread_mutex_t balance_lock, credits_lock, debits_lock;

void * transactions(void * args) {
  int v;
	
  for(int i = 0; i < NUM_TRANS; i++) {
    v = (int) random() % NUM_TRANS;
  
    if (random() % 2) {
      // CREDIT
      
      pthread_mutex_lock(&balance_lock);
      balance = balance + v;
      pthread_mutex_unlock(&balance_lock);

      pthread_mutex_lock(&credits_lock);
      credits = credits + v;
      pthread_mutex_unlock(&credits_lock);
    } else {
      // DEBIT 

      pthread_mutex_lock(&balance_lock);
      balance = balance - v;
      pthread_mutex_unlock(&balance_lock);
    
      pthread_mutex_lock(&debits_lock);
      debits = debits + v;
      pthread_mutex_unlock(&debits_lock);
    }
  }
}

int main(int argc, char * argv[]) {
  int n_threads;
  pthread_t * threads;

  if (argc < 2) {
    fprintf(stderr, "[ERROR] Requires number of threads\n");
    exit(1);
  }

  if ((n_threads = atol(argv[1])) == 0) {
    fprintf(stderr, "[ERROR] Invalid number of threads value\n");
    exit(1);
  }

  threads = calloc(n_threads, sizeof(pthread_t));

  for (int i = 0; i < n_threads; i++) {
    pthread_create(&threads[i], NULL, transactions, NULL);
  }

  for (int i = 0; i < n_threads; i++) {
    pthread_join(threads[i], NULL);
  }

  printf("\tCredits:\t%d\n", credits);
  printf("\t Debits:\t%d\n", debits);
  printf("\tBalance:   \t%d\t\n", INIT_BALANCE+credits-debits);

  free(threads);

  return 0;
}
