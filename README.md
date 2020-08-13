# find-required-loops-in-digraph
Abstract: In this project, I will process a large digraph and find out all the required loops as fast as possible.

Environment: Ubuntu 16 (x64), C++ 11

Special Notice: This project comes from the Huawei software elite challenge 2020, the competition document is "复赛赛题说明.docx".

Understand the question: 

1. In the file "test_data.txt", each line represents a transfer record. The first is the sender's ID (32 unsigned int), the second is the receiver's ID (32 unsigned int) and the third is the amount of money (32 positive int). The total amount of transfer records should be less than 2,000,000 and A can only transfer to B once. 

2. I need to find out all the loops fulfilling the following requirements. 

>>a) The length of the loop should be no less than 3 and no more than 7. 
  
>>b) If A transfers x to B and B transfers y to C, 0.2<=y/x<=3.
  
>>c) The amount of loops should be no more than 20,000,000.
  
3. The output file "result.txt" should first include the total number of the required loops (1st line). In the following lines, each line represents a loop and they are sorted according to the dictionary sequence.

How to run this project?

>g++ -O3 512.cpp -o test -lpthread -fpic

>./test

Baseline:
----

1. read the test.txt through ifstream and getline()

2. using map to build a digraph

3. using dfs (deep first searching) to search all the possible loops

>>void dfs (begin,current,map,marked,len,money,pre_money)

>>>for (int i=0;i<map[current].size();i++)

>>>>if (len>=3 && map[current][i].ID==begin && moneyRequirement(money,map[current][i].money) && moneyRequirement(map[current][i].money,pre_money))

>>>>>storeLoop()

>>>>>return

>>>>if (len==7) return

>>>>if (!marked[map[current][i].ID])

>>>>>if (len==1) 

>>>>>>marked[map[current][i].ID]=true

>>>>>>dfs(begin,map[current][i].ID,map,marked,len+1,map[current][i].money,map[current][i].money)

>>>>>>marked[map[current][i].ID]=false

>>>>>else if (moneyRequirement(money,map[current][i].money))

>>>>>>marked[map[current][i].ID]=true

>>>>>>dfs(begin,map[current][i].ID,map,marked,len+1,map[current][i].money,pre_money)

>>>>>>marked[map[current][i].ID]=false

>>for key in map

>>>dfs(key,key,map,marked[]=false,1,0,0)

4. sort the found loops and delete the identical ones

5. write the "result.txt" file

1st Opt (eliminate the 4th step in Baseline)
----

In baseline, one loop might be searched for many times, e.g. 1->2->3->1 and 2->3->1->2. So we can save time by not searching the same loop. Moreover, sorting (according to the dictionary sequence) can also cost lots of time.

For the sorting problem, we can first sort map before DFS. After sorting, key is in the ascending order. Thus, we can ensure the first ID in the loop be the smallest one (no same loops will be found). Moreover, we can store the loops with different lengths in different vectors. Then, the answer can be generated in the dictionary sequence naturally.

Think of the time cost, we add a sorting process with a length of 2,000,000, but we eliminate a sorting process with a length of 200,000,000 and delete all identical loops!

2nd Opt (replace map by vector)
----

In baseline, we use map to indicate the digraph through the whole DFS process, calling an object in map costs more time compared with array. So can we replace map by array?

The biggest problem is, the ID is a 32 unsigned int, but we can't build an array with the size of 32-uint*2,000,000.

To solve this problem, we can map the ID into successive numbers (from 0). By trying, we know the amount of ID should be no more than 500,000. Unfortunately, by trying, we can know the times for each ID transfering out money vary a lot. So we still need to use vector<int> mapID[500000] and vector<unsigned_int> mapMoney[500000]. Moreover, when writing the "result.txt" file, we also need to remap the ID. In order to save time, when building the remap, we build it from successive numbers to strings (with comma). Then, we can save the time from converting ID to strings.

3rd Opt (muti-thread)
-----

In the competition, the Kun Peng processor we use has 4 threads, so we can implement 4 threads in our code. For the 2nd process, we can divide the read data evenly into four groups and build four digraphs at the same time. For the DFS process, we can divide the for loop into four sections and process them through four threads. We can't divide them evenly, because, apparently, smaller IDs have more probability to generate more loops (the first ID in a loop is always the smallest). By trying, the division points are num/40, num/20 and num/10.

4th Opt (replace vector by array)
-----

In baseline, in order to store the answer, we need to use vector< vector < int > >. However, push_back() is too slow, and emplace_back() is faster but not fast enough.
  
We already know the answer's size is n3×3+n4×4+n5×5+n6×6+n7×7. So we can seperate them. e.g. We have ans3, ans4, ans5, ans6 and ans7 instead of an intact ans. Apparently, in each group of loops, the number of loops should be no more than 50,000,000. So we can create one-dimension arrays with size of 3×20,000,000, 4×20,000,000, 5×20,000,000, 6×20,000,000 and 7×20,000,000.

5th Opt (replace ifstream and getline by mmap)
-----

In baseline, ifstream and getline costs lots of time because memory visits the disk through each getline. The "test_data.txt" is rather small compared with the "ans.txt". So can we read them into the memory at one time. The C provides a method called mmap to operate memory.

>int fd = open(testFile.c_str(), O_RDONLY);

>int len = lseek(fd, 0, SEEK_END);

>char* p = (char*)mmap(NULL, len, PROT_READ, MAP_PRIVATE, fd, 0);

Then I can visit the context by p[i].

6th Opt (replace bubble sort by quick sort)
----

In C++, we usually sort(begin,end) funtion to sort vectors and arrays. But in this case, we need to sort like this "1->2 should be in front of 1->3". It's not an ordinary sort, so we can't use the given sort funtion.

In baseline, we use bubble sort. But it is quite slow (O(n^2)) when dealing with big datas. So we can use the quick sort (O(nlogn)).

How to implement quick sort:

1. start with left=begin, right=end

2. if left<right, continue with the 3rd step, else, end the function

3. pivot=sort[left], low=left, high=right

4. if low<high, continue with the 5th step, else, end the loop

5. from high find the nearest high letting sort[high]<pivot

6. sort[low]=sort[high]

7. from low find the nearest low letting sort[low]>pivot

8. sort[high]=sort[low]

9. go back to step 4

10. sort[low]=pivot

Special notice: in step 4-10, we can ensure that all the numbers before the index low are smaller than those after index low.

11. let right=low-1, and go to the step 2

12. let left=low+1, and go to the step 2
