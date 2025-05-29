#include "rope.hpp"
#include "test.hpp"

int main() {
  // TODO: this functionality
  // create
  // destroy
  // get slice (start -> stop)
  // set slice (3 possible operations exact slice, expand slice, compress slice)
  test_slice();
  test_set_slice();
  // rope::node_t<3> *rope = rope::node_t<3>::create("0123456");
  // rope::breadth_first_debug_print(rope);
  // std::cout << "*********SET SLICE********\n";
  // rope::node_t<3>::set_slice(rope, "ab", 2, 4);
  // std::string str = rope::node_t<3>::slice(rope, 0, rope->size);
  // std::cout << str << '\n';
  // std::cout << "01ab6" << '\n';
  // rope::node_t<3>::destroy(rope);
  return 0;
}
