/*!
 * @brief This file contains the declaration of the HashTbl class and and its associated struct, HashEntry.
 *
 * HashTbl is a custom hash table that implements the Unordered Dictionary Abstract Data Type (ADT).
 *
 * @author Tobias dos Santos Neto, Wisla Alves Argolo e Selan R. dos Santos
 * @date June 24th, 2023.
 * @file hashtbl.h
 */

#ifndef HASHTBL_H
#define HASHTBL_H

#include <iostream>     // cout, endl, ostream
#include <forward_list> // forward_list
#include <algorithm>    // copy, find_if, for_each
#include <cmath>        // sqrt
#include <iterator>     // std::begin(), std::end()
#include <initializer_list>
#include <utility> // std::pair

/// Namespace containing the associative container HashTbl.
namespace ac 
{
    /*!
     * @struct HashEntry
     * @brief The HashEntry struct represents a single entry in the hash table that associates a key with data.
     * @tparam KeyType The key type.
     * @tparam DataType The data type.
     */
	template<class KeyType, class DataType>
	struct HashEntry {
        KeyType m_key; //!< Data key
        DataType m_data; //!< The data

        /*!
         * @brief Regular constructor.
         * @param kt_ Key of the entry.
         * @param dt_ Data of the entry.
         */
        HashEntry( KeyType kt_, DataType dt_ ) : m_key{kt_} , m_data{dt_}
        {/*Empty*/}

        /*!
         * @brief Overloaded << operator to display entry.
         * @param os_ The output stream.
         * @param he_ The HashEntry object to insert into the stream.
         * @return std::ostream& Reference to the output stream after inserting the object.
         */
        friend std::ostream & operator<<( std::ostream & os_, const HashEntry & he_ ) {
            os_ << "{" << he_.m_key << "," << he_.m_data << "}";
            // os_ << he_.m_data;
            return os_;
        }
    };

    /*!
     * @class HashTbl
     * @brief A template class representing a hash table container.
     *
     * @note This class implements an unordered dictionary through dynamic allocation of an array of lists of table entries.
     *
     * @tparam KeyType The key type.
     * @tparam DataType The data type.
     * @tparam KeyHash The hash function.
     * @tparam KeyEqual The key comparison function.
     */
	template< class KeyType,
		      class DataType,
		      class KeyHash = std::hash< KeyType >,
		      class KeyEqual = std::equal_to< KeyType > >
	class HashTbl {
        public:
            // Aliases
            using entry_type = HashEntry<KeyType,DataType>; //!< The type of individual entries in the hash table.
            using list_type  = std::forward_list< entry_type >; //!< The type of lists used to store entries.
            using size_type  = std::size_t; //!< The size type.

            /*!
             * @brief Default constructor.
             * @param table_sz_ The initial size of the table. Defaults to DEFAULT_SIZE.
             */
            explicit HashTbl( size_type table_sz_ = DEFAULT_SIZE );

            /*!
             * @brief Copy constructor.
             * 
             * @param source The HashTbl object to be copied.
             */
            HashTbl( const HashTbl& source);

            /*!
             * @brief Constructs a hash table from an initializer list.
             * 
             * It checks if the elements of the initializer list have the same key. 
             * If the key has already been inserted, the data of that entry is overwritten.
             *
             * @param ilist The initializer list of entries.
             */
            HashTbl( const std::initializer_list< entry_type > & ilist);

            /*!
             * @brief Overloaded assignment operator that assigns an hash table to the current hash table.
             * @param clone The another hash table to assign to the current hash table.
             * @return HashTbl& A reference to hash table updated.
             */
            HashTbl& operator=( const HashTbl& clone);


            /*!
             * @brief Overloaded assignment operator that assigns an initializer list to the hash table.
             *
             * It checks if the elements of the initializer list have the same key. 
             * If the key has already been inserted, the data of that entry is overwritten.
             * 
             * @param other The initializer list to assign to the hash table.
             * @return HashTbl& A reference to hash table updated.
             */
            HashTbl& operator=( const std::initializer_list< entry_type > & ilist);

            /*!
             * @brief Destructor.
             */
            virtual ~HashTbl();

