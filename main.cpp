#include <bits/stdc++.h>
#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
using namespace std;


//function declarations
void run1thread();
void create2threads();
void create3threads();
void create4threads();
void create5threads();
void create6threads();
void create7threads();
void create8threads();
void create9threads();
void create10threads();
void fill_numbers(int N);
void print_values(double duration);
float median(vector<int> numbers, int l, int r);
void runthreads(int N);

vector<int> numbers;// holds numbers

//holds statistical values
int min_;
int max_;
int range_;
int mode_;
double median_;
long long int sum_;
double mean_;
double harmonic_mean_;
double standard_dev_;
double interquartile_range_;



int main(int argc, char* argv[]){
    //take thread count as program argument
    if(argc < 3) {cout << "Please specify the thread count"<< endl; return 0;}

    int N = atoi(argv[1]);
    int threadN = atoi(argv[2]);
    //create random numbers
    fill_numbers(N);
    //run functions and threads
    auto start = chrono::high_resolution_clock::now();
    runthreads(threadN);
    chrono::duration<double, milli> interval = (chrono::high_resolution_clock::now() - start);
    
    double duration = interval.count();
    print_values(duration);
    
    return 0;
}


//creates random N numbers
void fill_numbers(int N){
    srand(time(NULL));
    for(int i = 0; i< N;i++){
        numbers.push_back(1000+rand()%9000);
    }
}

void runthreads(int threadN){
    if(threadN==0|| threadN ==1) run1thread();
    else if(threadN==2) create2threads();
    else if(threadN==3) create3threads();
    else if(threadN==4) create4threads();
    else if(threadN==5) create5threads();
    else if(threadN==6) create6threads();
    else if(threadN==7) create7threads();
    else if(threadN==8) create8threads();
    else if(threadN==9) create9threads();
    else if(threadN==10) create10threads();
    else {cout << "Please enter thread count 0 to 10!"<<endl;exit(0);}
}



float median(vector<int> array, int l, int r){
    int mid = (l+r)/2;
    int len = r-l+1;
    if(len == 1){return (float) array[mid];}
    if(len%2==1){
        return (float) array[mid];
    }
    return (float) (array[mid]+array[mid+1])/2;
}

void print_values(double duration){
    ofstream myoutput("output.txt");
    myoutput.precision(5);
    myoutput << min_ << endl;
    myoutput << max_ << endl;
    myoutput << range_ << endl;
    myoutput << mode_ << endl;
    myoutput << fixed << median_ << endl;
    myoutput << sum_ << endl;
    myoutput << fixed << mean_ << endl;
    myoutput << fixed << harmonic_mean_ << endl;
    myoutput << fixed << standard_dev_ << endl;
    myoutput << fixed <<interquartile_range_ << endl;
    myoutput << duration/1000 << endl;
    myoutput.close();
}

//calculation functions below
void assign_min(){
    int tmp_min = INT_MAX;
    for(int i= 0; i< numbers.size(); i++){
        tmp_min = min(numbers[i], tmp_min);
    }
    min_ = tmp_min;
}

void assign_max(){
    int tmp_max = 0;
    for(int i =0; i< numbers.size(); i++){
        tmp_max = max(numbers[i], tmp_max);
    }
    max_ = tmp_max;
}

void assign_range(){
    int tmp_min = INT_MAX;
    int tmp_max = 0;
    for(int i= 0; i<numbers.size(); i++ ){
        if(numbers[i]>tmp_max) tmp_max = numbers[i];
        else if (numbers[i]<tmp_min)  tmp_min = numbers[i];
    }
    range_ =tmp_max- tmp_min;
}
void assign_mode(){
    vector<int> arr = numbers;
    sort(arr.begin(), arr.end());
    int number = arr[0];
    int tmp_mode = number;
    int ans= arr[0];
    int max_occur =1;
    int tmp=1;
    for(int i=1; i< numbers.size(); i++){
        if(arr[i]==number){
            tmp++;
        }
        else {
            if(tmp > max_occur){
                max_occur = tmp;
                tmp_mode = number;
            }
            tmp=1;
            number=arr[i];
        }
    }
    mode_ = tmp_mode;

}

void assign_median(){
    vector<int> tmp = numbers;
    sort(tmp.begin(), tmp.end());
    int N = tmp.size();
    median_ = median(tmp, 0, N - 1);
}

void assign_sum(){
    long long int tmp = 0;
    for(int i = 0; i< numbers.size(); i++){
        tmp+=numbers[i];
    }
    sum_ = tmp;
}

void assign_mean(){
    int N = numbers.size();
    long long int sum=0;
    for(int i=0; i< numbers.size(); i++){
        sum+=numbers[i];
    }
    mean_ = (double) sum / (double) N;
}

void assign_harmonic_mean(){
    float tmp_harmonic_sum = 0;
    int N = numbers.size();
    for(int i = 0; i< N; i++){
        tmp_harmonic_sum+= 1/ (float) numbers[i];
    }
    harmonic_mean_ = N /tmp_harmonic_sum;
}

