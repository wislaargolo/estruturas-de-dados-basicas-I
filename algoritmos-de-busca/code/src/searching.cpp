/*!
 * \file searching.cpp
 * Binary search, Linear search, Upper bound, lower bound implementation for an array of integers.
 * \author Selan R. dos Santos
 * \date June 17th, 2021.
 */

#include "searching.h"

namespace sa
{

    /*!
     * Performs a **linear search** for `value` in `[first;last)` and returns a pointer to the location of `value` in the range `[first,last]`, or `last` if no such element is found.
     * \param first Pointer to the begining of the data range.
     * \param last Pointer just past the last element of the data range.
     * \param value The value we are looking for.
     */
    value_type *lsearch(value_type *first, value_type *last, value_type value)
    {
        for (int *ptr = first; ptr != last; ptr++)
        {
            if (*ptr == value)
                return ptr;
        }
        return last;
    }

    /*!
     * Performs a **binary search** for `value` in `[first;last)` and returns a pointer to the location of `value` in the range `[first,last]`, or `last` if no such element is found.
     * \note The range **must** be sorted.
     * \param first Pointer to the begining of the data range.
     * \param last Pointer just past the last element of the data range.
     * \param value The value we are looking for.
     */
    value_type *bsearch(value_type *first, value_type *last, value_type value)
    {
        int *aux = last;
        while (first != last)
        {
            int *mid = first + (last - first) / 2;
            if (value == *mid)
            {
                return mid;
            }
            if (value > *mid)
            {
                first = mid + 1;
            }
            else
            {
                last = mid;
            }
        }
        return aux; // STUB
    }

    /*!
     * Performs a **binary search** in a recursive way for `value` in `[first;last)` and returns a pointer to the location of `value` in the range `[first,last]`, or a null pointer if no such element is found.
     * \note The range **must** be sorted.
     * \param first Pointer to the begining of the data range.
     * \param last Pointer just past the last element of the data range.
     * \param value The value we are looking for.
     */
    value_type *aux_rec_bsearch(value_type *first, value_type *last, value_type value)
    {
        int *mid = first + (last - first) / 2;
        if (value == *mid)
        {
            return mid;
        }
        while (first != last)
        {
            if (value > *mid)
            {
                first = mid + 1;
                return aux_rec_bsearch(first, last, value);
            }
            else
            {
                last = mid;
                return aux_rec_bsearch(first, last, value);
            }
        }
        return nullptr;
    };

    /*!
     * Function responsable to call the recursive binary search
     * \note The range **must** be sorted.
     * \param first Pointer to the begining of the data range.
     * \param last Pointer just past the last element of the data range.
     * \param value The value we are looking for.
     */
    value_type *bsearch_rec(value_type *first, value_type *last, value_type value)
    {
        value_type *aux = aux_rec_bsearch(first, last, value);
        return aux ? aux : last;
    }

    /*!
     * Returns a pointer to the first element in the range `[first, last)` that is _not less_  than (i.e. greater or equal to) `value`, or `last` if no such element is found.
     * \note The range **must** be sorted.
     * \param first Pointer to the begining of the data range.
     * \param last Pointer just past the last element of the data range.
     * \param value The value we are looking for.
     */
    value_type *lbound(value_type *first, value_type *last, value_type value)
    {
        int *result = last;

        while (first != last)
        {
            int *mid = first + (last - first) / 2;

            if (*mid >= value)
            {
                result = mid;
                last = mid;
            }
            else
            {
                first = mid + 1;
            }
        }

        return result;
    }

    /*!
     * Returns a pointer to the first element in the range `[first, last)` that is _greater_  than `value`, or `last` if no such element is found.
     * \note The range **must** be sorted.
     * \param first Pointer to the begining of the data range.
     * \param last Pointer just past the last element of the data range.
     * \param value The value we are looking for.
     */
    value_type *ubound(value_type *first, value_type *last, value_type value)
    {
        int *result = last;

        while (first != last)
        {
            int *mid = first + (last - first) / 2;

            if (*mid > value)
            {
                result = mid;
                last = mid;
            }
            else
            {
                first = mid + 1;
            }
        }

        return result;
    }
}