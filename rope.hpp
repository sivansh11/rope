#ifndef ROPE_HPP
#define ROPE_HPP

#include <cstring>
#include <string>
#include <iostream>
// #include <stack>

namespace rope {

namespace utils {

template <typename _Tp>
bool in_range(_Tp start, _Tp n, _Tp end) {
    if (n >= start && n < end) return true;
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
        
        template <typename rope_node_allocator>
        static rope_node *new_rope_node(const std::string& str, rope_node_allocator& _rope_node_allocator) {
            rope_node *node = _rope_node_allocator.allocate(1);
            node->parent = nullptr;
            new_rope_node_impl(node, str.data(), 0, str.size(), _rope_node_allocator);
            return node;
        }

        template <typename rope_node_allocator>
        static void delete_rope_node(rope_node *node, rope_node_allocator& _rope_node_allocator) {
            delete_rope_node_impl(node, _rope_node_allocator);
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
        char at(size_t n) {
            if (n >= count) {
                // throw std::runtime_error("n > count");
                return '\0';
            }
            return at_impl(n);
        }

        // this call can change the node
        template <typename rope_node_allocator>
        static void insert(rope_node *&node, char ch, size_t n, rope_node_allocator& _rope_node_allocator) {
            insert_impl(node, ch, n, _rope_node_allocator);
        }

        template <typename rope_node_allocator>
        static void erase(rope_node *&node, size_t n, rope_node_allocator& _rope_node_allocator) {
            erase_impl(node, n, _rope_node_allocator);
        }

    private:
        // l is included
        // r is not included
        template <typename rope_node_allocator>
        static void new_rope_node_impl(rope_node *node, const char *str, size_t l, size_t r, rope_node_allocator& _rope_node_allocator) {
            // TODO: remove recursion
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
            rope_node *current_node;

            size_t stack_idx = 0;
            rope_node *stack[64];

            stack[stack_idx++] = node;

            while (stack_idx) {
                current_node = stack[--stack_idx];

                if (current_node->right) stack[stack_idx++] = current_node->right;
                if (current_node->left) stack[stack_idx++] = current_node->left;

                if (!current_node->left && !current_node->right) {
                    // leaf
                    for (size_t i = 0; i < current_node->count; i++) {
                        out_str->push_back(current_node->ch_buff[i]);
                    }
                }
            }
        }

        char at_impl(size_t n) {
            // traversed is how much forward we have went
            // n is the target 

            rope_node *current_node;

            size_t traversed = 0;

            size_t stack_idx = 0;
            // maybe use std::stack instead of this as 64 might be less
            rope_node *stack[64];
            // push
            stack[stack_idx++] = this;

            while (stack_idx) {
                // pop
                current_node = stack[--stack_idx];

                // push
                if (utils::in_range(traversed, n, traversed + current_node->count)) {
                    if (current_node->right) stack[stack_idx++] = current_node->right;
                    if (current_node->left) stack[stack_idx++] = current_node->left;
                } else {
                    // if skipping branch
                    traversed += current_node->count;
                    continue;
                }

                if (!current_node->left && !current_node->right) {
                    // leaf
                    if (utils::in_range(traversed, n, traversed + current_node->count)) {
                        return current_node->ch_buff[n - traversed];
                    } else {
                        traversed += current_node->count;
                    }
                }
            }
            return '\0';
        }

        template <typename rope_node_allocator>
        static void insert_impl(rope_node *&node, char ch, size_t n, rope_node_allocator& _rope_node_allocator) {
            if (n >= node->count) {
                auto r1 = node;
                auto r2 = new_rope_node(std::string(&ch, 1), _rope_node_allocator);

                node = concate(r1, r2, _rope_node_allocator);
                
                return;
            } 
            rope_node *current_node;

            size_t traversed = 0;
            size_t stack_idx = 0;
            rope_node *stack[64];
            stack[stack_idx++] = node;

            while (stack_idx) {
                // pop
                current_node = stack[--stack_idx];

                if (utils::in_range(traversed, n, traversed + current_node->count)) {
                    if (current_node->right) stack[stack_idx++] = current_node->right;
                    if (current_node->left) stack[stack_idx++] = current_node->left;
                } else {
                    traversed += current_node->count;
                    continue;
                }
                
                if (!current_node->left && !current_node->right) {
                    // leaf
                    if (utils::in_range(traversed, n, traversed + current_node->count)) {
                        // add character here 

                        size_t idx = n - traversed;

                        bool space_found = false;

                        // either current node has space
                        if (current_node->count < MAX_CH_BUFF_LENGTH) {

                            char temp[MAX_CH_BUFF_LENGTH + 1];
                            for (size_t i = 0; i < current_node->count + 1; i++) {
                                if (i < idx) {
                                    temp[i] = current_node->ch_buff[i];
                                }
                                if (i == idx) {
                                    temp[i] = ch;
                                }
                                if (i > idx) {
                                    temp[i] = current_node->ch_buff[i - 1];
                                } 
                            }
                            std::memcpy(current_node->ch_buff, temp, current_node->count + 1);
                            current_node->count++;
                            space_found = true;
                        }

                        // current node is the left of parent, right of parent has space
                        // need to put in left, but right has space
                        if (current_node->parent->left == current_node && current_node->parent->right->count < MAX_CH_BUFF_LENGTH) {
                            
                            char temp[MAX_CH_BUFF_LENGTH * 2];
                            for (size_t i = 0; i < current_node->count + current_node->parent->right->count + 1; i++) {
                                if (i < idx) {
                                    temp[i] = current_node->ch_buff[i];
                                }
                                if (i == idx) {
                                    temp[i] = ch;
                                }
                                if (i > idx) {
                                    if (i - 1 < current_node->count) {
                                        temp[i] = current_node->ch_buff[i - 1];
                                    } else {
                                        temp[i] = current_node->parent->right->ch_buff[i - 1 - MAX_CH_BUFF_LENGTH];
                                    }
                                }
                            }
                            std::memcpy(current_node->ch_buff, temp, MAX_CH_BUFF_LENGTH);
                            std::memcpy(current_node->parent->right->ch_buff, temp + MAX_CH_BUFF_LENGTH, current_node->parent->right->count + 1);
                            current_node->parent->right->count++;
                            space_found = true;
                        }

                        // no space found, create new children
                        if (!space_found) {
                            current_node->left = _rope_node_allocator.allocate(1);
                            current_node->right = _rope_node_allocator.allocate(1);
                            current_node->left->left = current_node->left->right = current_node->right->left = current_node->right->right = nullptr;

                            current_node->left->parent = current_node->right->parent = current_node;
                            char temp[MAX_CH_BUFF_LENGTH + 1];
                            for (size_t i = 0; i < current_node->count + 1; i++) {
                                if (i < idx) {
                                    temp[i] = current_node->ch_buff[i];
                                }
                                if (i == idx) {
                                    temp[i] = ch;
                                }
                                if (i > idx) {
                                    temp[i] = current_node->ch_buff[i - 1];
                                } 
                            }

                            std::memcpy(current_node->left->ch_buff, temp, MAX_CH_BUFF_LENGTH);
                            std::memcpy(current_node->right->ch_buff, temp + MAX_CH_BUFF_LENGTH, 1);
                            current_node->count++;
                            current_node->left->count = MAX_CH_BUFF_LENGTH;
                            current_node->right->count = 1;
                        }

                        // update all parents count
                        while (current_node->parent) {
                            
                            current_node->parent->count = current_node->parent->left->count + current_node->parent->right->count;
                            current_node = current_node->parent;
                        }

                        return;

                    } else {
                        traversed += current_node->count;
                    }
                }
            }
        }

        template <typename rope_node_allocator>
        static void erase_impl(rope_node *&node, size_t n, rope_node_allocator& _rope_node_allocator) {
            if (n >= node->count) {
                throw std::runtime_error("out of bounds");
            }
            rope_node *current_node;

            size_t traversed = 0;
            size_t stack_idx = 0;

            rope_node *stack[64];

            stack[stack_idx++] = node;

            while (stack_idx) {
                // pop
                current_node = stack[--stack_idx];

                // push
                if (utils::in_range(traversed, n, traversed + current_node->count)) {
                    if (current_node->right) stack[stack_idx++] = current_node->right;
                    if (current_node->left) stack[stack_idx++] = current_node->left;
                } else {
                    // if skipping branch
                    traversed += current_node->count;
                    continue;
                }

                if (!current_node->left && !current_node->right) {
                    // leaf
                    if (utils::in_range(traversed, n, traversed + current_node->count)) {
                        
                        size_t idx = n - traversed;

                        char temp[MAX_CH_BUFF_LENGTH];

                        for (size_t i = 0; i < current_node->count; i++) {
                            if (i < idx) {
                                temp[i] = current_node->ch_buff[i];
                            }
                            if (i == idx) {
                                // do nothing, skip
                            }
                            if (i > idx) {
                                temp[i - 1] = current_node->ch_buff[i];
                            }
                        }
                        std::memcpy(current_node->ch_buff, temp, current_node->count - 1);
                        current_node->count--;

                        while (current_node->parent) {
                            current_node->parent->count = current_node->parent->left->count + current_node->parent->right->count;
                            current_node = current_node->parent;
                        }

                        return;
                    } else {
                        traversed += current_node->count;
                    }
                }
            }
        }
    };
    
public:
    typedef _Alloc<rope_node> rope_node_allocator;
    
    rope(const std::string& str) : _rope_node_allocator(), _rope_node(rope_node::new_rope_node(str, _rope_node_allocator)) {}
    ~rope() {
        rope_node::delete_rope_node(_rope_node, _rope_node_allocator);
    }

    std::string to_string() {
        return _rope_node->to_string();
    }

    void concate(const std::string& str) {
        rope_node *new_rope_node = rope_node::new_rope_node(str, _rope_node_allocator);
        _rope_node = rope_node::concate(_rope_node, new_rope_node, _rope_node_allocator);
    }
    
    char at(size_t n) {
        return _rope_node->at(n);
    }

    void insert(char ch, size_t n) {
        rope_node::insert(_rope_node, ch, n, _rope_node_allocator);
    }

    void erase(size_t n) {
        // TODO: return iterator maybe
        rope_node::erase(_rope_node, n, _rope_node_allocator);
    }

private:

    rope_node_allocator _rope_node_allocator;
    rope_node *_rope_node;
}; 

} // namespace rope

#endif