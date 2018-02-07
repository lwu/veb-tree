C++ is an important computer language sometimes used to implement common applications (web browsers) and networked communication systems.

This brief bit of C++ source code came from a graduate-level academic training exercise regarding different models of computation.
It expresses a more complex variant of the common binary search tree (BST), which is a fundamental way of structuring data.

Textbook BSTs are implemented with indirect pointers rather than contiguous memory allocation, which has implications regarding
space used per node and "locality" of access.

Since a lot of computer time can be spent accessing (looking up) data, we should seek to reduce this cost when possible,
as faster programs can mean a smaller utility bill in CPU cycles. These days, such bills add up for companies looking to scale and for data centers that host cloud-based applications. Given that data centers can use up quite a bit of physical space and energy, it makes sense to be mindful of the actual energy expended to support underlying hardware in the application "cloud".

One way to do this is via "caching" with multi-level (hierarchical) "cache" data structures. 
However, it turns out that the size of this hardware or software cache can have an impact on both hardware cost 
and the eventual performance. Thus, hardware engineers and software builders (both operating system and application-level)
spend time optimizing their cache hierarchies.

However, this means that performance can be sensitive to the size of the cache at each level, making it hard to tweak appropriately.
To address this, work has been done in cache-oblivious data structures and algorithms used to work on those constructs.

An academic research survey paper by MIT professor Erik Demaine in 2002 (http://erikdemaine.org/papers/BRICS2002/paper.pdf)
summarizes some previous theoretical work in the area. While at the time such forward-thinking ideas were not yet immediately
uselful, a decade+ later the principles were applied to production database systems.

This particular code implements a van Emde Boas search tree (https://en.wikipedia.org/wiki/Van_Emde_Boas_tree), so that
performance may be theoretically cache-size oblivious--generally a good thing. 
It also happens to do so in a way that the tree is stored as a contiguous array.

The code isn't quite ready to drop into real applications yet but it provides insight into how structuring how data is stored
might make certain computer programs faster.
