#ifndef ROPE_HPP
#define ROPE_HPP

#include <cstring>
#include <string>
#include <cmath>
#include <cassert>
#include <iostream>
// #include <stack>

namespace rope {

namespace utils {

template <typename _Tp>
bool in_range(_Tp start, _Tp pos, _Tp end) {
    if (pos >= start && pos < end) return true;
    return false;
}

} // namespace utils

// MAX_CH_BUFF_LENGTH is how much data to store per node
template <size_t MAX_CH_BUFF_LENGTH, template <typename _Tp> typename _Alloc = std::allocator>
class rope {
    static_assert(MAX_CH_BUFF_LENGTH > 0);
public:
    struct rope_node {
        rope_node *left, *right, *parent;
        size_t count;  // if left == right == nullptr, then count is the number of elements in ch_buff, otherwise count is sum of children 
        char ch_buff[MAX_CH_BUFF_LENGTH]; 

        bool is_leaf() {
            return !left && !right;
        }
        
        template <typename rope_node_allocator>
        static rope_node *new_rope_node(const std::string& str, rope_node_allocator& _rope_node_allocator) {
            rope_node *node = _rope_node_allocator.allocate(1);
            node->parent = nullptr;
            new_rope_node_impl(node, str.data(), 0, str.size(), _rope_node_allocator);
            return node;
        }

        // atm it is assumed only root node on root node delete is called
        // deleting anyother node (internal or leaf) will work but will break insert because that assumes left and right to be either defined together or not defined together
        template <typename rope_node_allocator>
        static void delete_rope_node(rope_node *node, rope_node_allocator& _rope_node_allocator) {
            rope_node *parent = node->parent;
            delete_rope_node_impl(node, _rope_node_allocator);
            node = parent;
            if (node) {
                while (node->parent) {
                    node->parent->count = node->parent->left->count + node->parent->right->count;
                    node = node->parent;
                }
            }
        }

        template <typename rope_node_allocator>
        static rope_node *concate(rope_node *r1, rope_node *r2, rope_node_allocator& _rope_node_allocator) {
            rope_node *new_rope_node = _rope_node_allocator.allocate(1);
            r1->parent = r2->parent = new_rope_node;
            new_rope_node->parent = nullptr;
            new_rope_node->left = r1;
            new_rope_node->right = r2;
            new_rope_node->count = r1->count + r2->count;
            return new_rope_node;
        }

        std::string to_string() {
            std::string str;
            to_string_impl(this, &str);
            return str;
        }

        // returns \0 if failed
        char at(size_t pos) {
            if (pos >= count) {
                // throw std::runtime_error("pos > count");
                return '\0';
            }
            return at_impl(pos);
        }

        // this call can change the structure
        template <typename rope_node_allocator>
        static void insert(rope_node *&node, size_t pos, char ch, rope_node_allocator& _rope_node_allocator) {
            const char *str = &ch;
            set_slice(node, str, 1, pos, 0, _rope_node_allocator);
        }

        // this call can change the structure
        // str should be null terminated
        template <typename rope_node_allocator>
        static void insert(rope_node *&node, size_t pos, const char *str, rope_node_allocator& _rope_node_allocator) {
            size_t size = std::strlen(str);
            set_slice(node, str, size, pos, 0, _rope_node_allocator);
        }

        template <typename rope_node_allocator>
        static void erase(rope_node *&node, size_t pos, rope_node_allocator& _rope_node_allocator) {
            set_slice(node, "", 0, pos, 1, _rope_node_allocator);
        }

        template <typename rope_node_allocator>
        static void erase(rope_node *&node, size_t pos, size_t n, rope_node_allocator& _rope_node_allocator) {
            set_slice(node, "", 0, pos, n, _rope_node_allocator);
        }

        static void slice(rope_node *node, size_t pos, size_t n, char *ret_str) {
            slice_impl(node, pos, n, ret_str);
        }

        template <typename rope_node_allocator>
        static void set_slice(rope_node *&node, const char *str, size_t size, size_t pos, size_t n, rope_node_allocator& _rope_node_allocator) {
            set_slice_impl(node, str, size, pos, n, _rope_node_allocator);
        }

    private:
        // struct postorder_stack_rope_node_traversal {
        //     postorder_stack_rope_node_traversal(rope_node *root_node) {
        //         push(root_node);
        //     }