void assign_standard_dev(){
    double tmp =0;
    int N = numbers.size();
    long long int tmp_sum=0;
    for(int i=0; i< numbers.size(); i++){
        tmp_sum+=numbers[i];
    }
    double tmp_mean = (double) tmp_sum / (double) N;
    for(int i=0; i< N; i++){
        tmp += pow((numbers[i] - tmp_mean), 2);
    }
    tmp /= (N-1);
    standard_dev_=sqrt(tmp);
}

void assign_IQR(){
    float first_quartile;
    float third_quartile;
    int N = numbers.size();
    vector<int> tmp = numbers;
    sort(tmp.begin(), tmp.end());
    if(N%2==0){
        first_quartile = median(tmp, 0, N/2-1);
        third_quartile = median(tmp, N/2, N-1);
    } else {
        first_quartile = median(tmp, 0, N/2-1);
        third_quartile = median(tmp, N/2+1, N-1);
    }
    interquartile_range_ = third_quartile - first_quartile;
}
//calculation functions above

void *min_thread(void* param){
    assign_min();
    pthread_exit(0);
}

void *max_thread(void* param){
    assign_max();
    pthread_exit(0);
}

void *range_thread(void* param){
    assign_range();
    pthread_exit(0);
}

void *mode_thread(void* param){
    assign_mode();
    pthread_exit(0);
}

void *median_thread(void* param){
    assign_median();
    pthread_exit(0);
}

void *sum_thread(void* param){
    assign_sum();
    pthread_exit(0);
}

void *mean_thread(void* param){
    assign_mean();
    pthread_exit(0);
}

void *harmonic_mean_thread(void* param){
    assign_harmonic_mean();
    pthread_exit(0);
}

void *standard_dev_thread(void* param){
    assign_standard_dev();
    pthread_exit(0);
}

void *IQR_thread(void* param){
    assign_IQR();

    pthread_exit(0);
}

void *min_max_thread(void* param){
    assign_min();
    assign_max();
    pthread_exit(0);
}

void *mode_median_thread(void* param){
    assign_mode();
    assign_median();
    pthread_exit(0);
}

void *mean_sum_thread(void* param){
    assign_sum();
    assign_mean();
    pthread_exit(0);
}

 
void *harmonic_iqr_thread(void* param){
    assign_harmonic_mean();
    assign_IQR();
    pthread_exit(0);
}

void *dev_range_thread(void* param){
    assign_standard_dev();
    assign_range();
    pthread_exit(0);
}


void *fiveoperation1(void* param){
    assign_min();
    assign_max();
    assign_range();
    assign_mode();
    assign_median();
    pthread_exit(0);
}

void *fiveoperation2(void* param){
    assign_sum();
    assign_mean();
    assign_standard_dev();
    assign_harmonic_mean();
    assign_IQR();
    pthread_exit(0);
}


void *fouroperation1(void* param){
    assign_min();
    assign_max();
    assign_range();
    assign_median();
    pthread_exit(0);
}

//sum, mean, standard dev
void *threeoperation1(void* param){
    assign_sum();
    assign_mean();
    assign_standard_dev();
    pthread_exit(0);
}
//mode, iqr, harmonic_mean
void *threeoperation2(void* param){
    assign_mode();
    assign_IQR();
    assign_harmonic_mean();
    pthread_exit(0);
}

//min, max, range
void *threeoperation3(void* param){
    assign_min();
    assign_max();
    assign_range();
    pthread_exit(0);
}

// run all operations
void run1thread(){
    assign_min();
    assign_max();
    assign_range();
    assign_mode();
    assign_median();
    assign_sum();
    assign_mean();
    assign_harmonic_mean();
    assign_standard_dev();
    assign_IQR();
}


//min, max, range, mode, median - sum, mean, standard dev, harmonic mean, iqr
void create2threads(){
    pthread_t tids[2];
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    pthread_create(&tids[0], &attr, fiveoperation1, NULL);
    pthread_create(&tids[1], &attr, fiveoperation2, NULL);

    pthread_join(tids[0], NULL);
    pthread_join(tids[1], NULL);

}




//min,  max, range, median -  sum, mean, standard dev -  mode, iqr, harmonic mean
void create3threads(){
    pthread_t tids[3];
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    pthread_create(&tids[0], &attr, fouroperation1, NULL);
    pthread_create(&tids[1], &attr, threeoperation1, NULL);
    pthread_create(&tids[2], &attr, threeoperation2, NULL);

    pthread_join(tids[0], NULL);
    pthread_join(tids[1], NULL);
    pthread_join(tids[2], NULL);

}

