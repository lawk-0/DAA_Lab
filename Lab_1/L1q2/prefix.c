#include <stdio.h>
int main()
{
    int n;
    scanf("%d", &n);
    int arr[n],arr1[n];
    for(int i = 0; i < n; i++)scanf("%d", &arr[i]);
    arr1[0] = arr[0];
    for(int i = 1; i < n; i++)arr1[i]=arr[i] + arr1[i-1];
    for(int i = 0; i < n; i++)printf("%d ", arr1[i]);
    /* code */
    return 0;
}
