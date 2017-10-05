NOTES (2016-10-01):

This project is intended to demonstrate the concepts from my CppCon 2016 talk 
"Implementing Lightweight Object Persistence in Modern C++".  It's also the
starting point for some ongoing R&D regarding allocators and allocator-aware 
containers.

Overview:

For a bottom-up understanding of the code, I recommend looking at files in the
following order:

 1. segmented_addressing_model.h - This header defines an addressing model per
    slides 32-34 in my talk as a class template.

 2. segmented_private_storage_model.h - This header defines a storage model
    per slides 35, 36, and 95 in my talk.

 3. synthetic_pointer_interface.h - This header defines some traits types to
    provide SFINAE help with synthetic pointers.  It also includes the headers
    that define the synthetic pointer types and their comparison operators.

 4. synthetic_typed_pointer_interface.h - This header defines a pointer 
    interface type per slides 37 and 38.  A sibling header defines a partial
    specialization for void pointers (synthetic_void_pointer_interface.h).

 5. segmented_leaky_allocation_strategy.h - This header defines an allocation
    strategy type per slide 40.  This trivial allocator just grabs the next
    available address in a segment, and deallocation is a no-op.

 6. rhx_allocator.h - This header defines a standard-conformant allocator 
    class template parametrized in terms of an allocation strategy type.

 7. demo.cpp - This source file defines six test functions that perform very
	simple tests of the "allocator-awareness" of forward_list, list, deque,
	vector, unordered_map, and map.  Each function:
	 a. adds some elements to a container;
	 b. prints the address of container, then iterates over and prints the 
	    elements of that container;
	 c. swaps the primary and shadow segments in the storage model, which
	    effiectively relocates the heap; and
	 d. again prints the address of container, then iterates over and prints
	    its elements.

The demo program provides some preliminary evidence that allocator awareness,
synthetic pointers, and relocation work with basic_string, forward_list, list, 
deque, vector, and unordered_map, at least with Clang 3.81; map also seems to
work correctly with Clang 3.81.

When building on linux with Clang, I just do something like this from the
<root>/code directory:

    $ clang++ -stdlib=libc++ -std=c++14 -g -I./include  \
              src/segmented_private_storage_model.cpp   \
              src/demo.cpp -o /tmp/reloc_demo

For map on VS2015 SP3, allocator awareness and synthetic pointers work, but
relocation does not.  In particular, iteration goes into an endless loop after
carying out a relocation.  This behavior occurs in both debug and release 
modes.

Going Forward:

There's quite a bit to do:
 1. Experiment with other non-traditional allocators and synthetic pointers.
    In particular, there are a few variations on the syn_ptr theme that I'd
    like to experiment with.
 2. Perform more exhaustive testing of the synthetic pointer types used by
    those allocators.
 3. Perform more exhaustive testing of the containers' allocator awareness
    using those allocators.
 3. Measure performance for the above.
 4. Compare compatibility and performance across compilers.
 
