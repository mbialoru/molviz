#!/usr/bin/python3
""" Small script to generate header and source file for C++ """

import sys
import argparse
import logging

from pathlib import Path

logger = logging.getLogger(__name__)

INCLUDE_PATH = Path("../include")


class convert_to_pathlib(argparse.Action):
    def __call__(self, parser, namespace, values, option_string=None):
        setattr(namespace, self.dest, Path(values))


def setup_argv() -> argparse.Namespace:
    parser = argparse.ArgumentParser()
    parser.add_argument("filename", type=str)
    parser.add_argument("source", nargs="?", default=Path("."), action=convert_to_pathlib)
    parser.add_argument("include", nargs="?", default=Path("."), action=convert_to_pathlib)
    parser.add_argument("-ns", "--namespace", type=str, action="store")
    parser.add_argument("-c", "--class", type=str, action="store")
    return parser.parse_args()


def setup_logger(debug: bool = False) -> None:
    """Create logging configuration."""
    log_format = "%(levelname)-8s %(message)s"
    logging.basicConfig(
        level=logging.DEBUG if debug else logging.INFO,
        format=log_format,
        stream=sys.stdout)


def main(argv: argparse.Namespace):
    filename_lower = argv.filename.replace(" ", "").lower()
    filename_upper = argv.filename.replace(" ", "").upper()

    # write header
    header_path = getattr(argv, "include") / f"{filename_lower}.hpp"
    logger.info(f"writing header {header_path}")

    if header_path.exists() and header_path.is_file():
        logger.warning(f"header file {header_path} exists, skipping")
    else:
        with open(header_path, "wt") as f:
            f.write(f"#ifndef {filename_upper}_HPP\n")
            f.write(f"#define {filename_upper}_HPP\n\n")
            f.write("#pragma once\n\n")
            f.write("// Includes here\n\n")
            if namespace := getattr(argv, "namespace"):
                f.write(f"\nnamespace {namespace} {{\n\n")
            if name := getattr(argv, "class"):
                f.write(f"class {name}\n{{\n")
                f.write("public:\n")
                f.write(f"  {name}();\n")
                f.write(f"  ~{name}() = default;\n\n")
                f.write("private:\n")
                f.write("};")
            if namespace := getattr(argv, "namespace"):
                f.write(f"\n\n}}// namespace {namespace}")
            f.write("\n\n")
            f.write(f"#endif// {filename_upper}_HPP")

    # write source
    source_path = getattr(argv, "source") / f"{filename_lower}.cpp"
    logger.info(f"writing source {source_path}")

    if source_path.exists() and source_path.is_file():
        logger.warning(f"source file {source_path} exists, skipping")
    else:
        with open(source_path, "wt") as f:
            try:
                f.write(f'#include "{header_path.relative_to(INCLUDE_PATH)}"\n')
            except ValueError:
                f.write(f'#include "{header_path}"\n')
            if namespace := getattr(argv, "namespace"):
                f.write(f"\nusing namespace {namespace};\n")
            if name := getattr(argv, "class"):
                f.write(f"\n{name}::{name}() {{}};")


if __name__ == "__main__":
    setup_logger()
    sys.exit(main(setup_argv()) or 0)
