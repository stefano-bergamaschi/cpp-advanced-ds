#ifndef disjoint_set_union
#define disjoint_set_union

#include <stdexcept>
#include <unordered_map>
#include <vector>

namespace ds{
    template<typename T>
    class dsu{
        private:
            std::vector<size_t> parent_;
            std::vector<size_t> size_;
            std::unordered_map<T, size_t> element_to_id_;
            std::vector<T> id_to_element_;
            size_t next_index_ = 0;
            size_t set_count_ = 0;

            // * @brief Finds the root of the set containing the element at the given index, applying path compression.
            // * @param The index of the element.
            // * @return The index of the root of the set containing the element.
            size_t find_root_by_index(size_t index){
                size_t root = index;
                while(root != parent_[root]) root = parent_[root];

                size_t current = index;
                while(current != root){
                    size_t next = parent_[current];
                    parent_[current] = root;
                    current = next;
                }

                return root;
            }

            // * @brief Converts an element to its corresponding index in the DSU.
            // * @param The element to convert.
            // * @return The index corresponding to the element.
            size_t convert_to_index(T& element) {
                return element_to_id_[element];
            }

            
        public:
            using value_type = T;
            using std::swap;
            
            // * @brief Default constructor for the DSU, creating an empty DSU with no elements or sets.
            dsu() = default;

            // * @brief Constructs a DSU with a given initial capacity, reserving space for that many elements to optimize performance.
            // * @param capacity The initial capacity for the DSU.
            explicit dsu(size_t capacity){
                reserve(capacity);
            }  

            // * @brief Copy constructor for the DSU, creating a new DSU that is a copy of the given DSU.
            // * @param other The DSU to copy.
            dsu(const dsu&) = default;
            
            // * @brief Move constructor for the DSU, creating a new DSU by moving the resources of the given DSU.
            // * @param other The DSU to move.
            dsu(dsu&&) noexcept = default;
            
            // * @brief Copy constructor for the DSU, creating a new DSU that is a copy of the given DSU.
            // * @param other The DSU to copy.
            dsu& operator=(const dsu&) = default;
            
            // * @brief Move constructor for the DSU, creating a new DSU by moving the resources of the given DSU.
            // * @param other The DSU to move.
            dsu& operator=(dsu&&) noexcept = default;

            // * @brief Destructor for the DSU, cleaning up any resources used by the DSU.
            ~dsu() = default;
        
            // * @brief Checks if the element is already in the DSU.
            // * @param The element to check.
            // * @return True if the element is in the DSU, false otherwise.
            bool contains(T element) const {
                return element_to_id_.find(element) != element_to_id_.end();
            }

            // * @brief Adds an element to the DSU. If the element is already in the DSU, it does nothing.
            // * @param The element to add.
            // * @return True if the element was added, false if it was already in the DSU.
            bool add_element(T element){
                if(contains(element)) return false;

                element_to_id_[element] = next_index_;
                id_to_element_.push_back(element);
                parent_.push_back(next_index_);
                size_.push_back(1);

                next_index_++;
                set_count_++;

                return true;
            }

            // * @brief Unites two elements into the same set.
            // * @param The first element.
            // * @param The second element.
            // * @return True if the elements were in different sets and were united, false otherwise.
            bool unite(T element1, T element2){
                if(!contains(element1) or !contains(element2)) return false;

                size_t index1 = find_root_by_index(convert_to_index(element1));
                size_t index2 = find_root_by_index(convert_to_index(element2));

                if(index1 == index2) return false;

                if(size_[index1] < size_[index2]){
                    parent_[index1] = index2;
                    size_[index2] += size_[index1];
                }  
                else{
                    parent_[index2] = index1;
                    size_[index1] += size_[index2];
                } 
                set_count_--;
                
                return true;
            }

            // * @brief Unites two elements into the same set only if both elements are already in the DSU and are in different sets.
            // * @param The first element.
            // * @param The second element.
            // * @return True if the elements were in different sets and were united, false otherwise.
            // * @throws runtime_error if either element is not in the DSU.
            bool strict_unite(T element1, T element2){
                if(!contains(element1) or !contains(element2)) throw std::runtime_error("Element not in DSU");
                return unite(element1, element2);
            }

