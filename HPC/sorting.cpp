#include<iostream>
#include<vector>
#include<omp.h> // OpenMP header for parallel computing
using namespace std;

// Function to perform bubble sort
void bubble(int a[],int n){
    int i,j;
    bool swapped=true;
    while(swapped){
        swapped=false;
        bool lswapped=false;
        // Parallelize the loop using OpenMP
        #pragma omp parallel for shared(a,n) private(i,j) reduction(||:lswapped)
        for(int i=0;i<n-1;i++){
            if(a[i]>a[i+1]){
                swap(a[i],a[i+1]);
                lswapped=true;
            }
        }
        swapped=lswapped||swapped;
    }
}

// Function to merge two sorted subarrays
void merge(int a[],int left,int mid,int right){
    int n1=mid-left+1;
    int n2=right-mid;
    int leftA[n1],rightA[n2];
    for(int i=0;i<n1;i++) leftA[i]=a[left+i];
    for(int i=0;i<n2;i++) rightA[i]=a[mid+1+i];
    
    int i=0,j=0,k=left;
    while(i<n1 && j<n2){
        if(leftA[i]<=rightA[j]){
            a[k]=leftA[i];
            i++;
        }
        else{
            a[k]=rightA[j];
            j++;
        }
        k++;
    }
    while(i<n1){
        a[k]=leftA[i];
        i++;
        k++;
    }
    while(j<n2){
        a[k]=rightA[j];
        j++;
        k++;
    }
}

// Function to perform parallel merge sort
void mergeSort(int a[],int left,int right){
    if(left>=right){
        return;
    }
    int mid=left+(right-left)/2;
    
    #pragma omp parallel sections
    {
        // Parallelize the recursive calls to mergeSort using OpenMP sections
        #pragma omp section
        mergeSort(a,left,mid); // Recursively sort left subarray
        
        #pragma omp section
        mergeSort(a,mid+1,right); // Recursively sort right subarray
        
    }
    merge(a,left,mid,right); // Merge the sorted subarrays
}

// Function to print the sorted array
void okPrint(int a[],int n){
    cout<<"\nTHE SORTED ARRAY IS: "<<endl;
    for(int i=0;i<n;i++){
        cout<<a[i]<<" ";
    }
}

int main(){
    int n;
    cout<<"\nEnter number of elements into array:"; cin>>n;
    cout<<"\nEnter array elements:-\n";
    int vec[n],cores;
    for(int i=0;i<n;i++){
        cin>>vec[i];
    }
    
    cout<<"\nEnter no of cores to use:- ";
    cin>>cores;
    while(1){
        cout<<"\n1.BUBBLE SORT";
        cout<<"\n2.MERGE SORT";
        cout<<"\n3.QUIT";
        cout<<"\nEnter choice:";int c;
        cin>>c;
        switch(c){
            case 1: 
            {
                double time=omp_get_wtime();
                bubble(vec,n); // Call bubble sort function
                double sTime=omp_get_wtime()-time;
                cout<<"\nSequential Time taken is: "<<sTime;
                
                time=omp_get_wtime();
                bubble(vec,n); // Call bubble sort function again
                double pTime=omp_get_wtime()-time;
                cout<<"\nParallel Time taken is: "<<pTime;
                
                okPrint(vec,n); // Print the sorted array
                break;
            }
            case 2: 
            {
                double time=omp_get_wtime();
                mergeSort(vec,0,n-1); // Call merge sort function
                double sTime=omp_get_wtime()-time;
                cout<<"\nSequential time taken:- "<<sTime;
                
                time=omp_get_wtime();
                mergeSort(vec,0,n-1); // Call merge sort function again
                double pTime=omp_get_wtime()-time;
                cout<<"\nParallel time taken:- "<<pTime;
                
                okPrint(vec,n); // Print the sorted array
                break;
            }
            case 3: exit(1); // Exit the program
        }
    }
}

