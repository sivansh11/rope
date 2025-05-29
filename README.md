# BUILD INSTRUCTIONS
```
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build && ./build/rope
```

# HOW TO USE
```
// BUFFER_LENGTH is the number of char elements in each node_t
// keeping it too low will increase memory consumption
// keeping it high will slow down text insertions
rope::node_t<BUFFER_LENGTH>

rope::node_t<BUFFER_LENGTH>::create(str); // creates the rope from string
rope::node_t<BUFFER_LENGTH>::slice(rope, str, start, n); // creates substring from rope (for example the rope is ABCD, the start is 1 and n is 2, substring is BC)
rope::node_t<BUFFER_LENGTH>::set_slice(rope, str, size, start, n); // replaces a substring with another substring which may or maynot be of the same size
                                            // the new substring may be bigger or smaller 
                                            // for example:
                                            // same size: rope is ABCD, new substring is 12, size is 2 (from size of substring), start is 1, n is 2 
                                            // new rope would be A12D
                                            // different size: rope is ABCD, new substring is 1, size is 1, start is 1, n is 2
                                            // new rope is A1D
                                            // different size: rope is ABCD, new substring is 123, size is 3, start is 1, n is 2
                                            // new rope is A123D
```
