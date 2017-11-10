Fast C++ Delegates
==================

**Note: The implementation of delegates is inspired by several articles<sup
  id="a1">[1](#f1)</sup><sup>,</sup><sup id="a2">[2](#f2)</sup><sup>,</sup><sup
  id="a3">[3](#f3)</sup>on [CodeProject](https://www.codeproject.com) and
  CppEvent<sup id="a4">[4](#f4)</sup>.**

## Overview

Delegates are object-oriented function pointers and wildly used in this project,
for example, they are used to build a signal object, or replace the callback
function in native wayland wrapper classes. To be precise, a delegate in
[SkLand](https://github.com/zhanggyb/skland) is an object which contains 2 pointers: one pointer to an object,
another to a member function in corresponding class, created by a template
class. Invoke a delegate just works as call the member function directly.

**Note:** Delegate in [SkLand](https://github.com/zhanggyb/skland) does not support static methods or global
functions. A delegate to a static method is meanless (it's not object-oriented),
use
[`std::bind`](http://en.cppreference.com/w/cpp/utility/functional/bind),
[`std::function`](http://en.cppreference.com/w/cpp/utility/functional/function)
or just function pointer as you want.

You use a template class defined in `skland/core/delegate.hpp` to create a new
delegate.

For example, if you have a class `A` and an instance `a`:

``` c++
class A {
public:
  
  A();
  ~A();
  
  void OnNotifyInA(int num);
};

A a;
```

then you can use a delegate points to the `OnNotifyInA()` member function of
`a`:

``` c++
#include "core/delegate.hpp"

Delegate<void(int)> d(&a, &A::OnNotifyInA);
// or use a static method: auto d = Delegate<void(int)>::FromMethod(&a, &A::OnNotifyInA);
```

now you can use the delegate to call the assigned member function:

``` c++
d(1);	// This is the same as a.OnNotifyInA(1);
```

Until here it is like
the [`std::bind`](http://en.cppreference.com/w/cpp/utility/functional/bind) in
C++11 STL, you can use `std::bind` in the same way:

``` c++
auto b = std::bind(&A::OnNotifiyInA, &a, 1);
// ^ The third template argument of std::bind is the default parameter.

b();
```

But
unlike [`std::bind`](http://en.cppreference.com/w/cpp/utility/functional/bind),
a delegate can be assigned to another member function of another type which has
the same arguments, or compared with each other. This is a very important feature
which makes delegate more flexible to be used
in [signals and slots](signals_and_slots.md).

For example:

``` c++
#include "core/delegate.hpp"

class A {
public:
  A();
  ~A();
  
  void OnNotifyInA(int num);
};

class B {
public:
  B();
  ~B();
    
  void OnNotifyInB(int num);
};

A a;
B b;

auto delegate1 = Delegate<void(int)>(&a, &A::OnNotifyInA);
auto delegate2 = Delegate<void(int)>(&b, &B::OnNotifyInB);

auto bind1 = std::bind(&A::OnNotifyInA, &a, 1);
auto bind2 = std::bind(&B::OnNotifyInB, &b, 1);

delegate1.Equal(&a, &A::OnNotifyInA);  // Compare the given object and member function directly
delegate1 == delegate2;  // return false
delegate2 = delegate1;   // OK, now delegate2 delegates to member function OnNotifyInA() of object a
delegate1 == delegate2;  // return true after delegate2 = delegate1;

bind1 == bind2;  // Compile error! bind objects cannot be compared
bind2 = bind1;   // Compile error! a bind object cannot be assigned to another one
```

## Implementation

...

----

<small>
<b id="f1">1</b>
[Member Function Pointers and the Fastest Possible C++ Delegates](http://www.codeproject.com/Articles/7150/Member-Function-Pointers-and-the-Fastest-Possible),
author: Don Clugston. [↩](#a1)<br>
<b id="f2">2</b>
[The Impossibly Fast C++ Delegates](http://www.codeproject.com/Articles/11015/The-Impossibly-Fast-C-Delegates), author: Sergey Ryazanov. [↩](#a2)<br>
<b id="f3">3</b>
[Fast C++ Delegate: Boost.Function 'drop-in' replacement and multicast](http://www.codeproject.com/Articles/18389/Fast-C-Delegate-Boost-Function-drop-in-replacement), author: JaeWook Choi. [↩](#a3)<br>
<b id="f4">4</b>
[CppEvents](http://code.google.com/p/cpp-events/), author: Nickolas V. Pohilets. [↩](#a4)
</small>
