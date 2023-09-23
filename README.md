# CT0442-JsonParserProject
This repository is related to the University course CT0442: "Programming and laboratory". A basic requirement to pass the course was to do a project assigned by the professor in the specifications he presented to us.

This project provides a JSON parser implemented in C++ using the following files:

- `json.hpp`: Header file containing the `json` class definition and related functions.
- `json.cpp`: Source file containing the implementation of the `json` class and related functions.

## Description

The JSON parser is designed to parse and manipulate JSON data in C++. It provides a `json` class with various methods for handling different JSON data types, such as null, boolean, number, string, list, and dictionary.

## Features of json.cpp

`json.cpp` contains the implementation of the `json` class methods and operators defined in `json.hpp`. It includes functions for:

- Parsing JSON data from a JSON file.
- Outputting JSON data to a JSON file.
- Handling different JSON data types (null, boolean, number, string, list, and dictionary).

## Structure of json.cpp

The `json.cpp` file is organized as follows:

1. Implementation of the `json` class methods, such as `is_list`, `is_dictionary`, `is_string`, `is_number`, `is_bool`, and `is_null`.
2. Accessors and modifiers for JSON data, such as `get_number`, `get_bool`, `get_string`, `set_string`, `set_bool`, `set_number`, `set_null`, `set_list`, and `set_dictionary`.
3. Iterators for lists and dictionaries, such as `begin_list`, `end_list`, `begin_dictionary`, and `end_dictionary`.
4. Overloaded input and output stream operators for reading and writing JSON data.

## Description of the "parse" Function

The `parse` function in `json.cpp` takes a string as input and returns a `json` object. It checks the input string for different JSON data types and sets the corresponding value in the `json` object. If the input string is empty, it throws a `json_exception` with a message indicating that the input is empty or incorrect [1].

## Exception for Debugging

The `json_exception` struct is defined in `json.hpp` and is used to throw exceptions when an error occurs during parsing or other operations. The `json_exception` struct contains a `msg` field, which is a string that stores the error message [2]. This exception can be used to debug the code or the input files by providing more information about the error encountered.