            /*!
             * @brief Inserts a new item in the table by associating a key with data.
             *
             * If the given key already exists in the hash table, then it overwrites the data associated with that key.
             *
             * @param key_ The key of the item.
             * @param new_data_ The data of the item.
             * @return bool True if the insertion is successful, False if the key already exists in the table.
             */
            bool insert( const KeyType & key_, const DataType & new_data_);

            /*!
             * @brief Retrieves the data associated with a given key.
             * @param key_ The key to search for.
             * @param data_item_ The variable to store the retrieved data.
             * @return bool True if the key is found, False otherwise.
             */
            bool retrieve( const KeyType & key_, DataType & data_item_) const;

            /*!
             * @brief Erases a hash table item from its key.
             * @param key_ The key of the item to be erased.
             * @return bool True if the erasure is successful, False if the key is not found.
             */
            bool erase( const KeyType & key_);

            /*!
             * @brief Removes all elements from the lists in the table.
             */
            void clear();

            /*!
             * @brief Checks if the hash table is empty.
             * @return bool True if the hash table is empty, False otherwise.
             */
            bool empty() const;

            /*!
             * @brief Returns the size of the hash table.
             * @return size_type The number of elements in the hash table.
             */
            inline size_type size() const { return m_count; };

            /*!
             * @brief Accesses the data associated with a given key.
             *
             * If the key is not found, an out of range exception is thrown.
             *
             * @param key_ The key of the item to be accessed.
             * @return DataType& Reference to the data associated with the key.
             */
            DataType& at( const KeyType& key_);

            /*!
             * @brief Accesses the data associated with a given key of the hash table using the square bracket.
             * 
             * If the key is not found, a default entry is entered and its reference is returned.
             *
             * @param key The key of the item to be accessed.
             * @return DataType& Reference to the data associated with the key.
             */
            DataType& operator[]( const KeyType& key_);

            /*!
             * @brief Provides the count of elements in the table that are in the collision list associated with a given key.
             * @param key_ The given key for the count.
             * @return size_type The number of elements with the specified key.
             */
            size_type count( const KeyType& key_) const;

            /*!
             * @brief Returns the maximum load factor of the hash table. 
             * @return float The maximum load factor.
             */
            float max_load_factor() const { return m_max_load_factor; }

            /*!
             * @brief Sets the maximum load factor of the hash table. 
             * @param mlf The new maximum load factor.
             */
            void max_load_factor(float mlf) { m_max_load_factor = mlf; }

            /*!
             * @brief Overloaded << operator to display hash table.
             * @param os_ The output stream.
             * @param ht_ The hash table from which data will be inserted into the stream.
             * @return std::ostream& Reference to the output stream after inserting the table data.
             */
            friend std::ostream & operator<<( std::ostream & os_, const HashTbl & ht_ ) {
                // for each linked list in the table...
                for (size_t i{0}; i < ht_.m_size; ++i) {
                    os_ << "[" << i << "]-> ";
                    // for each element in the linked list, print the data of the entry
                    for (const auto& entry : ht_.m_table[i]) 
                        os_ << entry.m_data << " ";
                    os_ << "\n";
                }
                return os_;
            }

        private:
            /*!
             * @brief Finds the next prime number greater or equal than the given number. 
             * @param n_ The given number.
             * @return size_type The next prime number.
             */
            static size_type find_next_prime( size_type n_ );

            /*!
             * @brief Checks if a number is prime.
             * 
             * Uses the Sieve of Eratosthenes.
             *
             * @param n_ The number to check.
             * @return bool True if the number is prime, false otherwise.
             */
            static bool prime( size_type n_ ); 

            /*!
             * @brief Adjusts the table when the load factor exceeds the maximum load factor value.
             *
             * The items in the table are redistributed according to the new table size, which corresponds to the smallest 
             * prime number greater than or equal to twice the size of the table before the rehash() call.
             *
             */
            void rehash( void );

        private:
            size_type m_size; //!< The size of the table.
            size_type m_count;//!< The number of elements in the table.
            float m_max_load_factor; //!< The maximum load factor value.
            // std::unique_ptr< std::forward_list< entry_type > [] > m_table;
            std::forward_list< entry_type > *m_table; //!< Table of lists for table entries.
            static const short DEFAULT_SIZE = 11;
    };

} // MyHashTable
#include "hashtbl.inl"
#endif
