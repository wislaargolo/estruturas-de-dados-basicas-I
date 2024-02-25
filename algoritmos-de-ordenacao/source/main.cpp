/*!
 * @brief Benchmarking suit to compare sorting algorithms under various situations.
 * @file main.cpp
 */

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>
#include <string>
#include <cassert>
#include <algorithm>
#include <functional>
#include <random>
#include <numeric>
using std::function;

#include "lib/sorting.h"

#define pattern 0.0

//=== ALIASES

/// Data type we are working with.
using value_type = long int;
/// Size type.
using size_type = long int;
/// Comparator alias (necessary to create a SortingCollection object).
using Comparator = std::function<bool(const value_type &, const value_type &)>;
/// Alias for duration measurement.
using duration_t = std::chrono::duration<double>;
/// Alias for sorting functions
using Sorting = std::function<void(value_type*, value_type*, Comparator)>;
/// Alias for scenario functions
using ArrayOrganizer = std::function<void(value_type*, value_type*, double)>;


//=== FUNCTION IMPLEMENTATION.

/// Print time difference in an humam readable form.
void print_diff(const std::chrono::time_point<std::chrono::steady_clock> &start,
                const std::chrono::time_point<std::chrono::steady_clock> &end) {
  auto diff = end - start;
  // Milliseconds (10^-3)
  std::cout << "\t\t>>> "
            << std::chrono::duration<double, std::milli>(diff).count() << " ms"
            << std::endl;
  // Nanoseconds (10^-9)
  std::cout << "\t\t>>> "
            << std::chrono::duration<double, std::nano>(diff).count() << " ns"
            << std::endl;
  // Seconds
  auto diff_sec = std::chrono::duration_cast<std::chrono::seconds>(diff);
  std::cout << "\t\t>>> " << diff_sec.count() << " s" << std::endl;
}


/// The running options
struct RunningOptions {
  constexpr static size_t min_sample_sz{100};   //!< Minimum sample size.
  constexpr static size_t max_sample_sz{100000}; //!< Maximum sample size.
  constexpr static int n_samples{25};      //!< The number of samples to collect.
  constexpr static short n_runs{5};       //!< How many runs per average.
  ///Returns the sample size step, based on the `[min,max]` sample sizes and # of samples.
  static size_type sample_step() {
    return static_cast<float>(max_sample_sz - min_sample_sz) / (n_samples - 1);
  }
};

/// Output formatting options.
struct FormattingOptions{
  constexpr static short field_width{10}; //!< Width of the data field.
  constexpr static short precision{3}; //!< Digits length.
};

/// Comparison function for the test experiment.
constexpr bool compare(const int &a, const int &b) { return (a < b); }

/// @brief Struct to define a sorting algorithm.
struct Algorithm{
  std::string name; //!< Name of the sorting algorithm.
  Sorting func; //!< Sorting function.

  /// @brief Constructor to initialize an Algorithm object.
  ///
  /// @param algorithmName Name of the sorting algorithm.
  /// @param sortingFunction Function implementing the sorting algorithm.
  Algorithm(const std::string algorithmName, Sorting sortingFunction)
    : name(algorithmName), func(sortingFunction) {}
};

/// @brief Struct to define a sorting scenario.
struct Scenario{
  std::string name; //!< Name of the scenario.
  ArrayOrganizer func; //!< Function to organize array for the scenario.


  /// @brief Constructor to initialize a Scenario object.
  ///
  /// @param algorithmName Name of the scenario.
  /// @param sortingFunction Function implementing the scenario.
  Scenario(const std::string algorithmName, ArrayOrganizer sortingFunction)
    : name(algorithmName), func(sortingFunction) {}
};

/// @brief Sorting function.
///
/// Sorts an array in ascending order.
///
/// @param first Pointer/iterator to the beginning of the range we wish to sort.
/// @param last Pointer/iterator to the location just past the last valid value
///  of the range we wish to sort.
/// @param unused An unused parameter of double type.
void sorted(value_type* first, value_type* last, double /*unused*/) {
  std::sort(first, last);
}

/// @brief Sorting function.
///
/// Reverses an array, effectively sorting it in descending order.
///
/// @param first Pointer/iterator to the beginning of the range we wish to sort.
/// @param last Pointer/iterator to the location just past the last valid value
///  of the range we wish to sort.
/// @param unused An unused parameter of double type.
void descending(value_type* first, value_type* last, double /*unused*/) {
  std::reverse(first, last);
}

/// @brief Randomizing function.
///
/// Shuffles an array in a random order.
///
/// @param first Pointer/iterator to the beginning of the range to be shuffled.
/// @param last Pointer/iterator to the location just past the last valid value of the range to be shuffled.
/// @param unused An unused parameter of double type.
void all_random(value_type* first, value_type* last, double /*unused*/){
  std::random_device rd;
  std::mt19937 gen(rd());

  std::shuffle(first, last, gen);
}


