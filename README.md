# find-required-loops-in-digraph
Abstract: In this project, I will process a large digraph and find out all the required loops as fast as possible.

Environment: Ubuntu 16 (x64), C++ 11

Special Notice: This project comes from the Huawei software elite challenge 2020, the competition document is "复赛赛题说明.docx".

Understand the question: 

1. In the file "test_data.txt", each line represents a transfer record. The first is the sender's ID (32 unsigned int), the second is the receiver's ID (32 unsigned int) and the third is the amount of money (32 positive int). The total amount of transfer records should be less than 2,000,000 and A can only transfer to B once. 

2. I need to find out all the loops fulfilling the following requirements. 

  2.1) The length of the loop should be no less than 3 and no more than 7. 
  
  2.2) If A transfers x to B and B transfers y to C, 0.2<=y/x<=3.
  
  2.3) The amount of loops should be no more than 20,000,000.
  
3. The output file "result.txt" should first include 
