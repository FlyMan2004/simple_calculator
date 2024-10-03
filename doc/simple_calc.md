# Simple Calculator

Calculator is a very useful tool in learning, engineering and many concepts. 
It seems very easy to create one compared with your powerful phone. That's correct.

This paper will guide you to create a calculator with only std library in C++.

## Level 0: "1 + 1 = ?", "2 - 1 = ?"

### Description

The lv.0 is for your revision of C++ concepts. So it doesn't involve too many things. You need to write a program to deal with the following requirements.

In the first line, there is one integer `${n}` as input to indicate the lines of expressions.

In each of the following `${n}` lines, there will be two integers `${a}` and `${b}` as well as one operator *`${op}`*, which can only be `'+'` or `'-'`.

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

## Level 1: Take Tok, Put Tok

### Background

In real implementation of a usable calculator, tokenize the input is vital for multiple reasons.  
Below is Q&A with ChatGPT about this.

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

- ***`${integer}`***: a **31-bit** unsigned integer like `0`, `256`, `20240921`  
- ***`${op}`***: binary operator `+` or `-`  
- ***`${expr}`***: *`${integer} ${op} ${integer}`*  
- ***`${terminator}`***: `;`
- ***`${statement}`***: *`${expr} ${terminator}`* or *`${terminator}`*

Each *`${statement}`* or *`${expr}`* can be within multiple lines;  
spaces between *`${integer}`*, *`${op}`*, *`${terminator}`* should be ignored;

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
"${variable_name}": {
    "variable_type": "${variable_type}"
    "kind": "${kind}", 
    "value": "${value}"
}
```
`${variable_name}` is the name of the token variable;  
`${variable_type}` is the type name of the variable;  
`${kind}` is the kind of the token;  
`${value}` is the value store in the token.

Printing the value of each *`${expr}`* is recommended but not enforced. If you want, print one value for each *`${statement}`* per line to **`stdout`**.

### Input Sample:
```
1 + 2;2+3;
1    -
  3
    ;;
