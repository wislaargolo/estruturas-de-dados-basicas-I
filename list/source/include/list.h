/*!
 * @brief This file contains the implementation of a List class and the nested Iterator and Const_Iterator classes.
 *
 * The Lista class provides a doubly linked list implementation.
 *
 * @author Tobias dos Santos Neto, Wisla Alves Argolo e Selan R. dos Santos
 * @date June 17th, 2023.
 * @file list.h
 */

#ifndef _LIST_H_
#define _LIST_H_

#include <iostream> // cout, endl

#include <algorithm> // copy
#include <cassert>   // assert()
#include <cstddef>   // std::ptrdiff_t
#include <iterator>  // bidirectional_iterator_tag
#include <type_traits>

#include <stdexcept>

/// Sequence container namespace
namespace sc
{ // linear sequence. Better name: sequence container (same as
  // STL).

  /*!
   * @brief A template class representing a list container.
   *
   * \note It provides bidirectional iterators to access and traverse the elements in the list.
   * This iterator only works for traversing elements inside the same list.
   *
   * @date May, 2nd 2017.
   * @author Selan R. dos Santos
   *
   * @tparam T The type of elements to be stored in the container.
   */
  template <typename T>
  class list
  {
  private:
    /*!
     * @brief A struct representing a node in the list.
     *
     */
    struct Node
    {
      T data;     //!< Type of information to be stored in the container.
      Node *next; //!< Pointer to the next node.
      Node *prev; //!< Pointer to the previous node.

      /*!
       * @brief Node constructor.
       * @param d The data to be stored in the node. Defaults to T{}.
       * @param n Pointer to the next node. Defaults to nullptr.
       * @param p Pointer to the previous node. Defaults to nullptr.
       */
      Node(const T &d = T{}, Node *n = nullptr, Node *p = nullptr)
          : data{d}, next{n}, prev{p}
      { /* empty */
      }
    };

    /*!
     * @brief A class representing a biderectional const_iterator defined over a doubly linked list.
     */
  public:
    class const_iterator : public std::bidirectional_iterator_tag
    {
      //=== Some aliases to help writing a clearer code.
    public:
      using value_type = T;              //!< The type of the value stored in the list.
      using pointer = T *;               //!< Pointer to the value.
      using reference = T &;             //!< reference to the value.
      using const_reference = const T &; //!< const reference to the value.
      using difference_type = std::ptrdiff_t;
      using iterator_category = std::bidirectional_iterator_tag; //!< Category of the iterator.

    private:
      Node *m_ptr; //!< The raw pointer.

    public:
      /*!
       * @brief const_iterator constructor.
       * @param ptr Raw pointer to the current node. Defaults to nullptr.
       */
      const_iterator(Node *ptr = nullptr) : m_ptr(ptr)
      { /* empty */
      }

      /*!
       * @brief const_iterator destructor.
       */
      ~const_iterator() = default;

      /*!
       * @brief Copy constructor for const_iterator.
       * @param other The const_iterator to be copied.
       */
      const_iterator(const const_iterator &other) : m_ptr(other.m_ptr){/* empty */};

      /*!
       * @brief Overloaded assignment operator.
       * @param other Reference to the const_iterator to use as the data source.
       * @return Reference to the updated const_iterator to use for concatenation operations.
       */
      const_iterator &operator=(const const_iterator &other)
      {
        // update the pointer only if the iterates are different
        if (this != &other)
          m_ptr = other.m_ptr;
        return *this;
      }

      /*!
       * @brief Dereference operator
       *
       * Access the content the const_iterator points to.
       *
       * @return Reference to the value the const_iterator currently points to.
       */
      reference operator*() { return m_ptr->data; }

      /*!
       * @brief Dereference operator
       *
       * Access the content the const_iterator points to.
       *
       * @return A const_reference to the value the const_iterator currently points to.
       */
      const_reference operator*() const { return m_ptr->data; }

