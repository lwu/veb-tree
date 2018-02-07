/*

A cache-oblivious [0] version of a van Emde Boas search tree [1],
stored implicitly as an array.

0: http://erikdemaine.org/papers/BRICS2002/paper.pdf
1: https://en.wikipedia.org/wiki/Van_Emde_Boas_tree

Thanks to Jeff Erickson for suggesting the problem, and to John
Fischer and Yasutaka Furukawa for pointing out how to encode an
element's position using its bitstring as a traversal code.

Leslie Wu

*/

#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

// Data 
const char* veb_str = "HDLBACFEGJIKNMO";

inline int power_of_two(int exponent) { return 1 << exponent; }

// Compute things similar to the hyperfloor and hyperceiling
void hyper_compute(int n, int& d, int& D, int& subtree_size, int& subtree_leaf_count)
{	
	int h = (int)ceil(log((float)n+1)/log(2.0f));
	d = h / 2;

	D = power_of_two(d) - 1;

	int delta = h - d;
	subtree_size = power_of_two(delta) - 1;

	subtree_leaf_count = power_of_two(h - 1);
}

// Implicit van Emde Boas binary search
int veb_search(const char* veb_array, int length, char elt)
{	
	int d, D, subtree_size, subtree_leaf_count;
	hyper_compute(length, d, D, subtree_size, subtree_leaf_count);

	if (length > 1) {
		// Recurse on top half of tree
		int subtree_index = veb_search(veb_array, D, elt);
		if (subtree_index < 0) return subtree_index;

		int offset = subtree_index * subtree_size + D;

		// If not in top half, use subtree index to find place in bottom half
		int bottom_subtree_index = veb_search(veb_array + offset, subtree_size, elt);
		return subtree_leaf_count*subtree_index + bottom_subtree_index;
	} else {
		char root = veb_array[0];

		if (elt == root) {
			std::cout << "Found " << elt << " at index " 
				  << (int)(veb_array - veb_str) << std::endl;
			return -1;
		}

		return (elt < root) ? 0 : 1;		
	}
}

int tree_size(int depth) { return power_of_two(depth) - 1; }

// Given a sorted index in the range [1, 2^{tree_height}-1], return the VEB address
int veb_index(int n, int tree_height)
{       
	if (tree_height <= 1) return n;

	// Chop n's bitstring into two halves
	int bottom_half = tree_height / 2;
	int top_half = tree_height - bottom_half;

	// Store each half
	int top_n = n >> bottom_half;
	int bottom_n = n & (power_of_two(bottom_half) - 1);

	// Recurse
	if (bottom_n == 0) return veb_index(top_n, top_half);

	int top_size = power_of_two(top_half) - 1;      
	int subtree_size = power_of_two(bottom_half) - 1;

	int top_address = top_n * tree_size(bottom_half) + tree_size(top_half);
	int bot_address = veb_index(bottom_n, bottom_half);

	return top_address + bot_address;
}


int main()
{	
	// Print mapping from sorted index to VEB index
	int k = 4;
	int pow_k = power_of_two(k);

	for (int i=1; i < pow_k; i++) {
		int idx = veb_index(i, k);
		std::cout << "i = " << i << " -> " << idx << std::endl;
	}

	// To create implicit VEB structure, generate VEB addresses as satellite data,
	// tag, then sort	
	const char* alpha = "ABCDEFGHIJKLMNO";
	
	int len = strlen(alpha);
	vector<pair<int,char> > indices(len);
	int power = power_of_two((int)(ceil(log((float)len)/log(2.0))));
	int i;
	for (i=0; i < len; i++) {
		int idx = veb_index(i+1, power);
		indices[i] = make_pair(idx, alpha[i]);
	}
	sort(begin(indices), end(indices));

	for (i=0; i < len; i++) {
		std::cout << indices[i].second;
	}

	std::cout << std::endl;

	// Search array
	int length = (int)strlen(veb_str);	

	const char* search = "HDLBACFEGJIKNMOabc";
	int search_len = (int)strlen(search);

	for (int i=0; i < search_len; i++) {
		char elt = search[i];
		cout << "Looking for " << elt << "..." << endl;

		veb_search(veb_str, length, elt);
	}

	return 0;
}
