#include <sys/types.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>

#define READ_END 0
#define WRITE_END 1


int* delete(int* p, int index, int n) {
  p[index] = -1;
  int *p2 = (int*)malloc((n-1)*sizeof(int));
  int i=0,j=0;
  for(i=0;i<index;i++) {
    p2[j] = p[i];
    j++;
  }
  for(i=index+1;i<n;i++){
    p2[j] = p[i];
    j++;
  }
  return p2;
}

int prime(int x) {
  // 0 -> Prime,  1 -> Not Prime
  int val =0;
  int i;
  for(i=2;i<x;i++) {
    if(x%i ==0) {
      val++;
    }
  }
  if(val ==0) {
    return 0;
  }
  else {
    return 1;
  }
}

int main() {
  //Array is p
  int *p;
  p = (int*)malloc(10*sizeof(int));
  int i,n;
  printf("Enter (even) size:\n");
  scanf("%d",&n);
  printf("Enter numbers:\n");
  for(i=0;i<n;i++) {
    scanf("%d",&(p[i]));
  }
  int count = n;

  int fd[2];
  pid_t pid;

  if(pipe(fd) == -1) {
    fprintf(stderr,"Pipe Failed");
    return 1;
  }

  pid = fork();

  if(pid<0) {
    fprintf(stderr,"Fork Failed");
    return 1;
  }
  if(pid>0) {
    //Parent process
    while(count != 0) {
      puts("entered parent");
      int xindex = rand()%count;
      int x = p[xindex];
      // check for equality
      int yindex = rand()%count;
      int y = p[yindex];
      while(xindex == yindex){
        yindex = rand()%count;
        y = p[yindex];
      }

      //SWAP x,y and xindex yindex
      if(x > y){
        int temp;
        temp = x;
        x=y;
        y=temp;
        temp = xindex;
        xindex=yindex;
        yindex=temp;
      }

      printf("x & y: %d %d\n",x,y);
      fflush(stdout);
      p = delete(p,yindex,count);
      count--;
      p = delete(p,xindex,count);
      count--;
      printf("Printing s after delete\n");
      int tmp;
      for(tmp=0;tmp<count;tmp++) {
        printf("%d ",p[tmp]);
      }
      printf("\n");

      close(fd[READ_END]);
      write(fd[WRITE_END],&y, sizeof(int));
      // close(fd[WRITE_END]);
      sleep((unsigned int)(x/3));
    }
    wait();
  }
  else {
    sleep(1);
    //Child Process
    int ypipe;
    int S=0;
    int j;
    puts("entered child");
    for(i=count;i>0;i-=2){
      close(fd[WRITE_END]);
      int loc = count-i;
      // for(j=0;j<loc;j++){
        read(fd[READ_END],&ypipe, sizeof(int));
      // }
      printf("yp: %d\n",ypipe );
      // close(fd[READ_END]);
      if(prime(ypipe) == 0) {
        S+=ypipe;
      }
      printf("S: %d\n", S );
      sleep((int)(ypipe%3));
    }



  }

}