      /*!
       * @brief Overloaded pre-increment operator.
       * @return Incremented const_iterator.
       */
      const_iterator operator++()
      {
        // if the current pointer is not null, it is possible to increment it
        if (m_ptr != nullptr)
          m_ptr = m_ptr->next;
        return *this;
      }

      /*!
       * @brief Overloaded post-increment operator.
       * @return const_iterator before incrementing.
       */
      const_iterator operator++(int)
      {
        const_iterator dummy = *this; // creating a copy of the current const_iterator
        // if the current pointer is not null, it is possible to increment it
        if (m_ptr != nullptr)
          m_ptr = m_ptr->next;
        return dummy;
      }

      /*!
       * @brief Overloaded pre-decrement operator.
       * @return Decremented const_iterator.
       */
      const_iterator operator--()
      {
        // if the current pointer is not null, it is possible to decrement it
        if (m_ptr != nullptr)
          m_ptr = m_ptr->prev;
        return *this;
      }

      /*!
       * @brief Overloaded post-decrement operator.
       * @return const_iterator before decrementing.
       */
      const_iterator operator--(int)
      {
        const_iterator dummy = *this; // creating a copy of the current const_iterator
        // if the current pointer is not null, it is possible to decrement it
        if (m_ptr != nullptr)
          m_ptr = m_ptr->prev;
        return dummy;
      }

      /*!
       * @brief Overloaded equality operator.
       * @param rhs const_iterator for comparison.
       * @return True if the two iterators point to the same element, False otherwise.
       */
      bool operator==(const const_iterator &rhs) const { return m_ptr == rhs.m_ptr; }

      /**
       * @brief Overloaded inequality operator.
       * @param rhs const_iterator for comparison.
       * @return True if the two iterators do not point to the same element, False otherwise.
       */
      bool operator!=(const const_iterator &rhs) const { return !(*this == rhs); }

      /*!
       * @brief Friend class declaration for list<T>.
       *
       * \note We need friendship so the list<T> class may access the m_ptr field.
       *
       * @tparam T The type of element stored in the list container.
       */
      friend class list<T>;

      /*!
       * @brief Friend function to output the const_iterator to the stream.
       *
       * @param os_ The output stream.
       * @param s_ The const_iterator to be output.
       * @return A reference to the output stream.
       */
      friend std::ostream &operator<<(std::ostream &os_,
                                      const const_iterator &s_)
      {
        os_ << "[@" << s_.m_ptr << ", val = " << s_.m_ptr->data << "]";
        return os_;
      }
    };

    /*!
     * @brief A class representing a biderectional iterator defined over a doubly linked list.
     */
    class iterator : public std::bidirectional_iterator_tag
    {
      //=== Some aliases to help writing a clearer code.
    public:
      using value_type = T;              //!< The type of the value stored in the list.
      using pointer = T *;               //!< Pointer to the value.
      using reference = T &;             //!< reference to the value.
      using const_reference = const T &; //!< const reference to the value.
      using difference_type = std::ptrdiff_t;
      using iterator_category = std::bidirectional_iterator_tag; //!< Category of the iterator.

    private:
      Node *m_ptr; //!< The raw pointer.

    public:
      /*!
       * @brief iterator constructor.
       * @param ptr Raw pointer to the current node. Defaults to nullptr.
       */
      iterator(Node *ptr = nullptr) : m_ptr(ptr)
      { /* empty */
      }

      /*!
       * @brief iterator destructor.
       */
      ~iterator() = default;

      /*!
       * @brief Copy constructor for iterator.
       * @param other The iterator to be copied.
       */
      iterator(const iterator &other) : m_ptr(other.m_ptr){/* empty */};

      /*!
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

      /*!
       * @brief Dereference operator
       *
       * Access the content the iterator points to.
       *
       * @return Reference to the value the iterator currently points to.
       */
      reference operator*() { return m_ptr->data; }

      /*!
       * @brief Dereference operator
       *
       * Access the content the iterator points to.
       *
       * @return A const_reference to the value the iterator currently points to.
       */
      const_reference operator*() const { return m_ptr->data; }

