Mir hacking guide
=================

Building mir
-----------

Mir is built using cmake. There are other options, but here's one way to
build the system:

    $ mkdir build
    $ cd build
    $ cmake ..
    $ make -j 8
    $ ctest


Coding mir
----------

There's a coding style guide in the guides subdirectory. To build it into an
html file:

    $ make guides


Code structure
--------------

Code structure: include

The include subdirectory contains header files "published" by corresponding parts
of the system. For example, include/mir/option/option.h provides a system-wide interface
for accessing runtime options published by the options component.

In many cases, there will be interfaces defined that are used by the the component
and implemented elsewhere. E.g. the compositor uses RenderView which is implemented
by the surfaces component.

Files under the include directory should contain minimal implementation detail: interfaces
should not expose platform or implementation technology types etc. (And as public methods
are normally implementations of interfaces they do not use these types.)


Code structure: src

This comprises the implementation of mir. Header files for use within the component
should be put here. The only headers from the source tree that should be included are
ones from the current component (ones that do not require a path component).


Code structure: test

This contains unit, integration and acceptance tests written using gtest/gmock. Tests
largely depend upon the public interfaces of components - but tests of units within
a component will include headers from within the source tree.


Code structure: 3rd_party

Third party code imported into our source tree for use in mir. We try not to change
anything to avoid maintaining a fork.


Error handling strategy
-----------------------

If a function cannot meet its post-conditions it throws an exception and meets
AT LEAST the basic exception safety guarantee. It is a good idea to document the
strong and no-throw guarantees. http://www.boost.org/community/exception_safety.html


Documentation
-------------

There are design notes and an architecture diagram (.dia) in the design
subdirectory.


Getting mir
-----------

If you're reading this file then you've probably solved this one. ;)

However, for completeness mir is a project on LaunchPad (https://launchpad.net/mir)
to grab a copy use the command:

    $ bzr branch lp:mir
