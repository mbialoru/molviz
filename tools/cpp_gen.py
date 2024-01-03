#!/usr/bin/python3
""" Small script to generate header and source file for C++ """

import sys
import argparse
import logging

from pathlib import Path

LOGGER = logging.getLogger(__name__)


class convert_to_pathlib(argparse.Action):
    def __call__(self, parser, namespace, values, option_string=None):
        setattr(namespace, self.dest, Path(values))


def setup_argv() -> argparse.Namespace:
    parser = argparse.ArgumentParser()
    parser.add_argument("cpp", nargs="?", default=Path("."), action=convert_to_pathlib)
    parser.add_argument("hpp", nargs="?", default=Path("."), action=convert_to_pathlib)
    parser.add_argument("-include", nargs="?", action=convert_to_pathlib, default=Path("."))
    parser.add_argument("-ipp", help="generate ipp file", default=False, action="store_true")
    parser.add_argument("-ns", "--namespace", type=str, action="store")
    parser.add_argument("-c", "--class", type=str, action="store")
    parser.add_argument("-ow", "--overwrite", help="allow overwrite", default=False, action="store_true")
    parser.add_argument("filename", type=str, nargs="?", default="")
    return parser.parse_args()


def check_file_exists(file_path: Path, overwrite: bool):
    if file_path.exists() and file_path.is_file():
        if overwrite:
            LOGGER.warning("file exists, overwriting")
            return False
        else:
            LOGGER.warning("file exists, skipping")
            return True
    else:
        return False


def setup_logger(debug: bool = False) -> None:
    """Create logging configuration."""
    log_format = "%(levelname)-8s %(message)s"
    logging.basicConfig(
        level=logging.DEBUG if debug else logging.INFO,
        format=log_format,
        stream=sys.stdout)


def main(argv: argparse.Namespace):
    if not argv.filename and getattr(argv, "class"):
        setattr(argv, "filename", getattr(argv, "class").lower())

    filename_lower = argv.filename.replace(" ", "").lower()
    filename_upper = argv.filename.replace(" ", "").upper()

    # write implementation
    if argv.ipp:
        ipp_path = argv.hpp / f"{filename_lower}.ipp"

        LOGGER.info(f"writing implementation {ipp_path}")

        if not check_file_exists(ipp_path, argv.overwrite):
            # create empty implementation file
            ipp_path.touch()

    # write header
    hpp_path = argv.hpp / f"{filename_lower}.hpp"

    LOGGER.info(f"writing header {hpp_path}")

    if not check_file_exists(hpp_path, argv.overwrite):
        with open(hpp_path, "wt") as hpp_file:
            hpp_file.write(f"#ifndef {filename_upper}_HPP\n")
            hpp_file.write(f"#define {filename_upper}_HPP\n\n")
            hpp_file.write("#pragma once\n\n")
            hpp_file.write("// Includes here\n")
            if namespace := argv.namespace:
                hpp_file.write(f"\nnamespace {namespace} {{\n\n")
            if name := getattr(argv, "class"):
                hpp_file.write(f"class {name}\n{{\n")
                hpp_file.write("public:\n")
                hpp_file.write(f"  {name}();\n")
                hpp_file.write(f"  ~{name}() = default;\n\n")
                hpp_file.write("private:\n")
                hpp_file.write("};")
            if argv.ipp:
                hpp_file.write(f"\n\n#include \"{ipp_path.relative_to(argv.include)}\"")
            if namespace := argv.namespace:
                hpp_file.write(f"\n\n}}// namespace {namespace}")
            hpp_file.write("\n\n")
            hpp_file.write(f"#endif// {filename_upper}_HPP")

    # write source
    cpp_path = argv.cpp / f"{filename_lower}.cpp"
    LOGGER.info(f"writing source {cpp_path}")

    if not check_file_exists(cpp_path, argv.overwrite):
        with open(cpp_path, "wt") as cpp_file:
            try:
                cpp_file.write(f'#include "{hpp_path.relative_to(argv.include)}"\n')
            except ValueError:
                cpp_file.write(f'#include "{hpp_path}"\n')
            if namespace := argv.namespace:
                cpp_file.write(f"\nusing namespace {namespace};\n")
            if name := getattr(argv, "class"):
                cpp_file.write(f"\n{name}::{name}() {{}};")


if __name__ == "__main__":
    setup_logger()
    sys.exit(main(setup_argv()) or 0)
