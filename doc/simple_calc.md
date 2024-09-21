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
For 100% of the dataset, `0 <= ${a, b, n} < 2^32`.

## Level 1: Take tok, put tok

### Background

In real implementation of a usable calculator, tokenize the input is vital for multiple reasons.  
Belew is Q&A with ChatGPT about this.

> User:  
> I'm learning the basics and implementation of compiler. Can you please tell me the reason why it's important to separate the inputs into several tokens when perform the lexical analysis?

> ChatGPT 4o mini:  
> Separating inputs into tokens during lexical analysis is crucial for several reasons:
> 
> 1. **Simplification**: Tokens simplify the syntax analysis phase by breaking down the input into manageable pieces. This makes it easier to understand the structure of the source code.
> 
> 2. **Error Detection**: By identifying tokens, the lexer can detect invalid characters or sequences early, which helps in providing meaningful error messages.
> 
> 3. **Efficiency**: Processing the input as tokens reduces the complexity of the parsing process. The parser can work with a finite set of token types rather than the raw input characters.
> 
> 4. **Language Abstraction**: Tokens represent meaningful constructs in the programming language (e.g., keywords, identifiers, literals), abstracting away the specific syntax details. This abstraction allows for easier implementation of language rules.
> 
> 5. **Support for Multiple Languages**: A lexer can be designed to recognize different languages by defining different sets of tokens, making the compiler more versatile.
> 
> 6. **Facilitating Parsing**: With tokens, the parser can use grammar rules more effectively, as it deals with discrete units rather than continuous character streams.
> 
> By organizing the input into tokens, the lexical analyzer lays the groundwork for effective parsing and further stages of compilation.

### Description

Based on previous work, the grammar of the level-1 calculator input can be described as:

- `${integer}`: a **31-bit** unsigned integer like `0`, `256`, `20240921`  
- `${op}`: binary operator `+` or `-`  
- `${expr}`: `${integer} ${op} ${integer}`  
- `${statement}`: `${expr};` or `;`

Each `${statement}` or `${expr}` can be within multiple lines;  
blank characters between `${integer}`, `${op}`, `';'` should be ignored;

Example of valid input:  
```
1 + 2;2+3;
1    -
  3
    ;
;
```

For a real calculator, user won't tell how many input will be given. So specially, we say that **the null statement**:
```
;
```
is considered as the end of input.

**Now you are asked to tokenize the input of user**. Then print out the info of the token under processing to **`stderr`** in this format:
```
"${varible_name}": {
    "variable_type": "${variable_type}"
    "kind": "${kind}", 
    "value": "${value}"
}
```
`${variable_name}` is the name of the token variable;  
`${variable_type}` is the type name of the variable;  
`${kind}` is the kind of the token;  
`${value}` is the value store in the token.

Printing the value of each `${expr}` is recommanded but not enforced. If you want, print one value for each `${statement}` per line to **`stdout`**.

### Input sample:
```
1 + 2;2+3;
1    -
  3
    ;;
```

### Output sample of `stderr` (required):
```
"${varible_name}": {
    "variable_type": "${variable_type}"
    "kind": "int", 
    "value": "1"
},
"${varible_name}": {
    "variable_type": "${variable_type}"
    "kind": "op", 
    "value": "+"
},
"${varible_name}": {
    "variable_type": "${variable_type}"
    "kind": "int", 
    "value": "2"
},
"${varible_name}": {
    "variable_type": "${variable_type}"
    "kind": "end_of_statement", 
    "value": ";"
},
"${varible_name}": {
    "variable_type": "${variable_type}"
    "kind": "int", 
    "value": "2"
},
"${varible_name}": {
    "variable_type": "${variable_type}"
    "kind": "op", 
    "value": "+"
},
"${varible_name}": {
    "variable_type": "${variable_type}"
    "kind": "int", 
    "value": "3"
},
"${varible_name}": {
    "variable_type": "${variable_type}"
    "kind": "end_of_statement", 
    "value": ";"
},"${varible_name}": {
    "variable_type": "${variable_type}"
    "kind": "int", 
    "value": "1"
},
"${varible_name}": {
    "variable_type": "${variable_type}"
    "kind": "op", 
    "value": "-"
},
"${varible_name}": {
    "variable_type": "${variable_type}"
    "kind": "int", 
    "value": "3"
},
"${varible_name}": {
    "variable_type": "${variable_type}"
    "kind": "end_of_statement", 
    "value": ";"
},
"${varible_name}": {
    "variable_type": "${variable_type}"
    "kind": "end_of_statement", 
    "value": ";"
}
```
**Remember to replace `${...}` with correct content.**

### Output sample of `stdout` (optional):
```
3
5
-2
```

### Data range

For 100% of the dataset, `0 <= ${integer} < 2^31`, `${op} == '+' || ${op} == '-'`, all of inputs are valid and **only one** null statement as the last statement.