      /*!
       * @brief Overloaded pre-increment operator.
       * @return Incremented iterator.
       */
      iterator operator++()
      {
        // if the current pointer is not null, it is possible to increment it
        if (m_ptr != nullptr)
          m_ptr = m_ptr->next;
        return *this;
      }

      /*!
       * @brief Overloaded post-increment operator.
       * @return iterator before incrementing.
       */
      iterator operator++(int)
      {
        iterator dummy = *this; // creating a copy of the current iterator
        // if the current pointer is not null, it is possible to increment it
        if (m_ptr != nullptr)
          m_ptr = m_ptr->next;
        return dummy;
      }

      /*!
       * @brief Overloaded pre-decrement operator.
       * @return Decremented iterator.
       */
      iterator operator--()
      {
        // if the current pointer is not null, it is possible to decrement it
        if (m_ptr != nullptr)
          m_ptr = m_ptr->prev;
        return *this;
      }

      /*!
       * @brief Overloaded post-decrement operator.
       * @return iterator before decrementing.
       */
      iterator operator--(int)
      {
        iterator dummy = *this; // creating a copy of the current iterator
        // if the current pointer is not null, it is possible to decrement it
        if (m_ptr != nullptr)
          m_ptr = m_ptr->prev;
        return dummy;
      }

      /*!
       * @brief Overloaded equality operator.
       * @param rhs iterator for comparison.
       * @return True if the two iterators point to the same element, False otherwise.
       */
      bool operator==(const iterator &rhs) const { return m_ptr == rhs.m_ptr; }

      /**
       * @brief Overloaded inequality operator.
       * @param rhs iterator for comparison.
       * @return True if the two iterators do not point to the same element, False otherwise.
       */
      bool operator!=(const iterator &rhs) const { return !(*this == rhs); }

      /*!
       * @brief Friend class declaration for list<T>.
       *
       * \note We need friendship so the list<T> class may access the m_ptr field.
       *
       * @tparam T The type of element stored in the list container.
       */
      friend class list<T>;

      /*!
       * @brief Friend function to output the iterator to the stream.
       *
       * @param os_ The output stream.
       * @param s_ The iterator to be output.
       * @return A reference to the output stream.
       */
      friend std::ostream &operator<<(std::ostream &os_, const iterator &s_)
      {
        os_ << "[@" << s_.m_ptr << ", val = " << s_.m_ptr->data << "]";
        return os_;
      }
    };

    //=== Private members.
  private:
    size_t m_len; //!< The length of the list.
    Node *m_head; //!< The head node.
    Node *m_tail; //!< The tail node.

  public:
    //=== Public interface

    //=== [I] Special members

    /*!
     * @brief Default constructor.
     *
     * Creates an empty list with only head and tail nodes.
     */
    list()
    {
      /*  Head & tail nodes.
       *     +---+    +---+
       *     |   |--->|   |--+
       *     | H |    | T |  |
       *  +--|   |<---|   |  |
       *  |  +---+    +---+  |
       *  |                  |
       * ===                ===
       *  =                  =
       */
      m_len = 0;
      m_head = new Node();
      m_tail = new Node();

      m_head->next = m_tail;
      m_tail->prev = m_head;
    }

    /*!
     * @brief Constructs with a given quantity of nodes with default values of T.
     * @param count The quantity used to initialize the list.
     */
    explicit list(size_t count)
    {
      m_len = count;
      m_head = new Node();
      m_tail = new Node();

      m_head->next = m_tail;
      m_tail->prev = m_head;

      Node *curr = m_head;

      // add the nodes with default values to the list
      for (auto i{0}; i < count; ++i)
      {
        Node *nn = new Node();
        curr->next = nn;
        nn->prev = curr;
        curr = nn;
      }
      // update the tail of the list
      curr->next = m_tail;
      m_tail->prev = curr;
    }

