#ifndef rope_HPP
#define rope_HPP

#include <memory>
#include <cstring>
#include <cassert>
#include <cmath>


#ifndef NDEBUG

#else

#endif

namespace rope {

namespace utils {

template <typename type>
inline bool in_range(type start, type value, type end) {
    return value >= start && value < end;
}

inline bool memcpy_bound_check(char *begin, char *end, char *start, char *stop) {
    return start <= stop && in_range(begin, start, end) && in_range(begin, stop - 1, end);  // - 1 because stop address is never written to
}

} // namespace utils

template <size_t BUFFER_LENGTH, template <typename type> typename allocator = std::allocator>
class rope_t {
    static_assert(BUFFER_LENGTH > 0);
public:
    struct rope_node_t {
        rope_node_t *left, *right, *parent;
        size_t count;
        char ch_buff[BUFFER_LENGTH];
        bool is_leaf() { return !left && !right; }  // if left and right does not exist

        template <typename rope_node_allocator_t>
        static rope_node_t *rope_node(const std::string& str, rope_node_allocator_t& rope_node_allocator) {
/            rope_node_t *root_node = rope_node_allocator.allocate(1);
            root_node->parent = nullptr;
            rope_node_impl(root_node, str.data(), 0, str.size(), rope_node_allocator);
            return root_node;
        }

        template <typename rope_node_allocator_t>
        static void delete_rope_node(rope_node_t *node, rope_node_allocator_t& rope_node_allocator) {
            rope_node_t *parent = node->parent;
            delete_rope_node_impl(node, rope_node_allocator);
            node = parent;
            fix_parent_count(node);
        }

        template <typename rope_node_allocator_t>
        static rope_node_t *concate(rope_node_t *r1, rope_node_t *r2, rope_node_allocator_t& rope_node_allocator) {
            assert(!r1->parent && !r2->parent); // r1 and r2 should not have any parents
            rope_node_t *rope_node = rope_node_allocator.allocate(1);
            r1->parent = r2->parent = rope_node;
            rope_node->parent = nullptr;
            rope_node->left = r1;
            rope_node->right = r2;
            rope_node->count = r1->count + r2->count;
            return rope_node;
        }

        static void slice(rope_node_t *node, size_t pos, size_t n, char *o_str) {
            slice_impl(node, pos, n, o_str);
        }

        // this call can change the node, maybe in future make this a ** instead of *& ?
        template <typename rope_node_allocator_t>
        static void set_slice(rope_node_t *&root_node, const char *str, size_t size, size_t pos, size_t n, rope_node_allocator_t& rope_node_allocator) {
            set_slice_impl(root_node, str, size, pos, n, rope_node_allocator);
        }

    private:
        static void fix_parent_count(rope_node_t *node) {
            if (node) {
                while (node->parent) {
                    node->parent->count = node->parent->left->count + node->parent->right->count;
                    node = node->parent;
                }
            }
        }

        template <typename rope_node_allocator_t>
        static void rope_node_impl(rope_node_t *node, const char *str, size_t l, size_t r, rope_node_allocator_t& rope_node_allocator) {
            if ((r - l) > BUFFER_LENGTH) {
                size_t m = (l + r) / 2;

                node->left = rope_node_allocator.allocate(1);
                node->left->parent = node;
                rope_node_impl(node->left, str, l, m, rope_node_allocator);
                node->right = rope_node_allocator.allocate(1);
                node->right->parent = node;
                rope_node_impl(node->right, str, m, r, rope_node_allocator);
                
                node->count = node->left->count + node->right->count;
            } else {
                node->left = node->right = nullptr;
                node->count = r - l;
                size_t index = 0;
                for (size_t i = l; i < r; i++) {
                    node->ch_buff[index++] = str[i];
                }
            }
        }

        template <typename rope_node_allocator_t>
        static void delete_rope_node_impl(rope_node_t *node, rope_node_allocator_t& rope_node_allocator) {
            if (!node) return;
            if (node->parent) {
                if (node->parent->left == node) {
                    node->parent->count -= node->count;
                    node->parent->left = nullptr;
                }
                if (node->parent->right == node) {
                    node->parent->count -= node->count;
                    node->parent->right = nullptr;
                }
            }
            delete_rope_node_impl(node->left, rope_node_allocator);
            delete_rope_node_impl(node->right, rope_node_allocator);
            rope_node_allocator.deallocate(node, 1);
        }

        template <size_t STACK_SIZE>
        struct preorder_stack_rope_node_traversal_t {
            preorder_stack_rope_node_traversal_t(rope_node_t *root_node) {
                assert(root_node);  // root node cant be null
                push(root_node);
            }

