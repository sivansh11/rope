#include "rope.hpp"

#include <iostream>
#include <unordered_map>
#include <queue>
#include <cassert>
#include <fstream>
#include <sstream>

// didnt know what to call this, its just an allocater that checks if all the allocations are deallocated
template <typename _Tp>
class checking_allocator {
public:
    checking_allocator() = default;
    ~checking_allocator() noexcept(false) {
        if (_allocations.size()) {
            throw std::runtime_error("not all allocations are cleared");
        }
    }

    typedef _Tp value_type;
    value_type *allocate(size_t n) {
        value_type *p = reinterpret_cast<value_type *>(::operator new(n * sizeof(value_type)));
        _allocations.insert({size_t(p), n});
        return p;
    }

    void deallocate(value_type *p, size_t n) {
        auto itr = _allocations.find(size_t(p));
        if (itr == _allocations.end()) {
            throw std::runtime_error("pointer not allocated by allocator");
        }
        if (itr->second != n) {
            throw std::runtime_error("not all elements cleared");
        }
        _allocations.erase(itr);
        ::operator delete(p);
    }

private:
    std::unordered_map<size_t, size_t> _allocations;
};

void test() {
    // test to_string and at
    {
        rope::rope<2, checking_allocator /* allocator is optional, if not given std::allocator is used*/> rope("hello");
        assert(rope.to_string() == "hello");
        assert(rope.at(0) == 'h');
        assert(rope.at(1) == 'e');
        assert(rope.at(2) == 'l');
        assert(rope.at(3) == 'l');
        assert(rope.at(4) == 'o');
        assert(rope.at(5) == '\0');
        assert(rope.at(6) == '\0');
    }
    // test to_string and at
    {
        rope::rope<3, checking_allocator> rope("hello");
        assert(rope.to_string() == "hello");
        assert(rope.at(0) == 'h');
        assert(rope.at(1) == 'e');
        assert(rope.at(2) == 'l');
        assert(rope.at(3) == 'l');
        assert(rope.at(4) == 'o');
        assert(rope.at(5) == '\0');
        assert(rope.at(6) == '\0');
    }
    // test concate
    {
        rope::rope<2, checking_allocator> rope("hello");
        rope.concate(" people");
        assert(rope.to_string() == "hello people");
        assert(rope.at(0) == 'h');
        assert(rope.at(1) == 'e');
        assert(rope.at(2) == 'l');
        assert(rope.at(3) == 'l');
        assert(rope.at(4) == 'o');
        assert(rope.at(5) == ' ');
        assert(rope.at(6) == 'p');
        assert(rope.at(7) == 'e');
        assert(rope.at(8) == 'o');
        assert(rope.at(9) == 'p');
        assert(rope.at(10) == 'l');
        assert(rope.at(11) == 'e');
        assert(rope.at(12) == '\0');
        assert(rope.at(13) == '\0');
    }
    // test concate
    {
        rope::rope<3, checking_allocator> rope("hello");
        rope.concate(" people");
        assert(rope.to_string() == "hello people");
        assert(rope.at(0) == 'h');
        assert(rope.at(1) == 'e');
        assert(rope.at(2) == 'l');
        assert(rope.at(3) == 'l');
        assert(rope.at(4) == 'o');
        assert(rope.at(5) == ' ');
        assert(rope.at(6) == 'p');
        assert(rope.at(7) == 'e');
        assert(rope.at(8) == 'o');
        assert(rope.at(9) == 'p');
        assert(rope.at(10) == 'l');
        assert(rope.at(11) == 'e');
        assert(rope.at(12) == '\0');
        assert(rope.at(13) == '\0');
    }
    // insert test
    {
        rope::rope<2, checking_allocator> rope("hello");
        rope.insert('0', 3);
        assert(rope.to_string() == "hel0lo");
        rope.insert('0', 4);
        assert(rope.to_string() == "hel00lo");
        rope.insert('0', 6);
        assert(rope.to_string() == "hel00l0o");
    }
    // insert test
    {
        rope::rope<3, checking_allocator> rope("hello");
        rope.insert('0', 3);
        assert(rope.to_string() == "hel0lo");
        rope.insert('0', 4);
        assert(rope.to_string() == "hel00lo");
        rope.insert('0', 6);
        assert(rope.to_string() == "hel00l0o");
    }
    // erase test
    {
        rope::rope<2, checking_allocator> rope("hello");
        rope.erase(2);
        assert(rope.to_string() == "helo");
    }
    // itr test 
    {
        rope::rope<3, checking_allocator> rope("hello");
        auto itr = rope.begin();
        assert(*itr++ = 'h');
        assert(*itr++ = 'e');
        assert(*itr++ = 'l');
        assert(*itr++ = 'l');
        assert(*itr++ = 'o');
        std::string rope_fill;
        for (auto ch : rope) {
            rope_fill.push_back(ch);
        }
        assert(rope_fill == "hello");
    }
    // itr test
    {
        rope::rope<2, checking_allocator> rope("hello");
        for (auto& ch : rope) {
            ch = '0';
        }
        assert(rope.to_string() == "00000");
    }
}

// TODO: use std::path
std::string read_string_from_file(const std::string &file_path) {
    const std::ifstream input_stream(file_path, std::ios_base::binary);

    if (input_stream.fail()) {
        throw std::runtime_error("Failed to open file");
    }

    std::stringstream buffer;
    buffer << input_stream.rdbuf();

    return buffer.str();
}

int main() {
    test();

    rope::rope<20> rope_test(read_string_from_file("../rope.hpp"));
    std::cout << rope_test << '\n';
    std::cout << rope_test.node_count() << '\n';

    return 0;
}