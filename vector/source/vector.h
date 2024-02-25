/*!
 * @brief This file contains the implementation of a Vector class and an Iterator.
 *
 * The Vector class provides a dynamic array implementation.
 *
 * The Iterator class, used along with the Vector class, provides the functionality
 * to iterate over the elements of the Vector both in forward and reverse directions.
 *
 * @author Tobias dos Santos Neto e Wisla Alves Argolo
 * @date May 29st, 2023.
 * @file vector.h
 */

#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <exception>        // std::out_of_range
#include <iostream>         // std::cout, std::endl
#include <memory>           // std::unique_ptr
#include <iterator>         // std::advance, std::begin(), std::end(), std::ostream_iterator
#include <algorithm>        // std::copy, std::equal, std::fill
#include <initializer_list> // std::initializer_list
#include <cassert>          // assert()
#include <limits>           // std::numeric_limits<T>
#include <cstddef>          // std::size_t

/// Sequence container namespace.
namespace sc
{
  /**
   * @class MyForwardIterator
   * @brief Implements the infrastructure to support a bidirectional iterator.
   * @tparam T Type of the elements the iterator will point to.
   */
  template <class T>
  class MyForwardIterator
  {
  public:
    typedef MyForwardIterator iterator; //!< Alias to iterator.
    // Below we have the iterator_traits common interface
    typedef std::ptrdiff_t
        difference_type;              //!< Difference type used to calculated distance between
                                      //!< iterators.
    typedef T value_type;             //!< Value type the iterator points to.
    typedef T *pointer;               //!< Pointer to the value type.
    typedef T &reference;             //!< Reference to the value type.
    typedef const T &const_reference; //!< Reference to the value type.
    typedef std::bidirectional_iterator_tag
        iterator_category; //!< Iterator category.

    /*! @brief Create an iterator around a raw pointer.
     * \param pt_ raw pointer to the container.
     */
    MyForwardIterator(pointer pt = nullptr) : m_ptr(pt)
    { /* empty */
    }

    /**
     * @brief Construct a new MyForwardIterator object (copy constructor)
     * @param other Iterator used to initialize the current iterator.
     */
    MyForwardIterator(const iterator &other) : m_ptr(other.m_ptr)
    { /* empty */
    }

    /**
     * @brief Overloaded assignment operator.
     * @param other Reference to the iterator to use as the data source.
     * @return Reference to the updated iterator to use for concatenation operations.
     */
    iterator &operator=(const iterator &other)
    {
      // update the pointer only if the iterates are different
      if (this != &other)
        m_ptr = other.m_ptr;
      return *this;
    }

    /**
     * @brief Dereference operator
     *
     * Access the content the iterator points to.
     *
     * @return Reference to the value the iterator currently points to.
     */
    reference operator*(void) const
    {
      assert(m_ptr != nullptr);
      return *m_ptr;
    }

    /**
     * @brief Overloaded `->` operator.
     * @return Pointer the iterator currently points to.
     */
    pointer operator->(void) const
    {
      assert(m_ptr != nullptr);
      return m_ptr;
    }

    /**
     * @brief Overloaded pre-increment operator.
     * @return Incremented iterator.
     */
    iterator operator++(void)
    {
      ++m_ptr;      // increasing the pointer
      return *this; // returning the new value
    }

    /**
     * @brief Overloaded post-increment operator.
     * @return Iterator before incrementing.
     */
    iterator operator++(int)
    {
      iterator dummy{*this}; // creating a copy of the current value
      ++m_ptr;               // increasing the pointer
      return dummy;          // returning the old value
    }

    /**
     * @brief Overloaded pre-decrement operator.
     * @return Decremented iterator.
     */
    iterator operator--(void)
    {
      --m_ptr;      // decreasing the pointer
      return *this; // returning the new value
    }

    /**
     * @brief Overloaded post-decrement operator.
     * @return Iterator before decrementing.
     */
    iterator operator--(int)
    {
      iterator dummy{*this}; // creating a copy of the current value
      --m_ptr;               // decreasing the pointer
      return dummy;          // returning the old value
    }

