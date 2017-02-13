# 高效C++无锁队列实现

https://github.com/cameron314/concurrentqueue

Linux kernel里面从来就不缺少简洁，优雅和高效的代码，只是我们缺少发现和品味的眼光。在Linux kernel里面，简洁并不表示代码使用神出鬼没的超然技巧，相反，它使用的不过是大家非常熟悉的基础数据结构，但是kernel开发者能从基础的数据结构中，提炼出优美的特性。
kfifo就是这样的一类优美代码，它十分简洁，绝无多余的一行代码
kfifo

# 1.什么是CAS原子操作

在研究无锁之前，我们需要首先了解一下CAS原子操作——Compare & Set，或是 Compare & Swap，现在几乎所image有的CPU指令都支持CAS的原子操作，X86下对应的是 CMPXCHG 汇编指令。
大家应该还记得操作系统里面关于“原子操作”的概念，一个操作是原子的(atomic)，如果这个操作所处的层(layer)的更高层不能发现其内部实现与结构。原子操作可以是一个步骤，也可以是多个操作步骤，但是其顺序是不可以被打乱，或者切割掉只执行部分。有了这个原子操作这个保证我们就可以实现无锁了。
CAS原子操作在维基百科中的代码描述如下:

```
int compare_and_swap(int* reg, int oldval, int newval)
{
  ATOMIC();
  int old_reg_val = *reg;
  if (old_reg_val == oldval)
     *reg = newval;
  END_ATOMIC();
  return old_reg_val;
}
```

也就是检查内存*reg里的值是不是oldval，如果是的话，则对其赋值newval。上面的代码总是返回old_reg_value，调用者如果需要知道是否更新成功还需要做进一步判断，为了方便，它可以变种为直接返回是否更新成功，如下：

```
bool compare_and_swap (int *accum, int *dest, int newval)
{
  if ( *accum == *dest ) {
      *dest = newval;
      return true;
  }
  return false;
}
```

除了CAS还有以下原子操作：
Fetch And Add，一般用来对变量做 +1 的原子操作。

```
<< atomic >>
function FetchAndAdd(address location, int inc) {
    int value := *location
    *location := value + inc
    return value
}
```
 
Test-and-set，写值到某个内存位置并传回其旧值。汇编指令BST。

```
#define LOCKED 1
 
int TestAndSet(int* lockPtr) {
    int oldValue;
 
    // Start of atomic segment
    // The following statements should be interpreted as pseudocode for
    // illustrative purposes only.
    // Traditional compilation of this code will not guarantee atomicity, the
    // use of shared memory (i.e. not-cached values), protection from compiler
    // optimization, or other required properties.
    oldValue = *lockPtr;
    *lockPtr = LOCKED;
    // End of atomic segment
 
    return oldValue;
}
```
 
Test and Test-and-set，用来实现多核环境下互斥锁，

```
boolean locked := false // shared lock variable
procedure EnterCritical() {
  do {
    while (locked == true) skip // spin until lock seems free
  } while TestAndSet(locked) // actual atomic locking
}
```

# 2.CAS 在各个平台下的实现
 
## 2.1 Linux GCC 支持的 CAS

GCC4.1+版本中支持CAS的原子操作（完整的原子操作可参看 GCC Atomic Builtins）

```
bool __sync_bool_compare_and_swap (type *ptr, type oldval type newval, ...)
type __sync_val_compare_and_swap (type *ptr, type oldval type newval, ...)
```

## 2.2  Windows支持的CAS
在Windows下，你可以使用下面的Windows API来完成CAS：（完整的Windows原子操作可参看MSDN的InterLocked Functions）
```
InterlockedCompareExchange ( __inout LONG volatile *Target,
                                __in LONG Exchange,
                                __in LONG Comperand);
```

## 2.3  C++ 11支持的CAS
C++11中的STL中的atomic类的函数可以让你跨平台。（完整的C++11的原子操作可参看 Atomic Operation Library）

```
template< class T >
bool atomic_compare_exchange_weak( std::atomic<T>* obj,
                                   T* expected, T desired );
template< class T >
bool atomic_compare_exchange_weak( volatile std::atomic<T>* obj,
                                   T* expected, T desired );
```
 
# 3.CAS原子操作实现无锁的性能分析
 
image3.1测试方法描述
 
这里由于只是比较性能，所以采用很简单的方式，创建10个线程并发执行，每个线程中循环对全局变量count进行++操作（i++)，循环加2000000次，这必然会涉及到并发互斥操作，在同一台机器上分析 加普通互斥锁、CAS实现的无锁、Fetch And Add实现的无锁消耗的时间，然后进行分析。

自行测试：大概的结果
无锁操作在性能上远远优于加锁操作，消耗时间仅为加锁操作的1/3左右，无锁编程方式确实能够比传统加锁方式效率高，经上面测试可以发现，可以快到3倍左右。所以在极力推荐在高并发程序中采用无锁编程的方式可以进一步提高程序效率。
