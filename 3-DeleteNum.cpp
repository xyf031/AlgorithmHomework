#include "stdio.h"
const int MAX_N = 100001;
void solution(int &K, int data[]);
void deleteNum(int data[], int index);
int getNext(int data[], int thisOne);

int main(int argc, const char * argv[]) {
    int T, K, data[MAX_N];
    scanf("%d", &T);
    for (int i = 0; i < T; i ++) {
        solution(K, data);
    }
    
    printf("\n\nHello, World!\n");////////////////////
    return 0;
}

void solution(int &K, int data[]) {
    char c;
    int ii = 0;
    scanf("%c", &c);
    while (c != ' ') {
        if (c <= '9' && c >= '0') {
            data[ii ++] = (int)(c - 48);
        }
        scanf("%c", &c);
    }
    data[ii] = -2;
    scanf("%d", &K);
    
    /*///
    printf("%d\n", K);
    for (int i = 0; i < MAX_N; i ++) {
        printf("%d ", data[i]);
        if (data[i] == -2) {
            printf("\n");
            break;
        }
    }
    *////
    
    for (int i = 0; i < K; i ++) {
        deleteNum(data, 0);
    }
    for (int i = 0; i < MAX_N; i ++) {
        if (data[i] == -2) {
            break;
        } else if (data[i] == -1) {
            continue;
        } else {
            printf("%d", data[i]);
        }
    }
    printf("\n");
    
}

void deleteNum(int data[], int index) {
    if (data[index] == -1) {
        deleteNum(data, index + 1);
        return;
    }
    if (data[index] < getNext(data, index)) {
        data[index] = -1;
        return;
    } else {
        deleteNum(data, index + 1);
    }
}

int getNext(int data[], int thisOne) {
    if (data[thisOne + 1] == -2) {
        return 10;
    } else if (data[thisOne + 1] == -1) {
        return getNext(data, thisOne + 1);
    } else {
        return data[thisOne + 1];
    }
}