    /*!
     * @brief Constructs a list with the contents of the range [first, last).
     * @param first Iterator to the beginning of the range.
     * @param last  Iterator to the location in the range just after the last element to be considered.
     *
     * \note The type of 'first' and 'last' is InputIt, which is a template parameter.
     */
    template <typename InputIt>
    list(InputIt first, InputIt last)
    {
      m_len = std::distance(first, last);
      m_head = new Node();
      m_tail = new Node();

      m_head->next = m_tail;
      m_tail->prev = m_head;

      Node *curr = m_head;

      // add the nodes with values of the range [first, last) to the list
      for (auto i{first}; i != last; ++i)
      {
        Node *nn = new Node(*i);
        curr->next = nn;
        nn->prev = curr;
        curr = nn;
      }
      // update the tail of the list
      curr->next = m_tail;
      m_tail->prev = curr;
    }

    /*!
     * @brief Copy constructor.
     * @param clone_ The list to be cloned.
     */
    list(const list &clone_)
    {
      m_len = clone_.size();
      m_head = new Node();
      m_tail = new Node();

      m_head->next = m_tail;
      m_tail->prev = m_head;
      // store the nodes from which the data will be copied to the list
      Node *runner = clone_.m_head->next;

      Node *curr = m_head;

      // add the nodes with values of another list to the current list
      for (auto i{0}; i < clone_.size(); ++i)
      {
        Node *nn = new Node(runner->data);
        curr->next = nn;
        nn->prev = curr;
        curr = nn;
        runner = runner->next;
      }
      // update the tail of the list
      curr->next = m_tail;
      m_tail->prev = curr;
    }

    /*!
     * @brief Constructs a list from an initializer list.
     * @param ilist_ The initializer list to initialize the list with.
     */
    list(std::initializer_list<T> ilist_)
    {
      m_len = ilist_.size();
      m_head = new Node();
      m_tail = new Node();

      m_head->next = m_tail;
      m_tail->prev = m_head;

      Node *curr = m_head;

      // add the nodes with values of initializer list to the current list
      for (auto i{ilist_.begin()}; i != ilist_.end(); ++i)
      {
        Node *nn = new Node(*i);
        curr->next = nn;
        nn->prev = curr;
        curr = nn;
      }
      // update the tail of the list
      curr->next = m_tail;
      m_tail->prev = curr;
    }

    /*!
     * @brief Destructor.
     * Deletes all nodes and clears the list.
     */
    ~list()
    {
      clear();
      delete m_head;
      delete m_tail;
    }

    /*!
     * @brief Overloaded assignment operator that assigns an list to the current list.
     * @param rhs The another list to assign to the current list.
     * @return A reference to list updated.
     */
    list &operator=(const list &rhs)
    {
      // protects against self-assignmen
      if (this != &rhs)
      {
        // delete the current nodes from the list
        clear();

        m_len = rhs.size();

        Node *curr = m_head;
        Node *runner = rhs.m_head->next;

        // add the nodes with values of another list to the current list
        for (auto i{0}; i < rhs.size(); ++i)
        {
          Node *nn = new Node(runner->data);
          curr->next = nn;
          nn->prev = curr;
          curr = nn;
          runner = runner->next;
        }
        // update the tail of the list
        curr->next = m_tail;
        m_tail->prev = curr;
      }

      return *this;
    }

    /*!
     * @brief Overloaded assignment operator that assigns an initializer list to the list.
     * @param ilist_ The initializer list to assign to the list.
     * @return A reference to list updated.
     */
    list &operator=(std::initializer_list<T> ilist_)
    {

      // delete the current nodes from the list
      clear();

      m_len = ilist_.size();
      Node *curr = m_head;

      // update the list with new nodes containing values from the ilist_
      for (auto i{ilist_.begin()}; i != ilist_.end(); ++i)
      {
        Node *nn = new Node(*i);
        curr->next = nn;
        nn->prev = curr;
        curr = nn;
      }
      // update the tail of the list
      curr->next = m_tail;
      m_tail->prev = curr;

      return *this;
    }

    //=== [II] ITERATORS