        //     std::pair<rope_node *, size_t> next_node(size_t pos) {
        //         // skipping
        //         size_t traversed = 0;
        //         while (should_continue()) {
        //             current_node = pop();
        //             if (utils::in_range(traversed, pos, traversed + current_node->count)) {
        //                 if (current_node->left) push(current_node->left);
        //                 if (current_node->right) push(current_node->right);
        //                 if (current_node->is_leaf()) {
        //                     return { current_node, traversed };
        //                 } 
        //             } else {
        //                 traversed += current_node->count;
        //             }
        //         }
        //     }

        //     rope_node *next_node() {
        //         // no skipping
        //         while (should_continue()) {
        //             current_node = pop();
        //             if (current_node->left) push(current_node->left);
        //             if (current_node->right) push(current_node->right);
        //             if (current_node->is_leaf()) {
        //                 return current_node;
        //             }
        //         }
        //         return nullptr;
        //     }

        //     bool should_continue() { return stack_idx; }
        //     void push(rope_node *node) { stack[stack_idx++] = node; }
        //     rope_node *pop() { 
        //         if (stack_idx == 0) throw std::runtime_error("can't pop from an empty stack");
        //         return stack[--stack_idx]; 
        //     }
        // public:
        //     rope_node *current_node;
        // private:
        //     size_t stack_idx = 0;
        //     rope_node *stack[64];
        // };

        struct preorder_stack_rope_node_traversal {
            
            preorder_stack_rope_node_traversal(rope_node *root_node) {
                push(root_node);
            }

            std::pair<rope_node *, size_t> next_node(size_t pos) {
                size_t traversed = 0;
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
                return { nullptr, 0 };
            }

            rope_node *next_node() {
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

            bool should_continue() { return stack_idx; }
            void push(rope_node *node) { stack[stack_idx++] = node; }
            rope_node *pop() {
                if (stack_idx == 0) throw std::runtime_error("cant pop from empty stack");
                return stack[--stack_idx];
            }

        public:
            rope_node *current_node;
        private:
            size_t stack_idx = 0;
            rope_node *stack[64];
        };

        // l is included
        // r is not included
        template <typename rope_node_allocator>
        static void new_rope_node_impl(rope_node *node, const char *str, size_t l, size_t r, rope_node_allocator& _rope_node_allocator) {
            // TODO: remove recursion maybe
            if ((r - l) > MAX_CH_BUFF_LENGTH) {
                size_t m = (l + r) / 2;

                node->left = _rope_node_allocator.allocate(1);
                node->left->parent = node;
                new_rope_node_impl(node->left, str, l, m, _rope_node_allocator);
                node->right = _rope_node_allocator.allocate(1);
                node->right->parent = node;
                new_rope_node_impl(node->right, str, m, r, _rope_node_allocator);

                node->count = node->left->count + node->right->count;

            } else {
                // leaf
                node->left = node->right = nullptr;
                node->count = r - l;
                size_t index = 0;
                for (size_t i = l; i < r; i++) {
                    node->ch_buff[index++] = str[i];
                }
            }
        }

        template <typename rope_node_allocator>
        static void delete_rope_node_impl(rope_node *node, rope_node_allocator& _rope_node_allocator) {
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
            if (node->left) {
                delete_rope_node_impl(node->left, _rope_node_allocator);
            }
            if (node->right) {
                delete_rope_node_impl(node->right, _rope_node_allocator);
            }
            _rope_node_allocator.deallocate(node, 1);
        }

        void to_string_impl(rope_node *node, std::string *out_str) {
            preorder_stack_rope_node_traversal preorder_stack_rope_node_traversal(node);
            rope_node *current_node = preorder_stack_rope_node_traversal.next_node();
            while (current_node) {
                for (size_t i = 0; i < current_node->count; i++) 
                    out_str->push_back(current_node->ch_buff[i]);
                current_node = preorder_stack_rope_node_traversal.next_node();
            }
        }

        char at_impl(size_t pos) {
            preorder_stack_rope_node_traversal preorder_stack_rope_node_traversal(this);
            auto [node, traversed] = preorder_stack_rope_node_traversal.next_node(pos);
            if (node) 
                return node->ch_buff[pos - traversed];
            else return '\0';
        }