    /**
     * @brief Overloaded forward self jump operator.
     * @param offset The value used to increment the iterator.
     * @return Reference to the iterator after incrementing.
     */
    iterator &operator+=(difference_type offset)
    {
      // iterator &it{*this}; // creating a reference to the current value
      // for (difference_type i = 0; i < offset; ++i)
      //   ++it;    // increasing the pointer offset times
      // return it; // returning the new value

      m_ptr += offset; // increasing the pointer
      return *this;    // returning the new value
    }

    /**
     * @brief Overloaded backward self jump operator.
     * @param offset The value used to decrement the iterator
     * @return Reference to the iterator after decrementing.
     */
    iterator &operator-=(difference_type offset)
    {
      // iterator &it{*this}; // creating a reference to the current value
      // for (difference_type i = 0; i < offset; ++i)
      //   --it;    // decreasing the pointer offset times
      // return it; // returning the new value

      m_ptr -= offset; // decreasing the pointer
      return *this;    // returning the new value
    }
    /**
     * @brief Overloaded less than operator.
     * @param ita First iterator to compare.
     * @param itb Second iterator to compare.
     * @return True if the first iterator points to an element before the second iterator, False otherwise.
     */
    friend bool operator<(const iterator &ita, const iterator &itb) { return ita.m_ptr < itb.m_ptr; }

    /**
     * @brief Overloaded greater than operator.
     * @param ita First iterator to compare.
     * @param itb Second iterator to compare.
     * @return True if the first iterator points to an element after the second iterator, False otherwise.
     */
    friend bool operator>(const iterator &ita, const iterator &itb) { return ita.m_ptr > itb.m_ptr; }

    /**
     * @brief Overloaded great than or equal operator.
     * @param ita First iterator to compare.
     * @param itb Second iterator to compare.
     * @return True if the first iterator points to the same element or after the second iterator, False otherwise.
     */
    friend bool operator>=(const iterator &ita, const iterator &itb) { return ita.m_ptr >= itb.m_ptr; }

    /**
     * @brief Overloaded less than or equal operator.
     * @param ita First iterator to compare.
     * @param itb Second iterator to compare.
     * @return True if the first iterator points to the same element or before the second iterator, False otherwise.
     */
    friend bool operator<=(const iterator &ita, const iterator &itb) { return ita.m_ptr <= itb.m_ptr; }

    /**
     * @brief Overloaded forward jump operator (offset + it).
     * @param offset The value on the left-hand side of the addition operator.
     * @param it The iterator on the right-hand side of the addition operator.
     * @return Iterator after incrementing.
     */
    friend iterator operator+(difference_type offset, iterator it)
    {
      iterator dummy{it}; // creating a copy of the current value
      dummy += offset;    // increasing the pointer
      return dummy;       // returning the new value
    }

    /**
     * @brief Overloaded forward jump operator (it + offset).
     * @param offset The value on the right-hand side of the addition operator.
     * @param it The iterator on the left-hand side of the addition operator.
     * @return Iterator after incrementing.
     */
    friend iterator operator+(iterator it, difference_type offset)
    {
      iterator dummy{it}; // creating a copy of the current value
      dummy += offset;    // increasing the pointer
      return dummy;       // returning the new value
    }

    /**
     * @brief Overloaded backward jump operator operator.
     * @param it The iterator to decrement.
     * @param offset The value used to decrement the iterator.
     * @return Iterator after decrementing.
     */
    friend iterator operator-(iterator it, difference_type offset)
    {
      iterator dummy{it}; // creating a copy of the current value
      dummy -= offset;    // decreasing the pointer
      return dummy;       // returning the new value
    }

    /**
     * @brief Overloaded equality operator.
     * @param rhs_ Iterator for comparison.
     * @return True if the two iterators point to the same element, False otherwise.
     */
    bool operator==(const iterator &rhs_) const { return m_ptr == rhs_.m_ptr; }

    /**
     * @brief Overloaded inequality operator.
     * @param rhs_ Iterator for comparison.
     * @return True if the two iterators do not point to the same element, False otherwise.
     */
    bool operator!=(const iterator &rhs_) const { return !(*this == rhs_); }

    /**
     * @brief Overloaded subtraction operator.
     * @param rhs_  Iterator for subtracting from the current iterator.
     * @return The difference between the two iterators.
     */
    difference_type operator-(const iterator &rhs_) const { return m_ptr - rhs_.m_ptr; }

