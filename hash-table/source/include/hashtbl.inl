#include "hashtbl.h"

namespace ac
{
    /// Regular constructor
    template <typename KeyType, typename DataType, typename KeyHash, typename KeyEqual>
    HashTbl<KeyType, DataType, KeyHash, KeyEqual>::HashTbl(size_type sz)
    {
        // dynamically allocates in *m_table an array whose size is determined to be the smallest prime number ≥ the value specified in sz
        m_size = find_next_prime(sz);
        m_count = 0;
        m_table = new list_type[m_size];
        m_max_load_factor = 1.0;
    }

    /// Copy constructor
    template <typename KeyType, typename DataType, typename KeyHash, typename KeyEqual>
    HashTbl<KeyType, DataType, KeyHash, KeyEqual>::HashTbl(const HashTbl &source)
    {
        // updates the attributes according to source
        m_size = source.m_size;
        m_count = source.m_count;
        m_table = new list_type[m_size];
        m_max_load_factor = source.m_max_load_factor;

        // assigns the collision lists from source to the current table.
        for (auto i{0}; i < m_size; ++i)
            m_table[i] = source.m_table[i];
    }

    /// Initializer constructor
    template <typename KeyType, typename DataType, typename KeyHash, typename KeyEqual>
    HashTbl<KeyType, DataType, KeyHash, KeyEqual>::HashTbl(const std::initializer_list<entry_type> &ilist)
    {
        // updates the attributes according to ilist
        m_size = find_next_prime(ilist.size());
        m_count = 0;
        m_table = new list_type[m_size];
        m_max_load_factor = 1.0;

        // insert copies of the entries from ilist into the current table
        for (auto i{ilist.begin()}; i != ilist.end(); ++i)
            insert(i->m_key, i->m_data);
    }

    /// Overloaded assignment operator that takes another hash table.
    template <typename KeyType, typename DataType, typename KeyHash, typename KeyEqual>
    HashTbl<KeyType, DataType, KeyHash, KeyEqual> &
    HashTbl<KeyType, DataType, KeyHash, KeyEqual>::operator=(const HashTbl &clone)
    {
        // avoids self-assignment
        if (this != &clone)
        {
            clear();
            // if the size of the current table and the received table are different, memory allocation is required
            if (clone.m_size != m_size)
            {
                delete[] m_table;
                m_size = clone.m_size;
                m_table = new list_type[m_size];
            }

            m_max_load_factor = clone.m_max_load_factor;
            m_count = clone.m_count;

            // assigns the collision lists from clone to the current table.
            for (auto i{0}; i < m_size; ++i)
                m_table[i] = clone.m_table[i];
        }

        return *this;
    }

    /// Overloaded assignment operator that takes an initializer list
    template <typename KeyType, typename DataType, typename KeyHash, typename KeyEqual>
    HashTbl<KeyType, DataType, KeyHash, KeyEqual> &
    HashTbl<KeyType, DataType, KeyHash, KeyEqual>::operator=(const std::initializer_list<entry_type> &ilist)
    {
        clear();

        // if the size of the current table and the received initializer list are different, memory allocation is required
        if (ilist.size() != m_size)
        {
            delete[] m_table;
            m_size = find_next_prime(ilist.size());
            m_table = new list_type[m_size];
        }

        m_max_load_factor = 1.0;

        // insert copies of the entries from initializer list into the current table
        for (auto i{ilist.begin()}; i != ilist.end(); ++i)
            insert(i->m_key, i->m_data);

        return *this;
    }

    /// Destructor.
    template <typename KeyType, typename DataType, typename KeyHash, typename KeyEqual>
    HashTbl<KeyType, DataType, KeyHash, KeyEqual>::~HashTbl()
    {
        // clears all collision lists
        clear();
        // deallocates the memory associated with the array
        delete[] m_table;
    }

    /// Insert.
    template <typename KeyType, typename DataType, typename KeyHash, typename KeyEqual>
    bool HashTbl<KeyType, DataType, KeyHash, KeyEqual>::insert(const KeyType &key_, const DataType &new_data_)
    {
        KeyHash hash;
        KeyEqual equal;

        // calculates the position of the list in which the new element will be inserted
        size_type end = hash(key_) % m_size;
        // searches in the list at the calculated position
        for (auto &entry : m_table[end])
        {
            // if the key is equal to the key of any element, returns false after updating the data
            if (equal(entry.m_key, key_))
            {
                entry.m_data = new_data_;
                return false;
            }
        }

        // inserts the new element at the end of the list at the calculated position
        m_table[end].push_front(entry_type(key_, new_data_));
        ++m_count;

        // if the load factor is greater than the maximum load factor, rehashing is required
        if (m_count > m_size * m_max_load_factor)
            rehash();

        return true;
    }

    /// Clear.
    template <typename KeyType, typename DataType, typename KeyHash, typename KeyEqual>
    void HashTbl<KeyType, DataType, KeyHash, KeyEqual>::clear()
    {
        // clears each collision list
        for (auto i{0}; i < m_size; ++i)
            m_table[i].clear();
        m_count = 0;
    }

    /// Empty.
    template <typename KeyType, typename DataType, typename KeyHash, typename KeyEqual>
    bool HashTbl<KeyType, DataType, KeyHash, KeyEqual>::empty() const
    {
        return m_count == 0;
    }

