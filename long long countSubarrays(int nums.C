long long countSubarrays(int* nums, int numsSize, int minK, int maxK) {
    long long total_subarrays = 0;
    
    // Track the latest index positions of minK, maxK, and invalid values
    int min_idx = -1;
    int max_idx = -1;
    int bad_idx = -1;
    
    for (int i = 0; i < numsSize; i++) {
        // 1. If the element falls outside the range, it invalidates future subarrays starting before it
        if (nums[i] < minK || nums[i] > maxK) {
            bad_idx = i;
        }
        
        // 2. Log the most recent appearances of the target bounds
        if (nums[i] == minK) {
            min_idx = i;
        }
        if (nums[i] == maxK) {
            max_idx = i;
        }
        
        // 3. Find the closest index that satisfies both minK and maxK being present
        int valid_left_boundary = (min_idx < max_idx) ? min_idx : max_idx;
        
        // 4. Calculate how many valid starting positions exist for a subarray ending at index i
        long long valid_starts = (long long)valid_left_boundary - bad_idx;
        
        if (valid_starts > 0) {
            total_subarrays += valid_starts;
        }
    }
    
    return total_subarrays;
}