    /*!
     * @brief Returns an iterator to the first element in the list.
     * @return An iterator to the first element of the list.
     */
    iterator begin() { return iterator{m_head->next}; }

    /*!
     * @brief Returns a const iterator to the first element in the list.
     * @return A const_iterator to the first element of the list.
     */
    const_iterator cbegin() const { return const_iterator{m_head->next}; }

    /*!
     * @brief Returns an iterator to the element just past the last valid element of the list.
     * @return An iterator to the location just past the last valid element.
     */
    iterator end() { return iterator{m_tail}; }

    /*!
     * @brief Returns a const iterator to the beginning of the list.
     * @return A const iterator to the first element.
     */
    const_iterator cend() const { return const_iterator{m_tail}; }

    //=== [III] Capacity/Status

    /*!
     * @brief Checks if the list is empty.
     * @return True if the list is empty, False otherwise.
     */
    [[nodiscard]] bool empty() const { return m_len == 0; }

    /*!
     * @brief Returns the number of elements in the list.
     * @return The number of elements in the list.
     */
    [[nodiscard]] size_t size() const { return m_len; }

    //=== [IV] Modifiers

    /*!
     * @brief Removes all elements from the list.
     */
    void clear() { erase(begin(), end()); }

    /*!
     * @brief Access the first element in the list.
     * @return The first element in the list.
     */
    T front() { return m_head->next->data; }

    /*!
     * @brief A const function that access the first element in the list.
     * @return The first element in the list.
     */
    T front() const { return m_head->next->data; }

    /*!
     * @brief Access the last element in the list.
     * @return The last element in the list.
     */
    T back() { return m_tail->prev->data; }

    /*!
     * @brief A const function that access the last element in the list.
     * @return The last element in the list.
     */
    T back() const { return m_tail->prev->data; }

    /*!
     * @brief Inserts an element to the front of the list.
     * @param value_ The value of the element to insert.
     */
    void push_front(const T &value_)
    {
      // create a new node with the value to be inserted
      Node *nn = new Node(value_);
      nn->next = m_head->next;
      nn->prev = m_head;
      m_head->next->prev = nn;
      m_head->next = nn;

      ++m_len;
    }

    /*!
     * @brief Inserts an element to the end of the list.
     * @param value_ The value of the element to insert.
     */
    void push_back(const T &value_)
    {
      // create a new node with the value to be inserted
      Node *nn = new Node(value_);
      nn->prev = m_tail->prev;
      nn->next = m_tail;
      m_tail->prev->next = nn;
      m_tail->prev = nn;

      ++m_len;
    }

    /*!
     * @brief Removes the first element of the list.
     */
    void pop_front()
    {
      if (empty())
        return;

      Node* temp = m_head->next;
      m_head->next = temp->next;
      temp->next->prev = m_head;
      delete temp;
      --m_len;

      //Node *temp = m_head;
      //m_head = temp->next;
      //m_head->prev = nullptr;
      //delete temp;
      //--m_len;
    }

    /*!
     * @brief Removes the last element of the list.
     */
    void pop_back()
    {
      if (empty())
        return;
      Node* temp = m_tail->prev;
      m_tail->prev = temp->prev;
      temp->prev->next = m_tail;
      delete temp;
      --m_len;

      // Node *temp = m_tail;
      // m_tail = temp->prev;
      // m_tail->next = nullptr;
      // delete temp;
      // --m_len;
    }

    //=== [IV-a] MODIFIERS W/ ITERATORS

    /*!
     * @brief Replaces the contents of the list with copies of the elements in the range [first_, last_).
     * @param first_ Iterator to the beginning of the range.
     * @param last_ Iterator to the location in the range just after the last element to be considered.
     */
    template <class InItr>
    void assign(InItr first_, InItr last_)
    {
      // delete the current nodes from the list
      clear();

      m_len = std::distance(first_, last_);
      Node *curr = m_head;

      // update the list with new nodes containing values from the range
      for (auto i{first_}; i != last_; ++i)
      {
        Node *nn = new Node(*i);
        curr->next = nn;
        nn->prev = curr;
        curr = nn;
      }
      // update the tail of the list
      curr->next = m_tail;
      m_tail->prev = curr;
    }

