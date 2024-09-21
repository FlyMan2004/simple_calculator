# Simple Calculator

Calculator is a very useful tool in learning, engineering and many concepts. 
It seems very easy to create one compared with your powerful phone. That's correct.

This paper will guide you to create a calculator with only std library in C++.

## Level 0: "1 + 1 = ?", "2 - 1 = ?"

### Description

The lv.0 is for your revision of C++ concepts. So it doesn't involve too many things. You need to write a program to deal with the following requirements.

In the first line, there is one integer `${n}` as input to indicate the lines of expressions.

In each of the following `${n}` lines, there will be two integers `${a}` and `${b}` as well as one operator `${op}`, which can only be `+` or `-`.

Your output should be the answer of the math expression. 

Each line of the input strictly follow such format: `${a} ${op} ${b}`. You don't need to consider any possible error in the input. The output can be print as batch or interactive with input.

### Input sample

```
5
1 + 2
12 - 6
9 + 8
24 - 0
7 - 7
```

### Output sample

```
3
6
17
24
0
```

### Data range

For 20% of the dataset, `${op} == '+'`, `0 <= ${a, b} < 2^32`, `0 < ${n} < 100`;

For 50% of the dataset, `0 <= ${a} <= ${b} < 2^32`, `0 < ${n} < 100`;

For 100% of the dataset, `0 <= ${a, b, n} < 2^32`