        static void slice_impl(rope_node *&node, size_t pos, size_t n, char *ret_str) {
            size_t idx = 0;
            preorder_stack_rope_node_traversal preorder_stack_rope_node_traversal(node);
            auto [current_node, traversed] = preorder_stack_rope_node_traversal.next_node(pos);
            while (n) {
                if (!current_node) std::runtime_error("something went wrong");
                for (size_t i = pos - traversed; i < current_node->count; i++) {
                    // TODO: remove this for release builds
                    if (!utils::in_range(size_t(0), i, current_node->count)) {
                        throw std::runtime_error("out of bounds");
                    }
                    ret_str[idx++] = current_node->ch_buff[i];
                    n--;
                    if (!n) return;
                }
                pos += current_node->count - (pos - traversed);
                traversed += current_node->count;
                current_node = preorder_stack_rope_node_traversal.next_node();
            }
        }

        template <typename rope_node_allocator>
        static void set_slice_impl(rope_node *&node, const char *str, size_t size, size_t pos, size_t n, rope_node_allocator& _rope_node_allocator) {
            // TODO: test and add bound checks
            if (pos > node->count || pos + n > node->count) {
                throw std::runtime_error("out of bounds");
            }
            preorder_stack_rope_node_traversal preorder_stack_rope_node_traversal(node);
            if (pos == node->count) {
                auto [current_node, traversed] = preorder_stack_rope_node_traversal.next_node(pos - 1);
                assert(current_node);
                size_t str_idx = 0;
                if (current_node->count < MAX_CH_BUFF_LENGTH) {
                    for (size_t i = current_node->count; i < MAX_CH_BUFF_LENGTH; i++) {
                        current_node->ch_buff[i] = str[str_idx++];
                        if (str_idx == size) break;
                    }
                    current_node->count += str_idx;
                }
                if (str_idx == size) {
                    return;
                }
                // TODO: try fitting it into the existing nodes before creating new node ?
                rope_node *r1 = node;
                rope_node *r2 = _rope_node_allocator.allocate(1);
                r2->parent = nullptr;
                new_rope_node_impl(r2, str, str_idx, size, _rope_node_allocator);
                node = concate(r1, r2, _rope_node_allocator);
                return;
            }
            
            auto [current_node, traversed] = preorder_stack_rope_node_traversal.next_node(pos);

            if (current_node) {
                size_t overrides_left = size;
                size_t str_idx = 0;

                if (size < n) {
                    size_t num_nodes = size_t(std::floor(float(size) / float(MAX_CH_BUFF_LENGTH))) + 2;
                    rope_node *nodes_affected[num_nodes];
                    size_t node_idx = 0;

                    // n is always > overrides left
                    while (current_node) {
                        nodes_affected[node_idx++] = current_node;
                        if (node_idx > num_nodes) throw std::runtime_error("something is fucked up");
                        size_t idx = pos - traversed;
                        bool ignore = false;

                        // things that can happen per node

                        // n and overrides left >= current node count - idx
                            // just override like normal
                        if (!ignore) {
                            if (n >= current_node->count - idx && overrides_left >= current_node->count - idx) {
                                for (size_t i = idx; i < current_node->count; i++) {
                                    current_node->ch_buff[i] = str[str_idx++];
                                    n--;
                                    overrides_left--;
                                    pos++;
                                }
                                traversed += current_node->count;
                                ignore = true;
                            }
                        }

                        // n > current node count - idx but overrides left is < current node count - idx
                            // trim the current node count
                        if (!ignore) {
                            if (n >= current_node->count - idx && overrides_left < current_node->count - idx) {
                                size_t i = idx;
                                for (; i < current_node->count; i++) {
                                    if (overrides_left) {
                                        current_node->ch_buff[i] = str[str_idx++];
                                        n--;
                                        overrides_left--;
                                        pos++;
                                    } else {
                                        n -= current_node->count - i;
                                        assert(n < 1000000); // dumb overflow check, TODO: remove this
                                        break;
                                    }
                                }
                                if (overrides_left) traversed += current_node->count;
                                current_node->count = i;
                                ignore = true;
                            }
                        }

                        // both n and overrides left is < current node count - idx
                            // I need to pull back the buffer
                            // trim the current node count
                        if (!ignore) {
                            if (n < current_node->count - idx && overrides_left < current_node->count - idx) {
                                size_t i = idx;
                                size_t pull_back_from = size_t(-1);
                                for (; i < current_node->count; i++) {
                                    if (overrides_left) {
                                        current_node->ch_buff[i] = str[str_idx++];
                                        n--;
                                        overrides_left--;
                                        idx++;
                                    } else {
                                        if (n > i)
                                            pull_back_from = n - i;
                                        else 
                                            pull_back_from = n + i;
                                        n = 0;
                                        break;
                                    }
                                }
                                // pos += current_node->count - idx;
                                // traversed += current_node->count;
                                std::memcpy(current_node->ch_buff + idx, current_node->ch_buff + pull_back_from, current_node->count - pull_back_from);
                                current_node->count = current_node->count - (pull_back_from - idx);
                            }
                        }
                        if (!n) break;
                        current_node = preorder_stack_rope_node_traversal.next_node();
                    }

                    // TODO: parent node count update optimisation 
                    for (size_t i = 0; i < node_idx; i++) {
                        rope_node *node = nodes_affected[i];
                        while (node->parent) {
                            node->parent->count = node->parent->left->count + node->parent->right->count;
                            node = node->parent;
                        }
                    }
                    return;
                }
                
                if (size == n) {
                    while (current_node) {
                        size_t idx = pos - traversed;
                        for (size_t i = idx; i < current_node->count; i++) {
                            current_node->ch_buff[i] = str[str_idx++];
                            n--;
                            pos++;
                            if (!n) return;
                        }
                        if (!n) return;
                        traversed += current_node->count;
                        current_node = preorder_stack_rope_node_traversal.next_node();
                    }
                    throw std::runtime_error("something went wrong");
                }

                // size > n
                if (size > n) {
                    // n < overrides_left
                    size_t num_nodes = size_t(std::floor(float(size) / float(MAX_CH_BUFF_LENGTH))) + 2;
                    rope_node *nodes_affected[num_nodes];
                    size_t node_idx = 0;

                    while (current_node) {
                        nodes_affected[node_idx++] = current_node;
                        preorder_stack_rope_node_traversal.next_node();
                        if (node_idx > num_nodes) throw std::runtime_error("something is fucked up");
                        size_t idx = pos - traversed;
                        size_t i = idx;
                        if (!n) break;
                        for (; i < current_node->count; i++) {
                            current_node->ch_buff[i] = str[str_idx++];
                            n--;
                            overrides_left--;
                            pos++;
                            if (!n) break;
                        }
                        if (n) traversed += current_node->count;
                        if (!n) break;
                        if (current_node->count < MAX_CH_BUFF_LENGTH) {
                            size_t new_node_count = current_node->count;
                            size_t i = 0;
                            while (i < MAX_CH_BUFF_LENGTH - current_node->count) {
                                current_node->ch_buff[current_node->count + i] = str[str_idx++];
                                n--;
                                i++;
                                overrides_left--;
                                new_node_count++;
                                if (!n) break;
                            }
                            rope_node *next_node = preorder_stack_rope_node_traversal.current_node;
                            std::memcpy(next_node->ch_buff, next_node->ch_buff + i, next_node->count - 1);
                            next_node->count -= i;
                            current_node->count = new_node_count;
                            current_node = next_node;
                        }
                        if (!n) break;
                        current_node = preorder_stack_rope_node_traversal.current_node;
                    }

                    // now n is 0

                    size_t data_left = overrides_left;

                    // // shuffling of data
                    size_t idx = pos - traversed;

                    char temp[current_node->count - idx + data_left];

                    for (size_t i = 0; i < current_node->count - idx + data_left; i++) {
                        if (i < data_left) temp[i] = str[str_idx++];
                        else temp[i] = current_node->ch_buff[i - data_left + idx];
                    }

                    std::memcpy(current_node->ch_buff + idx, temp, current_node->count - idx);

                    char new_str[data_left];
                    std::memcpy(new_str, temp + current_node->count - idx, data_left);

                    // check if current node has space for all the left over data
                    if (current_node->count < MAX_CH_BUFF_LENGTH && MAX_CH_BUFF_LENGTH - current_node->count >= data_left) {
                        std::memcpy(current_node->ch_buff + current_node->count, new_str, data_left);
                        current_node->count += data_left;
                    } else {
                        rope_node *next_node = preorder_stack_rope_node_traversal.current_node;
                        if (next_node && (current_node->count + next_node->count + data_left <= MAX_CH_BUFF_LENGTH * 2)) {
                            nodes_affected[node_idx++] = next_node;
                            if (node_idx > num_nodes) throw std::runtime_error("something is fucked up");

                            char temp[2 * MAX_CH_BUFF_LENGTH];
                            size_t current_idx = 0, next_idx = 0;
                            str_idx = 0;
                            for (size_t i = 0; i < current_node->count + next_node->count + data_left; i++) {
                                if (i < current_node->count) temp[i] = current_node->ch_buff[current_idx++];
                                else if (i - current_node->count < next_node->count) temp[i] = new_str[str_idx++];
                                else temp[i] = next_node->ch_buff[next_idx++];
                            }
                            std::memcpy(current_node->ch_buff, temp, MAX_CH_BUFF_LENGTH);
                            std::memcpy(next_node->ch_buff, temp + MAX_CH_BUFF_LENGTH, current_node->count + next_node->count + data_left - MAX_CH_BUFF_LENGTH);
                            current_node->count = MAX_CH_BUFF_LENGTH;
                            next_node->count = current_node->count + next_node->count + data_left - MAX_CH_BUFF_LENGTH;
                        } else {
                            char temp[current_node->count + data_left];

                            size_t current_idx = 0;
                            str_idx = 0;
                            for (size_t i = 0; i < current_node->count + data_left; i++) {
                                if (i < current_node->count) temp[i] = current_node->ch_buff[current_idx++];
                                else temp[i] = new_str[str_idx++];
                            }

                            rope_node *new_node = _rope_node_allocator.allocate(1);
                            new_node->parent = nullptr;
                            new_rope_node_impl(new_node, temp, 0, current_node->count + data_left, _rope_node_allocator);

                            rope_node *parent = current_node->parent;

                            delete_rope_node_impl(current_node, _rope_node_allocator);

                            for (size_t i = 0; i < node_idx; i++) {
                                if (nodes_affected[i] == current_node) {
                                    nodes_affected[i] = new_node;
                                }
                            }

                            if (parent) {
                                if (!parent->left) parent->left = new_node;
                                if (!parent->right) parent->right = new_node;

                                new_node->parent = parent;
                            } else {
                                node = new_node;
                            }
                        }
                    }

                    // TODO: parent node count update optimisation 
                    for (size_t i = 0; i < node_idx; i++) {
                        rope_node *node = nodes_affected[i];
                        while (node->parent) {
                            node->parent->count = node->parent->left->count + node->parent->right->count;
                            node = node->parent;
                        }
                    }
                    return;
                }

            } else throw std::runtime_error("something went wrong");
        }
    };
    
public:
    typedef _Alloc<rope_node> rope_node_allocator;
    
