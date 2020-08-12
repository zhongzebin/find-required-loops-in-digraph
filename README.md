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

1st Opt (eliminate the 4th step in Baseline)
----

In baseline, one loop might be searched for many times, e.g. 1->2->3->1 and 2->3->1->2. So we can save time by not searching the same loop. Moreover, sorting (according to the dictionary sequence) can also cost lots of time.

For the sorting problem, we can first sort map before DFS. After sorting, key is in the ascending order. Thus, we can ensure the first ID in the loop be the smallest one (no same loops will be found). Moreover, we can store the loops with different lengths in different vectors. Then, the answer can be generated in the dictionary sequence naturally.

Think of the time cost, we add a sorting process with a length of 2,000,000, but we eliminate a sorting process with a length of 200,000,000 and delete all identical loops!

2nd Opt (replace map by array)
----