/// @brief Shuffles a percentage of elements randomly.
///
/// Randomly swaps pairs of elements in a percentage of the total elements in the considered range.
/// @param first Pointer/iterator to the beginning of the range to be shuffled.
/// @param last Pointer/iterator to the location just past the last valid value of the range to be shuffled.
/// @param percent The percentage of elements to be shuffled.
void percent_random(value_type* first, value_type* last, double percent){
  int n = last - first;

  int I[n];

  for(int i = 0; i < n; i++)
      I[i] = i;

    std::random_device rd;
    std::mt19937 gen(rd());

    std::shuffle(I, I + n, gen);

    auto p = percent * n;

    for(int i = 0; i <= p; i+=2)
       std::swap(first[I[i]], first[I[i+1]]);
}

/// @brief This struct represents a collection of sorting algorithms.
///
/// It is used to manage multiple sorting algorithms, allowing you to move through each algorithm 
/// and check if all algorithms have been processed.
struct SortingCollection{
  int curr_algorithm; //!< Used to keep track of the current algorithm in the collection.
  std::vector<Algorithm> algorithms; //!< Stores the collection of algorithms managed by this object.

  /// @brief Constructor to initialize a SortingCollection object.
  ///
  /// @param input Collection of Algorithm objects.
  SortingCollection(std::vector<Algorithm> input) : algorithms(input) {}

  /// @brief Sets the current algorithm index to the first one in the collection.
  void reset(){
    curr_algorithm = 0;
  }

  /// @brief Moves to the next algorithm in the collection.
  void next() {
    curr_algorithm++;
  }

  /// @brief Checks if all algorithms in the collection have been processed.
  ///
  /// @return True if all algorithms have been processed, false otherwise.
  bool has_ended() {
    return curr_algorithm == algorithms.size();
  }

  /// @brief Returns the names of all algorithms in the collection.
  ///
  /// @return A string containing the names of all algorithms.
  std::string name(){
    std::string names;
    for(int i = 0; i < algorithms.size(); i++)
      names += "  " + algorithms[i].name;
    return names;
  }

};

/// @brief Struct to manage multiple sorting scenarios.
///
/// This struct is used to manage different scenarios in a sorting process.
struct DataSet{
  int curr_dataset = 0; //!< Current scenario/dataset being processed
  static const int max_size = 100000; //!< Maximum size of the dataset.
  value_type arr[max_size]; //!< Array containing the data.
  value_type arr_copy[max_size]; //!< Copy of the original array.
  std::vector<Scenario> scenarios; //!< Collection of scenarios.

  int curr_size; //!< Current size of the dataset.

  /// @brief Constructor for the DataSet.
  ///
  /// @param input Collection of Scenario objects.
  DataSet (std::vector<Scenario> input) : scenarios(input) {
    for(int i = 0; i < max_size; i++)
      arr[i] = i + 1;
  }

  /// @brief Sets the current scenario and applies its function to the data set.
  ///
  /// @param size Size of the data set for the current scenario.
  void set_scenario(int size) {
    curr_size = size;

    Scenario organizer = scenarios[curr_dataset];
    if(organizer.name == "ALL_RANDOM" || organizer.name == "ASCENDING_ORDER" || organizer.name == "DESCENDING_ORDER")
      organizer.func(arr, arr + curr_size, pattern);
    else if(organizer.name == "RANDOM_25")
      organizer.func(arr, arr + curr_size, 0.25);
    else if(organizer.name == "RANDOM_50")
      organizer.func(arr, arr + curr_size, 0.50);
    else if(organizer.name == "RANDOM_75")
      organizer.func(arr, arr + curr_size, 0.75);
    
    std::copy(arr, arr + curr_size, arr_copy);
  }

  /// @brief Resets the dataset to the configuration of the current scenario.
  void reset(){
    std::copy(arr_copy, arr_copy + curr_size, arr);
  }

  /// @brief Returns the name of the current scenario.
  ///
  /// @return A string containing the name of the current scenario.
  std::string name_scenario() {
    return scenarios[curr_dataset].name;
  }

  /// @brief Moves to the next scenario in the collection.
  void next(){
    curr_dataset++;
  }

  /// @brief Checks if all scenarios in the collection have been processed.
  ///
  /// @return True if all scenarios have been processed, false otherwise.
  bool has_ended(){
    return curr_dataset == scenarios.size();
  }

  /// @brief Returns a pointer to the beginning of the data set.
  ///
  /// @return Pointer to the beginning of the data set.
  value_type* begin_data() {
    return arr;
  }
  
  /// @brief Returns a pointer to the end of the current data set.
  ///
  /// @return Pointer to the end of the current data set.
  value_type* end_data() {
    return arr + curr_size;
  }

  /// @brief Prints the current data set.
  void imprimir(){
    for(int i=0; i<curr_size; i++){
      std::cout << arr[i] << ' ';
    }
  }
};

//=== CONSTANT DEFINITIONS.

/// Scenario for sorting in ascending order.
Scenario ASCENDING_ORDER {"ASCENDING_ORDER", sorted};

/// Scenario for sorting in descending order.
Scenario DESCENDING_ORDER {"DESCENDING_ORDER", descending};

/// Scenario with all elements randomly arranged.
Scenario ALL_RANDOM {"ALL_RANDOM", all_random};