    rope(const std::string& str) : _rope_node_allocator(), _rope_node(rope_node::new_rope_node(str, _rope_node_allocator)) {}
    ~rope() {
        rope_node::delete_rope_node(_rope_node, _rope_node_allocator);
    }

    rope(rope&) = delete;
    rope(const rope&) = delete;

    std::string to_string() const {
        return _rope_node->to_string();
    }

    void concate(const std::string& str) {
        rope_node *new_rope_node = rope_node::new_rope_node(str, _rope_node_allocator);
        _rope_node = rope_node::concate(_rope_node, new_rope_node, _rope_node_allocator);
    }
    
    char at(size_t pos) {
        return _rope_node->at(pos);
    }

    // str should be null terminated
    void insert(size_t pos, const char *str) {
        rope_node::insert(_rope_node, pos, str, _rope_node_allocator);
    }

    void erase(size_t pos) {
        // TODO: return iterator maybe
        rope_node::erase(_rope_node, pos, _rope_node_allocator);
    }

    void erase(size_t pos, size_t n) {
        rope_node::erase(_rope_node, pos, n, _rope_node_allocator);
    }
    
    size_t size() {
        return _rope_node->count;
    }

    size_t node_count() {
        size_t node_count = 0;
        rope_node *current_node;
        size_t stack_idx = 0;
        rope_node *stack[64];
        stack[stack_idx++] = _rope_node;
        while (stack_idx) {
            current_node = stack[--stack_idx];
            if (current_node->right) stack[stack_idx++] = current_node->right;
            if (current_node->left) stack[stack_idx++] = current_node->left;
            if (stack_idx >= 64) throw std::runtime_error("stack idx over 64");
            node_count++;
        }
        return node_count;        
    }
    