    /*!
     * @brief Replaces the contents of the list with copies of the elements in the initializer list 'ilist_'.
     * @param ilist_ The initializer list with the values to assign.
     */
    void assign(std::initializer_list<T> ilist_)
    {
      // delete the current nodes from the list
      clear();

      m_len = ilist_.size();
      Node *curr = m_head;

      // update the list with new nodes containing values from the ilist_
      for (auto i{ilist_.begin()}; i != ilist_.end(); ++i)
      {
        Node *nn = new Node(*i);
        curr->next = nn;
        nn->prev = curr;
        curr = nn;
      }
      // update the tail of the list
      curr->next = m_tail;
      m_tail->prev = curr;
    }

    /*!
     * @brief Inserts a new value in the list before the iterator 'pos_'.
     * @param pos_ An iterator to the position before which we want to insert the new data.
     * @param value_ The value we want to insert in the list.
     * @return An iterator to the new element in the list.
     */
    iterator insert(iterator pos_, const T &value_)
    {
      Node *aux = new Node(value_);
      aux->next = pos_.m_ptr;
      aux->prev = pos_.m_ptr->prev;
      pos_.m_ptr->prev->next = aux;
      pos_.m_ptr->prev = aux;
      ++m_len;

      return iterator{aux};
    }

    /*!
     * @brief Inserts a new value in the list before the iterator 'pos_'.
     * @param pos_ A const iterator to the position before which we want to insert the new data.
     * @param value_ The value we want to insert in the list.
     * @return An iterator to the new element in the list.
     */
    iterator insert(const_iterator pos_, const T &value_)
    {
      iterator aux(pos_.m_ptr);

      return insert(aux, value_);
    }

    /*!
     * @brief Inserts elements from the range [first_, last_) in the list before the iteratos 'pos_'.
     * @param pos_ An iterator to the position before which we want to insert the new data.
     * @param first_ An iterator to the beginning of the range to be inserted.
     * @param last_ An iterator to the location in the range just after the last element to be considered.
     * @return An iterator to the first of the newly inserted elements.
     */
    template <typename InItr>
    iterator insert(iterator pos_, InItr first_, InItr last_)
    {
      Node *aux = pos_.m_ptr->prev;
      while (first_ != last_)
      {
        insert(pos_, *first_);
        ++first_;
      }

      return iterator{aux->next};
    }

    /*!
     * @brief Inserts elements from the range [first_, last_) in the list before the iteratos 'pos_'.
     * @param pos_ A const iterator to the position before which we want to insert the new data.
     * @param first_ An iterator to the beginning of the range to be inserted.
     * @param last_ An iterator to the location in the range just after the last element to be considered.
     * @return An iterator to the first of the newly inserted elements.
     */
    template <typename InItr>
    iterator insert(const_iterator pos_, InItr first_, InItr last_)
    {
      iterator aux(pos_.m_ptr);

      return insert(aux, first_, last_);
    }

    /*!
     * @brief Inserts elements from the initializer list 'ilist_' in the list before the iteratos 'cpos_'.
     * @param cpos_ An iterator to the position before which we want to insert the new data.
     * @param ilist_ An initializer list containing elements to be inserted.
     * @return An iterator to the first of the newly inserted elements.
     */
    iterator insert(iterator cpos_, std::initializer_list<T> ilist_)
    {
      Node *aux = cpos_.m_ptr->prev;

      for (auto i{ilist_.begin()}; i != ilist_.end(); ++i)
      {
        insert(cpos_, *i);
      }

      return iterator{aux->next};
    }

    /*!
     * @brief Inserts elements from the initializer list 'ilist_' in the list before the iteratos 'cpos_'.
     * @param cpos_ A const iterator to the position before which we want to insert the new data.
     * @param ilist_ An initializer list containing elements to be inserted.
     * @return An iterator to the first of the newly inserted elements.
     */
    iterator insert(const_iterator cpos_, std::initializer_list<T> ilist_)
    {
      iterator aux(cpos_.m_ptr);

      return insert(aux, ilist_);
    }