    /**
     * @brief Overloaded stream extractor operator.
     * @param os_ The output stream.
     * @param p_ The iterator to insert into the stream.
     * @return Reference to the output stream after inserting the iterator.
     */
    friend std::ostream &operator<<(std::ostream &os_,
                                    const MyForwardIterator &p_)
    {
      os_ << "[@ " << p_.m_ptr << ": null ]";
      return os_;
    }

  private:
    pointer m_ptr; //!< The raw pointer.
  };

  /*!
   * @class vector
   * @brief This class implements the ADT list with dynamic array.
   *
   * sc::vector is a sequence container that encapsulates dynamic size arrays.
   *
   * The elements are stored contiguously, which means that elements can
   * be accessed not only through iterators, but also using offsets to
   * regular pointers to elements.
   * This means that a pointer to an element of a vector may be passed to
   * any function that expects a pointer to an element of an array.
   *
   * \tparam T The type of the elements.
   */
  template <typename T>
  class vector
  {
    //=== Aliases
  public:
    using size_type = unsigned long; //!< The size type.
    using value_type = T;            //!< The value type.
    using pointer = value_type *;    //!< Pointer to a value stored in the container.
    using reference =
        value_type &;                           //!< Reference to a value stored in the container.
    using const_reference = const value_type &; //!< Const reference to a value
                                                //!< stored in the container.

    using iterator =
        MyForwardIterator<value_type>; //!< The iterator, instantiated from a
                                       //!< template class.
    using const_iterator =
        MyForwardIterator<const value_type>; //!< The const_iterator,
                                             //!< instantiated from a template
                                             //!< class.

  public:
    //=== [I] SPECIAL MEMBERS (6 OF THEM)

    /**
     * @brief Constructs a vector with a given quantity of default values of type T.
     * @param cp The quantity used to initialize the vector. Defaults to 0.
     */
    explicit vector(size_type cp = 0)
    {
      m_storage = new T[cp];
      m_capacity = cp;
      m_end = cp; // Vector starts empty
      for (size_type i{0}; i < m_end; ++i)
        m_storage[i] = T();
    }

    /*!
     * @brief Destructor
     * Deletes the storage of the vector.
     */
    virtual ~vector(void)
    {
      if (m_storage)
        delete[] m_storage;
    }

    /*!
     * @brief (Copy constructor) Constructs a vector with the deep copy of the contents of another vector.
     * @param other The other vector to be copied.
     */
    vector(const vector &other)
    {
      m_capacity = other.m_capacity;
      m_end = other.m_end;
      m_storage = new T[m_capacity];
      // copy the elements from other into this vector
      std::copy(other.m_storage, other.m_storage + other.m_end, m_storage);
    }

    /*!
     * @brief Constructs a vector with the elements from an initializer list.
     * @param il The initializer list to initialize the vector with.
     */
    vector(const std::initializer_list<T> &il)
    {
      m_capacity = il.size();
      m_storage = new T[m_capacity];
      m_end = m_capacity; // vector starts full
      // copy the elements from the il into this vector
      std::copy(il.begin(), il.end(), begin());
    }

    /*!
     * @brief Constructor a vector with the contents of the range [first, last).
     * @param first Iterator to the beginning of the range.
     * @param last Iterator to the location in the range just after the last element to be considered for insertion.
     */
    template <typename InputItr>
    vector(InputItr first, InputItr last)
    {
      m_capacity = std::distance(first, last);
      m_end = m_capacity;
      m_storage = new T[m_capacity];
      // copy the elements from the range [first, last) into this vector
      std::copy(first, last, begin());
    }

    /*!
     * @brief Overloaded assignment operator that assigns an vector to the current vector.
     * @param other The another vector to assign to the current vector.
     * @return A reference to vector updated.
     */
    vector &operator=(const vector &other)
    {
      // if the vectors are equal, simply return this vector
      if (this == &other)
        return *this;

      // if the vectors are different and their capacities are different, memory allocation is necessary
      if (m_capacity != other.m_capacity)
      {
        delete[] m_storage;
        m_storage = new T[other.m_capacity];
      }

      // update the properties and values of the vector
      m_end = other.m_end;
      m_capacity = other.m_capacity;
      std::copy(other.m_storage, other.m_storage + other.m_end, m_storage);

      return *this;
    }