```

### Output Sample of `stderr` (Required):
```json
"${variable_name}": {
    "variable_type": "${variable_type}"
    "kind": "int", 
    "value": "1"
},
"${variable_name}": {
    "variable_type": "${variable_type}"
    "kind": "op", 
    "value": "+"
},
"${variable_name}": {
    "variable_type": "${variable_type}"
    "kind": "int", 
    "value": "2"
},
"${variable_name}": {
    "variable_type": "${variable_type}"
    "kind": "end_of_statement", 
    "value": ";"
},
"${variable_name}": {
    "variable_type": "${variable_type}"
    "kind": "int", 
    "value": "2"
},
"${variable_name}": {
    "variable_type": "${variable_type}"
    "kind": "op", 
    "value": "+"
},
"${variable_name}": {
    "variable_type": "${variable_type}"
    "kind": "int", 
    "value": "3"
},
"${variable_name}": {
    "variable_type": "${variable_type}"
    "kind": "end_of_statement", 
    "value": ";"
},"${variable_name}": {
    "variable_type": "${variable_type}"
    "kind": "int", 
    "value": "1"
},
"${variable_name}": {
    "variable_type": "${variable_type}"
    "kind": "op", 
    "value": "-"
},
"${variable_name}": {
    "variable_type": "${variable_type}"
    "kind": "int", 
    "value": "3"
},
"${variable_name}": {
    "variable_type": "${variable_type}"
    "kind": "end_of_statement", 
    "value": ";"
},
"${variable_name}": {
    "variable_type": "${variable_type}"
    "kind": "end_of_statement", 
    "value": ";"
}
```
**Remember to replace `${...}` with correct content.**

### Output Sample of `stdout` (Optional):
```
3
5
-2
```

### Data Range

For 100% of the dataset, `0 <= ${integer} < 2^31`, `${op} == '+' || ${op} == '-'`, all of inputs are valid and **only one** null statement as the last statement.

### Note

Please write your code based on framework.cxx.  
If your compiler complain that it does not support std::format, try to upgrade to gcc 14 or later.  
Compile with following arguments: `"-std=c++23 -Wall -Wextra -Werror -O3"`

## Interlude 0: The Missing Semester of Your CS Education

### The Story before Stories

#### [Why we are teaching this class](https://missing.csail.mit.edu/about/)

To Brief, for your and cooperators' hair and lifetime.

#### All you need for this project

Since this "semester" is quite long, only these tools will be musts here:

- Git  
- GNU Make  
- Shell: Bash, Zsh, ...  
- Compiler: GCC, Clang  
- etc.  

#### Tutorials and documentations for the above

> [Git Tutorial](https://www.w3schools.com/git/)  
> [Makefile Tutorial By Example](https://makefiletutorial.com/)  
> [Linux/Unix Tutorial](https://www.tutorialspoint.com/unix/index.htm)  
> [GCC online documentation - GNU Project](https://gcc.gnu.org/onlinedocs/)  
> [Welcome to Clang's documentation!](https://clang.llvm.org/docs/index.html)

### Tasks you need to do

#### Task 0: Setup your own github account and local git env

As the project becomes complicated, you're likely to need a tools to manage it. Git is a modern choice for this scenario. 

> **User**:  
> What's the benefit of using it compared with manual control?  

> **ChatGPT**:  
> Benefits of Git over Manual Control:
> 1. **Error prevention**: Manually copying files for different versions of a project is prone to human error. Git ensures every change is tracked properly.
> 2. **Collaboration**: Git facilitates collaboration by managing contributions from multiple people efficiently, which is nearly impossible with manual control.
> 3. **History tracking**: Git automatically keeps track of changes over time, allowing developers to easily compare, revert, or understand code differences.
> 4. **Branching**: With manual control, creating a separate version of the project for testing or new features can be cumbersome. Git allows easy branching and merging.
> 5. **Backup**: Since every developer has a full copy of the project’s history, Git acts as a natural backup system. Manual systems often lack proper backup mechanisms.
> 
> Overall, Git is powerful, efficient, and scalable, making it an essential tool for modern software development, particularly for open-source projects where collaboration and community involvement are crucial.

There are 3 things to do:
1. Create an account on [GitHub](https://github.com/).  
2. Install [GitHub Desktop](https://desktop.github.com/download/) on Microsoft Windows; [GitHub CLI](https://github.com/cli/cli/releases) on Linux. If you're confident, [git](https://git-scm.com/downloads) is also a good choice.  
3. Login your GitHub account or setup your git manually.

#### Task 1: [Fork a repo](https://www.w3schools.com/git/git_remote_fork.asp) for management all the code

Since now, you need to fork the `template` branch as your initial project organization. All files should be well-placed in correct folder.

> Note: The template may be updated. Please try to keep it up-to-date.

Below is a project dir based on the template:

```
./
|-- doc/
|   |-- simple_calc.html
|   `-- simple_calc.md
|-- include/
|   |-- expr_eval.hxx
|   |-- op.hxx
|   |-- token.hxx
|   `-- utility.hxx
|-- src/
|   |-- calculator_core.cxx
|   `-- expr_eval.cxx
|-- test/
|   `-- example_0.calc
|-- framework.cxx
`-- Makefile
```

> Note: This may differ from yours.

#### Task 2: Clone the newly created repo to local

This depends on what tools used. References:

- GitHub Desktop: https://docs.github.com/en/desktop
- GitHub CLI: https://cli.github.com/manual/
- Git: https://git-scm.com/doc

Once you finished, you can start to rearrange your src.

#### Task 3: Before rewriting...

Generally speaking, you need to follow these rules in this project:

- `include/`: Headers of the project. Usually with postfix `.hxx`, `.hpp`, `.h`, etc.
- `doc/`: Documentation of the project. 
- `src/`: Source codes of implementations that you think suitable to place in standalone files. Usually with postfix `.cxx`, `.cpp`, `.CC`, `.c`, etc.
- `test/`: Test suite for the project. 
- `bin/`: Binaries generated by toolchains.
- `.gitignore`: This file includes the content shouldn't be tracked by git, like binaries, temp files...
- `Makefile`: Supporting file for `make`. See: "[Makefile Tutorial By Example](https://makefiletutorial.com/)".