    /*!
     * @brief Erases the node pointed by 'it_' and returns an iterator.
     * @param it_ The node we wish to delete.
     * @return An iterator to the node following the deleted node.
     */
    iterator erase(iterator it_)
    {
      // create a temporary pointer to the node following the node to be removed
      Node *temp = it_.m_ptr->next;
      // update the pointers of the node preceding and succeeding the one to be removed
      it_.m_ptr->prev->next = temp;
      temp->prev = it_.m_ptr->prev;

      // free memory and update the size of the list
      delete it_.m_ptr;
      --m_len;

      return iterator(temp);
    }

    /*!
     * @brief Erases items from [start; end).
     * @param start An iterator to the beginning of the range to be erased.
     * @param end An iterator to the location just after the last element to be erased.
     * @return An iterator just past the last deleted node.
     */
    iterator erase(iterator start, iterator end)
    {
      // erase each node in the range [start, end)
      while (start != end)
        start = erase(start);

      return end;
    }

    /*!
     * @brief Erases the node pointed by 'it_' and returns an iterator.
     * @param it_ The node we wish to delete.
     * @return An iterator to the node following the deleted node.
     */
    iterator erase(const_iterator it_)
    {
      // creates a copy of the constant iterator and calls the erase function (for non-constant iterators)
      iterator new_it(it_.m_ptr);

      return erase(new_it);
    }

    /*!
     * @brief Erases items from [start; end).
     * @param start An iterator to the beginning of the range to be erased.
     * @param end An iterator to the location just after the last element to be erased.
     * @return An iterator just past the last deleted node.
     */
    iterator erase(const_iterator start, const_iterator end)
    {
      // creates a copy of the constant iterators and calls the insert function (for non-constant iterators)
      iterator new_start(start.m_ptr);
      iterator new_end(end.m_ptr);

      return erase(new_start, new_end);
    }

    //=== [V] UTILITY METHODS

    /*!
     * @brief Merges two sorted lists into one.
     *
     * The 'other' list becomes empty after the operation.
     *
     * @param other A reference to the other list with which the current list is to be merged.
     */
    void merge(list &other)
    {
      // only perform the merge if the current list is different from the one passed as a parameter
      if (this != &other)
      {
        // create two iterators to traverse the current list and the other list
        iterator it1 = begin();
        iterator it2 = other.begin();

        // traverse both lists and insert the elements in the correct order
        while (it1 != end() && it2 != other.end())
        {
          if (*it2 < *it1)
          {
            // insert(it1, *it2);
            it1.m_ptr->prev->next = it2.m_ptr;
            it2.m_ptr->prev = it1.m_ptr->prev;
            it1.m_ptr->prev = it2.m_ptr;
            ++it2;
            it2.m_ptr->prev->next = it1.m_ptr;
          }
          else
            ++it1;
        }

        // if there are still elements in the other list, insert them at the end of the current list
        while (it2 != other.end())
        {
          // insert(it1, *it2);
          it1.m_ptr->prev->next = it2.m_ptr;
          it2.m_ptr->prev = it1.m_ptr->prev;
          it1.m_ptr->prev = it2.m_ptr;
          ++it2;
          it2.m_ptr->prev->next = it1.m_ptr;
        }

        // update the size of the current list and empty the other list
        m_len += other.size();
        // other.clear();
        other.m_head->next = other.m_tail;
        other.m_tail->prev = other.m_head;
        other.m_len = 0;
      }
    }

