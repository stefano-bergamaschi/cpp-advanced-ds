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
    };
}





#endif