//min, max, range - sum, mean, standard dev - median, mode - iqr, harmonic mean
void create4threads(){
    pthread_t tids[4];
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    pthread_create(&tids[2], &attr, mode_median_thread, NULL); //mode, median 
    pthread_create(&tids[3], &attr, harmonic_iqr_thread, NULL);//harmonic, iqr 
    pthread_create(&tids[0], &attr, threeoperation3, NULL);    //min, max, range
    pthread_create(&tids[1], &attr, threeoperation1, NULL);    //sum, mean, standard dev

    pthread_join(tids[0], NULL );
    pthread_join(tids[1], NULL );
    pthread_join(tids[2], NULL );
    pthread_join(tids[3], NULL );


}

//operations run on 5 threads
void create5threads(){
    pthread_t tids[5];
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    pthread_create(&tids[0], &attr, min_max_thread, NULL);
    pthread_create(&tids[1], &attr, mean_sum_thread, NULL);
    pthread_create(&tids[2], &attr, mode_median_thread, NULL);
    pthread_create(&tids[3], &attr, harmonic_iqr_thread, NULL);
    pthread_create(&tids[4],&attr, dev_range_thread, NULL);

    //join threads
    for(int i = 0; i< 5; i++){
        pthread_join(tids[i], NULL);
    }

}

void create6threads(){
    pthread_t tids[6];
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    pthread_create(&tids[0], &attr, min_max_thread, NULL);
    pthread_create(&tids[1], &attr, dev_range_thread, NULL);
    pthread_create(&tids[2], &attr, mode_median_thread, NULL);
    pthread_create(&tids[3], &attr, mean_sum_thread, NULL);
    pthread_create(&tids[4], &attr, IQR_thread, NULL);
    pthread_create(&tids[5], &attr, harmonic_mean_thread, NULL);

    //join threads
    for(int i = 0; i< 6; i++){
        pthread_join(tids[i], NULL);
    }
}

void create7threads(){
    pthread_t tids[7];
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    pthread_create(&tids[0], &attr, min_max_thread, NULL);
    pthread_create(&tids[1], &attr, dev_range_thread, NULL);
    pthread_create(&tids[2], &attr, mode_median_thread, NULL);
    pthread_create(&tids[3], &attr, mean_thread, NULL);
    pthread_create(&tids[4], &attr, IQR_thread, NULL);
    pthread_create(&tids[5], &attr, harmonic_mean_thread, NULL);
    pthread_create(&tids[6], &attr, sum_thread, NULL);
    //join threads
    for (int i = 0 ; i< 7; i++){
        pthread_join(tids[i], NULL);
    }
}
void create8threads(){
    pthread_t tids[8];
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    pthread_create(&tids[0], &attr, min_max_thread, NULL);
    pthread_create(&tids[1], &attr, dev_range_thread, NULL);
    pthread_create(&tids[2], &attr, mode_thread, NULL);
    pthread_create(&tids[3], &attr, mean_thread, NULL);
    pthread_create(&tids[4], &attr, IQR_thread, NULL);
    pthread_create(&tids[5], &attr, harmonic_mean_thread, NULL);
    pthread_create(&tids[6], &attr, sum_thread, NULL);
    pthread_create(&tids[7], &attr, median_thread, NULL);
    //join threads
    for (int i = 0 ; i< 8; i++){
        pthread_join(tids[i], NULL);
    }
}

void create9threads(){
    pthread_t tids[9];
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    pthread_create(&tids[0], &attr, min_max_thread, NULL);
    pthread_create(&tids[1], &attr, standard_dev_thread, NULL);
    pthread_create(&tids[2], &attr, mode_thread, NULL);
    pthread_create(&tids[3], &attr, mean_thread, NULL);
    pthread_create(&tids[4], &attr, IQR_thread, NULL);
    pthread_create(&tids[5], &attr, harmonic_mean_thread, NULL);
    pthread_create(&tids[6], &attr, sum_thread, NULL);
    pthread_create(&tids[7], &attr, median_thread, NULL);
    pthread_create(&tids[8], &attr, range_thread, NULL);

    //join threads
    for (int i = 0 ; i< 9; i++){
        pthread_join(tids[i], NULL);
    }
}



void create10threads(){
    pthread_t tids[10];
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    pthread_create(&tids[9], &attr, IQR_thread, NULL);
    pthread_create(&tids[4], &attr, median_thread, NULL);
    pthread_create(&tids[0], &attr, min_thread, NULL);
    pthread_create(&tids[1], &attr, max_thread, NULL);
    pthread_create(&tids[2], &attr, range_thread, NULL);
    pthread_create(&tids[3], &attr, mode_thread, NULL);

    pthread_create(&tids[5], &attr, sum_thread, NULL);
    pthread_create(&tids[6], &attr, mean_thread, NULL);
    pthread_create(&tids[7], &attr, harmonic_mean_thread, NULL);
    pthread_create(&tids[8], &attr, standard_dev_thread, NULL);

    //join threads
    for(int i = 0; i< 10; i++){
        pthread_join(tids[i], NULL);
    }

}