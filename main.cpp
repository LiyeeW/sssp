#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <pthread.h>
#define Thread_NUM 2//�߳���

struct Para_type {
    int n;
    int** adj;
    int* dist;
    int* visited;
    int begin, end;
    int select;
};

void thread(void* arg)
{
    struct Para_type* ar = (struct Para_type*)arg;
    int n = ar->n;
    int** adj = ar->adj;
    int* dist = ar->dist;
    int* visited = ar->visited;
    int begin = ar->begin;
    int end = ar->end;
    int select = ar->select;

    /*********  Begin  **********/
    //����select�����ھӵľ���

    //��Ϊ���ʶ�����ѡ��һ����������Ķ���

    /*********  End  **********/

    ar->select = select;
}

void computeSSSP(int** adj, int n) {
    //nΪ������,adjΪһ��n*n�Ķ�ά����
    //adj[i][j]Ϊ����i������j�ľ��룬��adj[i][j]Ϊ-1����ʾ�޷���iֱ�ӵ���j
    //���⣬adj[i][j]��adj[j][i]δ�����
    int source = 0;//���ö���0ΪԴ��
    int* dist = (int*)malloc(n * sizeof(int));//dist[i]��ʾ����i�붥��0�ľ���
    int* visited = (int*)malloc(n * sizeof(int));//visited[i]��ʾ����i�Ƿ��ѷ���

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
    while (count < n) {
        /*********  Begin  **********/
        //���ö��̣߳�ʵ��ÿ���߳�ѡ����һ���ֲ�����Ķ��㣬ѡ����Ҫ������ѡ��һ��ȫ������Ķ���


        /*********  End  **********/

        visited[select] = 1;//������������״̬Ϊ�ѷ���
        count++;//��������һ
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