    void reorder() {
        std::string temp_string = to_string();
        rope_node::delete_rope_node(_rope_node, _rope_node_allocator);
        _rope_node = rope_node::new_rope_node(temp_string, _rope_node_allocator);
    }

    // TODO: add an overlaod that takes in a char * and doesnt return
    std::string slice(size_t pos, size_t n) {
        char *temp = reinterpret_cast<char *>(alloca(n));
        rope_node::slice(_rope_node, pos, n, temp);
        return std::string(temp, n);
    }

    void set_slice(const std::string& str, size_t pos, size_t n) {
        rope_node::set_slice(_rope_node, str.data(), str.size(), pos, n, _rope_node_allocator);
    }

    // essentially a preorder character iterator
    struct iterator {
        using value_type = char;
        using pointer = char *;
        using reference = char&;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::forward_iterator_tag;

        iterator(rope_node *p, size_t traversed) : root_node(p), traversed(traversed) {}

        iterator(rope_node *p) : root_node(p) {
            push(p);
            while (should_continue()) {
                current_node = pop();
                if (current_node->right) push(current_node->right);
                if (current_node->left) push(current_node->left);
                if (current_node->is_leaf()) {
                    if (current_node->count == 0) continue;
                    current_ch = &current_node->ch_buff[current_count];
                    traversed++;
                    return;
                }                    
            }
            throw std::runtime_error("something is wrong here");
        }

