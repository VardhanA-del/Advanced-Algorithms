#include <iostream>
using namespace std;

int main() {
    int vrdn_size, vrdn_target;

    cout << "Enter array size: ";
    cin >> vrdn_size;

    int vrdn_nums[20];
    cout << "Enter array elements: ";
    for (int vrdn_i = 0; vrdn_i < vrdn_size; vrdn_i++) {
        cin >> vrdn_nums[vrdn_i];
    }

    cout << "Enter target value: ";
    cin >> vrdn_target;

    int vrdn_index1 = -1, vrdn_index2 = -1;

    for (int vrdn_i = 0; vrdn_i < vrdn_size; vrdn_i++) {
        for (int vrdn_j = vrdn_i + 1; vrdn_j < vrdn_size; vrdn_j++) {
            if (vrdn_nums[vrdn_i] + vrdn_nums[vrdn_j] == vrdn_target) {
                vrdn_index1 = vrdn_i;
                vrdn_index2 = vrdn_j;
                break;
            }
        }
        if (vrdn_index1 != -1) break;
    }

    if (vrdn_index1 != -1)
        cout << "Output: [" << vrdn_index1 << ", " << vrdn_index2 << "]" << endl;
    else
        cout << "No valid pair found." << endl;

    return 0;
}
