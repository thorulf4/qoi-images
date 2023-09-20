# Qoi images for C++
This library attempts to provide [qoi](https://qoiformat.org/) format with a simplistic and modern interface.
The code depends on the following 
* GCC with C++20 support tested with GCC-11
* CMake 2.26


### Usage example
Two classes are provided Image and TransparentImage each containing RGB and RGBA pixels respectively.

Reading an image from a file using iterators
```c++
auto file = std::ifstream{"image.qoi", std::ios_base::binary};
auto image = qoi::Image{std::istreambuf_iterator{file}, {}};
```


Similarly writing to a file using output iterators
```c++
auto file = std::ofstream{"output_image.qoi", std::ios_base::binary};
image.write(std::ostreambuf_iterator{file});
```


Decoding an image from a vector using ranges
```c++
auto byte_data = std::vector<char>{...};
auto image = qoi::TransparentImage{byte_data};
```

Decoding to a standard container can be
```c++
auto out_data = std::vector<char>{};
image.write(std::back_inserter{out_data});
```

### CMake integration
Dropping the following into your cmake will make the header only target `Qoi::qoi` available
```cmake
###
### Automatic fetching of the qoi image encoder / decoder
### This cmake file provides one header only target Qoi::qoi
###

find_package(Qoi 1.0.0 QUIET)

if(Qoi_FOUND)
    message(STATUS "Using Qoi found at ${Qoi_CONFIG}")
else(Qoi_FOUND)
include(FetchContent)
    message(STATUS "Fetching Qoi from github")
    FetchContent_Declare(
        qoi
        GIT_REPOSITORY https://github.com/thorulf4/qoi-images.git
        GIT_TAG main
    )

    FetchContent_GetProperties(qoi)
    if(NOT qoi_POPULATED)
        FetchContent_Populate(qoi)
        add_subdirectory(${qoi_SOURCE_DIR} ${qoi_BINARY_DIR} EXCLUDE_FROM_ALL)
    endif()
endif(Qoi_FOUND)
```