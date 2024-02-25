/*!
 * @brief Several types of sorting algorithms that work on a data range.
 * @author Tobias dos Santos Neto e Wisla Alves Argolo
 * @date May 20st, 2023.
 * @file sorting.h
 */

#ifndef SORTING_H
#define SORTING_H

#include <sstream>
using std::ostringstream;
#include <iterator>
using std::next;
using std::ostream_iterator;
#include <functional>
using std::function;
using std::less;
#include <vector>
using std::vector;
#include <array>
using std::array;
#include <algorithm>
using std::copy;
using std::for_each;
using std::max_element;
#include <cmath>
using std::pow;
#include <string>
using std::string;
using std::to_string;

namespace sa
{ // sa = sorting algorithms
  /// Prints out the range to a string and returns it to the client.
  template <typename FwrdIt>
  std::string to_string(FwrdIt first, FwrdIt last)
  {
    std::ostringstream oss;
    oss << "[ ";
    while (first != last)
    {
      oss << *first++ << " ";
    }
    oss << "]";
    return oss.str();
  }

  //{{{ RADIX SORT
  /*!
   * @brief Implements the Radix Sorting Algorithm based on the less
   * significant digit (LSD).
   *
   * @tparam DataType The type of elements that are being sorted.
   * @tparam Comparator A Comparator type function that returns true if the first
   * argument is less than the second argument.
   *
   * @param first Pointer/iterator to the beginning of the range we wish to sort.
   * @param last Pointer/iterator to the location just past the last valid value
   * of the range we wish to sort.
   * @param unused An unused parameter of Comparator type.
   */
  template <typename DataType, typename Comparator>
  void radix(DataType *first, DataType *last, Comparator /*unused*/)
  {
    // Calculates the number of digits of the largest element in the range
    size_t n_digits = std::to_string(*std::max_element(first, last)).size();

    // Iterates over all digits
    for (size_t i = 0; i < n_digits; i++)
    {
      std::array<std::vector<DataType>, 10> buckets; // one bucket for each digit from 0 to 9
      // For each number in the range, calculate the index of the bucket
      // based on the current digit and place the number in that bucket
      for (auto p = first; p != last; p++)
      {
        int index = (int)(*p / std::pow(10, i)) % 10;
        buckets[index].push_back(*p);
      }

      // Copy the numbers from the buckets back to the original range,
      // preserving the order of the elements within each bucket
      auto it = first;
      for (auto &bucket : buckets)
        it = std::copy(bucket.begin(), bucket.end(), it);
    }
  }
  //}}} RADIX SORT

  //{{{ INSERTION SORT
  /*!
   * @brief Implements the Insertion Sort algorithm.
   *
   * @tparam DataType The type of elements that are being sorted.
   * @tparam Comparator A Comparator type function that returns true if the first
   * argument is less than the second argument.
   *
   * @param first Pointer/iterator to the beginning of the range we wish to sort.
   * @param last Pointer/iterator to the location just past the last valid value
   * of the range we wish to sort.
   * @param cmp A Comparator function object to determine the relative order of elements.
   */
  template <typename DataType, typename Compare>
  void insertion(DataType *first, DataType *last, Compare cmp)
  {
    // Create a pointer starting from second position to iterate over the array
    auto fast = first + 1;

    while (fast != last)
    {
      auto runner = fast;

      // Running until it reaches the start or when the previous
      // element is greater than itself
      while (runner != first && cmp(*runner, *(runner - 1)))
      {
        // Swaps the lesser element to a closer position
        std::swap(*runner, *(runner - 1));
        runner--;
      }
      fast++;
    }
  }
  //}}} INSERTION SORT

  //{{{ SELECTION SORT
  /*!
   * @brief Implements the Selection Sort algorithm.
   *
   * @tparam DataType The type of elements that are being sorted.
   * @tparam Comparator A Comparator type function that returns true if the first
   * argument is less than the second argument.
   *
   * @param first Pointer/iterator to the beginning of the range we wish to sort.
   * @param last Pointer/iterator to the location just past the last valid value
   * of the range we wish to sort.
   * @param cmp A Comparator function object to determine the relative order of elements.
   */
  template <typename DataType, typename Compare>
  void selection(DataType *first, DataType *last, Compare cmp)
  {

    // Traverse the interval from left to right (last - first - 1) times
    for (auto i = first; i != (last - 1); i++)
    {
      auto pMin = i;

      // Find the smallest element in the interval between the remaining n-i elements
      for (auto j = i + 1; j != last; j++)
      {
        if (cmp(*j, *pMin))
          pMin = j;
      }
      std::swap(*i, *pMin); // Insert the found element in the correct position
    }
  }
  //}}} SELECTION SORT