            std::pair<rope_node_t *, size_t> next_node(size_t pos, size_t traversed) {
                if (current_node) {
                    assert(current_node->is_leaf());
                    size_t idx = pos - traversed;
                    if (idx >= current_node->count) traversed += current_node->count; // update traversed, work in next node
                    else return { current_node, traversed };  // dont update anything, work in same node
                }
                while (should_continue()) {
                    current_node = pop();
                    if (utils::in_range(traversed, pos, traversed + current_node->count)) {
                        if (current_node->right) push(current_node->right);
                        if (current_node->left) push(current_node->left);
                        if (current_node->is_leaf()) {
                            return { current_node, traversed };
                        }
                    } else {
                        traversed += current_node->count;
                    }
                }
                current_node = nullptr;
                return { current_node, traversed };
            }

            std::pair<rope_node_t *, size_t> next_node(size_t pos) {
                size_t traversed = 0;
                return next_node(pos, traversed);
            }
        
            rope_node_t *next_node() {
                while (should_continue()) {
                    current_node = pop();
                    if (current_node->right) push(current_node->right);
                    if (current_node->left) push(current_node->left);
                    if (current_node->is_leaf()) {
                        return current_node;
                    }
                }    
                current_node = nullptr;
                return current_node;
            }

        private:
            bool should_continue() { return stack_idx; }
            void push(rope_node_t *node) {
                assert(stack_idx < STACK_SIZE);  // overflow
                stack[stack_idx++] = node;
            }
            rope_node_t *pop() {
                assert(stack_idx > 0);  // underflow
                return stack[--stack_idx];
            }

            rope_node_t *current_node = nullptr;
            size_t stack_idx = 0;
            rope_node_t *stack[STACK_SIZE];
        };

        static void slice_impl(rope_node_t *node, size_t pos, size_t n, char *o_str) {
            assert(pos <= node->count && pos + n <= node->count);  // bounds check
            size_t str_idx = 0;
            preorder_stack_rope_node_traversal_t<64> preorder_stack_rope_node_traversal(node);
            auto [current_node, traversed] = preorder_stack_rope_node_traversal.next_node(pos);
            while (n) {
                assert(current_node);  // in general shouldnt be null ?
                size_t idx = pos - traversed;
                for (size_t i = idx; i < current_node->count; i++) {
                    assert(utils::in_range(size_t{0}, i, current_node->count));
                    o_str[str_idx++] = current_node->ch_buff[i];
                    n--; if (!n) return;
                }
                pos += current_node->count - idx;
                traversed += current_node->count;
                current_node = preorder_stack_rope_node_traversal.next_node();
            }
            assert(false);  // shouldnt really happend ?
        }