            // * @brief Checks if two elements are in the same set.
            // * @param The first element.
            // * @param The second element.
            // * @return True if the elements are in the same set, false otherwise.
            bool same_set(T element1, T element2){
                if(!contains(element1) or !contains(element2)) return false;
                if(find_root_by_index(convert_to_index(element1)) == find_root_by_index(convert_to_index(element2))) return true;
                return false;
            }

            // * @brief Checks if two elements are in the same set only if both elements are already in the DSU.
            // * @param The first element.
            // * @param The second element.
            // * @return True if the elements are in the same set, false otherwise.
            // * @throws runtime_error if either element is not in the DSU.
            bool strict_same_set(T element1, T element2){
                if(!contains(element1) or !contains(element2)) throw std::runtime_error("Element not in DSU");
                return same_set(element1, element2);
            }

            // * @brief Returns the number of elements in the DSU.
            // * @return The number of elements in the DSU.
            size_t size() const {
                return next_index_;
            }

            // * @brief Returns the size of the set containing the element.
            // * @param The element.
            // * @return The size of the set containing the element.
            size_t set_size(T element){
                if(!contains(element)) return 0;
                return size_[find_root_by_index(convert_to_index(element))];
            }

            // * @brief Returns the number of sets in the DSU.
            // * @return The number of sets in the DSU.
            size_t set_count() const {
                return set_count_;
            }

            // * @brief Returns all elements in the set containing the element.
            // * @param The element.
            // * @return A vector containing all elements in the set.
            // std::vector<T> get_set(T element){
            //     if(!contains(element)) return std::vector<T>{};
            //     std::vector<T> result;
            //     size_t root_index = find_root_by_index(convert_to_index(element));
            //     for(size_t i = 0; i < next_index_; i++){
            //         if(find_root_by_index(i) == root_index) result.push_back(id_to_element_[i]);
            //     }
            //     return result;
            // }

            // * @brief Returns all sets in the DSU as a vector of vectors, where each inner vector contains the elements of a set.
            // * @return A vector of vectors, where each inner vector contains the elements of a
            // vector<vector<T>> get_all_sets(){
            //     std::vector<std::vector<T>> result;
            //     std::unordered_map<size_t, std::vector<T>> root_to_elements;
            //     for(size_t i = 0; i < next_index_; i++){
            //         size_t root_index = find_root_by_index(i);
            //         root_to_elements[root_index].push_back(id_to_element_[i]);
            //     }
            //     for(auto& pair : root_to_elements){
            //         result.push_back(pair.second);
            //     }
            //     return result;
            // }

            // * @brief Checks if the DSU is empty.
            // * @return True if the DSU is empty, false otherwise.
            bool empty() const {
                return next_index_ == 0;
            }

            // * @brief Clears the DSU, removing all elements and sets.
            void clear(){
                parent_.clear();
                size_.clear();
                element_to_id_.clear();
                id_to_element_.clear();
                next_index_ = 0;
                set_count_ = 0;
            }

            // * @brief Reserves space for a given number of elements in the DSU to optimize memory usage and performance.
            // * @param size The number of elements to reserve space for.
            void reserve(size_t size){
                parent_.reserve(size);
                size_.reserve(size);
                element_to_id_.reserve(size);
                id_to_element_.reserve(size);
            }


            // * @brief Shrinks the capacity of the DSU's internal data structures to fit their current size, optimizing memory usage.
            void fit(){
                parent_.shrink_to_fit();
                size_.shrink_to_fit();
                element_to_id_.shrink_to_fit();
                id_to_element_.shrink_to_fit();
            }

            // * @brief Applies path compression to all elements in the DSU, optimizing the structure for future find operations.
            void compress_path(){
                for(size_t i = 0; i < next_index_; i++){
                    find_root_by_index(i);
                }
            }
    };
}





#endif