  //{{{ BUBBLE SORT
  /*!
   * @brief Implements the classic version Bubble Sort algorithm.
   *
   * @tparam DataType The type of elements that are being sorted.
   * @tparam Comparator A Comparator type function that returns true if the first
   * argument is less than the second argument.
   *
   * @param first Pointer/iterator to the beginning of the range we wish to sort.
   * @param last Pointer/iterator to the location just past the last valid value
   * of the range we wish to sort.
   * @param cmp A Comparator function object to determine the relative order of elements.
   */
  template <typename DataType, typename Compare>
  void bubble(DataType *first, DataType *last, Compare cmp)
  {

    // Iterates over the array n-1 times starting from the second position
    for (auto i = first + 1; i != last; i++)
    {

      // Iterates over the array n-1 times ending in the second last position
      for (auto j = first; j != last - 1; j++)
      {

        // Swaps the values when the lesser one becomes after the greater one
        if (cmp(*(j + 1), *j))
        {
          std::swap(*j, *(j + 1));
        }
      }
    }
  }
  //}}} BUBBLE SORT

  //{{{ SHELL SORT
  /*!
   * @brief Implements the classic version Shell Sort algorithm.
   *
   * @tparam DataType The type of elements that are being sorted.
   * @tparam Comparator A Comparator type function that returns true if the first
   * argument is less than the second argument.
   *
   * @param first Pointer/iterator to the beginning of the range we wish to sort.
   * @param last Pointer/iterator to the location just past the last valid value
   * of the range we wish to sort.
   * @param cmp A Comparator function object to determine the relative order of elements.
   */
  template <typename DataType, typename Compare>
  void shell(DataType *first, DataType *last, Compare cmp)
  {
    int size = std::distance(first, last);
    int gap = size / 2; // Set the initial value of gap to half the size of the interval

    // While gap is greater than 0, continue the sorting process
    while (gap > 0)
    {
      // Iterate over the interval, starting from the element at position of the gap
      for (auto *i = first + gap; i != last; i++)
      {
        auto j = i - gap;
        auto key = *i;

        // Compare the elements with a distance of "gap" positions while j not exceeds the
        // limits of the interval and the element in "key" is not in the correct position
        while (j >= first && cmp(key, *j))
        {
          *(j + gap) = *j;
          j -= gap;
        }
        // Insert the element "key" in the correct position
        *(j + gap) = key;
      }
      gap /= 2; // Reduce the gap at each iteration
    }
  }
  //}}} SHELL SORT

  /*!
   * @brief Merges two sorted ranges into one.
   *
   * @tparam DataType The type of elements that are being sorted.
   * @tparam Compare A Comparator type function that returns true if the first
   * argument is less than the second argument.
   *
   * @param l_first Pointer/iterator to the beginning of the first sorted range.
   * @param l_last Pointer/iterator to the location just past the last valid value of the first range.
   * @param r_first Pointer/iterator to the beginning of the second sorted range.
   * @param r_last Pointer/iterator to the location just past the last valid value of the second range.
   * @param first Pointer/iterator to the beginning of the original range where the merged output is stored.
   * @param cmp A Comparator function object to determine the relative order of elements.
   */
  template <typename DataType, typename Compare>
  // [l_first; l_last)
  // [r_first; r_last)
  void merging(DataType *l_first, DataType *l_last, DataType *r_first,
               DataType *r_last, DataType *first, Compare cmp)
  {

    // While there are elements in both left and right intervals
    while (l_first != l_last && r_first != r_last)
    {
      // If the element from the left interval is smaller, copy it to the original interval
      if (cmp(*l_first, *r_first))
      {
        *first = *l_first;
        l_first++;
      }
      else
      {
        // Otherwise, copy the element from the right interval
        *first = *r_first;
        r_first++;
      }
      first++;
    }

    // If there are still elements in the left interval, copy them to the original interval
    while (l_first != l_last)
    {
      *first = *l_first;
      l_first++;
      first++;
    }

    // If there are still elements in the right interval, copy them to the original interval
    while (r_first != r_last)
    {
      *first = *r_first;
      r_first++;
      first++;
    }
  }