        template <typename rope_node_allocator_t>
        static void set_slice_impl(rope_node_t *&node, const char *str, size_t size, size_t pos, size_t n, rope_node_allocator_t& rope_node_allocator) {
            assert(pos <= node->count && pos + n <= node->count);
            preorder_stack_rope_node_traversal_t<64> preorder_stack_rope_node_traversal(node);
            size_t str_idx = 0;
            // adding to the end, u cant delete at node->count, can only add
            if (pos == node->count) {
                auto [current_node, traversed] = preorder_stack_rope_node_traversal.next_node(pos - 1);  // get the node with the last char
                assert(current_node);  // in general shouldnt be null ?
                if (current_node->count < BUFFER_LENGTH) {  // last node has space to accomodate 
                    for (size_t i = current_node->count; i < BUFFER_LENGTH; i++) {
                        current_node->ch_buff[i] = str[str_idx++];
                        if (str_idx == size) break;
                    }
                    current_node->count += str_idx;  // increase node count by how many characters we were able to shove
                    fix_parent_count(current_node);  // if node count is updated, the parents need to get updated
                }
                if (str_idx == size) return;  // early return, no need to create new space
                rope_node_t *r1 = node;
                rope_node_t *r2 = rope_node_allocator.allocate(1);
                r2->parent = nullptr;
                rope_node_impl(r2, str, str_idx, size, rope_node_allocator);
                node = concate(r1, r2, rope_node_allocator);
                return;
            }
            auto [current_node, traversed] = preorder_stack_rope_node_traversal.next_node(pos);
            assert(current_node);  // shouldnt really happen ?
            
            size_t num_max_affected_nodes = size_t(std::ceil(float(size) / float(BUFFER_LENGTH))) + 3;
            rope_node_t *affected_nodes[num_max_affected_nodes];
            size_t affected_node_idx = 0;

            // case size == n  // size of writes == selection region 
            if (size == n) {
                while (current_node) {
                    assert(current_node);  // should never fail in general ?
                    size_t idx = pos - traversed;
                    for (size_t i = idx; i < current_node->count; i++) {
                        current_node->ch_buff[i] = str[str_idx++];
                        size--;  // size reduced with each write
                        n--;  // selection region reduced with each write
                        pos++;  // advance the position every write
                        if (!n) return;  // no more selection or writes anymore, no need to update any count, so can leave
                    }
                    auto pair = preorder_stack_rope_node_traversal.next_node(pos, traversed);
                    current_node = pair.first;
                    traversed = pair.second;
                }
                assert(false); // should never reach this
            }

            // case size < n  // write less than the selection region
            if (size < n) {
                // complete all writes first
                if (size) {
                    while (current_node) {
                        affected_nodes[affected_node_idx++] = current_node;
                        assert(affected_node_idx <= num_max_affected_nodes);

                        assert(current_node);  // should never fail in general ?
                        size_t idx = pos - traversed;
                        for (size_t i = idx; i < current_node->count; i++) {
                            current_node->ch_buff[i] = str[str_idx++];
                            size--;  // size of writes reduced with each writes
                            n--;  // selection region reduces with with write
                            pos++;  // advance position with each write
                            if (!size) break;  // leave for, no more writes
                        }
                        auto pair = preorder_stack_rope_node_traversal.next_node(pos, traversed);
                        current_node = pair.first;
                        traversed = pair.second;
                        if (!size) break;  // leave while
                    }
                }
                
                assert(!size); // all writes should be done here

                assert(current_node); // should be valid here
                while (current_node) {
                    if (affected_nodes[affected_node_idx - 1] != current_node) {  // dont push same node as current node
                        affected_nodes[affected_node_idx++] = current_node;
                        assert(affected_node_idx <= num_max_affected_nodes);
                    }
                    
                    // things that can happen
                        // case 1: [elem, elem, <-]
                            // in this case, trim the node count
                            // n = n - (current_node->count - idx)
                            // current_node->count = idx
                        // case 2: [elem, ->, <-, elem]
                            // move elements back n spaces from idx
                            // current_node->count -= n
                        // case 3: [->,<-] : idx == 0
                            // in this case set the node count to 0
                            // n = n - current_node->count
                            // current_node->count = 0
                        // case 4: [->, elem, elem] == 0
                            // in this case  memcpy and move the array by n elements 
                            // and set the node count
                            // current_node->count -= n

                    size_t idx = pos - traversed;
                    if (idx == 0) {
                        if (n >= current_node->count) {  // case 3
                            n -= current_node->count;
                            current_node->count = 0;
                        } else {  // case 4
                            std::memcpy(current_node->ch_buff, current_node->ch_buff + n, current_node->count - n);
                            current_node->count -= n;
                            n = 0;
                        }
                    } else {
                        if (idx + n >= current_node->count) {  // case 1
                            n -= (current_node->count - idx);
                            current_node->count = idx;
                        } else {  // case 2
                            std::memcpy(current_node->ch_buff + idx, current_node->ch_buff + idx + n, current_node->count - (idx + n));
                            current_node->count -= n;
                            n = 0;
                        }
                    }

                    if (!n) break;
                    traversed += current_node->count; // value is always updated to the start of every nodes affected value
                    current_node = preorder_stack_rope_node_traversal.next_node();
                }

                for (size_t i = 0; i < affected_node_idx; i++) {
                    rope_node_t *node = affected_nodes[i];
                    // TODO: (optimization) double update parent node count optimisation
                    fix_parent_count(node);
                }
                return;
            }

            // case size > n // write more than the selection region            
            if (size > n) {
                if (n) {
                    while (current_node) {
                        affected_nodes[affected_node_idx++] = current_node;
                        assert(affected_node_idx <= num_max_affected_nodes);

                        assert(current_node);  // should never fail in general ?
                        size_t idx = pos - traversed;
                        for (size_t i = idx; i < current_node->count; i++) {
                            current_node->ch_buff[i] = str[str_idx++];
                            size--;  // size of writes reduced with each writes
                            n--;  // selection region reduces with with write
                            pos++;  // advance position with each write
                            if (!n) break;  // leave for, no more selections
                        }
                        // TODO: (optimization) if current_node->count < BUFFER_LENGTH, write any extra writes here and pull back from next node 
                        auto pair = preorder_stack_rope_node_traversal.next_node(pos, traversed);
                        current_node = pair.first;
                        traversed = pair.second;
                        if (!n) break;  // leave while
                    }
                }

                assert(!n);  // all selections should be done here

                assert(current_node);

                size_t idx = pos - traversed;

                // shuffle data
                char new_str[size];  // copy left over str into new buffer (hack, probably can do it without this, need to do it cause str is const)
                char temp[current_node->count - idx];  // left over part


                // debug only checks
                char *new_str_end = &new_str[size];
                char *new_str_begin = new_str;
                char *temp_end = &temp[current_node->count - idx];
                char *temp_begin = temp;

                // TODO: debug bound assert for str
                std::memcpy(new_str, str + str_idx, size);
                str_idx = 0;

                std::memcpy(temp, current_node->ch_buff + idx, (current_node->count - idx));
                std::memcpy(current_node->ch_buff + idx, new_str, std::min(size, (current_node->count - idx)));
                if (size <= (current_node->count - idx)) {
                    std::memcpy(current_node->ch_buff + idx + size, temp, (current_node->count - idx) - size);
                    std::memcpy(new_str, temp + (current_node->count - idx) - size, size);
                } else {
                    std::memcpy(new_str, new_str + (current_node->count - idx), size - (current_node->count - idx));
                    std::memcpy(new_str + size - (current_node->count - idx), temp, (current_node->count - idx));                        
                }
                
                // try to put the how much ever u can of the new str data
                if (current_node->count < BUFFER_LENGTH) {
                    for (size_t i = current_node->count; i < BUFFER_LENGTH; i++) {
                        current_node->ch_buff[i] = new_str[str_idx++];
                        size--;  // reduce write size
                        pos++;
                        if (!size) break;
                    }
                    current_node->count += str_idx;
                    if (affected_nodes[affected_node_idx - 1] != current_node) {  // dont push same node as current node
                        affected_nodes[affected_node_idx++] = current_node;
                        assert(affected_node_idx <= num_max_affected_nodes);
                    }
                }

                // create new nodes
                if (size) {
                    // TODO: (optimization) try add in atleast 1 node ahead to avoid soo many node allocations
                    assert(current_node->count == BUFFER_LENGTH);
                    current_node->left = rope_node_allocator.allocate(1);
                    current_node->right = rope_node_allocator.allocate(1);
                    current_node->left->left = current_node->left->right = current_node->right->left = current_node->right->right = nullptr;
                    current_node->left->parent = current_node->right->parent = current_node;
                    std::memcpy(current_node->left->ch_buff, current_node->ch_buff, BUFFER_LENGTH);
                    current_node->left->count = BUFFER_LENGTH;
                    rope_node_impl(current_node->right, new_str + str_idx, 0, size, rope_node_allocator);

                    affected_nodes[affected_node_idx++] = current_node->left;
                    assert(affected_node_idx <= num_max_affected_nodes);
                    affected_nodes[affected_node_idx++] = current_node->right;
                    assert(affected_node_idx <= num_max_affected_nodes);
                }


                for (size_t i = 0; i < affected_node_idx; i++) {
                    rope_node_t *node = affected_nodes[i];
                    // TODO: (optimization) double update parent node count optimisation
                    fix_parent_count(node);
                }
                return;
            }
            assert(false); // should never come here
        }
    };

    typedef allocator<rope_node_t> rope_node_allocator_t;

    rope_t(const std::string& str) : _rope_node_allocator(), _root_node(rope_node_t::rope_node(str, _rope_node_allocator)) {}

    ~rope_t() {
        rope_node_t::delete_rope_node(_root_node, _rope_node_allocator);
    }

    void slice(size_t pos, size_t n, char *o_str) const {
        rope_node_t::slice(_root_node, pos, n, o_str);
    }

    void set_slice(const char *str, size_t size, size_t pos, size_t n) {
        rope_node_t::set_slice(_root_node, str, size, pos, n, _rope_node_allocator);
    }

    std::string to_string() const {
        std::string str;
        str.resize(size());
        rope_node_t::slice(_root_node, 0, size(), str.data());
        return str;
    }

    size_t size() const {
        return _root_node->count;
    }

    // TODO: add insert, erase

private:

    rope_node_allocator_t _rope_node_allocator;
    rope_node_t *_root_node;
}; 

} // namespace rope

template <size_t BUFFER_LENGTH, template <typename type> typename allocator>
std::ostream& operator <<(std::ostream& o, const rope::rope_t<BUFFER_LENGTH, allocator>& rope) {
    o << rope.to_string();
    return o;
}

#endif
