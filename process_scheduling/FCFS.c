#include <stdio.h>
#include <stdlib.h>
 
typedef struct {
    int pid;                // 进程ID
    int arrival_time;       // 到达时间
    int burst_time;         // 服务时间（CPU执行时间）
    int start_time;         // 开始执行时间
    int finish_time;        // 完成时间
    int turnaround_time;    // 周转时间
    float weighted_turnaround_time; // 带权周转时间
} Process;
 
int compare_arrival_time(const void *a, const void *b) {
    Process *p1 = (Process *)a;
    Process *p2 = (Process *)b;
    return p1->arrival_time - p2->arrival_time;
}
 
void FCFS_scheduling(Process *processes, int n) {
    qsort(processes, n, sizeof(Process), compare_arrival_time);
    int current_time = 0;
    for (int i = 0; i < n; i++) {
        if (current_time < processes[i].arrival_time) {
            current_time = processes[i].arrival_time;
        }
        processes[i].start_time = current_time;
        processes[i].finish_time = current_time + processes[i].burst_time;
        processes[i].turnaround_time = processes[i].finish_time - processes[i].arrival_time;
        processes[i].weighted_turnaround_time = (float)processes[i].turnaround_time / processes[i].burst_time;
        current_time = processes[i].finish_time;
    }
}
 
void print_result(Process *processes, int n) {
    int total_turnaround_time = 0;
    float total_weighted_turnaround_time = 0.0;
    printf("PID\t到达时间\t服务时间\t开始时间\t完成时间\t周转时间\t带权周转时间\n");
    for (int i = 0; i < n; i++) {
        total_turnaround_time += processes[i].turnaround_time;
        total_weighted_turnaround_time += processes[i].weighted_turnaround_time;
        printf("%d\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%.2f\n",
               processes[i].pid,
               processes[i].arrival_time,
               processes[i].burst_time,
               processes[i].start_time,
               processes[i].finish_time,
               processes[i].turnaround_time,
               processes[i].weighted_turnaround_time);
    }
    printf("平均周转时间: %.2f\n", (float)total_turnaround_time / n);
    printf("平均带权周转时间: %.2f\n", total_weighted_turnaround_time / n);
}
 
int main() {
    int n;
    printf("请输入进程数目: ");
    scanf("%d", &n);
    Process *processes = (Process *)malloc(n * sizeof(Process));
    for (int i = 0; i < n; i++) {
        processes[i].pid = i + 1;
        printf("请输入进程%d的到达时间: ", processes[i].pid);
        scanf("%d", &processes[i].arrival_time);
        printf("请输入进程%d的服务时间: ", processes[i].pid);
        scanf("%d", &processes[i].burst_time);
    }
    FCFS_scheduling(processes, n);
    print_result(processes, n);
    free(processes);
    return 0;
}