    /*!
     * @brief Overloaded assignment operator that assigns an initializer list to the vector.
     * @param il The initializer list to assign to the vector.
     * @return A reference to vector updated.
     */
    vector &operator=(const std::initializer_list<T> il)
    {
      // if the list and the vector have different capacities, memory allocation is necessary.
      if (m_capacity != il.size())
      {
        delete[] m_storage;
        m_storage = new T[il.size()];
      }

      // update the properties and values of the vector
      m_end = il.size();
      m_capacity = il.size();
      std::copy(il.begin(), il.end(), m_storage);

      return *this;
    }

    //=== [II] ITERATORS

    /*!
     * @brief Returns an iterator to the beginning of the vector.
     * @return An iterator to the first element.
     */
    iterator begin(void) { return iterator(m_storage); }

    /*!
     * @brief Returns an iterator to the element just past the last valid element of the vector.
     * @return An iterator to the location just past the last valid element.
     */
    iterator end(void) { return iterator(m_storage + m_end); }

    /*!
     * @brief Returns a const iterator to the beginning of the vector.
     * @return A const iterator to the first element.
     */
    const_iterator cbegin(void) const { return const_iterator(m_storage); }

    /*!
     * @brief Returns a const iterator to the element just past the last valid element of the vector.
     * @return A const iterator to the ocation just past the last valid element.
     */
    const_iterator cend(void) const { return const_iterator(m_storage + m_end); }

    // [III] Capacity

    /*!
     * @brief Returns the number of elements in the vector.
     * @return The number of elements in the vector.
     */
    size_type size(void) const { return m_end; }

    /*!
     * @brief Returns the maximum number of elements that the vector can hold.
     * @return The maximum number of elements.
     */
    size_type capacity(void) const { return m_capacity; }

    /*!
     * @brief Checks if the vector has elements.
     * @return True if the vector is empty, False otherwise.
     */
    bool empty(void) const { return m_end == 0; }

    // [IV] Modifiers

    /*!
     * @brief Removes all elements from the vector
     *
     * Changes the logical size of the vector to 0.
     *
     */
    void clear(void) { m_end = 0; }

    void push_front(const_reference value);

    /*!
     * @brief Adds a new element to the end of the vector.
     * @param value The value of the element to add.
     */
    void push_back(const_reference value)
    {
      // if the vector is full, memory allocation (doubling) is required.
      if (full())
        reserve((m_capacity + 1) * 2);
      m_storage[m_end++] = value;
    }

    /*!
     * @brief Removes the last element of the vector.
     * @throws std::length_error If the vector is empty.
     */
    void pop_back(void)
    {
      if (empty())
        throw std::length_error("pop_back called on empty vector");
      --m_end;
    }

    void pop_front(void);

    /*!
     * @brief Inserts elements in the range [first_, last_) at the given position.
     * @param pos_ Iterator to the location where the insertion of new elements begins.
     * @param first_ Iterator to the beginning of the range to insert.
     * @param last_ Iterator to the location in the range just after the last element to be considered for insertion
     * @return An iterator to the location in vector of first inserted element.
     */
    template <typename InputItr>
    iterator insert(iterator pos_, InputItr first_, InputItr last_)
    {
      // call the auxiliary function to allocate the necessary memory for those elements and receive the updated pos
      pos_ = prepare_insertion(pos_, std::distance(first_, last_));
      // copy the elements from the range [first, last) to the vector starting from pos_
      std::copy(first_, last_, pos_);
      return pos_;
    }

    /*!
     * @brief Inserts elements in the range [first_, last_) at the given position.
     * @param pos_ Const iterator to the location where the insertion of new elements begins.
     * @param first_ Iterator to the beginning of the range to insert.
     * @param last_ Iterator to the location in the range just after the last element to be considered for insertion
     * @return An iterator to the location in vector of first inserted element.
     */
    template <typename InputItr>
    iterator insert(const_iterator pos_, InputItr first_, InputItr last_)
    {
      // creates a copy of the constant iterator and calls the insert function (for non-constant iterators)
      iterator new_pos(m_storage + std::distance(cbegin(), pos_));

      return insert(new_pos, first_, last_);
    }

