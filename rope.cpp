#include "rope.hpp"

#include <iostream>

namespace rope {

// namespace internal {

// // l is included
// // r is not included
// void create_rope_node(rope_node_t *node, const char *data, uint32_t l, uint32_t r) {
    
//     if ((r - l) > MAX_LEAF_LENGHT) {
//         node->data = nullptr;
//         node->count = 0;

//         uint32_t m = (l + r) / 2;

//         node->left = new rope_node_t;
//         node->left->parent = node;
//         create_rope_node(node->left, data, l, m);
//         node->right = new rope_node_t;
//         node->right->parent = node;
//         create_rope_node(node->right, data, m, r);
//     } else {
//         node->left = node->right = nullptr;
//         node->count = (r - l);
//         node->data = new char[node->count];
//         uint32_t idx = 0;
//         for (uint32_t i = l; i < r; i++) {
//             node->data[idx++] = data[i];
//         }
//     }
// }

// void to_string(rope_node_t *rope_node, std::string *out_string) {
//     if (rope_node == nullptr) return;
//     if (rope_node->left == nullptr && rope_node->right == nullptr) {
//         // leaf
//         for (uint32_t i = 0; i < rope_node->count; i++) {
//             out_string->push_back(rope_node->data[i]);
//         }
//         return;   
//     }
//     to_string(rope_node->left, out_string);
//     to_string(rope_node->right, out_string);
// }

// rope_node_t *create_rope_node(const std::string& data) {
//     rope_node_t *new_rope = new rope_node_t;
//     new_rope->parent = nullptr;
//     internal::create_rope_node(new_rope, data.data(), 0, data.size());
//     return new_rope;
// }

// void delete_rope_node(rope_node_t *rope_node) {
//     // probably can optimise it a bit, but who the cares :D
//     if (rope_node == nullptr) return;
//     if (rope_node->left) delete_rope_node(rope_node->left);
//     if (rope_node->right) delete_rope_node(rope_node->right);
//     if (rope_node->count) 
//         delete[] rope_node->data;
//     else {
//         delete rope_node->left;
//         delete rope_node->right;
//     }
// }

// void print_node(rope_node_t *rope_node) {
//     std::string str = to_string(rope_node);
//     std::cout << str;
// }

// rope_node_t *concatenate_node(rope_node_t *r1, rope_node_t *r2) {
//     rope_node_t *new_rope = new rope_node_t;
//     r1->parent = r2->parent = new_rope;
//     new_rope->left = r1;
//     new_rope->right = r2;
//     return new_rope;
// }

// std::string to_string(rope_node_t *rope_node) {
//     std::string out_str;
//     internal::to_string(rope_node, &out_str);
//     return out_str;
// }

// } // namespace internal


// rope_t::rope_t(const std::string& data)
//   : _rope_node(internal::create_rope_node(data)) {}


// rope_t::~rope_t() {
//     internal::delete_rope_node(_rope_node);
// }

// std::string rope_t::to_string() {
//     return internal::to_string(_rope_node);
// }

} // namespace rope
