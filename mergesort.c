/**
 * This file implements parallel mergesort.
 */
#include <pthread.h>
#include <stdio.h>
#include <string.h> /* for memcpy */
#include <stdlib.h> /* for malloc */
#include "mergesort.h"

/* this function will be called by mergesort() and also by parallel_mergesort(). */

void merge(int leftstart, int leftend, int rightstart, int rightend){
	int index;
	if (leftstart<rightstart){
		index=leftstart;
	}	
	else{
		index=rightstart;
	}

	int left=leftstart;
	int right=rightstart;
	while (left<=leftend && right<=rightend){
		if (A[left]<=A[right]){
			B[index]=A[left];
			index++;
			left++;
		}
		else{
			B[index]=A[right];
			index++;
			right++;
		}
	}
	//void *memcpy(void *dest, const void *src, size_t n);
	int sizeLeft=leftend-left+1;
	int sizeRight=rightend-right+1;
	memcpy(&B[index], &A[left], sizeLeft*sizeof(int));
	memcpy(&B[index], &A[right], sizeRight*sizeof(int));
	int total=rightend-leftstart+1;
	memcpy(&A[leftstart],&B[leftstart],total*sizeof(int));

}

/* this function will be called by parallel_mergesort() as its base case. 此函数使用单线程执行合并排序*/
void mergesort(int left, int right){
	if (left<right){
		int middle=(left+right)/2;
		mergesort(left,middle);
		mergesort(middle+1,right);
		merge(left,middle,middle+1,right);	
	}

}
/*	struct argument *arg=buildArgs(0, n-1, 0);
	parallel_mergesort(arg);
	sorting_time = getMilliSeconds() - start_time; */
/* this function will be called by the testing program. 通过这个来将数据传入*/
void * parallel_mergesort(void *arg){
	//get arguement
	struct argument *args =(struct argument*) arg;
	int left =args->left;
	int right=args->right;
	int level =args->level;
	//while (level<=cutoff) {
	if (level<cutoff) {
		int middle =(left+right)/2;
		level++;
		//struct argument *arg=buildArgs(0, n-1, 0);
		struct argument *th1argument=buildArgs(left, middle, level);
		struct argument *th2argument=buildArgs(middle+1, right, level);
		pthread_t th1,th2;
		//int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);
		pthread_create(&th1, NULL, parallel_mergesort, th1argument);
		pthread_create(&th2, NULL, parallel_mergesort, th2argument);
		//int pthread_join(pthread_t thread, void **retval);
		pthread_join(th1, NULL);
		pthread_join(th2, NULL);
		merge(left,middle,middle+1,right);
		free(args);

	}
	if (level==cutoff)
	{
		/* code */
		mergesort(left,right);
		free (args);
	}
	
		return NULL;
}

/* we build the argument for the parallel_mergesort function. */

struct argument * buildArgs(int left, int right, int level){
	struct argument *p = malloc(sizeof(struct argument));
	p->left = left;
    p->right = right;
    p->level = level;
    return p;
	
}