        reference operator*() { 
            return *current_ch; 
        }

        iterator& operator++() {
            // check if there are unread ch
            if (current_count + 1 < current_node->count) {
                current_count++;
                traversed++;
                current_ch = &current_node->ch_buff[current_count];
                return *this;
            }        

            current_count = 0;

            // all ch of current node is done, get next node
            while (should_continue()) {
                current_node = pop();
                if (current_node->right) push(current_node->right);
                if (current_node->left) push(current_node->left);
                if (current_node->is_leaf()) break;                    
            }

            if (!should_continue() && !current_node->is_leaf()) throw std::runtime_error("something is wrong");
            current_ch = &current_node->ch_buff[current_count];
            traversed++;

            return *this;
        }
        iterator operator++(int) {
            auto result = *this;
            operator++();
            return result;
        }
        iterator& operator+=(difference_type pos) {
            for (size_t i = 0; i < pos; i++) {
                operator++();
            }

            return *this;
        }
        
        friend bool operator==(const iterator& lhs, const iterator& rhs) {
            return lhs.traversed == rhs.traversed;
        }
        friend bool operator!=(const iterator& lhs, const iterator& rhs) {
            return lhs.traversed != rhs.traversed;
        }
        friend bool operator<(const iterator& lhs, const iterator& rhs) {
            return lhs.traversed < rhs.traversed;
        }
        friend bool operator>(const iterator& lhs, const iterator& rhs) {
            return rhs < lhs;
        }
        friend bool operator<=(const iterator& lhs, const iterator& rhs) {
            return !(rhs < lhs);
        }
        friend bool operator>=(const iterator& lhs, const iterator& rhs) {
            return !(lhs < rhs);
        }

        friend iterator operator+(const iterator& it, difference_type pos) {
            iterator temp = it;
            temp += pos;
            return temp;
        }
        friend iterator operator+(difference_type pos, const iterator& it) {
            return it + pos;
        }
        

        size_t stack_idx = 0;
        rope_node *stack[64];
        rope_node *current_node = nullptr;
        size_t current_count = 0;
        size_t traversed = 0;
        char *current_ch;

        rope_node *root_node;

    private:

        void push(rope_node *node) {
            stack[stack_idx++] = node;
        }

        rope_node *pop() {
            current_node = stack[--stack_idx];
            return current_node;
        }

        bool should_continue() {
            return stack_idx;
        } 
        
    };

    iterator begin() {
        return { _rope_node };
    }

    iterator end() {
        return { _rope_node, _rope_node->count + 1 };
    }

private:

    rope_node_allocator _rope_node_allocator;
    rope_node *_rope_node;
}; 

} // namespace rope

template <size_t MAX_CH_BUFF_LENGTH, template <typename _Tp> typename _Alloc>
std::ostream& operator <<(std::ostream& o, const rope::rope<MAX_CH_BUFF_LENGTH, _Alloc>& rope) {
    o << rope.to_string();
    return o;
}

#endif