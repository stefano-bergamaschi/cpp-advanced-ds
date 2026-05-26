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

            size_t convert_to_index(T element){
                return element_to_id[element];
            }

            
        public:
            bool contains(T element){
                pair<T, size_t> iterator = element_to_id_.find(element);
                if(iterator == element_to_id.end()) return false;
                return true;
            }

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

            bool same_set(T element1, T element2){
                if(add_element(element1) or add_element(element2)) return false;
                if(find_root_by_index(convert_to_index(element1)) == find_root_by_index(convert_to_index(element1))) return true;
                return false;
            }

            size_t size(){
                return next_index_;
            }

            size_t set_size(T element){
                return size_[find_root_by_index(convert_to_index(element))];
            }

            size_t set_count(){
                return set_count_;
            }

            vector<T> get_set(T element){
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



            

            
    };
}





#endif