#include <iostream>
#include <omp.h>
#include <vector>
#include <random>
#include <math.h>

#include <functional>
#include <x86intrin.h>
#include <immintrin.h>

class Point{
    private:
        std::vector<double> vec;
        int d;
        double distance;

        double genRand(){
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(0, 100000);

            return ((double)dis(gen) / 100000.0);
        }
        
        double getRandBig(){
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(0, 60000);

            return ((double)dis(gen) / 100000.0);
        }

        double getRandVeryBig(){
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(0, 50000);

            return ((double)dis(gen) / 100000.0);
        }

    public:

        Point(int dim){

            d = dim;

            //vec.clear();

//            std::srand(static_cast<unsigned int>(time(1)));

            for(int i = 0; i < dim; i++){
                //std::srand(time(0));
                //vec.push_back((double)rand()/RAND_MAX);
                if(dim < 6){ 
                    vec.push_back(genRand());
                }
                else if(dim < 12){
                    vec.push_back(getRandBig());
                }
                else{
                    vec.push_back(getRandVeryBig());
                }
                //std::cout<<vec.at(vec.size()-1);
            }

            distance = 0;

            for(int i = 0; i < dim; i++){
                //std::cout<<vec[i]<<std::endl;
                distance += std::pow(vec[i], 2);
            }

            //std::cout << distance << std::endl;

            distance = std::sqrt(distance);



            //distance = fmod(distance, 1);



            distance = 1 - distance;

            while(distance > 1 || distance < 0){
                Point p(dim);
                distance = p.getDistance();
            }

            //std::cout<< "exited" <<std::endl;
        }

        Point(int dim, int x){

            x++;

            d = dim;

            //vec.clear();

//            std::srand(static_cast<unsigned int>(time(1)));

            for(int i = 0; i < dim; i++){
                //std::srand(time(0));
                //vec.push_back((double)rand()/RAND_MAX);
                if(dim < 6){ 
                    vec.push_back(genRand());
                }
                else if(dim < 12){
                    vec.push_back(getRandBig());
                }
                else{
                    vec.push_back(getRandVeryBig());
                }
                //std::cout<<vec.at(vec.size()-1);
            }

            distance = 0;

            for(int i = 0; i < dim; i++){
                //std::cout<<vec[i]<<std::endl;
                distance += std::pow(vec[i], 2);
            }

            //std::cout << distance << std::endl;

            distance = std::sqrt(distance);



            //distance = fmod(distance, 1);



            distance = 1 - distance;

            while(distance > 1 || distance < 0){
                Point p(dim);
                distance = p.getDistance();
            }

            //std::cout<< "exited" <<std::endl;
        }

        int getVal(int index){
            return vec.at(index);
        }

        double getDistance(){
            return distance;
        }

};


int code(){ 

    std::vector<int> distribution(100, 0);

    for(int dim = 2; dim <= 16; dim++){ // change back to 16 later

        for(int i = 0; i < 10000; i++){

            Point p(dim);

            int index = p.getDistance() * 100;

            //std::cout<<index <<std::endl;

            distribution[index]++;
        }
        
        
        std::cout << "For dimension = " << dim << ":" << std::endl;

        
        for(int i = 0; i < 100; i++){
        std::cout<< "From " << (double)i/100 << " to " << (double)(i+1)/100
                << " there are " << distribution[i] << " number of points" << std::endl;
        } 
        
        

        for(int i = 0; i < 100; i++){
            distribution[i] = 0;
        }
        
    }


    return 0;
}


int codeP(){ 

    std::vector<std::vector<int>> v;

    for(int i = 0; i <= 14; i++){ 
        std::vector<int> distribution(100, 0);
        
        v.push_back(distribution);
    }


    omp_set_num_threads(8);

    #pragma omp parallel for ordered
    for(int dim = 2; dim <= 16; dim++){ // change back to 16 later

        for(int i = 0; i < 10000; i++){

            Point p(dim);

            int index = p.getDistance() * 100;

            //std::cout<<index <<std::endl;

            v[dim-2][index]++;
        }
        
        #pragma omp ordered
        std::cout << "For dimension = " << dim << ":" << std::endl;

        
        
        for(int i = 0; i < 100; i++){
        std::cout<< "From " << (double)i/100 << " to " << (double)(i+1)/100
                << " there are " << v[dim-2][i] << " number of points" << std::endl;
        } 
        
        

        for(int i = 0; i < 100; i++){
            v[dim-2][i] = 0;
        }
        
    }


    return 0;
}




inline auto start_tsc() {
    _mm_lfence();
    auto tsc = __rdtsc();
    _mm_lfence();
    return tsc;
}

inline auto stop_tsc() {
    unsigned int aux;
    auto tsc = __rdtscp(&aux);
    _mm_lfence();
    return tsc;
}

int main(){


    unsigned long long start = start_tsc();

    /*
    1) Figure out what dimension we're in 
    2) Generate random points from within a given dimension (10,000?)
    3) Use euclidean distance formula to determine the distance from the center point
    4) Subtract from radius (radius always = 1)
    5) Store in an array (sizeof(arr) == 100)
    6) Print the contents of the array 
    */
    

    std::cout<< "Run in Sequence: " << std::endl;
    code();
    
    unsigned long long end = stop_tsc();

    //std::cout << double(end - start)/(double(1800000000)) << std::endl;


    start = start_tsc();

    std::cout<< "Run in Parallel: " << std::endl;
    //std::cout<< "Num threads " << omp_get_num_threads() << std::endl;
    codeP();

    end = stop_tsc();

    //std::cout << double(end - start)/(double(1800000000)) << std::endl;


    return 0;   
}
