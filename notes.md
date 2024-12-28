## 浅拷贝（Shallow Copy）
### 浅拷贝是指在复制对象时，只复制对象的基本内容（例如成员变量的值），而不复制对象内的指针所指向的内存区域。换句话说，浅拷贝后，原对象和新对象共享同一块动态内存。
***Shallow copy refers to copying only the basic contents of an object (such as the values of its member variables) without copying the memory region pointed to by the object's pointers. In other words, after a shallow copy, the original object and the new object share the same block of dynamic memory.***

## 深拷贝（Deep Copy）
### 深拷贝是指在复制对象时，不仅复制对象的基本内容，还复制对象内指针所指向的内存区域。换句话说，深拷贝后，原对象和新对象拥有各自独立的动态内存，互不影响。
***Deep copy refers to copying not only the basic contents of an object but also the memory region pointed to by the object's pointers. In other words, after a deep copy, the original object and the new object have their own independent blocks of dynamic memory, unaffected by each other.***

---

## vector实现原理 (Implementation Principle of std::vector)
### std::vector 是 C++ 标准模板库（STL）中的动态数组容器。它以连续内存块的形式存储数据，具有动态扩展能力，支持快速随机访问和高效的尾部插入。
***std::vector is a dynamic array container in the C++ Standard Template Library (STL). It stores data in a contiguous block of memory, has the ability to dynamically resize, supports fast random access, and allows efficient insertion at the end.***

1. 结构 (structure)
```
class vector {
    T* data;
    size_t size;
    size_t capacity;
};
```
2. 动态扩展 (dynamic resizing)：容量不足时分配新内存（通常为倍增）=> 复制内容 => 释放旧内存  
***(when the capacity is insufficient, allocate new memory (typically doubling the size) => copy the contents => release the old memory)***

---

# IO复用机制 (I/O Multiplexing Mechanism)
## select
### select 通过传递一个文件描述符集合（read、write、exception 集合）给内核，阻塞程序直到其中一个或多个文件描述符的状态发生变化。它会不断扫描集合，检查状态是否满足条件。
### 每次调用时，select 都需要重新传递整个文件描述符集合。
***select works by passing a set of file descriptors (read, write, exception sets) to the kernel, blocking the program until one or more file descriptors change state. It continuously scans the set to check if conditions are met.***  
***Each call to select requires the entire file descriptor set to be passed again.***

## poll
### poll 使用一个 pollfd 结构体数组来记录每个文件描述符的状态。通过内核查询每个描述符的状态，阻塞程序直到状态发生变化。与 select 不同，poll 可以动态扩展文件描述符数量，但仍需在每次调用时重新传递整个文件描述符列表。
***poll uses an array of pollfd structures to record the state of each file descriptor. The kernel checks the state of each descriptor, blocking the program until a state change occurs. Unlike select, poll can dynamically handle more file descriptors but still requires the entire descriptor list to be passed on each call.***

## epoll
### epoll 通过内核维护一个事件表，分为以下三个步骤：
1. 注册：使用 epoll_ctl 将文件描述符添加到内核中的事件表中。
2. 监控：调用 epoll_wait 阻塞进程，等待事件触发。 
3. 事件返回：只返回状态发生变化的文件描述符，避免了全量扫描。  
由于事件表在内核中持久存在，程序无需每次都重新传递文件描述符集合，因此性能更高。  
### epoll uses an event table maintained by the kernel, consisting of three main steps:
1. Registration: Use epoll_ctl to add file descriptors to the kernel's event table.
2. Monitoring: Call epoll_wait to block the process and wait for events.
3. Event Return: Only descriptors with state changes are returned, avoiding full set scanning.  
Since the event table persists in the kernel, the program doesn't need to re-pass the descriptor set each time, making it more efficient.

--- 

## 关于构造函数 (About the Constructor)
1. 定义有参构造后，系统定义的默认无参构造会失效，如果想继续使用必须自行定义无参构造。  
***After defining a parameterized constructor, the system-defined default constructor will be disabled. If you want to continue using it, you must define the default constructor manually.***
2. 如果类中有const或引用成员或不可被拷贝的对象(std::thread)，系统不会生成默认的拷贝构造函数。  
***If a class has const or reference members or objects that cannot be copied (e.g., std::thread), the system will not generate the default copy constructor.***
3. class A 是 class B 的成员，构造B时，先构造A再构造B，析构Ｂ时，先析构Ｂ再析构Ａ。  
***If class A is a member of class B, when constructing B, A is constructed first, followed by B. When destructing B, B is destructed first, followed by A.***
4. class A 是 class B 的基类，构造B时，先构造A再构造B，析构Ｂ时，先析构Ｂ再析构Ａ。  
***If class A is a base class of class B, when constructing B, A is constructed first, followed by B. When destructing B, B is destructed first, followed by A.***
5. 拷贝构造为复制资源，原有资源可以继续使用，移动构造为转移资源的所有权，原有资源会失效。  
***A copy constructor duplicates resources, allowing the original resources to still be used, while a move constructor transfers the ownership of resources, causing the original resources to become invalid.***
6. 初始化列表必须拥用于初始化常量成员、引用成员和基类。  
***Initialization lists must be used to initialize constant members, reference members, and base classes.***
7. 虚析构函数的作用：通过基类指针删除派生类对象时能正确地调用派生类的析构函数,如果基类不写为虚析构,只有基类会被析构而子类未被析构,从而导致内存泄漏.  
***The purpose of a virtual destructor: When deleting a derived class object through a base class pointer, it ensures that the destructor of the derived class is called correctly. If the base class destructor is not virtual, only the base class will be destructed, and the derived class will not be, leading to memory leaks.***
8. 对于全局或静态对象,析构函数按照创建的逆序进行析构.  
***For global or static objects, destructors are called in the reverse order of their creation.***
9. RAII是一种C++编程范式,对象的生命周期控制资源的获取和释放，从而确保在对象销毁时自动释放相关资源，避免资源泄漏。  
***RAII is a C++ programming paradigm where the lifetime of an object controls the acquisition and release of resources, ensuring that resources are automatically released when the object is destroyed, thus preventing resource leaks.***