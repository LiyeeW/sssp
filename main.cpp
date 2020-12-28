#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <pthread.h>
#define Thread_NUM 2//线程数

struct Para_type {
    int n;  //什么用
    int** adj;
    int* dist;
    int* visited;
    int begin, end; //从1~n
    int* select;
};

void* thread(void* arg)
{
    struct Para_type* ar = (struct Para_type*)arg;
    int n = ar->n;
    int** adj = ar->adj;
    int* dist = ar->dist;
    int* visited = ar->visited;
    int begin = ar->begin;
    int end = ar->end;
    int select = *(ar->select); 
    int* select_p = ar->select; //用指针返回select结果

    int j, i;

    /*********  Begin  **********/
    //更新select顶点邻居的距离
    for(j=begin-1;j<end;j++){//更新邻居距离
        if(adj[select][j]!=-1&&dist[select]+adj[select][j]<dist[j])
            dist[j]=dist[select]+adj[select][j];
    }


    //在为访问顶点中选择一个距离最近的顶点
    int min_dist=INT_MAX;
    for(i=begin-1;i<end;i++){//选择最近的顶点
        if(!visited[i]&&dist[i]<min_dist){
            min_dist=dist[i];
            select=i;
        }
    }
    *(select_p)=select;
    /*********  End  **********/

}

void computeSSSP(int** adj, int n) {
    //n为顶点数,adj为一个n*n的二维数组
    //adj[i][j]为顶点i到顶点j的距离，若adj[i][j]为-1，表示无法从i直接到达j
    //此外，adj[i][j]与adj[j][i]未必相等
    int source = 0;//设置顶点0为源点
    int* dist = (int*)malloc(n * sizeof(int));//dist[i]表示顶点i与顶点0的距离
    int* visited = (int*)malloc(n * sizeof(int));//visited[i]表示顶点i是否已访问

    int i, j;
    for (i = 0;i < n;i++) {
        if (i == source) {
            dist[i] = 0;
            visited[i] = 1;
        }
        else {
            dist[i] = INT_MAX;
            visited[i] = 0;
        }
    }
    int select = source;
    int count = 0;

    int min_dist;

    pthread_t tid[Thread_NUM];  //线程号
    int select_p[Thread_NUM];   //线程的局部最短
    struct Para_type a[Thread_NUM];
    for(i=0;i<Thread_NUM;i++){
        a[i].n = n;
        a[i].adj = adj;
        a[i].dist = dist;
        a[i].visited = visited;
    }
    
    int begin;  //每次的起始
    int step = (n/Thread_NUM)+((n%Thread_NUM)?1:0); //一个线程的跨度
    while (count < n) {
        /*********  Begin  **********/
        //利用多线程，实现每个线程选择了一个局部最近的顶点，选择需要在其中选择一个全局最近的顶点
        begin = 1;
        for(i=0;i<Thread_NUM;i++){
            select_p[i] = select;
            a[i].begin = begin;
            begin+=step;
            a[i].end = (begin-1<n)?(begin-1):n;
            a[i].select = &(select_p[i]);
            pthread_create(&(tid[i]), NULL, thread, &(a[i]));
        }
        min_dist=INT_MAX;
        for(i=0;i<Thread_NUM;i++){
            pthread_join(tid[i], NULL);
            if(!visited[select_p[i]]&&dist[select_p[i]]<min_dist){
                min_dist=dist[select_p[i]];
                select=select_p[i];
            }
        }


        /*********  End  **********/

        visited[select] = 1;//更新最近顶点的状态为已访问
        count++;//访问数加一
    }
    for (i = 0;i < n;i++) {
        printf("%d", dist[i]);
        if (i < n - 1) printf(" ");
        else printf("\n");
    }
}

int main()
{
    FILE* fp = fopen("file5", "r");
    int i, j;
    int n;
    int m;
    int** adj;
    fscanf(fp, "%d", &n);
    adj = (int**)malloc(n * sizeof(int*));
    for (i = 0;i < n;i++) {
        adj[i] = (int*)malloc(n * sizeof(int));
    }
    fscanf(fp, "%d", &m);
    for (i = 0;i < n;i++) {
        for (j = 0;j < n;j++) {
            adj[i][j] = -1;
        }
    }
    for (i = 0;i < m;i++) {
        int src, dst, weight;
        fscanf(fp, "%d", &src);
        fscanf(fp, "%d", &dst);
        fscanf(fp, "%d", &weight);
        adj[src][dst] = weight;
    }
    computeSSSP(adj, n);
    return 0;
}
