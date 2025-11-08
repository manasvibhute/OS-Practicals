#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

int binarySearch(int arr[], int n, int target){
    int left = 0, right = n-1;
    while(left <= right){
        int mid = left + (right - left)/2;
        if(arr[mid] < target){
            left = mid + 1;
        }else if(arr[mid] > target){
            right = mid - 1;
        }else{
            return mid;
        }
    }
    return -1;
}

int main(int argc, char *argv[]){
    printf("This is the child file\n");
    
    if(argc <= 1){
        printf("Usage %s <numbers>\n", argv[0]);
        return 1;
    }
    
    int arr[argc-1];
    for(int i=1; i<argc-1; i++){
        arr[i-1] = atoi(argv[i]);
    }
    
    int search;
    printf("\nEnter the number to search: ");
    scanf("%d", &search);
    int bs = binarySearch(arr, argc, search);
    if(bs == -1){
        printf("Target not found");
    }else{
        printf("Target %d found at %d", search, bs);
    }
    return 0;
}