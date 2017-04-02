We are required to modify the code in main.c to support multithreading. To implement this we need to include the pthread header file in main and employ pthread_create and pthread_join to do this 
successfully. The unmodified version of main reports an error when multiple threads are called and we are required to change exactly that.

Beginning with main.c, the code that we really need to modify is in the main function. Before making any changes I tried to interpret and comprehend as much of the program logic as I could. There 
were a lot of variables to keep track of and that was perhaps the most confusing part to begin with. Then I went on to isolate the majority of the code in main into a separate function multithreading.

I declared an array of type args_struct to store parameters for each thread. Then I allocated memory to all the arguments that will be stored inside the struct which will subsequently be passed as an 
argument to multithreading. Since pthread_create allows only a void* argument it was difficult at first to figure out a way to do this efficiently.

Talking about improvement in performance, it is pretty evident from the stats displayed while executing make clean check:

time ./srt 1-test.ppm >1-test.ppm.tmp

real	0m41.565s
user	0m41.555s
sys	0m0.003s
mv 1-test.ppm.tmp 1-test.ppm
time ./srt 2-test.ppm >2-test.ppm.tmp

real	0m21.349s
user	0m42.474s
sys	0m0.022s
mv 2-test.ppm.tmp 2-test.ppm
time ./srt 4-test.ppm >4-test.ppm.tmp

real	0m11.541s
user	0m45.331s
sys	0m0.002s
mv 4-test.ppm.tmp 4-test.ppm
time ./srt 8-test.ppm >8-test.ppm.tmp

real	0m5.760s
user	0m44.651s
sys	0m0.003s

It is evident here that as the number of threads increasing, there is a significant decrease in real time.
