#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 10  // 缓冲区大小
#define LOOP_COUNT 10   // 生产者/消费者循环次数

// 缓冲区结构
int buffer[BUFFER_SIZE];
int in = 0;  // 生产者写入位置
int out = 0; // 消费者读取位置

// 信号量定义
sem_t mutex;   // 互斥信号量：保护缓冲区访问
sem_t empty;   // 同步信号量：空闲缓冲区数量
sem_t full;    // 同步信号量：已填充数据数量

// 生产者线程函数：循环生产数据并写入缓冲区
void *producer(void *arg) {
    int id = *(int *)arg;  // 生产者ID
    for (int i = 0; i < LOOP_COUNT; i++) {
        // 1. 生产数据（模拟耗时操作）
        int data = rand() % 100;  // 生成0-99的随机数作为数据
        sleep(1);  // 模拟生产耗时（可调整）

        // 2. 等待空闲缓冲区（empty信号量减1，若为0则阻塞）
        sem_wait(&empty);
        // 3. 等待互斥锁（保证缓冲区操作原子性）
        sem_wait(&mutex);

        // 4. 写入缓冲区
        buffer[in] = data;
        printf("生产者%d: 生产数据%d，写入位置%d\n", id, data, in);
        in = (in + 1) % BUFFER_SIZE;  // 循环更新写入位置

        // 5. 释放互斥锁
        sem_post(&mutex);
        // 6. 已填充数据数量+1（唤醒可能等待的消费者）
        sem_post(&full);
    }
    printf("生产者%d: 完成%d次生产，退出\n", id, LOOP_COUNT);
    pthread_exit(NULL);
}

// 消费者线程函数：循环从缓冲区读取数据并消费
void *consumer(void *arg) {
    int id = *(int *)arg;  // 消费者ID
    for (int i = 0; i < LOOP_COUNT; i++) {
        // 1. 等待已填充数据（full信号量减1，若为0则阻塞）
        sem_wait(&full);
        // 2. 等待互斥锁（保证缓冲区操作原子性）
        sem_wait(&mutex);

        // 3. 读取缓冲区数据
        int data = buffer[out];
        printf("消费者%d: 读取位置%d，消费数据%d\n", id, out, data);
        out = (out + 1) % BUFFER_SIZE;  // 循环更新读取位置

        // 4. 释放互斥锁
        sem_post(&mutex);
        // 5. 空闲缓冲区数量+1（唤醒可能等待的生产者）
        sem_post(&empty);

        // 6. 模拟消费耗时
        sleep(1);
    }
    printf("消费者%d: 完成%d次消费，退出\n", id, LOOP_COUNT);
    pthread_exit(NULL);
}

int main() {
    pthread_t prod_tid, cons_tid;  // 生产者、消费者线程ID
    int prod_id = 1, cons_id = 1;  // 线程ID标识

    // 1. 初始化信号量
    sem_init(&mutex, 0, 1);   // 互斥信号量，初值1
    sem_init(&empty, 0, BUFFER_SIZE);  // 空闲缓冲区数=10
    sem_init(&full, 0, 0);    // 已填充数据数=0

    // 2. 创建生产者和消费者线程
    pthread_create(&prod_tid, NULL, producer, &prod_id);
    pthread_create(&cons_tid, NULL, consumer, &cons_id);

    // 3. 等待线程执行完成
    pthread_join(prod_tid, NULL);
    pthread_join(cons_tid, NULL);

    // 4. 销毁信号量（释放资源）
    sem_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    printf("所有线程执行完毕，程序退出\n");
    return 0;
}