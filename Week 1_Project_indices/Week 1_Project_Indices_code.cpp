#include <iostream>
using namespace std;

void twoSum(int nums[], int n, int target) {
    // checking each pair
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (nums[i] + nums[j] == target) {
                //  correct indices pair
                cout << "Output: [" << i << ", " << j << "]" << endl;
                return; // stop after 1st answer
            }
        }
    }
}

int main() {
    int f, target;

    cout << "Enter number of elements in the array: ";
    cin >> f;

    int* nums = new int[f];

    cout << "Enter " << f << " integers: ";
    for (int i = 0; i < f; i++) {
        cin >> nums[i];
    }

    cout << "Enter the target value: ";
    cin >> target;

    // Function call
    twoSum(nums, f, target);

    return 0;
}
