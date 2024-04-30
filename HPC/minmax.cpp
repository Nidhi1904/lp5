#include<iostream>
#include<omp.h> // OpenMP library for parallel programming
#include<vector>
using namespace std;

#define Array_Size 1000; // Defining array size as a constant

int main() {
    int i, cores, n; 
    vector<double> arr; // Declare a vector to store array elements
    double sum = 0.0, avg = 0.0, min, max; 
    double ss, sp, es, ep; // Variables to store start and end times for sequential and parallel execution
    cout << "\nEnter size of array:- \n";
    cin >> n; // Input the size of the array
    if (n <= 0) {
        cout << "\nEnter valid size!"; // Check if the size is valid
        return 1; // Return with an error code if size is invalid
    }
    arr.resize(n); // Resize the vector to accommodate 'n' elements
    cout << "\nEnter array elements" << endl;
    for (int i = 0; i < n; i++) {
        cin >> arr[i]; // Input array elements
    }
    cout << "\nEnter no of threads to use: ";
    cin >> cores; // Input the number of threads to use for parallelization
    ss = omp_get_wtime(); // Get start time for sequential execution
    sum = 0.0;
    avg = 0.0; 
    min = arr[0]; 
    max = arr[0]; 
    for (int i = 0; i < n; i++) {
        sum += arr[i]; // Calculate sum of array elements
        if (arr[i] < min) {
            min = arr[i]; // Update minimum if current element is less than min
        }
        if (arr[i] > max) {
            max = arr[i]; // Update maximum if current element is greater than max
        }
    }
    avg = sum / n; // Calculate average
    es = omp_get_wtime(); // Get end time for sequential execution
    cout << "Sequential sum=" << sum << endl;
    cout << "Sequential average=" << avg << endl;
    cout << "Sequential minimum=" << min << endl;
    cout << "Sequential maximum=" << max << endl;
    cout << "Sequential execution time=" << es - ss << " secs " << endl;
    
    // RESET VARIABLES
    
    sp = omp_get_wtime(); // Get start time for parallel execution
    sum = 0.0; // Reset sum to 0
    min = arr[0]; // Reset min to the first element of the array
    max = arr[0]; // Reset max to the first element of the array
    #pragma omp parallel num_threads(cores) shared(arr, min, max) private(i) // Start parallel region with specified number of threads
    {
    
    	//omp_set_num_threads(cores);
        double local_sum = 0; // Declare local_sum variable to hold partial sum for each thread
        double local_min = arr[0]; // Declare local_min variable to hold partial minimum for each thread
        double local_max = arr[0]; // Declare local_max variable to hold partial maximum for each thread
        #pragma omp for // Distribute loop iterations among threads
        for (int i = 0; i < n; i++) {
            local_sum += arr[i]; // Calculate partial sum for each thread
            if (arr[i] < min) {
                local_min = arr[i]; // Update partial minimum if current element is less than min
            }
            if (arr[i] > max) {
                local_max = arr[i]; // Update partial maximum if current element is greater than max
            }
        }
        #pragma omp atomic // Ensure atomic update of sum
        sum += local_sum; // Accumulate partial sums into the global sum
        #pragma omp critical // Ensure critical section for updating min
        if (local_min < min) {
            min = local_min; // Update min if local_min is less than min
        }
        #pragma omp critical // Ensure critical section for updating max
        if (local_max > max) {
            max = local_max; // Update max if local_max is greater than max
        }
        //int thread_id = omp_get_thread_num();
        //cout<<omp_get_thread_num()
    } // End of parallel region
    avg = sum / n; // Calculate average
    ep = omp_get_wtime(); // Get end time for parallel execution
    cout << endl;
    cout << "Parallel Sum=" << sum << endl;
    cout << "Parallel average=" << avg << endl;
    cout << "Parallel minimum=" << min << endl;
    cout << "Parallel maximum=" << max << endl;
    cout << "Parallel execution time=" << ep - sp << " secs " << endl;
    return 0;
}

