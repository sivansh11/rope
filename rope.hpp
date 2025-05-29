#ifndef ROPE_HPP
#define ROPE_HPP

#include <assert.h>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <ios>
#include <iostream>
#include <string>
#include <vector>

namespace rope {

template <uint32_t BUFFER_LENGTH> struct node_t;

namespace impl {

inline bool in_range(size_t v, size_t start, size_t end) {
  return v >= start && v < end;
}

template <uint32_t BUFFER_LENGTH>
void create(node_t<BUFFER_LENGTH> *node, const char *str, size_t l, size_t r) {
  assert(r >= l);
  node->size = r - l;
  if ((r - l) > BUFFER_LENGTH) {
    size_t m = (l + r) / 2;
    node->left = new node_t<BUFFER_LENGTH>();
    node->left->parent = node;
    create(node->left, str, l, m);
    node->right = new node_t<BUFFER_LENGTH>();
    node->right->parent = node;
    create(node->right, str, m, r);
  } else {
    node->left = node->right = nullptr;
    node->size = r - l;
    size_t index = 0;
    for (size_t i = l; i < r; i++) {
      node->data[index++] = str[i];
    }
  }
}

template <uint32_t BUFFER_LENGTH> void destroy(node_t<BUFFER_LENGTH> *node) {
  if (!node->is_leaf()) {
    assert(node->left &&
           node->right); // for now every parent needs to have 2 children
    destroy(node->left);
    destroy(node->right);
  }
  delete node;
}

template <uint32_t BUFFER_LENGTH>
void debug_print(node_t<BUFFER_LENGTH> *node) {
  // node_t *left = nullptr, *right = nullptr, *parent = nullptr;
  // uint32_t size;
  // char data[BUFFER_LENGTH];
  std::cout << "NODE: " << std::hex << node << '\n';
  std::cout << "PARENT: " << std::hex << node->parent << '\n';
  std::cout << "CHILD LEFT: " << std::hex << node->left << '\n';
  std::cout << "CHILD RIGHT: " << std::hex << node->right << '\n';
  if (node->is_leaf()) {
    std::cout << node->size << " out of " << BUFFER_LENGTH << " occupied.\n";
    std::cout << "DATA: ";
    for (uint32_t i = 0; i < node->size; i++)
      std::cout << node->data[i];
    std::cout << '\n';
  } else {
    std::cout << "SIZE: " << node->size << '\n';
  }
}

template <uint32_t BUFFER_LENGTH>
void slice(node_t<BUFFER_LENGTH> *root, char *str, size_t pos, size_t n) {
  assert(!root->parent); // node should be root
  assert(str);           // str should be pre allocated
  std::vector<node_t<BUFFER_LENGTH> *> stack{};
  stack.push_back(root);
  size_t end = pos + n;
  size_t traverse = 0;
  size_t str_index = 0;
  // TODO: optimise traversal
  // atm I traverse over all the leaf nodes from start, in theory I can skip
  // nodes smartly
  while (n) {
    node_t<BUFFER_LENGTH> *current_node = stack.back();
    stack.pop_back();
    // debug_print(current_node);
    // std::cout << "*********************************\n";
    if (!current_node->is_leaf()) {
      stack.push_back(current_node->right);
      stack.push_back(current_node->left);
    } else {
      if (in_range(pos, traverse, traverse + current_node->size)) {
        for (size_t i = pos - traverse; i < current_node->size; i++) {
          str[str_index++] = current_node->data[i];
          pos++;
          n--;
          if (!n)
            break;
        }
      }
      traverse += current_node->size;
    }
  }
  assert(pos == end);
}

template <uint32_t BUFFER_LENGTH>
void set_slice(node_t<BUFFER_LENGTH> *root, const char *str, size_t size,
               size_t pos, size_t n) {
  // 3 cases possible
  // case 1) size > n
  // case 2) size < n
  // case 3) size == n

  // first reduce till either size or n becomes 0
  // then do an insertion

  assert(!root->parent); // node should be root
  assert(str);           // str should be pre allocated
  std::vector<node_t<BUFFER_LENGTH> *> stack{};
  stack.push_back(root);
  size_t end = pos + n;
  size_t traverse = 0;
  size_t str_index = 0;
  // TODO: optimise traversal
  // atm I traverse over all the leaf nodes from start, in theory I can skip
  // nodes smartly
  while (size && n) {
    node_t<BUFFER_LENGTH> *current_node = stack.back();
    stack.pop_back();
    // debug_print(current_node);
    // std::cout << "*********************************\n";
    if (!current_node->is_leaf()) {
      stack.push_back(current_node->right);
      stack.push_back(current_node->left);
    } else {
      if (in_range(pos, traverse, traverse + current_node->size)) {
        for (size_t i = pos - traverse; i < current_node->size; i++) {
          current_node->data[i] = str[str_index++];
          pos++;
          n--;
          size--;
          traverse++;
          if (!n || !size)
            break;
        }
      } else {
        traverse += current_node->size;
      }
    }
  }
}

} // namespace impl

// TODO: compress struct
template <uint32_t BUFFER_LENGTH> struct node_t {
  node_t *left = nullptr, *right = nullptr, *parent = nullptr;
  uint32_t size;
  char data[BUFFER_LENGTH];

  bool is_leaf() { return !left && !right; }

  static node_t *create(const std::string &str) {
    node_t *root = new node_t;
    impl::create(root, str.data(), 0, str.size());
    return root;
  }

  static void destroy(node_t *root) {
    assert(!root->parent); // node should be root
    impl::destroy(root);
  }

  static void debug_print(node_t *node) { impl::debug_print(node); }

  static std::string slice(node_t *root, size_t pos, size_t n) {
    std::string str;
    str.resize(n);
    assert(!root->parent); // node should be root
    impl::slice(root, str.data(), pos, n);
    return str;
  }

  static std::string set_slice(node_t *root, const std::string &str, size_t pos,
                               size_t n) {
    assert(!root->parent); // node should be root
    impl::set_slice(root, str.data(), str.size(), pos, n);
    return str;
  }
};

template <uint32_t BUFFER_LENGTH>
void breadth_first_debug_print(node_t<BUFFER_LENGTH> *node) {
  assert(!node->parent); // node should be root
  std::vector<node_t<BUFFER_LENGTH> *> stack;
  stack.push_back(node);
  while (stack.size()) {
    node_t<BUFFER_LENGTH> *current_node = stack.back();
    stack.pop_back();
    node_t<BUFFER_LENGTH>::debug_print(current_node);
    std::cout << "*****************************\n";
    if (!current_node->is_leaf()) {
      stack.push_back(current_node->right);
      stack.push_back(current_node->left);
    }
  }
}

} // namespace rope

#endif // !ROPE_HPP
