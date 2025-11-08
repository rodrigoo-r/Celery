<div align="center">
    <img src="assets/logo.png" height="80" width="80">
    <h1>Celery</h1>
    <p>A High-Performance C++ STL-like library.</p>
</div>

---

## Overview

Celery is a modern C++ library that provides a collection
of high-performance data structures and algorithms inspired
by the C++ Standard Template Library (STL).
It aims to offer efficient and easy-to-use alternatives for
common programming tasks, focusing on performance and usability.

Celery aims to be a modern, high-performance alternative predecessor
of [Zelix STL](https://github.com/zelix-lang/STL.git), as the latter
was not designed with strong emphasis on readability and maintainability,
which are key aspects of Celery's design philosophy.

---

## Installation

The only official way to install Celery is via CMake.
To include Celery in your CMake project, add the following lines to your `CMakeLists.txt`:

> **Note:** Celery requires **C++20 or higher.**  
> Supporting legacy standards is **not (and never will be)** a priority.  
> The goal is clean, modern, high-performance code — not backward compatibility.

```cmake
include(FetchContent)

FetchContent_Declare(
    Celery
    GIT_REPOSITORY https://github.com/zelix-lang/celery
    GIT_TAG        {latest_release_tag} # Replace with the latest release tag
)

FetchContent_MakeAvailable(Celery)
target_link_libraries(${PROJECT_NAME} PRIVATE Celery::celery)
```

You may also be able to build it from source by cloning the repository
and running the `build.sh` script.

Though the C++ standard library guarantees Windows compatibility,
we do not actively test (and do not plan to do so) Celery on Windows, 
instead focusing on Linux and macOS. However, as long as you have a 
C++20-compliant compiler, Celery should work on Windows as well.

---

## Contributing

Contributions are welcome! If you would like to contribute to Celery,
please refer to [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

---

## License

Celery is licensed under the Apache License 2.0.
See [LICENSE](LICENSE) for more details.