    /*!
     * @brief Insert a value into the list before the position given.
     * @param pos_ Iterator to the location where to insert the new element.
     * @param value_ The value of the new element.
     * @return An iterator to the location in vector of the inserted element.
     */
    iterator insert(iterator pos_, const_reference value_)
    {
      // call prepare_insertion for a single element.
      pos_ = prepare_insertion(pos_, 1);
      *pos_ = value_;

      return pos_;
    }

    /*!
     * @brief Insert a value into the list before the position given.
     * @param pos_ Const iterator to the location where to insert the new element.
     * @param value_ The value of the new element.
     * @return An iterator to the location in vector of the inserted element.
     */
    iterator insert(const_iterator pos_, const_reference value_)
    {
      // creates a copy of the constant iterator and calls the insert function (for non-constant iterators)
      iterator new_pos(m_storage + std::distance(cbegin(), pos_));

      return insert(new_pos, value_);
    }

    /*!
     * @brief Inserts elements from the initializer list at the given position.
     * @param pos_ Iterator to the location where the insertion of new elements begins.
     * @param ilist_ Initializer list with the elements to insert.
     * @return An iterator to the location in vector of first inserted element.
     */
    iterator insert(iterator pos_, std::initializer_list<value_type> ilist_)
    {
      // call the auxiliary function to allocate the necessary memory for those elements and receive the updated pos
      pos_ = prepare_insertion(pos_,  std::distance(ilist_.begin(), ilist_.end()));
      // copy the elements from the ilist_ to the vector starting from pos_
      std::copy(ilist_.begin(), ilist_.end(), pos_);

      return pos_;
    }

    /*!
     * @brief Inserts elements from the initializer list ilist_ at the given position.
     * @param pos_ Const iterator to the location where the insertion of new elements begins.
     * @param ilist_ Initializer list with the elements to insert.
     * @return An iterator to the location in vector of first inserted element.
     */
    iterator insert(const_iterator pos_, std::initializer_list<value_type> ilist_)
    {
      // creates a copy of the constant iterator and calls the insert function (for non-constant iterators)
      iterator new_pos(m_storage + std::distance(cbegin(), pos_));

      return insert(new_pos, ilist_);
    }

    /*!
     * @brief Increases the capacity of the vector to at least the specified value.
     *
     * If the given value is greater than the current capacity, memory allocation is required. Otherwise, no action is needed.
     *
     * @param new_cap The new capacity to increase the vector to.
     */
    void reserve(size_type new_cap)
    {
      // if new_cap is greater than the current capacity, memory allocation is required
      if (new_cap > m_capacity)
      {
        T *aux = new T[new_cap];
        std::copy(m_storage, m_storage + m_end, aux);
        delete[] m_storage;
        m_storage = aux;
        m_capacity = new_cap;
      }
    }

    /*!
     * @brief Requests the removal of unused capacity.
     */
    void shrink_to_fit(void)
    {
      // if the capacity is greater than the size, memory allocation is required
      if (m_capacity > m_end)
      {
        T *aux = new T[m_end];
        std::copy(m_storage, m_storage + m_end, aux);
        delete[] m_storage;
        m_storage = aux;
        m_capacity = m_end;
      }
    }

    /*!
     * @brief Replaces the content of the vector with copies of a specific value.
     * @param count_ Number of copies to assign to the vector.
     * @param value_ Value to be assigned.
     */
    void assign(size_type count_, const_reference value_)
    {
      // check if it is necessary to allocate memory for the new content
      allocate_and_resize(count_);
      // the content of the vector is set to count_ copies of value_
      for (size_type i{0}; i < m_end; ++i)
        m_storage[i] = value_;
    }

    /*!
     * @brief Replaces the content of the vector with elements from an initializer list.
     * @param ilist Initializer list from which the elements will be assigned.
     */
    void assign(const std::initializer_list<T> &ilist)
    {
      // check if it is necessary to allocate memory for the new content
      allocate_and_resize(ilist.size());
      // the content of the vector is updated to the values of the list
      std::copy(ilist.begin(), ilist.end(), begin());
    }

    /*!
     * @brief Replaces the content of a vector with values from a range [first, last).
     * @param first_ Iterator to first element in the range from which the elements will be assigned.
     * @param last_ Iterator to the location just past the last element to be assigned.
     */
    template <typename InputItr>
    void assign(InputItr first, InputItr last)
    {
      // check if it is necessary to allocate memory for the new content
      allocate_and_resize(std::distance(first, last));
      // the content of the vector is updated to the values of the range
      std::copy(first, last, begin());
    }

