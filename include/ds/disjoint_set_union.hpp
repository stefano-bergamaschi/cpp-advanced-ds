#ifndef disjoint_set_union
#define disjoint_set_union

#include <vector>
#include <unordered_map>

namespace ds{
    template<typename T>
    class dsu{
        private:
            vector<size_t> parent_;
            vector<size_t> size_;
            unordered_map<T, size_t> element_to_id_;
            vector<T> id_to_element_;
            size_t next_index_;
            size_t set_count_;

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
            size_t convert_to_index(T element){
                return element_to_id[element];
            }

            
        public:
            // * @brief Checks if the element is already in the DSU.
            // * @param The element to check.
            // * @return True if the element is in the DSU, false otherwise.
            bool contains(T element){
                pair<T, size_t> iterator = element_to_id_.find(element);
                if(iterator == element_to_id.end()) return false;
                return true;
            }

            // * @brief Adds an element to the DSU. If the element is already in the DSU, it does nothing.
            // * @param The element to add.
            // * @return True if the element was added, false if it was already in the DSU.
            bool add_element(T element){
                if(contains(element)) return false;

                element_to_id_[next_index_] = element;
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
                add_element(element1);
                add_element(element2);

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
            bool strict_unite(T element1, T element2){
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

            // * @brief Checks if two elements are in the same set.
            // * @param The first element.
            // * @param The second element.
            // * @return True if the elements are in the same set, false otherwise.
            bool same_set(T element1, T element2){
                if(add_element(element1) or add_element(element2)) return false;
                if(find_root_by_index(convert_to_index(element1)) == find_root_by_index(convert_to_index(element1))) return true;
                return false;
            }

            // * @brief Checks if two elements are in the same set only if both elements are already in the DSU.
            // * @param The first element.
            // * @param The second element.
            // * @return True if the elements are in the same set, false otherwise.
            bool strict_same_set(T element1, T element2){
                if(!contains(element1) or !contains(element2)) return false;
                if(find_root_by_index(convert_to_index(element1)) == find_root_by_index(convert_to_index(element1))) return true;
                return false;
            }

            // * @brief Returns the number of elements in the DSU.
            // * @return The number of elements in the DSU.
            size_t size(){
                return next_index_;
            }

            // * @brief Returns the size of the set containing the element.
            // * @param The element.
            // * @return The size of the set containing the element.
            size_t set_size(T element){
                if(add_element(element)) return 1;
                return size_[find_root_by_index(convert_to_index(element))];
            }

            // * @brief Returns the size of the set containing the element only if the element is already in the DSU.
            // * @param The element.
            // * @return The size of the set containing the element, or 0 if the element is not in the DSU.
            size_t strict_set_size(T element){
                if(!contains(element)) return 0;
                return size_[find_root_by_index(convert_to_index(element))];
            }   

            // * @brief Returns the number of sets in the DSU.
            // * @return The number of sets in the DSU.
            size_t set_count(){
                return set_count_;
            }

            // * @brief Returns all elements in the set containing the element.
            // * @param The element.
            // * @return A vector containing all elements in the set.
            vector<T> get_set(T element){
                if(add_element(element)) return vector<T>{element};
                vector<T> result;
                size_t root_index = find_root_by_index(convert_to_index(element));
                for(size_t i = 0; i < next_index_; i++){
                    if(find_root_by_index(i) == root_index) result.push_back(id_to_element_[i]);
                }
                return result;
            }

            // * @brief Returns all elements in the set containing the element only if the element is already in the DSU.
            // * @param The element.
            // * @return A vector containing all elements in the set, or an empty vector if the element is not in the DSU.
            vector<T> strict_same_set(T element){
                if(!contains(element)) return vector<T>{};
                vector<T> result;
                size_t root_index = find_root_by_index(convert_to_index(element));
                for(size_t i = 0; i < next_index_; i++){
                    if(find_root_by_index(i) == root_index) result.push_back(id_to_element_[i]);
                }
                return result;
            }

            // vector<vector<T>> get_all_sets(){
            //     vector<vector<T>> result;
            //     unordered_map<size_t, vector<T>> root_to_elements;
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
            bool empty(){
                return !next_index_;
            }

            // * @brief Clears the DSU, removing all elements and sets.
            bool clear(){
                parent_.clear();
                size_.clear();
                element_to_id_.clear();
                id_to_element_.clear();
                next_index_ = 0;
                set_count_ = 0;
            }
    };
}





#endif