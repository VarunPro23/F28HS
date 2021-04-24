Follow below instructions to run the C file. 

You can compile the `master-mind-terminal.c` file with the `gcc` compiler:

```bash
gcc -o cw3  master-mind-terminal.c

```

For running the program in debug mode, 
```bash
./cw3 -d

```
A test script is available to do unit-testing of the matching function. Run it like this from the command line
```bash
sh ./test.sh
```
Shown below is an example of performing unit testing and its result on the C file:
```bash
./cw3 -u 121 313
0 exact matches
1 approximate matches
```

The arm file can only be tested using this link : https://cpulator.01xz.net/?sys=arm-de1soc&d_audio=48000

Copy paste the arm code from its file on the link above and run.