    /*!
     * @brief Removes elements from a given range in the vector.
     * @param first Iterator to first element to be erased.
     * @param last Iterator to the location just past the last element element to be erased.
     * @throws std::length_error If the vector is empty.
     * @throws std::out_of_range If the range is invalid.
     * @return An iterator to the element that follows range before the call.
     */
    iterator erase(iterator first, iterator last)
    {
      // check if the vector is empty
      if (empty())
        throw std::length_error("vector is empty at erase() method");

      // check if the range is valid
      if (first < begin() || last > end())
        throw std::out_of_range("invalid range at erase() method");

      // the elements in the range are removed
      std::copy(last, end(), first);
      // the size is updated
      m_end -= std::distance(first, last);

      return first;
    }

    /*!
     * @brief Removes elements from a given range in the vector.
     * @param first Const iterator to first element to be erased.
     * @param last Const iterator to the location just past the last element element to be erased.
     * @return An iterator to the element that follows the range before the call.
     */
    iterator erase(const_iterator first, const_iterator last)
    {
      // creates a copy of the constant iterator and calls the erase function (for non-constant iterators)
      iterator new_first(m_storage + std::distance(cbegin(), first));
      iterator new_last(m_storage + std::distance(cbegin(), last));

      return erase(new_first, new_last);
    }

    /*!
     * @brief Removes the element at given position.
     * @param pos Const Iterator to the element to be erased.
     * @return An iterator to the element that follows pos before the call.
     */
    iterator erase(const_iterator pos)
    {
      // creates a copy of the constant iterator and calls the erase function (for non-constant iterators)
      iterator new_pos(m_storage + std::distance(cbegin(), pos));

      return erase(new_pos);
    }

    /*!
     * @brief Removes the element at given position.
     * @param pos Iterator to the element to be erased.
     * @throws std::out_of_range If the position is invalid.
     * @throws std::length_error If the vector is empty.
     * @return An iterator to the element that follows pos before the call.
     */
    iterator erase(iterator pos)
    {
      // check if the vector is empty
      if (empty())
        throw std::length_error("vector is empty at erase() method");

      // check if the position is valid
      if (pos < begin() || pos >= end())
        throw std::out_of_range("invalid position at erase() method");

      // the element at the given position is removed
      std::copy(pos + 1, end(), pos);
      // the size is updated
      --m_end;

      return pos;
    }

    // [V] Element access

    /*!
     * @brief Access the last element of the vector.
     * @return Const reference to the last element of the vector.
     */
    const_reference back(void) const { return m_storage[m_end - 1]; }

    /*!
     * @brief Access the first element of the vector.
     * @return Const reference to the first element of the vector.
     */
    const_reference front(void) const { return m_storage[0]; };

    /*!
     * @brief Access the last element of the vector.
     * @return Reference to the last element of the vector.
     */
    reference back(void) { return m_storage[m_end - 1]; }

    /*!
     * @brief Access the first element of the vector.
     * @return Reference to the first element of the vector.
     */
    reference front(void) { return m_storage[0]; }

    /*!
     * @brief Access specified element with no bounds checking.
     * @param idx Index of the element to be accessed.
     * @return Const reference to the requested element.
     */
    const_reference operator[](size_type idx) const { return m_storage[idx]; }

    /*!
     * @brief Access specified element with no bounds checking.
     * @param idx Index of the element to be accessed.
     * @return Reference to the requested element.
     */
    reference operator[](size_type idx) { return m_storage[idx]; }

    /*!
     * @brief Access specified element with bounds checking.
     * @param idx Index of the element to be accessed.
     * @throws std::out_of_range If the index is invalid.
     * @return Const reference to the requested element.
     */
    const_reference at(size_type idx) const
    {
      if (idx >= m_end)
        throw std::out_of_range("index out of range at at() method");
      return m_storage[idx];
    }

    /*!
     * @brief Access specified element with bounds checking.
     * @param idx Index of the element to be accessed.
     * @throws std::out_of_range If the index is invalid.
     * @return Reference to the requested element.
     */
    reference at(size_type idx)
    {
      if (idx >= m_end)
        throw std::out_of_range("index out of range at method");
      return m_storage[idx];
    }

