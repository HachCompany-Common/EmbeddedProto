import os
import re
import argparse

def find_header_files(search_path, pattern="*_EAMS_size_test.h"):
    """Find all matching header files in a directory and subdirectories."""
    matched_files = []
    for root, _, files in os.walk(search_path):
        for file in files:
            if re.fullmatch(pattern.replace("*", ".*"), file):
                matched_files.append(os.path.join(root, file))
    return matched_files

def extract_functions(header_file):
    """Extract function names from a header file (only void functions with no parameters)."""
    function_pattern = re.compile(r'\bvoid\s+(\w+)\s*\(\s*\)\s*')  # Match `void func();`
    functions = []
    with open(header_file, 'r', encoding='utf-8') as file:
        for line in file:
            match = function_pattern.search(line)
            if match:
                functions.append(match.group(1))  # Extract function name
    return functions

def generate_cpp_code(header_files, functions):
    """Generate a C++ file that includes all headers and calls all extracted functions."""
    includes = "\n".join(f'#include "{os.path.basename(header)}"' for header in header_files)
    function_calls = "\n    ".join(f"{fn}();" for fn in functions)

    return f"""{includes}

void call_all_functions() {{
    {function_calls}
}}
"""

def main():
    parser = argparse.ArgumentParser(description="Process header files and generate a C++ function.")
    parser.add_argument("--input", "-i", default=".", required=True, help="Input folder to search for header files.")
    parser.add_argument("--output", "-o", default=".", required=True, help="Output folder to save the generated file.")

    args = parser.parse_args()
    input_folder = os.path.abspath(args.input)
    output_folder = os.path.abspath(args.output)

    if not os.path.isdir(input_folder):
        print(f"Error: Input folder '{input_folder}' does not exist.")
        return

    if not os.path.isdir(output_folder):
        os.makedirs(output_folder, exist_ok=True)

    header_files = find_header_files(input_folder)

    all_functions = []
    for header in header_files:
        all_functions.extend(extract_functions(header))

    if header_files and all_functions:
        cpp_code = generate_cpp_code(header_files, all_functions)
        output_file_path = os.path.join(output_folder, "SizeTest.h")
        with open(output_file_path, "w", encoding="utf-8") as output_file:
            output_file.write(cpp_code)
        print(f"C++ function generated in '{output_file_path}'.")
    else:
        print("No matching headers or functions found.")

if __name__ == "__main__":
    main()
