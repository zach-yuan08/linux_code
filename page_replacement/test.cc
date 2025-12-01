#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REF_LEN 100  // 引用串最大长度
#define MAX_FRAME 10     // 物理块最大数量

// 工具函数：判断页面是否在内存中，返回索引（-1表示不在）
int isInMemory(int memory[], int frameNum, int page) {
    for (int i = 0; i < frameNum; i++) {
        if (memory[i] == page) {
            return i;  // 返回页面在内存中的位置
        }
    }
    return -1;  // 页面不在内存
}

// 工具函数：打印当前内存状态（置换图核心）
void printMemory(int memory[], int frameNum) {
    printf("| ");
    for (int i = 0; i < frameNum; i++) {
        if (memory[i] == -1) {  // -1表示物理块空闲
            printf("  ");
        } else {
            printf("%d ", memory[i]);
        }
        printf("| ");
    }
}

// 1. 先进先出（FIFO）页面置换算法
void fifoPageReplacement(int refString[], int refLen, int frameNum) {
    int memory[MAX_FRAME];  // 模拟内存物理块（-1表示空闲）
    int fifoQueue[MAX_FRAME];  // 记录页面进入内存的顺序（队列）
    int front = 0, rear = 0;  // 队列头尾指针
    int pageFault = 0;  // 缺页次数
    memset(memory, -1, sizeof(memory));  // 初始化内存为空闲

    printf("\n=== 先进先出（FIFO）页面置换算法 ===\n");
    printf("引用串：");
    for (int i = 0; i < refLen; i++) {
        printf("%d ", refString[i]);
    }
    printf("\n物理块数：%d\n", frameNum);
    printf("\n置换过程（每列代表一个物理块，空表示空闲）：\n");
    printf("访问页面 | 内存状态                | 缺页标记\n");
    printf("---------|-------------------------|--------\n");

    for (int i = 0; i < refLen; i++) {
        int currentPage = refString[i];
        int pos = isInMemory(memory, frameNum, currentPage);

        printf("%d        ", currentPage);
        printMemory(memory, frameNum);  // 打印当前内存状态

        if (pos == -1) {  // 页面不在内存，发生缺页
            pageFault++;
            printf("| 缺页\n");

            // 淘汰队列头部页面（最早进入的）
            int replacePos = fifoQueue[front];
            front = (front + 1) % frameNum;  // 队列指针后移

            // 新页面入内存和队列
            memory[replacePos] = currentPage;
            fifoQueue[rear] = replacePos;
            rear = (rear + 1) % frameNum;
        } else {  // 页面已在内存，不缺页
            printf("| 不缺页\n");
        }
    }

    // 计算缺页率
    float pageFaultRate = (float)pageFault / refLen * 100;
    printf("---------|-------------------------|--------\n");
    printf("缺页次数：%d\n", pageFault);
    printf("缺页率：%.2f%%\n", pageFaultRate);
}

// 2. 最近最久未使用（LRU）页面置换算法
void lruPageReplacement(int refString[], int refLen, int frameNum) {
    int memory[MAX_FRAME];  // 模拟内存物理块（-1表示空闲）
    int lastAccessTime[MAX_FRAME];  // 记录页面最后访问时间（用引用串索引表示）
    int pageFault = 0;  // 缺页次数
    memset(memory, -1, sizeof(memory));  // 初始化内存为空闲

    printf("\n=== 最近最久未使用（LRU）页面置换算法 ===\n");
    printf("引用串：");
    for (int i = 0; i < refLen; i++) {
        printf("%d ", refString[i]);
    }
    printf("\n物理块数：%d\n", frameNum);
    printf("\n置换过程（每列代表一个物理块，空表示空闲）：\n");
    printf("访问页面 | 内存状态                | 缺页标记\n");
    printf("---------|-------------------------|--------\n");

    for (int i = 0; i < refLen; i++) {
        int currentPage = refString[i];
        int pos = isInMemory(memory, frameNum, currentPage);

        printf("%d        ", currentPage);
        printMemory(memory, frameNum);  // 打印当前内存状态

        if (pos != -1) {  // 页面已在内存，更新最后访问时间
            lastAccessTime[pos] = i;  // 用当前引用串索引作为时间戳
            printf("| 不缺页\n");
        } else {  // 页面不在内存，发生缺页
            pageFault++;
            printf("| 缺页\n");

            // 寻找“最近最久未使用”的页面（lastAccessTime最小的）
            int replacePos = 0;
            for (int j = 1; j < frameNum; j++) {
                if (lastAccessTime[j] < lastAccessTime[replacePos]) {
                    replacePos = j;
                }
            }

            // 替换页面，更新最后访问时间
            memory[replacePos] = currentPage;
            lastAccessTime[replacePos] = i;
        }
    }

    // 计算缺页率
    float pageFaultRate = (float)pageFault / refLen * 100;
    printf("---------|-------------------------|--------\n");
    printf("缺页次数：%d\n", pageFault);
    printf("缺页率：%.2f%%\n", pageFaultRate);
}

// 主函数：输入参数、调用算法、输出结果
int main() {
    int refString[MAX_REF_LEN];  // 引用串
    int refLen;  // 引用串长度
    int frameNum;  // 物理块数
    int choice;  // 算法选择

    // 1. 输入引用串
    printf("请输入引用串长度（不超过%d）：", MAX_REF_LEN);
    scanf("%d", &refLen);
    if (refLen <= 0 || refLen > MAX_REF_LEN) {
        printf("引用串长度输入无效！\n");
        return 1;
    }

    printf("请输入引用串（空格分隔页面号）：");
    for (int i = 0; i < refLen; i++) {
        scanf("%d", &refString[i]);
    }

    // 2. 输入物理块数
    printf("请输入物理块数（不超过%d）：", MAX_FRAME);
    scanf("%d", &frameNum);
    if (frameNum <= 0 || frameNum > MAX_FRAME) {
        printf("物理块数输入无效！\n");
        return 1;
    }

    // 3. 选择算法
    printf("\n请选择页面置换算法：\n");
    printf("1. 先进先出（FIFO）\n");
    printf("2. 最近最久未使用（LRU）\n");
    printf("3. 两种算法都执行\n");
    printf("请输入选择（1/2/3）：");
    scanf("%d", &choice);

    // 4. 执行算法
    switch (choice) {
        case 1:
            fifoPageReplacement(refString, refLen, frameNum);
            break;
        case 2:
            lruPageReplacement(refString, refLen, frameNum);
            break;
        case 3:
            fifoPageReplacement(refString, refLen, frameNum);
            lruPageReplacement(refString, refLen, frameNum);
            break;
        default:
            printf("选择无效！\n");
            return 1;
    }

    return 0;
}