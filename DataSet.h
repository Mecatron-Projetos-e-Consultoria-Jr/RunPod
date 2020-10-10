#include <array>

template<typename T, int size>
class Data_Set{

    private:
        int n_elements = 0;
        std::array<T,size> data;
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
};