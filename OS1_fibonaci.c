#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<stdint.h>
/*
 * arr : 피보나치 계산 결과가 들어가게 될 전역변수 입니다.
 */
int *arr;
/*
 * 함수명 : makeFibo()
 * 작동원리 : 피보나치를 만드는 함수입니다.
 *          입력받은 숫자를 넘겨 받아 계산을 하게 됩니다.
 *          공간을 할당 한 뒤 피보나치 수열을 만들 수 있는
 *          최소의 0과 1을 할당하게 됩니다.
 *          계산을 통해 전역변수인 배열에 결과를 넣어주고 쓰레드를 종료합니다.
 */
void *makeFibo(void* Number){
    int Numb = *(int *)Number;
    int i = 0;
    if(Numb > 2){
        arr = (int*)malloc(sizeof(int)*Numb);
    } else{
        arr = (int*)malloc(sizeof(int)*2);
    }
    arr[0] = 0, arr[1] = 1;
    for(i = 2; i < Numb; i++){
        arr[i] = arr[i-1] + arr[i-2];
    }
    pthread_exit((void *)0);
}
int main(){
    //N은 입력받는 숫자입니다.
    int N = 0, i = 0;
    printf("input:");
    scanf("%d",&N);
    /*
     * thread를 만들어 N을 argument로 넘겨 줬습니다.
     * 계산 후 쓰레드가 종료가 되면 결과를 출력하게 됩니다.
     */
    pthread_t pid;

    pthread_create(&pid, NULL, (void *)makeFibo, &N);
    pthread_join(pid,NULL);
    for(i = 0; i < N; i++){
        printf("%d ",arr[i]);
        if(i != N-1){
            printf(", ");
        }
    }
    printf("\n");
    free(arr);
    return 0;
}
