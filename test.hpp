#include "rope.hpp"

inline void test() {
  {
    using node_t = rope::node_t<1>;
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 0, 0);
      assert(str == "");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 1, 0);
      assert(str == "");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 2, 0);
      assert(str == "");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 3, 0);
      assert(str == "");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 4, 0);
      assert(str == "");
      node_t::destroy(n);
    }

    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 0, 1);
      assert(str == "0");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 1, 1);
      assert(str == "1");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 2, 1);
      assert(str == "2");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 3, 1);
      assert(str == "3");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 4, 1);
      assert(str == "4");
      node_t::destroy(n);
    }

    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 0, 2);
      assert(str == "01");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 1, 2);
      assert(str == "12");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 2, 2);
      assert(str == "23");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 3, 2);
      assert(str == "34");
      node_t::destroy(n);
    }

    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 0, 3);
      assert(str == "012");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 1, 3);
      assert(str == "123");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 2, 3);
      assert(str == "234");
      node_t::destroy(n);
    }

    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 0, 4);
      assert(str == "0123");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 1, 4);
      assert(str == "1234");
      node_t::destroy(n);
    }

    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 0, 5);
      assert(str == "01234");
      node_t::destroy(n);
    }
  }

  {
    using node_t = rope::node_t<2>;
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 0, 0);
      assert(str == "");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 1, 0);
      assert(str == "");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 2, 0);
      assert(str == "");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 3, 0);
      assert(str == "");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 4, 0);
      assert(str == "");
      node_t::destroy(n);
    }

    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 0, 1);
      assert(str == "0");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 1, 1);
      assert(str == "1");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 2, 1);
      assert(str == "2");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 3, 1);
      assert(str == "3");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 4, 1);
      assert(str == "4");
      node_t::destroy(n);
    }

    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 0, 2);
      assert(str == "01");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 1, 2);
      assert(str == "12");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 2, 2);
      assert(str == "23");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 3, 2);
      assert(str == "34");
      node_t::destroy(n);
    }

    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 0, 3);
      assert(str == "012");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 1, 3);
      assert(str == "123");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 2, 3);
      assert(str == "234");
      node_t::destroy(n);
    }

    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 0, 4);
      assert(str == "0123");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 1, 4);
      assert(str == "1234");
      node_t::destroy(n);
    }

    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 0, 5);
      assert(str == "01234");
      node_t::destroy(n);
    }
  }

  {
    using node_t = rope::node_t<3>;
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 0, 0);
      assert(str == "");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 1, 0);
      assert(str == "");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 2, 0);
      assert(str == "");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 3, 0);
      assert(str == "");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 4, 0);
      assert(str == "");
      node_t::destroy(n);
    }

    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 0, 1);
      assert(str == "0");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 1, 1);
      assert(str == "1");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 2, 1);
      assert(str == "2");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 3, 1);
      assert(str == "3");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 4, 1);
      assert(str == "4");
      node_t::destroy(n);
    }

    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 0, 2);
      assert(str == "01");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 1, 2);
      assert(str == "12");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 2, 2);
      assert(str == "23");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 3, 2);
      assert(str == "34");
      node_t::destroy(n);
    }

    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 0, 3);
      assert(str == "012");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 1, 3);
      assert(str == "123");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 2, 3);
      assert(str == "234");
      node_t::destroy(n);
    }

    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 0, 4);
      assert(str == "0123");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 1, 4);
      assert(str == "1234");
      node_t::destroy(n);
    }

    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 0, 5);
      assert(str == "01234");
      node_t::destroy(n);
    }
  }

  {
    using node_t = rope::node_t<4>;
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 0, 0);
      assert(str == "");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 1, 0);
      assert(str == "");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 2, 0);
      assert(str == "");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 3, 0);
      assert(str == "");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 4, 0);
      assert(str == "");
      node_t::destroy(n);
    }

    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 0, 1);
      assert(str == "0");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 1, 1);
      assert(str == "1");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 2, 1);
      assert(str == "2");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 3, 1);
      assert(str == "3");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 4, 1);
      assert(str == "4");
      node_t::destroy(n);
    }

    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 0, 2);
      assert(str == "01");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 1, 2);
      assert(str == "12");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 2, 2);
      assert(str == "23");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 3, 2);
      assert(str == "34");
      node_t::destroy(n);
    }

    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 0, 3);
      assert(str == "012");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 1, 3);
      assert(str == "123");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 2, 3);
      assert(str == "234");
      node_t::destroy(n);
    }

    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 0, 4);
      assert(str == "0123");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 1, 4);
      assert(str == "1234");
      node_t::destroy(n);
    }

    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 0, 5);
      assert(str == "01234");
      node_t::destroy(n);
    }
  }

  {
    using node_t = rope::node_t<5>;
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 0, 0);
      assert(str == "");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 1, 0);
      assert(str == "");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 2, 0);
      assert(str == "");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 3, 0);
      assert(str == "");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 4, 0);
      assert(str == "");
      node_t::destroy(n);
    }

    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 0, 1);
      assert(str == "0");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 1, 1);
      assert(str == "1");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 2, 1);
      assert(str == "2");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 3, 1);
      assert(str == "3");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 4, 1);
      assert(str == "4");
      node_t::destroy(n);
    }

    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 0, 2);
      assert(str == "01");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 1, 2);
      assert(str == "12");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 2, 2);
      assert(str == "23");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 3, 2);
      assert(str == "34");
      node_t::destroy(n);
    }

    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 0, 3);
      assert(str == "012");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 1, 3);
      assert(str == "123");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 2, 3);
      assert(str == "234");
      node_t::destroy(n);
    }

    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 0, 4);
      assert(str == "0123");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 1, 4);
      assert(str == "1234");
      node_t::destroy(n);
    }

    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 0, 5);
      assert(str == "01234");
      node_t::destroy(n);
    }
  }

  {
    using node_t = rope::node_t<6>;
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 0, 0);
      assert(str == "");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 1, 0);
      assert(str == "");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 2, 0);
      assert(str == "");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 3, 0);
      assert(str == "");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 4, 0);
      assert(str == "");
      node_t::destroy(n);
    }

    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 0, 1);
      assert(str == "0");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 1, 1);
      assert(str == "1");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 2, 1);
      assert(str == "2");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 3, 1);
      assert(str == "3");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 4, 1);
      assert(str == "4");
      node_t::destroy(n);
    }

    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 0, 2);
      assert(str == "01");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 1, 2);
      assert(str == "12");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 2, 2);
      assert(str == "23");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 3, 2);
      assert(str == "34");
      node_t::destroy(n);
    }

    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 0, 3);
      assert(str == "012");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 1, 3);
      assert(str == "123");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 2, 3);
      assert(str == "234");
      node_t::destroy(n);
    }

    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 0, 4);
      assert(str == "0123");
      node_t::destroy(n);
    }
    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 1, 4);
      assert(str == "1234");
      node_t::destroy(n);
    }

    {
      node_t *n = node_t::create("01234");
      std::string str = node_t::slice(n, 0, 5);
      assert(str == "01234");
      node_t::destroy(n);
    }
  }
}
