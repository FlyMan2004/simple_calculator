# `calc` Language

This is a reference of the core `calc` language.

<!-- TODO: Complete the definition of this language referring to C/C++/Rust -->

## Basic Concepts

### Comments

#### Syntax

1. /\* *comment* \*/
2. // *comment*

1\. Known as \"multi-line\" comments. 
2\. Known as \"single-line\" comments. 

### ASCII Chart

Refer to [ASCII Chart - cppreference.com](https://en.cppreference.com/w/c/language/ascii)

### Encoding

All of inputs are limited to ASCII code.

### Punctuation

These are the punctuation symbols in `calc`.

#### **\{ \}**

- Delimit a compound statement. The compound statement may be part of a function definition.

#### **\[ \]**

- Subscript operator.

#### **\( \)**

- In an expression, indicate grouping.
- Function call operator.
- In an if, switch, while, do-while, or for statement, delimit the controlling clause.

#### **;**

- Indicate the end of
    - a statement (including the init-statement of a for statement)
    - a declaration or struct-declaration-list
- Separate the second and third clauses of a for statement.

<!-- TODO: Add more punctuation -->

### Identifier

An identifier is an arbitrarily long sequence of digits, underscores, lowercase and uppercase Latin letters. A valid identifier must begin with a non-digit character (Latin letter, underscore). Identifiers are case-sensitive (lowercase and uppercase letters are distinct).

Identifiers can denote the following types of entities:

- objects
- functions
- structure or union members
- enumeration constants
<!-- TODO: More cases require identifier -->

### Type

#### Type Classification

- fundamental types
  - the type `void`
  - the type `nil`
  - the type `byte`
  - arithmetic types
    - integral types
      - the type `bool`
      - character types
        - signed: `signed char<N>` where `N` is an integer constant expression that specifies the number of bits that are used to represent the type, including the sign bit. Each value of N designates a distinct type.
        - unsigned: `unsigned char<N>` where `N` is an integer constant expression that specifies the number of bits that are used to represent the type. Each value of N designates a distinct type.
      - signed integer types
        - bit-precise: `signed integer<N>` where `N` is an integer constant expression that specifies the number of bits that are used to represent the type, including the sign bit. Each value of N designates a distinct type.
        - infinite precision: `signed integer`
      - unsigned integer types
        - bit-precise: `unsigned integer<N>` where `N` is an integer constant expression that specifies the number of bits that are used to represent the type. Each value of N designates a distinct type. Specially, `unsigned integer<N>` can be implicitly casted to `bool`
        - infinite precision: `unsigned integer`
    - floating-point types
      - `fp<E, F, B>` where `E`, `F`, `B` are integer constant expression that specify the number of bits of "Exponent", 'Fraction" part of a floating-point type and base of this type.
- compound types
  - reference types
  - pointer types
    - native pointer types
    - fat pointer types
  - array types
  - function types
  - enumeration types
  - class types
<!-- This language will allow user to store v_table in fat pointer or object -->

<!-- TODO: More sections -->

### Enforced Optimization

#### Copy/Move elision

#### Constant Folding

#### Empty Base Optimization

## Keywords

| Keyword   | Usage                                     |
|-----------|-------------------------------------------|
| bool      | bool type: as the declaration of the type |
| break     |                                           |
| byte      |                                           |
| case      |                                           |
| catch     |                                           |
| class     |                                           |
| const     |                                           |
| constexpr |                                           |
| consteval |                                           |
| constinit |                                           |
| continue  |                                           |
| decltype  |                                           |
| default   |                                           |
