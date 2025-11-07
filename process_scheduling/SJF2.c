#include <stdio.h>
 
void sjf_preemptive(int n, int arrival_times[], int burst_times[]) {
    int remaining_times[n], wait_times[n], turnaround_times[n];
    int total_wait = 0, total_turnaround = 0;
    int completed = 0, current_time = 0, min_index;
 
    // 初始化
    for (int i = 0; i < n; i++) {
        remaining_times[i] = burst_times[i];
    }
 
    while (completed != n) {
        min_index = -1;
        for (int i = 0; i < n; i++) {
            if (arrival_times[i] <= current_time && remaining_times[i] > 0) {
                if (min_index == -1 || remaining_times[i] < remaining_times[min_index]) {
                    min_index = i;
                }
            }
        }
 
        if (min_index == -1) {
            current_time++;
            continue;
        }
 
        remaining_times[min_index]--;
        current_time++;
 
        if (remaining_times[min_index] == 0) {
            completed++;
            turnaround_times[min_index] = current_time - arrival_times[min_index];
            wait_times[min_index] = turnaround_times[min_index] - burst_times[min_index];
            total_wait += wait_times[min_index];
            total_turnaround += turnaround_times[min_index];
        }
    }
 
    printf("Process\tArrival Time\tBurst Time\tWait Time\tTurnaround Time\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t\t%d\t\t%d\t\t%d\t\t%d\n", i + 1, arrival_times[i], burst_times[i], wait_times[i], turnaround_times[i]);
    }
    printf("Average Wait Time: %.2f\n", (float)total_wait / n);
    printf("Average Turnaround Time: %.2f\n", (float)total_turnaround / n);
}
 
int main() {
    int arrival_times[] = {0, 2, 4};
    int burst_times[] = {5, 3, 2};
    int n = sizeof(burst_times) / sizeof(burst_times[0]);
    sjf_preemptive(n, arrival_times, burst_times);
    return 0;
}