    /// Retrieve.
    template <typename KeyType, typename DataType, typename KeyHash, typename KeyEqual>
    bool HashTbl<KeyType, DataType, KeyHash, KeyEqual>::retrieve(const KeyType &key_, DataType &data_item_) const
    {
        KeyHash hash;
        KeyEqual equal;

        // calculates the position of the list in which the searched element is located
        size_type pos = hash(key_) % m_size;
        // searches in the list at the calculated position
        for (auto &entry : m_table[pos])
        {
            // if the key is equal to the key of any element, updates the reference and returns true; otherwise, returns false
            if (equal(entry.m_key, key_))
            {
                data_item_ = entry.m_data;
                return true;
            }
        }

        return false;
    }

    /// Rehash.
    template <typename KeyType, typename DataType, typename KeyHash, typename KeyEqual>
    void HashTbl<KeyType, DataType, KeyHash, KeyEqual>::rehash(void)
    {
        // finds the new size of the list
        size_type new_size = find_next_prime(m_size * 2);

        size_type aux_count = m_count;
        list_type *aux = new list_type[new_size];

        KeyHash hash;
        // places the items from the table in their new positions
        for (auto i{0}; i < m_size; ++i)
        {
            for (auto &entry : m_table[i])
            {
                size_type pos = hash(entry.m_key) % new_size;
                aux[pos].push_front(entry);
            }
        }

        // frees the previously used memory and updates the table to the current size and memory space
        clear();
        delete[] m_table;
        m_size = new_size;
        m_table = aux;

        m_count = aux_count;
    }

    /// Erase.
    template <typename KeyType, typename DataType, typename KeyHash, typename KeyEqual>
    bool HashTbl<KeyType, DataType, KeyHash, KeyEqual>::erase(const KeyType &key_)
    {
        KeyHash hash;
        KeyEqual equal;

        // calculates the position of the list in which the element to be deleted is located
        size_type pos = hash(key_) % m_size;
        // iterator to the element before the key to be deleted
        auto prev = m_table[pos].before_begin();
        // iterate over the list at position pos in the hash table
        for (auto &entry : m_table[pos])
        {
            // if key to be deleted is found erase the element after 'prev' and return true
            if (equal(entry.m_key, key_))
            {
                m_table[pos].erase_after(prev);
                --m_count;
                return true;
            }
            ++prev;
        }

        return false;
    }

    /// Find next prime.
    template <typename KeyType, typename DataType, typename KeyHash, typename KeyEqual>
    std::size_t HashTbl<KeyType, DataType, KeyHash, KeyEqual>::find_next_prime(size_type n_)
    {
        // while n_ is not prime, increment and test again
        while (!prime(n_))
            ++n_;

        return n_;
    }

    /// Prime checking.
    template <typename KeyType, typename DataType, typename KeyHash, typename KeyEqual>
    bool HashTbl<KeyType, DataType, KeyHash, KeyEqual>::prime(size_type n_)
    {
        // if n_ is less than or equal to 1, it's not prime
        if (n_ <= 1)
            return false;
        // if n_ is greater than 1 and less than or equal to 3, it is prime
        if (n_ <= 3)
            return true;
        // if n_ is divisible by 2 or 3, it's not prime
        if (n_ % 2 == 0 || n_ % 3 == 0)
            return false;

        // tests if n_ is divisible by numbers of the form 6k + 1 and 6k - 1 (prime numbers) up to the square root of n_
        for (auto i{5}; i * i <= n_; i += 6)
        {
            // if it is divisible, it is not prime
            if (n_ % i == 0 || n_ % (i + 2) == 0)
                return false;
        }

        return true;
    }

    /// Counts the number of elements in a list.
    template <typename KeyType, typename DataType, typename KeyHash, typename KeyEqual>
    typename HashTbl<KeyType, DataType, KeyHash, KeyEqual>::size_type
    HashTbl<KeyType, DataType, KeyHash, KeyEqual>::count(const KeyType &key_) const
    {
        KeyHash hash;
        KeyEqual equal;

        // calculates the position of the list in which the searched element is located
        size_type pos = hash(key_) % m_size;
        // counts the number of elements in the list at the calculated position
        size_type count{0};
        for (auto &entry : m_table[pos])
            ++count;

        return count;
    }

    /// At.
    template <typename KeyType, typename DataType, typename KeyHash, typename KeyEqual>
    DataType &HashTbl<KeyType, DataType, KeyHash, KeyEqual>::at(const KeyType &key_)
    {
        KeyHash hash;
        KeyEqual equal;

        // calculates the position of the list in which the searched element is located
        size_type pos = hash(key_) % m_size;
        // searches for the item associated with the provided key
        for (auto &entry : m_table[pos])
        {
            if (equal(entry.m_key, key_))
                return entry.m_data;
        }

        throw std::out_of_range("Key not found");
    }

    /// Operator [].
    template <typename KeyType, typename DataType, typename KeyHash, typename KeyEqual>
    DataType &HashTbl<KeyType, DataType, KeyHash, KeyEqual>::operator[](const KeyType &key_)
    {
        KeyHash hash;
        KeyEqual equal;
        // calculates the position of the list in which the searched element is located
        size_type pos = hash(key_) % m_size;

        // searches for the item associated with the provided key
        for (auto &entry : m_table[pos])
        {
            if (equal(entry.m_key, key_))
                return entry.m_data;
        }

        // checks if, with the insertion, the load factor exceeds the maximum load factor
        if (++m_count > m_size * m_max_load_factor)
        {
            // if it exceeds, performs rehashing and updates the position of
            // the linked list in which the inserted item is located
            rehash();
            pos = hash(key_) % m_size;
        }

        // if the item associated with the provided key is not found,
        // performs the insertion of an item with the provided key and default data
        m_table[pos].push_front(entry_type(key_, DataType()));

        return m_table[pos].begin()->m_data;
    }
} // Namespace ac.
