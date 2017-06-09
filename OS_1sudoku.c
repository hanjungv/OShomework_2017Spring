#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
/*
 * dx, dy는 checkBox() 함수에서 사용하게 됩니다.
 * x, y위치를 준다면 총 9개의 요소를 반복문으로
 * 전부 체크할 수 있게 됩니다. 아래는 각 9방향 인덱스입니다.
 * (-1,-1)(0,-1)(1,-1)
 * (-1,0)(0,0)(1,0)
 * (-1,1)(0,1)(1,1)
 *
 * arr배열은 스도쿠 파일이 들어가게 됩니다.
 * result 배열은 thread에서 return하는 값들을 담게 됩니다.
 * 자세한 설명은 밑에 있습니다.
 * Result 변수가 11이 될 경우 valid한 스도쿠 입니다.
 * pidCnt는 이후에 박스를 체크하는 쓰레드를 만들 때 사용하게 됩니다.
 * startIdx 구조체 또한 checkBox()함수에서 사용하게 됩니다.
 * 시작 지점 인덱스를 담게됩니다.
 */
int dx[9] = {0 ,0 ,1 ,1 ,1 ,0 ,-1 ,-1 ,-1};
int dy[9] = {0, -1, -1, 0, 1, 1, 1, 0, -1};
int arr[9][9] = {0, };
int result[11] = {0, };
int Result = 0;
int pidCnt = 2;
typedef struct{
	int stX;
	int stY;
} startIdx;
/*
 * 함수명 : checkVertical()
 * 작동원리 : row값을 증가시키면서 해당되는 column을 체크합니다.
 * 			이미 check 배열이 1로 바뀌어있다면
 *          그 숫자가 이미 있다는 뜻이므로 0을 return하며
 * 			thread가 종료됩니다. 끝까지 마칠 경우
 *          적절한 스도쿠이기 때문에 1을 반환하며 쓰레드를 종료합니다.
 */
 void *checkVertical(){
     int col = 0, row = 0, i =0;
     for(col = 0; col < 9; col++){
         int check[10] = {0, };
         for(row = 0; row < 9; row++){
             if(check[arr[col][row]] == 0){
                 check[arr[col][row]] = 1;
             } else{
                 pthread_exit((void *)0);
             }
         }
     }
     pthread_exit((void *)1);
 }
 /*
  * 함수명 : checkHorizontal()
  * 작동원리 : 위 checkVertical()과 다르게 row를 체크하게 됩니다.
  * 			이미 check 배열이 1로 바뀌어있다면
  *          그 숫자가 이미 있다는 뜻이므로 0을 return하며
  * 			thread가 종료됩니다. 끝까지 마칠 경우
  *          적절한 스도쿠이기 때문에 1을 반환하며 쓰레드를 종료합니다.
  */
 void *checkHorizontal(){
     int row = 0, col = 0;
     for(row = 0; row < 9; row++){
         int check[10] = {0, };
         for(col = 0; col < 9; col++){
             if(check[arr[col][row]] == 0){
                 check[arr[col][row]] = 1;
             } else{
                 pthread_exit((void *)0);
             }
         }
     }
     pthread_exit((void *)1);
 }
 /*
  * 함수명 : checkBox(void* startPoint)
  * 작동원리 : startPoint를 통해 시작 x,y 인덱스를 넘겨받았습니다.
  * 			이를 curX, curY에 넣어
  *          만들어 놓은 9지점 dx, dy를 더해 9개의 지점을 체크하게 됩니다.
  *          만약 이미 방문한 지점이면 0을 반환하고 무사히 마칠 경우
  * 			1을 반환합니다.
  */
void *checkBox(void* startPoint){
    int check[10] = {0, };
    int i=0, nextX =0, nextY = 0;
    startIdx* stP = (startIdx *)startPoint;
    int curX = stP->stX;
    int curY = stP->stY;
    for(i = 0; i < 9; i++){
        nextX = curX + dx[i];
        nextY = curY + dy[i];
        if(check[arr[nextY][nextX]] == 0){
            check[arr[nextY][nextX]] = 1;
        } else{
            pthread_exit((void *)0);
        }
    }
    pthread_exit((void *)1);
}

int main(int argc, char **argv){
	/*
     * input file명을 입력받아 standard input을 하게 됩니다.
     * 입력받은 값은 2차원 배열 arr에 담기게 됩니다.
     */
	int i = 0, j = 0, x = 0, y = 0;
	startIdx *idx;
    freopen(argv[1], "r", stdin);
	for(i = 0; i < 9; i++){
	   for(j = 0; j < 9; j++){
		   scanf("%d",&arr[i][j]);
	   }
   	}
    /*
     * thread 생성 부분입니다.
     */
    pthread_t pid[11];
	/*
     * 쓰레드 아이디가 0인 것은 세로로 체크하는 역할을 하게 됩니다.
	 * 함수는 checkVertical() 입니다.
     * 쓰레드 아이디가 1인 것은 가로로 체크하는 역할을 하게 됩니다.
	 * 함수는 checkHorizontal() 입니다.
     * 두 함수는 argument로 넘겨줄 값이 없어 NULL을 주었습니다.
     */
	pthread_create(&pid[0], NULL, (void *)checkVertical, NULL);
	pthread_create(&pid[1], NULL, (void *)checkHorizontal, NULL);
	/*
     * 쓰레드 아이디가 2 ~ 10, 총 9개의 쓰레드는 각각의 3x3 박스를
	 * 체크하게 됩니다.
     * 시작 인덱스는 박스 각각 이렇게 됩니다. (x,y)순 입니다.
     * (1,1)(4,1)(7,1)
     * (1,4)(4,4)(7,4)
     * (1,7)(4,7)(7,7)
     * checkBox()함수에 argument로 index를 넘겨주기 위해
	 * startIdx구조체를 할당하여 x, y를 할당 한 뒤
     * 구조체로 argument를 넘겨주게 됩니다.
     * pidCnt는 2,3,4,....,10으로 증가하게 되는 쓰레드 id를
	 * 의미하게 됩니다.
     */
	 for(x = 1; x <= 7; x+=3){
         for(y = 1; y <= 7; y+=3){
             idx = (startIdx *)malloc(sizeof(startIdx));
             idx->stX = x;
             idx->stY = y;
             pthread_create(&pid[pidCnt], NULL, (void *)checkBox, (void *)idx);
             pidCnt++;
         }
     }
    /*
     * pthread_join에서 pthread_create한 쓰레드가 종료되기를
	 * 기다립니다.
     * i번 id를 가진 쓰레드는 호출된 함수가 종료될 때 pthread_exit을
	 * 통해 넘겨준 값을 result[i]에 담게 됩니다.
     * valid하면 1, 아니면 0을 반납하게 됩니다.
     */
    for(i = 0; i < 11; i++){
        pthread_join(pid[i], (void **)&result[i]);
    }
    /*
     * 11개의 쓰레드에서 모두 1이 넘어오지 않았다면 Valid하지 않
	 * 스도쿠 파일이란 뜻입니다.
     */
    for(i = 0; i < 11; i++){
        Result += result[i];
    }
    if(Result == 11) printf("Valid result !\n");
    else printf("Invalid result !\n");
	free(idx);
    return 0;
}
