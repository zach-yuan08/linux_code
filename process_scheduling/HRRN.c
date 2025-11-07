#include <stdio.h>
#include <stdlib.h>

// 进程结构体
typedef struct {
    int pid;             // 进程ID
    int arrivalTime;     // 到达时间
    int burstTime;       // 服务时间
    int remainTime;      // 剩余服务时间（用于标记是否完成）
    int startTime;       // 开始时间
    int finishTime;      // 完成时间
    int turnaroundTime;  // 周转时间
    double weightedTurnaroundTime; // 带权周转时间
} Process;

// 计算最高响应比的进程索引
int getHighestResponseRatio(Process* procs, int n, int currentTime) {
    int selected = -1;
    double maxRatio = -1.0;
    
    for (int i = 0; i < n; i++) {
        // 只考虑已到达且未完成的进程
        if (procs[i].arrivalTime <= currentTime && procs[i].remainTime > 0) {
            // 响应比 = 1 + 等待时间 / 服务时间
            int waitTime = currentTime - procs[i].arrivalTime;
            double ratio = 1.0 + (double)waitTime / procs[i].burstTime;
            
            if (ratio > maxRatio) {
                maxRatio = ratio;
                selected = i;
            }
        }
    }
    return selected;
}

int main() {
    int n;
    printf("请输入进程数目：");
    scanf("%d", &n);

    Process* procs = (Process*)malloc(n * sizeof(Process));
    if (procs == NULL) {
        printf("内存分配失败！\n");
        return 1;
    }

    // 输入进程信息
    for (int i = 0; i < n; i++) {
        procs[i].pid = i + 1;
        printf("请输入进程 %d 的到达时间：", i + 1);
        scanf("%d", &procs[i].arrivalTime);
        printf("请输入进程 %d 的服务时间：", i + 1);
        scanf("%d", &procs[i].burstTime);
        procs[i].remainTime = procs[i].burstTime; // 初始化剩余时间
    }

    // 调度过程
    int currentTime = 0;
    int completed = 0;
    double totalTurnaround = 0;
    double totalWeightedTurnaround = 0;

    printf("\nPID\t到达时间\t服务时间\t开始时间\t完成时间\t周转时间\t带权周转时间\n");

    while (completed < n) {
        // 选择当前最高响应比的进程
        int idx = getHighestResponseRatio(procs, n, currentTime);
        
        if (idx == -1) {
            // 无就绪进程，推进时间到下一个进程到达
            currentTime++;
            continue;
        }

        // 记录开始时间（首次运行时）
        if (procs[idx].remainTime == procs[idx].burstTime) {
            procs[idx].startTime = currentTime;
        }

        // 执行该进程（一次性完成）
        currentTime += procs[idx].remainTime;
        procs[idx].finishTime = currentTime;
        procs[idx].remainTime = 0; // 标记为完成
        completed++;

        // 计算周转时间和带权周转时间
        procs[idx].turnaroundTime = procs[idx].finishTime - procs[idx].arrivalTime;
        procs[idx].weightedTurnaroundTime = 
            (double)procs[idx].turnaroundTime / procs[idx].burstTime;

        // 累加统计
        totalTurnaround += procs[idx].turnaroundTime;
        totalWeightedTurnaround += procs[idx].weightedTurnaroundTime;

        // 输出当前进程信息
        printf("%d\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%.2f\n",
               procs[idx].pid,
               procs[idx].arrivalTime,
               procs[idx].burstTime,
               procs[idx].startTime,
               procs[idx].finishTime,
               procs[idx].turnaroundTime,
               procs[idx].weightedTurnaroundTime);
    }

    // 输出平均时间
    printf("\n平均周转时间：%.2f\n", totalTurnaround / n);
    printf("平均带权周转时间：%.2f\n", totalWeightedTurnaround / n);

    free(procs);
    return 0;
}