Before you start to rewrite your code, there is **still some other work** to do:  

- **Editor/IDE**: 
  - [`VSCode`](https://code.visualstudio.com/) is strongly recommended for its flexibility;  
  - [`Code::Blocks stable`](https://www.codeblocks.org/), as an IDE, is bundled with out-of-date toolchain while [`Code:Blocks nightly`](https://forums.codeblocks.org/index.php/board,20.0.html) is no longer an out-of-box IDE; 
  - [`Visual Studio`](https://visualstudio.microsoft.com/) itself is required on Windows but not suitable as IDE since `GNU make` is required. 
- **Compiler**: 
  - [**Choose that supports at least C++23**](https://en.cppreference.com/w/cpp/compiler_support/23). 
  - Latest version of [`GCC`](https://gcc.gnu.org/) or [`Clang`](https://clang.llvm.org/) is strongly recommended. 
  - Latest [`MSVC Preview`](https://visualstudio.microsoft.com/vs/preview/) is also acceptable.
- **Toolchain**: `GNU make` is a must for building the project; [`Bear`](https://github.com/rizsotto/Bear) is useful to generate [compilation database](https://clang.llvm.org/docs/JSONCompilationDatabase.html) for realtime diagnose with [`clangd`](https://clangd.llvm.org/).

> NOTE:  
> - `MSVC` is always shipped with Visual Studio.  
> - Linux is always a better choice for dev. Some tools work more smoothly on Linux.  
> - For Windows users:  
>   - Dev on Windows is sometimes annoying when setting up env. But you can use [`WSL2`](https://learn.microsoft.com/en-us/windows/wsl/about#what-is-wsl-2) and [`Ubuntu (Preview)`](https://apps.microsoft.com/detail/9p7bdvkvnxz6) to get benefits of **BOTH**. :)
>   - If dislike or cannot use `WSL2`, you can use [`msys2`](https://www.msys2.org/) instead. Native dev is for professors.
>   - When you use `Clang/LLVM` downloaded from [GitHub](https://github.com/llvm/llvm-project/releases/), it depends on `MSVC`. Therefore, **MSVC must be installed**.
> - **Good Luck** for Mac user.

#### Task 4: Rewrite your program

*Enjoy :)*

## Level 2: Token More and More and More...

### Background Knowledge

- **Regular Expression**
- **Recursion**
- **Recursive Descent Parser**
- ...

<!-- TODO: Complete this section... -->

### Description

It's believed that you've known how to convert the strings to tokens, a better form for further and easier operations. If you've implement the program in a proper way, it should be quite easy to extend it to accept more general form of *`${expr}`*:

> ***`${expr}`***: *`${integer} (${op} ${integer})*`*

Moreover, the evaluation of such expression now is required. You need to write a tiny interpreter to execute each *`${statement}`*.

In addition, the program should terminate normally when *`null statement`* occurs.

### Input Sample
```
1 + 5 - 8;
;
This-should-never-be-parsed...
```

### Sample of `stdout`
```
-2
```

### Sample of `stderr`
```json
"token": {
   "variable_type": "N11simple_calc5TokenE",
   "kind": "integer",
   "value": "1"
},
"token": {
   "variable_type": "N11simple_calc5TokenE",
   "kind": "op",
   "value": "+"
},
"token": {
   "variable_type": "N11simple_calc5TokenE",
   "kind": "integer",
   "value": "5"
},
"token": {
   "variable_type": "N11simple_calc5TokenE",
   "kind": "op",
   "value": "-"
},
"token": {
   "variable_type": "N11simple_calc5TokenE",
   "kind": "integer",
   "value": "8"
},
"token": {
   "variable_type": "N11simple_calc5TokenE",
   "kind": "end_of_statement",
   "value": ";"
},
"token": {
   "variable_type": "N11simple_calc5TokenE",
   "kind": "end_of_statement",
   "value": ";"
}
```
> NOTE: *`variable_name`* and *`variable_type`* can be different.