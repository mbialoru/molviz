#!/usr/bin/python3
import sys

""" Small script to generate header and implementation file for C++ """


def main():
    name = str(sys.argv[1])
    gen_class = False
    try:
        option = str(sys.argv[2])
        if option == "class":
            gen_class = True
    except IndexError:
        pass

    filename = name.replace(" ", "").lower()
    with open(f"{filename}.hpp", "wt") as f:
        f.write(f"#ifndef {name.upper()}_HPP\n")
        f.write(f"#define {name.upper()}_HPP\n\n")
        f.write("#pragma once\n\n")
        f.write("// Includes here\n\n")
        if gen_class:
            f.write(f"class {name}\n{{\n")
            f.write("public:\n")
            f.write(f"  {name}();\n")
            f.write(f"  ~{name}() = default;\n\n")
            f.write("private:\n\n")
            f.write("};\n\n")
        else:
            f.write("\n\n")
        f.write(f"#endif // {name.upper()}_HPP")

    with open(f"{filename}.cpp", "wt") as f:
        if gen_class:
            f.write(f'#include "{filename}.hpp"\n\n')
            f.write(f"{name}::{name}() {{}};")
        else:
            f.write(f'#include "{filename}.hpp"\n')


if __name__ == "__main__":
    if len(sys.argv) < 2 or len(sys.argv) > 3:
        print(
            """ Usage: python3 cpp_gen.py <name> <class>

        if <class> is present then generates files for class
        generic header/source pair otherwise.
        """
        )
        sys.exit(1)
    sys.exit(main() or 0)
