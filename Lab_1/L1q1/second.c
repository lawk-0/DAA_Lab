// wap to find second smallest and second largest of array of n integers

#include<stdio.h>

int main()
{
    int n;
    scanf("%d",&n);
    int arr[n];
    for(int i=0;i<n;i++)scanf("%d",&arr[i]);
    int min=arr[0],max=arr[0],sec_min=arr[n-1],sec_max=arr[0];
    for(int i=0;i<n;++i){if(min>arr[i])min=arr[i];if(max<arr[i])max=arr[i];}
    for(int i=0;i<n;++i){
        if(arr[i]>sec_max && arr[i]<max){
            sec_max=arr[i];
        }
        if(arr[i]<sec_min && arr[i]>min){
            sec_min=arr[i];
        }
    }
    printf("Second smallest: %d\n", sec_min);
    printf("Second largest: %d\n", sec_max);
    return 0;
}