    pointer data(void);
    const_reference data(void) const;

    // [VII] Friend functions.

    /*!
     * @brief Overloaded stream extractor operator. for class vector.
     * @param os_ The output stream.
     * @param v_ Vector to be inserted into the output stream.
     * @return Reference to the output stream after inserting the iterator.
     */
    friend std::ostream &operator<<(std::ostream &os_, const vector<T> &v_)
    {
      // O que eu quero imprimir???
      os_ << "{ ";
      for (auto i{0u}; i < v_.m_capacity; ++i)
      {
        if (i == v_.m_end)
          os_ << "| ";
        os_ << v_.m_storage[i] << " ";
      }
      os_ << "}, m_end=" << v_.m_end << ", m_capacity=" << v_.m_capacity;

      return os_;
    }

    /*!
     * @brief Swaps the contents of two vectors.
     * @param first_ First vector.
     * @param second_ Second vector.
     */
    friend void swap(vector<T> &first_, vector<T> &second_)
    {
      // enable ADL
      using std::swap;

      // Swap each member of the class.
      swap(first_.m_end, second_.m_end);
      swap(first_.m_capacity, second_.m_capacity);
      swap(first_.m_storage, second_.m_storage);
    }

  private:
    /*!
     * @brief Checks if the vector is full.
     * @return True if the vector's current size is equal to its capacity, False otherwise.
     */
    bool full(void) const { return m_end == m_capacity; }

    /*!
     * @brief Auxiliary function for insert operations.
     * @param pos_ Iterator to the location where to insert the new elements.
     * @param sz The number of elements that will be inserted.
     * @throws std::length_error If vector is empty and insertion position is not the beginning.
     * @throws std::out_of_range If insertion position is out of range.
     * @return An iterator to the location where new elements can be inserted.
     */
    iterator prepare_insertion(iterator pos_, size_type sz)
    {
      if (empty() && pos_ != begin())
        throw std::length_error("insert called on empty vector: only insertion in the first position allowed");
      if (pos_ < begin() || pos_ > end())
        throw std::out_of_range("insert called with out of range position");

      size_type aux = std::distance(begin(), pos_);
      // if the current number of elements plus the amount to be added is greater than the capacity,
      // memory allocation is required
      if (m_end + sz > m_capacity)
        reserve((m_end + sz) * 2);

      // update the position in case memory has been reallocated
      pos_ = begin() + aux;
      // shift the elements starting from pos to the right to make space for the new elements
      std::copy_backward(pos_, end(), end() + sz);
      // update the logical size
      m_end += sz;

      return pos_;
    }

    /*!
     * @brief Auxiliary function that allocates memory and assigns a specific size to the vector.
     * @param sz The number of elements to be assigned to the vector.
     */
    void allocate_and_resize(size_type sz)
    { // if the number of elements to be assigned to the vector is greater than the capacity,
      // memory allocation is required
      if (sz > m_capacity)
      {
        delete[] m_storage;
        m_storage = new T[sz];
        m_capacity = sz;
      }
      // update the logical size
      m_end = sz;
    }

    size_type
        m_end;            //!< The list's current size (or index past-last valid element).
    size_type m_capacity; //!< The list's storage capacity.
    T *m_storage;         //!< The list's data storage area.
  };

  // [VI] Operators

  /*!
   * @brief Checks if the contents of two vectores are equal,
   * @param lhs Left hand side operand.
   * @param rhs Right hand side operand.
   * @return True if vectors are equal, False otherwise.
   */
  template <typename T>
  bool operator==(const vector<T> &lhs, const vector<T> &rhs)
  {
    if (lhs.size() != rhs.size())
      return false;
    for (auto i = 0; i < lhs.size(); ++i)
    {
      if (lhs[i] != rhs[i])
        return false;
    }
    return true;
  }

  /*!
   * @brief Checks if the contents of two vectores are different,
   * @param lhs Left hand side operand.
   * @param rhs Right hand side operand.
   * @return True if vectors are different, False otherwise.
   */
  template <typename T>
  bool operator!=(const vector<T> &lhs, const vector<T> &rhs) { return !(lhs == rhs); }

} // namespace sc.

#endif
