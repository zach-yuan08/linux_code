#include <stdio.h>
 
void sjf_non_preemptive(int n, int burst_times[]) {
    int wait_times[n], turnaround_times[n];
    int total_wait = 0, total_turnaround = 0;
    int completed[n];
 
    // 初始化
    for (int i = 0; i < n; i++) {
        completed[i] = 0;
    }
 
    for (int i = 0; i < n; i++) {
        int min_index = -1;
        for (int j = 0; j < n; j++) {
            if (!completed[j] && (min_index == -1 || burst_times[j] < burst_times[min_index])) {
                min_index = j;
            }
        }
        completed[min_index] = 1;
        wait_times[min_index] = total_wait;
        total_wait += burst_times[min_index];
        turnaround_times[min_index] = wait_times[min_index] + burst_times[min_index];
        total_turnaround += turnaround_times[min_index];
    }
 
    printf("Process\tBurst Time\tWait Time\tTurnaround Time\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t\t%d\t\t%d\t\t%d\n", i + 1, burst_times[i], wait_times[i], turnaround_times[i]);
    }
    printf("Average Wait Time: %.2f\n", (float)total_wait / n);
    printf("Average Turnaround Time: %.2f\n", (float)total_turnaround / n);
}
 
int main() {
    int burst_times[] = {5,3,2};
    int n = sizeof(burst_times) / sizeof(burst_times[0]);
    sjf_non_preemptive(n, burst_times);
    return 0;
}
