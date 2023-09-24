# rinha-compiler

## Description

This is a simple tree-walking interpreter developed for participation in a compiler and interpreter competition. The project supports the Rinha programming language and includes features such as function declarations, conditionals, and mathematical expressions. It interprets a JSON file that contains an Abstract Syntax Tree (AST) representation of the code.

For more details on the competition and to access the project repository, visit [rinha-de-compiler](https://github.com/aripiprazole/rinha-de-compiler).

## Code Examples

Here are some code examples in Rinha that can be executed using this interpreter:

### Fibonacci

```js
let fib = fn (n) => {
  if (n < 2) {
    n
  } else {
    fib(n - 1) + fib(n - 2)
  }
};

print("fib: " + fib(42))
```

### Combination

```js
let combination = fn (n, k) => {
    let a = k == 0;
    let b = k == n;
    if (a || b)
    {
        1
    }
    else {
        combination(n - 1, k - 1) + combination(n - 1, k)
    }
};

print(combination(30, 12))
```

## Requirements

To run this interpreter locally, you will need the following tools and libraries:

- [Make](https://www.gnu.org/software/make/)
- [GCC](https://gcc.gnu.org/)
- [RapidJSON](https://rapidjson.org/)

## Data Preparation

Before interpreting a .rinha file, you need to prepare it by parsing it into a JSON file using the Rinha parser. To obtain it, you can use Cargo, Rust's package manager, from [Crates.io](https://crates.io/crates/rinha-parser):

```sh
cargo install rinha
```

Once you have the parser, you can proceed to parse your .rinha files into JSON as explained in the previous section.

## Usage

### Using Docker (Optional)

To run the interpreter using Docker in an isolated environment, follow these steps:

1. Build the Docker image:

   ```sh
   docker build . -f Dockerfile -t interpreter-cpp
   ```

2. Run the interpreter with your Rinha file as input:

   ```sh
   docker run --rm -v ./source.rinha.json:/var/rinha/source.rinha.json --memory=2gb --cpus=2 interpreter-cpp
   ```

### Local Compilation

If you prefer to compile and run the interpreter locally, follow these steps:

1. Compile the interpreter:

   ```sh
   make
   ```

2. Run the interpreter with your Rinha file as input:

   ```sh
   ./interpreter <yourfile.json>
   ```

## Contributions

Contributions to this project are welcome. If you wish to contribute, feel free to open issues, send pull requests, or contact the project maintainers.

## License

This project is licensed under [MIT License](https://github.com/magalhaesm/rinha-compiler/blob/main/LICENSE), if applicable.
