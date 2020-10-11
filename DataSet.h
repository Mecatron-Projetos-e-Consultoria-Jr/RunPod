#include <array>

template<typename T, int size>
class Data_Set{

    private:
        int n_elements = 0;
        std::array<T,size> data;

        // returns the module of the number
        inline T module(T data_point){
            if(data_point<0)
                return -1*data_point;
            else
                return data_point;
        }

    public:
        
        // Add the data_point to the end of the data_set and removes the first element if above the desired
        // size
        void push_back(T data_point){

            // If it did not reach its limit yet, just add the new datapoint to the next empty slot
            if(n_elements < size){
                data[n_elements] = data_point;
                
                // update the number of elements 
                n_elements++;
            }
            else{
                // Variables used to create the new array
                int counter = 0;    //* Counter used as index for the arrays
                std::array<T,size> new_array;  //* New array that will hold all the previous values(minus the first) + new datapoint

                // Iterate through the old data array
                for(auto& element:data){

                    // if we are at the last value from the old array, add the new datapoint to the new array 
                    if(counter+1 == size){
                        new_array[counter] = data_point;
                    }

                    // else, add the next element from the old array
                    else{
                        new_array[counter] = data[counter+1];
                    }
                    
                    // update the index 
                    counter++;
                }

                // Update the data array to the new array
                data = new_array;
            }
        }

        // Find the datapoint with greatest absolute value 
        T max_value(){
            T max = 0;

            // Iterate though the data set
            for(auto& data_point:data){
                // If the absolute value of the data_point is greater than the absolute value of the previous
                // max, set the data_point as current max
                if(module(data_point) > module(max)){
                    max = data_point;
                }
            }

            return max;
        }

        // Find the average for the data set
        double average(T divider){
            T sum = 0;
            for(auto&data_point:data){
                sum+=data_point;
            }
            return sum/divider;
        }
};
