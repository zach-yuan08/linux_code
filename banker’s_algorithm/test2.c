#include <stdio.h>
#include <stdlib.h>

void printState(int P, int R, int *available, int **max, int **allocation, int **need) {
    printf("\n========= 当前系统资源状态 =========\n");

    printf("Available：\n");
    for (int i = 0; i < R; i++)
        printf("R%d: %d  ", i, available[i]);
    printf("\n\n");

    printf("进程\tMax\t\tAllocation\tNeed\n");
    for (int i = 0; i < P; i++) {
        printf("P%d\t", i);
        for (int j = 0; j < R; j++) printf("%d ", max[i][j]);
        printf("\t\t");
        for (int j = 0; j < R; j++) printf("%d ", allocation[i][j]);
        printf("\t\t");
        for (int j = 0; j < R; j++) printf("%d ", need[i][j]);
        printf("\n");
    }
    printf("===================================\n");
}

/*
 安全性检查（Safety Algorithm）
 返回：1 表示安全，并将安全序列写入 safeSeq（长度为 P）
       0 表示不安全
*/
int isSafe(int P, int R, int *available, int **allocation, int **need, int *safeSeq) {
    int *work = (int *)malloc(R * sizeof(int));
    int *finish = (int *)malloc(P * sizeof(int));
    for (int i = 0; i < R; i++) work[i] = available[i];
    for (int i = 0; i < P; i++) finish[i] = 0;

    int count = 0;
    while (count < P) {
        int found = 0;
        for (int i = 0; i < P; i++) {
            if (!finish[i]) {
                int j;
                for (j = 0; j < R; j++) {
                    if (need[i][j] > work[j]) break;
                }
                if (j == R) { // P_i can finish
                    for (int k = 0; k < R; k++) work[k] += allocation[i][k];
                    safeSeq[count++] = i;
                    finish[i] = 1;
                    found = 1;
                }
            }
        }
        if (!found) {
            free(work);
            free(finish);
            return 0; // 无可运行进程 -> 不安全
        }
    }

    free(work);
    free(finish);
    return 1;
}

int main() {
    int P, R;
    printf("输入进程数 P 和 资源种类数 R（例如: 5 3）：\n");
    if (scanf("%d %d", &P, &R) != 2) {
        printf("输入错误。\n");
        return 1;
    }

    // 动态分配
    int *available = (int *)malloc(R * sizeof(int));
    int **max = (int **)malloc(P * sizeof(int *));
    int **allocation = (int **)malloc(P * sizeof(int *));
    int **need = (int **)malloc(P * sizeof(int *));
    for (int i = 0; i < P; i++) {
        max[i] = (int *)malloc(R * sizeof(int));
        allocation[i] = (int *)malloc(R * sizeof(int));
        need[i] = (int *)malloc(R * sizeof(int));
    }

    printf("输入 Available（%d 个整数）：\n", R);
    for (int i = 0; i < R; i++) scanf("%d", &available[i]);

    printf("输入 Max 矩阵（%d 行，每行 %d 个整数）：\n", P, R);
    for (int i = 0; i < P; i++)
        for (int j = 0; j < R; j++)
            scanf("%d", &max[i][j]);

    printf("输入 Allocation 矩阵（%d 行，每行 %d 个整数）：\n", P, R);
    for (int i = 0; i < P; i++)
        for (int j = 0; j < R; j++)
            scanf("%d", &allocation[i][j]);

    // 计算 Need = Max - Allocation
    for (int i = 0; i < P; i++)
        for (int j = 0; j < R; j++)
            need[i][j] = max[i][j] - allocation[i][j];

    // 打印当前状态
    printState(P, R, available, max, allocation, need);

    int *safeSeq = (int *)malloc(P * sizeof(int));
    if (isSafe(P, R, available, allocation, need, safeSeq)) {
        // 为了得到安全序列，我们需要重新运行并记录序列（isSafe 上面没有写入序列）
        // 这里我们用同样的算法但将序列记录下来
        int *work = (int *)malloc(R * sizeof(int));
        int *finish = (int *)malloc(P * sizeof(int));
        for (int i = 0; i < R; i++) work[i] = available[i];
        for (int i = 0; i < P; i++) finish[i] = 0;
        int count = 0;
        while (count < P) {
            for (int i = 0; i < P; i++) {
                if (!finish[i]) {
                    int j;
                    for (j = 0; j < R; j++)
                        if (need[i][j] > work[j]) break;
                    if (j == R) {
                        for (int k = 0; k < R; k++) work[k] += allocation[i][k];
                        safeSeq[count++] = i;
                        finish[i] = 1;
                        break;
                    }
                }
            }
        }
        free(work);
        free(finish);

        printf("\n系统处于【安全】状态。\n安全序列为： ");
        for (int i = 0; i < P; i++) printf("P%d ", safeSeq[i]);
        printf("\n");
    } else {
        printf("\n系统处于【不安全】状态！不存在使所有进程顺利完成的序列。\n");
    }

    // 释放内存
    for (int i = 0; i < P; i++) {
        free(max[i]);
        free(allocation[i]);
        free(need[i]);
    }
    free(max);
    free(allocation);
    free(need);
    free(available);
    free(safeSeq);

    return 0;
}

/*
 示例输入（可复制粘贴测试）：
5 3
3 3 2
7 5 3
3 2 2
9 0 2
2 2 2
4 3 3
0 1 0
2 0 0
3 0 2
2 1 1
0 0 2

 对应解释：
P = 5, R = 3
Available = {3,3,2}
Max 矩阵按 P0..P4 行输入
Allocation 矩阵按 P0..P4 行输入
*/

