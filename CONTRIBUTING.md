<div align="center">
    <img src="assets/logo.png" height="80" width="80">
    <h1>Celery</h1>
    <p>A High-Performance C++ STL-like library.</p>
</div>

---

## Overview

This document outlines the guidelines for contributing to the Celery project.
By participating in this project, you agree to abide by the following rules and best practices.

--- 

## How to Contribute

We welcome contributions from the community! Here are some ways you can contribute:

- **Reporting Issues:** If you encounter bugs or have feature requests, please open an 
issue on our [GitHub Issues page](https://github.com/rodrigoo-r/Celery/issues).
- **Submitting Pull Requests:** If you would like to contribute code, please fork the repository,
make your changes, and submit a pull request. Ensure that your code adheres to the project's
coding standards and includes appropriate tests.
- **Improving Documentation:** Help us improve our documentation by suggesting changes or
adding new content.
- **Code Reviews:** Participate in code reviews to help maintain the quality of the codebase.

---

## Contribution Guidelines

To ensure a smooth contribution process, please follow these guidelines:

- **Fork the Repository:** Start by forking the Celery repository to your GitHub account.
- **Create a Branch:** Create a new branch for your feature or bug fix. Use a descriptive name for the branch.
- **Write Clear Commit Messages:** Use clear and concise commit messages that describe the changes made.
- **Follow Coding Standards:** Adhere to the project's coding standards and best practices.
- **Update Documentation:** If your changes affect the documentation, please update it accordingly.
- **Submit a Pull Request:** Once your changes are ready, submit a pull request to the
main repository. Provide a clear description of the changes and any relevant context.
- **Respond to Feedback:** Be open to feedback and make necessary changes based on
reviews from maintainers and other contributors.

---

## Code of Conduct

We expect all contributors to adhere to the [Celery Code of Conduct](CODE_OF_CONDUCT.md).
Please read it to understand the standards we uphold in our community.

---

## License

By contributing to Celery, you agree that your contributions will be licensed under
the Apache License 2.0.

---

## Code Style

Please follow the existing code style and conventions used in the Celery codebase.
This includes formatting, naming conventions, and documentation practices.

We use clang-format for code formatting. You can find the configuration file
`.clang-format` in the root directory of the repository.

To format your code using clang-format, you can run the following command in your terminal:

```bash
clang-format -i path/to/your/file.cpp
```

However, `clang-format` alone does not suffice most of the time. 
Celery does not follow the default style strictly, instead prioritizing readability and consistency
over rigid adherence to established styles.

Here is a brief overview of our code style guidelines:

- **For curly braces:** We use Allman style for curly braces, meaning
the opening brace is placed on a new line.

For example, prefer this:
```cpp
if (condition)
{
    // code block
}
```

Over this:

```cpp
if (condition) {
    // code block
}
```

- **For templates:** Do not convolute template parameters in a single line,
break them into multiple lines for better readability.

For example, prefer this:
```cpp
template<
    typename T,
    unsigned int Capacity = 50,
    bool UseHeap = Capacity >= 256,
    typename Allocator = Celery::Pmr::ArrayAllocator<T>,
    // SFINAE to ensure Capacity > 0
    typename = std::enable_if_t<(Capacity > 0)>,
    // SFINAE to ensure Allocator is valid
    typename = std::enable_if_t<
        std::is_base_of_v<
            Celery::Pmr::ArrayAllocator<T>,
            Allocator
        >
    >
>
```

Over this:

```cpp
template<typename T, unsigned int Capacity = 50, bool UseHeap = Capacity >= 256,
    typename Allocator = Celery::Pmr::ArrayAllocator<T>,
    // SFINAE to ensure Capacity > 0
    typename = std::enable_if_t<(Capacity > 0)>,
    // SFINAE to ensure Allocator is valid
    typename = std::enable_if_t<
    std::is_base_of_v<Celery::Pmr::ArrayAllocator<T>, Allocator>>>
```

- **For function parameters:** When a function has multiple parameters,
consider breaking them into multiple lines for clarity.

For example, prefer this:
```cpp
void exampleFunction(
    int firstParam,
    const std::string& secondParam,
    double thirdParam
) {
    // Function implementation
}
```

Over this:

```cpp
void exampleFunction(int firstParam, const std::string& secondParam, double thirdParam)
{
    // Function implementation
}
```

- **For Inheritance:** When inheriting from multiple classes or interfaces,
consider listing each base class on a new line, and putting the colon in
the same line as the derived class name.

For example, prefer this:
```cpp
class DerivedClass :
    public BaseClass1,
    public BaseClass2,
    public Interface1
{
    // Class implementation
};
```

Over this:

```cpp
class DerivedClass : public BaseClass1, public BaseClass2, public Interface1
{
    // Class implementation
};
```

- **Use `Celery::Traits::` for common types:** 
When referring to common type traits, prefer using the `Celery::Traits::` namespace
for better clarity and consistency.

For example, prefer this:
```cpp
Celery::Traits::VeryLarge myVariable; // Alias for size_t
```

Over this:

```cpp
size_t myVariable;
```

- **Consistent Naming Conventions:** Follow the established naming conventions
used in the Celery codebase for variables, functions, classes, and other identifiers.

  - For namespaces, classes, structs, enums, typedefs, aliases, global constants/variables
  and function names, use `PascalCase`.

  - For parameters and local variables, use `snake_case`.

By following these guidelines, we can maintain a clean and consistent codebase
that is easy to read and contribute to. Thank you for helping us improve Celery!