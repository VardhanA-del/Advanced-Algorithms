#include <iostream>
using namespace std;

// Structure for a node in the Huffman Tree
struct vrdn_Node {
    char vrdn_char;      // character (like 'a', 'b', etc.)
    int vrdn_freq;       // frequency of the character
    vrdn_Node *vrdn_left, *vrdn_right; // left and right child
};

// Function to create a new node
vrdn_Node* vrdn_newNode(char vrdn_char, int vrdn_freq) {
    vrdn_Node* vrdn_temp = new vrdn_Node();
    vrdn_temp->vrdn_char = vrdn_char;
    vrdn_temp->vrdn_freq = vrdn_freq;
    vrdn_temp->vrdn_left = vrdn_temp->vrdn_right = NULL;
    return vrdn_temp;
}

// Swap two nodes (helper for heap operations)
void vrdn_swapNode(vrdn_Node** vrdn_a, vrdn_Node** vrdn_b) {
    vrdn_Node* vrdn_t = *vrdn_a;
    *vrdn_a = *vrdn_b;
    *vrdn_b = vrdn_t;
}

// Structure for Min-Heap
struct vrdn_MinHeap {
    int vrdn_size;        // current size of heap
    vrdn_Node** vrdn_array; // array of node pointers
};

// Heapify function to maintain min-heap property
void vrdn_heapify(vrdn_MinHeap* vrdn_heap, int vrdn_idx) {
    int vrdn_smallest = vrdn_idx;
    int vrdn_left = 2 * vrdn_idx + 1;
    int vrdn_right = 2 * vrdn_idx + 2;

    // Compare with left child
    if (vrdn_left < vrdn_heap->vrdn_size &&
        vrdn_heap->vrdn_array[vrdn_left]->vrdn_freq < vrdn_heap->vrdn_array[vrdn_smallest]->vrdn_freq)
        vrdn_smallest = vrdn_left;

    // Compare with right child
    if (vrdn_right < vrdn_heap->vrdn_size &&
        vrdn_heap->vrdn_array[vrdn_right]->vrdn_freq < vrdn_heap->vrdn_array[vrdn_smallest]->vrdn_freq)
        vrdn_smallest = vrdn_right;

    // If smallest is not the parent, swap and heapify again
    if (vrdn_smallest != vrdn_idx) {
        vrdn_swapNode(&vrdn_heap->vrdn_array[vrdn_smallest], &vrdn_heap->vrdn_array[vrdn_idx]);
        vrdn_heapify(vrdn_heap, vrdn_smallest);
    }
}

// Extract the node with minimum frequency (root of heap)
vrdn_Node* vrdn_extractMin(vrdn_MinHeap* vrdn_heap) {
    vrdn_Node* vrdn_temp = vrdn_heap->vrdn_array[0];
    vrdn_heap->vrdn_array[0] = vrdn_heap->vrdn_array[vrdn_heap->vrdn_size - 1];
    --vrdn_heap->vrdn_size;
    vrdn_heapify(vrdn_heap, 0);
    return vrdn_temp;
}

// Insert a node into the heap
void vrdn_insertHeap(vrdn_MinHeap* vrdn_heap, vrdn_Node* vrdn_node) {
    ++vrdn_heap->vrdn_size;
    int vrdn_i = vrdn_heap->vrdn_size - 1;
    vrdn_heap->vrdn_array[vrdn_i] = vrdn_node;

    // Fix the min-heap property
    while (vrdn_i && vrdn_heap->vrdn_array[vrdn_i]->vrdn_freq < vrdn_heap->vrdn_array[(vrdn_i - 1) / 2]->vrdn_freq) {
        vrdn_swapNode(&vrdn_heap->vrdn_array[vrdn_i], &vrdn_heap->vrdn_array[(vrdn_i - 1) / 2]);
        vrdn_i = (vrdn_i - 1) / 2;
    }
}

// Build the Huffman Tree
vrdn_Node* vrdn_buildHuffman(char vrdn_data[], int vrdn_freq[], int vrdn_size) {
    // Create a min-heap and allocate memory
    vrdn_MinHeap* vrdn_heap = new vrdn_MinHeap();
    vrdn_heap->vrdn_size = vrdn_size;
    vrdn_heap->vrdn_array = new vrdn_Node*[vrdn_size * 2]; // allocate enough space

    // Insert all characters into the heap
    for (int i = 0; i < vrdn_size; i++)
        vrdn_heap->vrdn_array[i] = vrdn_newNode(vrdn_data[i], vrdn_freq[i]);

    // Heapify (build min-heap)
    for (int i = (vrdn_heap->vrdn_size - 1) / 2; i >= 0; i--)
        vrdn_heapify(vrdn_heap, i);

    // Iterate until only one node (root) is left
    while (vrdn_heap->vrdn_size > 1) {
        // Extract two nodes with smallest frequency
        vrdn_Node* vrdn_left = vrdn_extractMin(vrdn_heap);
        vrdn_Node* vrdn_right = vrdn_extractMin(vrdn_heap);

        // Create a new internal node with sum of frequencies
        vrdn_Node* vrdn_top = vrdn_newNode('$', vrdn_left->vrdn_freq + vrdn_right->vrdn_freq);
        vrdn_top->vrdn_left = vrdn_left;
        vrdn_top->vrdn_right = vrdn_right;

        // Insert the new node back into the heap
        vrdn_insertHeap(vrdn_heap, vrdn_top);
    }

    // Return the root of Huffman Tree
    return vrdn_extractMin(vrdn_heap);
}

// Recursive function to print Huffman codes (Preorder traversal)
void vrdn_printCodes(vrdn_Node* vrdn_root, int vrdn_arr[], int vrdn_top) {
    // Traverse left with '0'
    if (vrdn_root->vrdn_left) {
        vrdn_arr[vrdn_top] = 0;
        vrdn_printCodes(vrdn_root->vrdn_left, vrdn_arr, vrdn_top + 1);
    }
    // Traverse right with '1'
    if (vrdn_root->vrdn_right) {
        vrdn_arr[vrdn_top] = 1;
        vrdn_printCodes(vrdn_root->vrdn_right, vrdn_arr, vrdn_top + 1);
    }
    // If leaf node, print the code
    if (!(vrdn_root->vrdn_left) && !(vrdn_root->vrdn_right)) {
        for (int i = 0; i < vrdn_top; i++)
            cout << vrdn_arr[i];
        cout << " ";
    }
}

int main() {
    // Input characters and their frequencies
    char vrdn_chars[] = {'a', 'b', 'c', 'd', 'e', 'f'};
    int vrdn_freqs[] = {5, 9, 12, 13, 16, 45};
    int vrdn_size = sizeof(vrdn_chars) / sizeof(vrdn_chars[0]);

    // Build Huffman Tree
    vrdn_Node* vrdn_root = vrdn_buildHuffman(vrdn_chars, vrdn_freqs, vrdn_size);

    // Array to store Huffman code during traversal
    int vrdn_arr[100], vrdn_top = 0;

    // Print Huffman codes in preorder
    vrdn_printCodes(vrdn_root, vrdn_arr, vrdn_top);

    cout << endl;
    return 0;
}
