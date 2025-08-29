# GoneLang Compiler Documentation

## 📂 Folder Structure

```
repo/
│
├── src/
│   └── compiler.c      # The complete GoneLang compiler implementation
│
└── example/
    └── main.gl         # Example GoneLang program
```

---

## ⚙️ Overview

GoneLang is a **minimal experimental language** with the file extension `.gl`. This compiler is written in C and translates GoneLang code into Linux x86-64 assembly. The compiler then assembles and links the program into an executable that runs and exits with a given status code.

Currently, GoneLang supports only one statement:

* `exit <int>;` → exit the program with the specified integer code.

Example:

```gl
exit 5;
```

This program exits with code `5`.

---

## 🔑 Components

### 1. **Token Definition** (`struct TOKEN`)

GoneLang programs are broken into tokens. Currently, only one token type exists:

* `_EXIT` → Represents the `exit` statement.

```c
enum TokenType{
    _EXIT = 1
};

struct TOKEN{
    enum TokenType type;
    int value;
};
```

---

### 2. **Tokeniser** (`tokenise`)

The `tokenise` function scans the source code from `example/main.gl`, identifies keywords, and extracts integer values.

Supported syntax:

```gl
exit <integer>;
```

If an unknown keyword or incorrect syntax is found, the compiler raises an error.

---

### 3. **Code Generator** (`codegen`)

The `codegen` function maps tokens to assembly instructions. For the `_EXIT` token, it generates Linux x86-64 system call assembly:

```asm
.global _start
_start:
    mov $<value>, %rdi   # exit code
    mov $60, %rax        # syscall: exit
    syscall
```

This assembly is written into `output.s`.

---

### 4. **Execution** (`execute`)

The compiler automatically:

1. Assembles the assembly file into an object file:

   ```sh
   as -64 output.s -o output.o
   ```
2. Links the object file into an executable:

   ```sh
   ld output.o -o output
   ```
3. Runs the executable and prints the exit status:

   ```sh
   ./output
   Program exited with code: <value>
   ```

---

## ▶️ Running the Compiler

1. Write a GoneLang program inside `example/main.gl`. Example:

   ```gl
   exit 7;
   ```

2. Compile and run the compiler:

   ```sh
   gcc src/compiler.c -o gonelang
   ./gonelang
   ```

3. Output:

   ```
   Token: EXIT, Value: 7
   Program exited with code: 7
   ```

---

## 🚧 Limitations

* Only supports `exit <int>;` and `print("{your text}")` or `printn` for newline
* No variables, expressions, or multiple statements.
* Error handling is basic.

---

## 📌 Future Improvements

* Add support for arithmetic expressions.
* Implement variables and assignments.
* Expand grammar beyond `exit`.
* Improve lexer (tokeniser) and parser for robustness.

---

## 👨‍💻 Author

GoneLang is an experimental language project to learn about **lexing, parsing, code generation, and system calls** in Linux x86-64.
