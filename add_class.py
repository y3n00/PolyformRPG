import os
import sys


def create_files(filename: str):
    os.makedirs("src", exist_ok=True)
    os.makedirs("include", exist_ok=True)

    with open(f"src/{filename}.cpp", "w") as cpp_file:
        cpp_file.write(f"#include \"../include/{filename}.hpp\"\n")

    with open(f"include/{filename}.hpp", "w") as hpp_file:
        hpp_file.write("#pragma once\n")
        hpp_file.write(f"class {filename} {{}};\n")

    print(f"Created src/{filename}.cpp and include/{filename}.hpp")


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python script_name.py filename")
        sys.exit(1)

    filename = sys.argv[1]
    create_files(filename)
