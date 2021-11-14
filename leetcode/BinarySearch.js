/**
 * @param {number[]} nums
 * @param {number} target
 * @return {number}
 */
let search = function (nums, target) {
  let id = -1;
  nums.forEach((num, i) => {
    if (num == target && ((id = i) || true)) {
      return;
    }
  });
  return id;
};


nums = [-1, 0, 3, 5, 9, 12];
target = 9;
console.log(search(nums, target));

nums1 = [5];
target1 = 5;
console.log(search(nums1, target1));

console.log(search(nums, target1));