  //{{{ MERGE SORT
  /*!
   * @brief Implements the Merge Sort algorithm.
   *
   * @tparam DataType The type of elements that are being sorted.
   * @tparam Comparator A Comparator type function that returns true if the first
   * argument is less than the second argument.
   *
   * @param first Pointer/iterator to the beginning of the range we wish to sort.
   * @param last Pointer/iterator to the location just past the last valid value
   * of the range we wish to sort.
   * @param cmp A Comparator function object to determine the relative order of elements.
   */
  template <typename DataType, typename Compare>
  void merge(DataType *first, DataType *last, Compare cmp)
  {

    int size = std::distance(first, last);

    // If there are two or more elements, sort the interval
    if (size >= 2)
    {
      auto m = size / 2;

      // Calculate the sizes of the left and right intervals
      auto n1 = m;
      auto n2 = size - m;

      // Create temporary arrays for the left and right intervals
      DataType *left = new DataType[n1];
      DataType *right = new DataType[n2];

      // Copy the elements to the temporary arrays
      std::copy(first, first + n1, left);
      std::copy(first + n1, last, right);

      // Sort the left and right intervals
      merge(right, right + n2, cmp);
      merge(left, left + n1, cmp);
      // Merge the sorted intervals into the original interval
      merging(left, left + n1, right, right + n2, first, cmp);

      // Free the memory of the temporary arrays
      delete[] left;
      delete[] right;
    }
  }
  //}}} MERGE SORT

  /*!
   * @brief Implements the median-of-three pivot selection strategy for quicksort.
   *
   * @tparam DataType The type of elements that are being sorted.
   * @tparam Compare A Comparator type function that returns true if the first
   * argument is less than the second argument.
   *
   * @param first Pointer/iterator to the beginning of the range we wish to sort.
   * @param last Pointer/iterator to the location just past the last valid value of the range.
   * @param cmp A Comparator function object to determine the relative order of elements.
   * @return Pointer/iterator to the element selected as pivot.
   */
  template <typename DataType, typename Compare>
  DataType *median_of_three(DataType *first, DataType *last, Compare cmp)
  {
    // Apply the pivot selection strategy of median of three to avoid stack overflow
    // in case the array is already sorted.
    auto mid = first + std::distance(first, last) / 2;
    auto last_v = last - 1;

    if (cmp(*mid, *first))
      std::swap(*mid, *first);
    if (cmp(*last_v, *first))
      std::swap(*last_v, *first);
    if (cmp(*last_v, *mid))
      std::swap(*last_v, *mid);

    return mid;
  }

  /*!
   * @brief Partitions a range of elements into two halves based on a pivot.
   *
   * This function uses the median of three strategy to select a pivot and then partitions 
   * the range into two subranges: one with elements less than the pivot and another with 
   * elements greater than or equal to the pivot.
   *
   * @tparam DataType The type of elements that are being sorted.
   * @tparam Compare A Comparator type function that returns true if the first
   * argument is less than the second argument.
   *
   * @param first Pointer/iterator to the beginning of the range we wish to partition.
   * @param last Pointer/iterator to the location just past the last valid value of the range.
   * @param cmp A Comparator function object to determine the relative order of elements.
   * @return Pointer/iterator to the pivot element in the partitioned range.
   */
  template <typename DataType, typename Compare>
  DataType *partition(DataType *first, DataType *last, Compare cmp)
  {

    DataType *pivo = median_of_three(first, last, cmp);
    std::swap(*pivo, *(last - 1)); // Place the pivot value at the last valid position of the range

    auto pivo_v = *(last - 1);

    DataType *pIndex = first; // Stores the correct location of the pivot

    // Partition the range based on the pivot, inserting all elements smaller than
    // the pivot to the left and greater or equal ones to the right
    // Find the correct index for the pivot
    for (auto i = first; i != (last - 1); i++)
    {
      if (cmp(*i, pivo_v))
      {
        std::swap(*pIndex, *i);
        pIndex++;
      }
    }

    // Move the pivot to its correct position
    std::swap(*pIndex, *(last - 1));
    return pIndex;
  }

  //{{{ QUICK SORT
  /*!
   * @brief Implements the Quick Sort algorithm.
   *
   * @tparam DataType The type of elements that are being sorted.
   * @tparam Comparator A Comparator type function that returns true if the first
   * argument is less than the second argument.
   *
   * @param first Pointer/iterator to the beginning of the range we wish to sort.
   * @param last Pointer/iterator to the location just past the last valid value
   * of the range we wish to sort.
   * @param cmp A Comparator function object to determine the relative order of elements.
   */
  template <typename DataType, typename Compare>
  void quick(DataType *first, DataType *last, Compare cmp)
  {

    // If the number of elements in the interval is less than one, no sorting is needed
    if (std::distance(first, last) >= 2)
    {
      DataType *pIndex = partition(first, last, cmp);
      // Recursively sort the left and right subintervals of the pivot
      quick(first, pIndex, cmp);    //[first, pIndex)
      quick(pIndex + 1, last, cmp); //[pIndex + 1, last)
    }
  }
  //}}} QUICK SORT
};     // namespace sa
#endif // SORTING_H
