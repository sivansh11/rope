#include "rope.hpp"

#include <iostream>
#include <unordered_map>
#include <queue>
#include <cassert>
#include <fstream>
#include <sstream>
#include <functional>
#include <chrono>
#include <thread>

// didnt know what to call this, its just an allocater that checks if all the allocations are deallocated
template <typename _Tp>
class checking_allocator {
public:
    using value_type = _Tp;
    checking_allocator() = default;
    ~checking_allocator() noexcept(false) {
        if (_allocations.size()) {
            throw std::runtime_error("not all allocations are cleared");
        }
    }

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
        rope.insert(3, "0");
        assert(rope.to_string() == "hel0lo");
        rope.insert(4, "0");
        assert(rope.to_string() == "hel00lo");
        rope.insert(6, "0");
        assert(rope.to_string() == "hel00l0o");
    }
    // insert test
    {
        rope::rope<3, checking_allocator> rope("hello");
        rope.insert(3, "0");
        assert(rope.to_string() == "hel0lo");
        rope.insert(4, "0");
        assert(rope.to_string() == "hel00lo");
        rope.insert(6, "0");
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
    // str vs rope
    {
        std::string str("hello");
        rope::rope<2, checking_allocator> rope("hello");
        str.insert(3, "0");
        rope.insert(3, "0");
        assert(rope.to_string() == str);
        str.erase(2, 2);
        rope.erase(2, 2);
        assert(rope.to_string() == str);
    }
    // slice
    // TODO: add test for slice
    // set_slice same size 
    {
        rope::rope<1, checking_allocator> rope1("0123456789");
        rope::rope<2, checking_allocator> rope2("0123456789");
        rope::rope<3, checking_allocator> rope3("0123456789");
        rope::rope<4, checking_allocator> rope4("0123456789");
        rope::rope<5, checking_allocator> rope5("0123456789");
        rope::rope<6, checking_allocator> rope6("0123456789");
        rope::rope<7, checking_allocator> rope7("0123456789");
        rope::rope<8, checking_allocator> rope8("0123456789");
        rope::rope<9, checking_allocator> rope9("0123456789");
        rope::rope<10, checking_allocator> rope10("0123456789");
        rope1.set_slice("abc", 3, 3);
        rope2.set_slice("abc", 3, 3);
        rope3.set_slice("abc", 3, 3);
        rope4.set_slice("abc", 3, 3);
        rope5.set_slice("abc", 3, 3);
        rope6.set_slice("abc", 3, 3);
        rope7.set_slice("abc", 3, 3);
        rope8.set_slice("abc", 3, 3);
        rope9.set_slice("abc", 3, 3);
        rope10.set_slice("abc", 3, 3);
        assert(rope1.to_string() == "012abc6789");
        assert(rope2.to_string() == "012abc6789");
        assert(rope3.to_string() == "012abc6789");
        assert(rope4.to_string() == "012abc6789");
        assert(rope5.to_string() == "012abc6789");
        assert(rope6.to_string() == "012abc6789");
        assert(rope7.to_string() == "012abc6789");
        assert(rope8.to_string() == "012abc6789");
        assert(rope9.to_string() == "012abc6789");
        assert(rope10.to_string() == "012abc6789");
    }
    // set_slice size of string is less than n
    {
        rope::rope<1, checking_allocator> rope1("0123456789");
        rope::rope<2, checking_allocator> rope2("0123456789");
        rope::rope<3, checking_allocator> rope3("0123456789");
        rope::rope<4, checking_allocator> rope4("0123456789");
        rope::rope<5, checking_allocator> rope5("0123456789");
        rope::rope<6, checking_allocator> rope6("0123456789");
        rope::rope<7, checking_allocator> rope7("0123456789");
        rope::rope<8, checking_allocator> rope8("0123456789");
        rope::rope<9, checking_allocator> rope9("0123456789");
        rope::rope<10, checking_allocator> rope10("0123456789");
        rope1.set_slice("ab", 3, 3);
        rope2.set_slice("ab", 3, 3);
        rope3.set_slice("ab", 3, 3);
        rope4.set_slice("ab", 3, 3);
        rope5.set_slice("ab", 3, 3);
        rope6.set_slice("ab", 3, 3);
        rope7.set_slice("ab", 3, 3);
        rope8.set_slice("ab", 3, 3);
        rope9.set_slice("ab", 3, 3);
        rope10.set_slice("ab", 3, 3);
        assert(rope1.to_string() == "012ab6789");
        assert(rope2.to_string() == "012ab6789");
        assert(rope3.to_string() == "012ab6789");
        assert(rope4.to_string() == "012ab6789");
        assert(rope5.to_string() == "012ab6789");
        assert(rope6.to_string() == "012ab6789");
        assert(rope7.to_string() == "012ab6789");
        assert(rope8.to_string() == "012ab6789");
        assert(rope9.to_string() == "012ab6789");
        assert(rope10.to_string() == "012ab6789");
    }
        // set_slice size of string is greater than n
    {
        rope::rope<1, checking_allocator> rope1("0123456789");
        rope::rope<2, checking_allocator> rope2("0123456789");
        rope::rope<3, checking_allocator> rope3("0123456789");
        rope::rope<4, checking_allocator> rope4("0123456789");
        rope::rope<5, checking_allocator> rope5("0123456789");
        rope::rope<6, checking_allocator> rope6("0123456789");
        rope::rope<7, checking_allocator> rope7("0123456789");
        rope::rope<8, checking_allocator> rope8("0123456789");
        rope::rope<9, checking_allocator> rope9("0123456789");
        rope::rope<10, checking_allocator> rope10("0123456789");
        rope::rope<20, checking_allocator> rope20("0123456789");
        rope1.set_slice("abcd", 3, 3);
        rope2.set_slice("abcd", 3, 3);
        rope3.set_slice("abcd", 3, 3);
        rope4.set_slice("abcd", 3, 3);
        rope5.set_slice("abcd", 3, 3);
        rope6.set_slice("abcd", 3, 3);
        rope7.set_slice("abcd", 3, 3);
        rope8.set_slice("abcd", 3, 3);
        rope9.set_slice("abcd", 3, 3);
        rope10.set_slice("abcd", 3, 3);
        rope20.set_slice("abcd", 3, 3);
        assert(rope1.to_string() == "012abcd6789");
        assert(rope2.to_string() == "012abcd6789");
        assert(rope3.to_string() == "012abcd6789");
        assert(rope4.to_string() == "012abcd6789");
        assert(rope5.to_string() == "012abcd6789");
        assert(rope6.to_string() == "012abcd6789");
        assert(rope7.to_string() == "012abcd6789");
        assert(rope8.to_string() == "012abcd6789");
        assert(rope9.to_string() == "012abcd6789");
        assert(rope10.to_string() == "012abcd6789");
        assert(rope20.to_string() == "012abcd6789");
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

// #include "new_rope.hpp"

int main() {
    test();

    rope::rope<9> rope("01234567");
    rope.insert(8, "89");
    std::cout << rope << '\n';

    // new_rope::rope_t<8> r("012345678");

    return 0;
}