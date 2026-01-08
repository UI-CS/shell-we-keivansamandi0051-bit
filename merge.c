#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

void merge(int a[], int l, int m, int r) {
    int i=l,j=m+1,k=0;
    int tmp[r-l+1];

    while(i<=m && j<=r)
        tmp[k++] = (a[i]<a[j]) ? a[i++] : a[j++];

    while(i<=m) tmp[k++] = a[i++];
    while(j<=r) tmp[k++] = a[j++];

    for(i=l,k=0;i<=r;i++,k++)
        a[i]=tmp[k];
}

void p_mergesort(int a[], int l, int r) {
    if(l>=r) return;

    int m = (l+r)/2;
    pid_t p1 = fork();

    if(p1==0) {
        p_mergesort(a,l,m);
        _exit(0);
    }

    pid_t p2 = fork();
    if(p2==0) {
        p_mergesort(a,m+1,r);
        _exit(0);
    }

    wait(NULL);
    wait(NULL);
    merge(a,l,m,r);
}

int main() {
    int arr[] = {9,4,7,2,1,6,5,8,3};
    int n = 9;

    p_mergesort(arr,0,n-1);

    for(int i=0;i<n;i++)
        printf("%d ",arr[i]);

    return 0;
}