/// Scenario with 25% of elements randomly arranged.
Scenario RANDOM_25 {"RANDOM_25", percent_random};

/// Scenario with 50% of elements randomly arranged.
Scenario RANDOM_50 {"RANDOM_50", percent_random};

/// Scenario with 75% of elements randomly arranged.
Scenario RANDOM_75 {"RANDOM_75", percent_random};

/// Selection sort algorithm.
Algorithm SELECTION {"SELECTION_SORT", sa::selection<value_type, Comparator>};

/// Insertion sort algorithm.
Algorithm INSERTION {"INSERTION_SORT", sa::insertion<value_type, Comparator>};

/// Bubble sort algorithm.
Algorithm BUBBLE {"BUBBLE_SORT", sa::bubble<value_type, Comparator>};

/// Shell sort algorithm.
Algorithm SHELL {"SHELL_SORT", sa::shell<value_type, Comparator>};

/// Merge sort algorithm.
Algorithm MERGE {"MERGE_SORT", sa::merge<value_type, Comparator>};

/// Quick sort algorithm.
Algorithm QUICK {"QUICK_SORT", sa::quick<value_type, Comparator>};

/// Radix sort algorithm.
Algorithm RADIX {"RADIX_SORT", sa::radix<value_type, Comparator>};


/// @brief The main function, entry point.
/// @param argc The number of command-line arguments.
/// @param argv An array of command-line arguments.
/// @return An integer representing the exit status of the program.
int main( int argc, char * argv[] ){

    /// Initializes a DataSet with multiple scenarios for sorting.
    DataSet dataset ({ASCENDING_ORDER, DESCENDING_ORDER, ALL_RANDOM, RANDOM_25, RANDOM_50, RANDOM_75}); 
    
    ///Initializes RunningOptions which holds the various options for how the sorting algorithm analysis should run.
    RunningOptions run; 

    ///Initializes a SortingCollection with a set of sorting algorithms to be tested on the datasets.
    SortingCollection sort_algs({BUBBLE, SELECTION, INSERTION, RADIX}); 

    // FOR EACH DATA SCENARIO DO...
    while(not dataset.has_ended()){
        // Open the output file for this dataset sceneario.
        std::ofstream out_file;
        out_file.open (dataset.name_scenario());

        // Mark it as the first time through, so we can send out the header before the data lines.
        bool printed_header{false};
        // Sample step defines the increase in size for each new array to sort (sample).
        size_type sample_step{run.sample_step()};
        // Collect data in a linear (arithmetic) scale.
        // FOR EACH SAMPLE SIZE DO...
        for ( auto ns{0} ; ns < run.n_samples ; ++ns ){

          // Calculate the current size of the tested sample
          int current_size = run.min_sample_sz + (sample_step * ns);  
          // Reset the algorithm counter
          sort_algs.reset();

          // Set the analyzed array to the correct size and scenario
          dataset.set_scenario(current_size);

          std::stringstream data_line;
          data_line << current_size;

            // FOR EACH SORTING ALGORITHM DO...
            // Select the first sorting algorithm.
            while (not sort_algs.has_ended()){
                // Reset the duration mean
                duration_t elapsed_time_mean{0.0};
                // Run each algorithm N_RUN times and calculate a sample avarage for each dependent variable.
                // FOR EACH RUN DO...This is necessary to reduce any measurement noise.
                for( auto ct_run(0) ; ct_run < run.n_runs; ++ct_run ) {
                    // Reset the array to its original state before being sorted
                    dataset.reset();

                    // Reset timer
                    auto start = std::chrono::steady_clock::now();
                    //================================================================================
                    sort_algs.algorithms[sort_algs.curr_algorithm].func(dataset.begin_data(), dataset.end_data(), compare);
                    //================================================================================
                    auto end = std::chrono::steady_clock::now();

                    // How long did it take?
                    auto diff( end - start );
                    
                    // -------------------------------------------------------------------------------
                    // Calculating a running (repeatedly updated) sample average.
                    // Updating elapsed time sample mean.
                    elapsed_time_mean = elapsed_time_mean + (diff - elapsed_time_mean ) / static_cast<double>(ct_run+1);
                } // Loop all runs for a single sample size.

                data_line << "  " << std::setw(5) << std::fixed << std::chrono::duration<double, std::milli>(elapsed_time_mean).count();
                
                sort_algs.next();
           } // Loop through all algorithms activated.
            // DATA COLLECTION FOR THIS SAMPLE SIZE (ROW) ENDS HERE.
            // If this is the first time, we must first print the header.
            if ( not printed_header ){
                // Print header here
                out_file << "Tamanho da amostra  Tempo (em ms)" << "\n";
                out_file << sort_algs.name() << "\n";
                printed_header=true;
            }
            // Send out data line to the output file.
            out_file << data_line.str() << '\n';
            // Reset dataline for the next row (sample size).
            data_line.str("");
        } // Loop through each sample size required.
        // Go to the next active scenario.
        dataset.next();
        // Close the file corresponding to this dataset.
        out_file.close();
    } // Loop data scenarios.

    return EXIT_SUCCESS;
}
