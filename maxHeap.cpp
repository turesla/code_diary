#include <iostream>
using namespace std;

typedef struct Heap
{
    int capacity;
    int size;
    int *Elem;
}Heap, *HeapQueue;

#define MaxData 32767

//最大堆
HeapQueue init(int maxElem)
{
    HeapQueue H;
    H = (HeapQueue)malloc(sizeof(Heap));
    H->capacity = maxElem;
    H->size = 0;
    H->Elem = (int*)malloc((maxElem+1) * sizeof(int));
    H->Elem[0] = MaxData;
    return H;
}

void InsertMax(int x, HeapQueue H)
{
    int i;
    for (i = ++H->size; H->Elem[i/2] < x; i /= 2)
    {
        H->Elem[i] = H->Elem[i/2];
    }
    H->Elem[i] = x;
}

int DeleteMax(HeapQueue H)
{
    int i, child;
    int MaxElem, LastElem;     //最大元素和最后一个元素
    MaxElem = H->Elem[1];    //1号开始
    LastElem = H->Elem[H->size--];
    for (int i = 1; i * 2 <= H->size; i = child)
    {
        child = i * 2;
        if (child != H->size && H->Elem[child + 1] > H->Elem[child])
        {
            child++;
        }
        if (LastElem < H->Elem[child])
        {
            H->Elem[i] = H->Elem[child];
        }
    }
    H->Elem[i] = LastElem;
    return MaxElem;
}

void MakeEmpty(HeapQueue H)
{
    H->size = 0;
}

int FindMax(HeapQueue H)
{
    return H->Elem[1];
}

void DestoryH(HeapQueue H)
{
    free(H->Elem);
    free(H);
}

int main()
{
    HeapQueue H = Init(4);
    int i;
    for (i = 1; i <= 3; i++)
    {
        InsertMax(i, H);
    }

    int a = DeleteMax(H);

    std::cout<< a << endl;
    return 1;
}







