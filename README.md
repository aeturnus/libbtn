# libbtn

My personal library of data structures and helpful code to use in classes and projects
that use C and/or C++ when there isn't access to STL.

It features the capacity for interfaces by vtables managed by each datastructure.

## vtable structure
Each struct contains a pointer to an "ops" struct defined for that struct. For instance,
`vector` will have a field called `vector_ops ops`. The `vector_ops` struct will contain
the ops structs for the necessary interfaces by tha name of that interface
e.g. `vector_ops` will have a field called `iterable_ops iterable`. Each interface
will provide macros which navigate these tables and invokes the necessary functions.
