#include <stdio.h>

#define P 5   // 进程数
#define R 3   // 资源种类数

int available[R];          // 可用资源
int max[P][R];             // 最大需求矩阵
int allocation[P][R];      // 已分配矩阵
int need[P][R];            // 需求矩阵

// 打印矩阵函数
void printState() {
    printf("\n========= 当前系统资源状态 =========\n");

    printf("Available：\n");
    for (int i = 0; i < R; i++)
        printf("R%d: %d  ", i, available[i]);
    printf("\n\n");

    printf("进程\tMax\t\tAllocation\tNeed\n");
    for (int i = 0; i < P; i++) {
        printf("P%d\t", i);
        for (int j = 0; j < R; j++)
            printf("%d ", max[i][j]);
        printf("\t\t");
        for (int j = 0; j < R; j++)
            printf("%d ", allocation[i][j]);
        printf("\t\t");
        for (int j = 0; j < R; j++)
            printf("%d ", need[i][j]);
        printf("\n");
    }
    printf("===================================\n");
}

// 安全性检查函数
int isSafe() {
    int work[R];
    int finish[P] = {0};
    int safeSeq[P];
    int count = 0;

    for (int i = 0; i < R; i++)
        work[i] = available[i];

    while (count < P) {
        int found = 0;
        for (int i = 0; i < P; i++) {
            if (finish[i] == 0) {
                int j;
                for (j = 0; j < R; j++)
                    if (need[i][j] > work[j])
                        break;
                if (j == R) {
                    for (int k = 0; k < R; k++)
                        work[k] += allocation[i][k];
                    safeSeq[count++] = i;
                    finish[i] = 1;
                    found = 1;
                }
            }
        }
        if (!found) {
            printf("\n系统处于【不安全】状态！\n");
            return 0;
        }
    }

    printf("\n系统处于【安全】状态。\n安全序列为： ");
    for (int i = 0; i < P; i++)
        printf("P%d ", safeSeq[i]);
    printf("\n");
    return 1;
}

// 资源请求函数
void requestResources(int pID, int request[R]) {
    printf("\n>>> 进程 P%d 请求资源：(", pID);
    for (int i = 0; i < R; i++) printf("%d ", request[i]);
    printf(")\n");

    for (int i = 0; i < R; i++) {
        if (request[i] > need[pID][i]) {
            printf("错误：请求量超过进程最大需求！\n");
            return;
        }
        if (request[i] > available[i]) {
            printf("错误：系统资源不足，P%d 需等待！\n", pID);
            return;
        }
    }

    // 试探性分配
    for (int i = 0; i < R; i++) {
        available[i] -= request[i];
        allocation[pID][i] += request[i];
        need[pID][i] -= request[i];
    }

    printf("\n试探性分配后系统状态：");
    printState();

    if (isSafe()) {
        printf("\n资源请求可以被满足，系统安全。\n");
    } else {
        // 回滚操作
        for (int i = 0; i < R; i++) {
            available[i] += request[i];
            allocation[pID][i] -= request[i];
            need[pID][i] += request[i];
        }
        printf("\n请求导致系统不安全，已回滚。\n");
    }
}

int main() {
    // 初始化数据
    int tempAvailable[R] = {3, 3, 2};
    int tempMax[P][R] = {
        {7, 5, 3},
        {3, 2, 2},
        {9, 0, 2},
        {2, 2, 2},
        {4, 3, 3}
    };
    int tempAlloc[P][R] = {
        {0, 1, 0},
        {2, 0, 0},
        {3, 0, 2},
        {2, 1, 1},
        {0, 0, 2}
    };

    // 拷贝数据并计算 need
    for (int i = 0; i < R; i++)
        available[i] = tempAvailable[i];

    for (int i = 0; i < P; i++)
        for (int j = 0; j < R; j++) {
            max[i][j] = tempMax[i][j];
            allocation[i][j] = tempAlloc[i][j];
            need[i][j] = max[i][j] - allocation[i][j];
        }

    printf("==== 银行家算法示例 ====\n");
    printState();

    printf("\n初始安全性检查：");
    isSafe();

    int req1[R] = {1, 0, 2};
    requestResources(1, req1);

    return 0;
}