    /*!
     * @brief Transfers all elements from 'other' to the current list. The elements are inserted before the element pointed to by 'pos'.
     *
     * The 'other' list becomes empty after the operation.
     *
     * @param pos A const_iterator to the position before which elements are transferred from 'other' list.
     * @param other A reference to the other list from which elements are to be moved to the current list.
     */
    void splice(const_iterator pos, list &other)
    {
      // only perform the splice if the current list is different from the one passed as a parameter
      if (this != &other)
      {
        m_len += other.size();
        // connect the first new node to the position before pos
        other.m_head->next->prev = pos.m_ptr->prev;
        pos.m_ptr->prev->next = other.m_head->next;

        // connect the last new node to pos
        other.m_tail->prev->next = pos.m_ptr;
        pos.m_ptr->prev = other.m_tail->prev;

        // make the other list empty
        other.m_head->next = other.m_tail;
        other.m_tail->prev = other.m_head;
        other.m_len = 0;
      }
    }

    /*!
     * @brief Reverses the order of the elements in the list.
     */
    void reverse()
    {

      Node *curr = m_head;
      Node *temp = nullptr;

      // swap the beginning and end of the list
      std::swap(m_head, m_tail);

      // swaps the previous and next of all nodes in the list
      while (curr != nullptr)
      {
        std::swap(curr->prev, curr->next);
        curr = curr->prev;
      }
    }

    /*!
     * @brief This method removes all consecutive duplicate elements from the container.
     *
     * Only the first element in each group of equal elements is left.
     *
     */
    void unique()
    {
      // it is only necessary to check for duplicate elements in non-empty lists
      if (!empty())
      {
        auto slow = begin();
        auto fast = begin();
        // 'fast' is always one step ahead of 'slow'
        ++fast;

        // while not all elements have been checked...
        while (fast != end())
        {
          // if two consecutive elements are equal, delete the second one and
          // 'fast' becomes the iterator for the element just after the deleted one
          if (*slow == *fast)
            fast = erase(fast);
          // otherwise, advance both 'slow' and 'fast'
          else
          {
            slow = fast;
            ++fast;
          }
        }
      }
    }

    /*!
     * @brief Sorts the elements in the list in non-decreasing order. The sorting is performed using merge sort.
     */
    void sort()
    { 
      // if there are two or more elements, sort the list
      if (size() >= 2)
      {

        size_t m = size() / 2;
        auto it_half = begin();
        // advance the iterator to the middle of the list
        std::advance(it_half, m);

        // create a new list to hold the right half of the original list
        list<T> right;

        // adjusts pointers to move the right half of the original list to 'right'
        right.m_tail->prev = m_tail->prev;
        m_tail->prev->next = right.m_tail;

        // adjusts pointers to isolate the left half in the original list
        m_tail->prev = it_half.m_ptr->prev;
        it_half.m_ptr->prev->next = m_tail;

        // adjusts pointers to move the right half of the original list to 'right'
        right.m_head->next = it_half.m_ptr;
        it_half.m_ptr->prev = right.m_head;

        right.m_len = size() - m;
        m_len = m;

        // sort the left (original list) and right (right list) halves
        sort();
        right.sort();

        // merge the sorted halves into the original list
        merge(right);
      }
    }
  };

  //=== [VI] OPETARORS

  /*!
   * @brief Checks if the contents of two lists are equal,
   * @param l1_ Left hand side operand.
   * @param l2_ Right hand side operand.
   * @return True if lists are equal, False otherwise.
   */
  template <typename T>
  inline bool operator==(const sc::list<T> &l1_, const sc::list<T> &l2_)
  {
    if (l1_.size() != l2_.size())
      return false;

    auto itl1 = l1_.cbegin();
    auto itl2 = l2_.cbegin();

    for (auto i{itl1}; i != l1_.cend(); ++i)
    {
      if (!(*i == *itl2))
        return false;
      ++itl2;
    }

    return true;
  }

  /*!
   * @brief Checks if the contents of two lists are different,
   * @param l1_ Left hand side operand.
   * @param l2_ Right hand side operand.
   * @return True if lists are different, False otherwise.
   */
  template <typename T>
  inline bool operator!=(const sc::list<T> &l1_, const sc::list<T> &l2_) { return !(l1_ == l2_); }
}
// namespace sc
#endif
