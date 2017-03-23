//
//  main.cpp
//  BigDataHomeWork02
//
//  Created by 肖一凡 on 14-12-19.
//  Copyright (c) 2014年 XYF. All rights reserved.
//

#include <iostream>
#include <cmath>
using namespace std;

const int m = 8;
const short r = (short) 4;
const int N = 100000000;
const int k = 50000000;
const int step = 1000000;  // 每隔step估计一次

int presentTime = 0;    // 当前时间，全局统一，从0开始计时。
int bucketMaxNum;       // 最大桶的size的对数上界，具体是ceil(log2((N + r) / r))。
int *pow2;              // 数组，长度是powLength=max(m, bucketMaxNum)。存储2的各次幂，避免重复计算。

struct bucket {
    int time = -1;
};


void printStore(const bucket* store, const short *index)
{
    cout << "\n--------------------\n";
    for (int i = 0; i < bucketMaxNum; i ++) {
        cout << "size= " << i << ", index= " << index[i] << "\n";
        for (int j = 0; j < r + 1; j ++) {
            cout << i << "," << j << ": time= " << store[i*(r+1) + j].time << '\n';
        }
    }
}


void keepDGIM(int size, short *index, bucket *store)
{
    
    if (size >= bucketMaxNum || store[size * (r+1) + index[size]].time == -1)
        return;
    
//    cout << "\n\nBefore: " << presentTime;
//    printStore(store, index);
    
    short tmp1, tmp2;
    tmp1 = index[size];
    tmp2 = (tmp1+1) % (r+1);
    
    if (size < bucketMaxNum - 1) {
        store[(size + 1) * (r + 1) + index[size + 1]].time = store[size * (r + 1) + tmp2].time;
        index[size + 1] = (index[size + 1] + 1) % (r + 1);
    }
    
    store[size * (r + 1) + tmp1].time = -1;
    store[size * (r + 1) + tmp2].time = -1;
    
//    cout << "\n\nAfter: ";
//    printStore(store, index);
    
    keepDGIM(size + 1, index, store);
    
}


void DGIM(bool data, short *index, bucket *store)
{
    if (data == 0)
        return;
    
    store[index[0]].time = presentTime;
    index[0] = (index[0] + 1) % (r + 1);
    
    keepDGIM(0, index, store);
    
}


void estimateDGIM(int &Sum, int &Last, bucket *store)
{
    int sum = 0, last = 0;
    bool existBucket = false;
    for (int i = 0; i < bucketMaxNum; i ++) {
        
        existBucket = false;
        for (int j = 0; j < r + 1; j ++) {
            if (store[i * (r+1) + j].time >= presentTime - k) {
                sum += pow2[i];
                existBucket = true;
            }
        }
        
        if (!existBucket) {
            last = i - 1;
            break;
        }
    }
    if (last == 0) {
        last = bucketMaxNum - 1;
    }
    
    sum -= pow2[last - 1];

    Sum = sum;
    Last = last;
}


int main(int argc, const char * argv[]) {
    
    
    //----------建桶相关初始化----------
    srand((unsigned int) time(0));
    bucketMaxNum = ceil(log2((N + r) / r)) + 1;  // 可以理论证明，此处保险起见，特意+1。
    short **bucketIndexS;     // 二维数组，共m个，每个长度是bucketMaxNum。不同尺寸的桶在内存中的存储位置。
                            //每个数的取值范围是0～r共r+1个取值。
    bucket **bucketStoreS;    // 二维数组，共m个，每个长度是bucketMaxNum * (r + 1)。存储所有桶。内存占用大小主要由此决定。
    
    int powLength = (m > bucketMaxNum)? m: bucketMaxNum;
    pow2 = (int *) malloc(powLength * sizeof(int));
    pow2[0] = 1;
    for (int i = 0; i < powLength - 1; i ++) {
        pow2[i+1] = 2 * pow2[i];
    }
    
    //空间复杂度由下面4行决定
    bucketIndexS = (short **) malloc(m * sizeof(short*));
    bucketStoreS = (bucket **) malloc(m * sizeof(bucket*));
    short *tmpIndex = (short*) malloc(m * bucketMaxNum * sizeof(short));
    bucket *tmpStore = (bucket*) malloc(m * bucketMaxNum * (r + 1) * sizeof(bucket));
    
    for (int k = 0; k < m; k ++) {
        bucketIndexS[k] = tmpIndex + k * bucketMaxNum;
        bucketStoreS[k] = tmpStore + k * bucketMaxNum * (r + 1);
        
        for (int i = 0; i < bucketMaxNum; i ++) {
            bucketIndexS[k][i] = (short) 0;
            for (int j = 0; j < r + 1; j ++) {
                bucketStoreS[k][i * (r + 1) + j].time = -1;  // 特别留意！角标的取值
            }
        }
    }
    
    
    //----------估值相关初始化----------
    long int sum = 0;
    int *count = (int*) malloc(m * sizeof(int));
    int *last = (int*) malloc(m * sizeof(int));

    
    //----------建桶----------
    int temp;
    while (presentTime < 1.3 * N) {
        temp = rand() % pow2[m] + 1;  // 此时temp是1～2^m的范围
        temp -= 1;  // 此时temp映射到0～(2^m-1)的范围，在估值结束后再加上N调整。
        for (int i = 0; i < m; i ++) {
            DGIM(temp % 2, bucketIndexS[i], bucketStoreS[i]);
            temp = temp / 2;
        }
        
        presentTime += 1;
        
        //----------估计结果----------
        if (presentTime >= N && (presentTime - N) % step == 0) {
            sum = 0;
            for (int i = 0; i < m; i ++) {
                estimateDGIM(count[i], last[i], bucketStoreS[i]);
                sum += count[i] * pow2[i];
                
                cout << i << " floor count= " << count[i] << "; last= " << last[i] << "; sum= " << sum << "\n";
            }
            
            cout << "\nPresent Time is: " << presentTime - 1 << "; The total num is: " << presentTime << "\n";
//            for (int i = 0; i < m; i ++) {
//                cout << "\n\n" << i << "th floor:";
//                printStore(bucketStoreS[i], bucketIndexS[i]);
//            }
            
            cout << "\nThe DGIM estimate of sum is: " << sum + N << "\n\n";  // 结果上加N回到原数据结果上
        }
    }
    
    return 0;
}
