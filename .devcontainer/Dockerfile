# [Choice] bionic (18.04), focal (20.04)
ARG VARIANT="sid-slim"
FROM debian:${VARIANT}

ARG USERNAME=vscode
ARG USER_UID=1000
ARG USER_GID=$USER_UID
ARG MIRROR=poland

# Set root password
RUN echo 'root:docker' | chpasswd

# Set fastest available mirror
RUN apt-get update -qq && export DEBIAN_FRONTEND=noninteractive && \
    apt-get install -y --no-install-recommends netselect-apt
RUN netselect-apt -c ${MIRROR} -t 15 -a amd64 -n unstable && \
    mv sources.list /etc/apt/sources.list && \
    apt-get update -qq && export DEBIAN_FRONTEND=noninteractive

# Install necessary packages available from standard repos
RUN apt-get install -y --no-install-recommends \
        software-properties-common wget apt-utils file zip \
        openssh-client gpg-agent socat rsync make ninja-build \
        git python3 python3-pip

# Install OpenGL libraries
RUN apt-get install -y --no-install-recommends \
        libgl1-mesa-dev libxext-dev

# Install cmake-format
RUN python3 -m pip install --upgrade pip setuptools --break-system-packages && \
    python3 -m pip install cmake-format --break-system-packages && \
    cmake-format --version

# Install conan
RUN python3 -m pip install conan --break-system-packages && \
    conan --version

# By default, anything you run in Docker is done as superuser.
# Conan runs some install commands as superuser, and will prepend `sudo` to
# these commands, unless `CONAN_SYSREQUIRES_SUDO=0` is in your env variables.
ENV CONAN_SYSREQUIRES_SUDO 0

# Some packages request that Conan use the system package manager to install
# a few dependencies. This flag allows Conan to proceed with these
# installations;
# leaving this flag undefined can cause some installation failures.
ENV CONAN_SYSREQUIRES_MODE enabled

# User-settable versions:
# This Dockerfile should support gcc-[7, 8, 9, 10, 11] and
# clang-[10, 11, 12, 13]
# Earlier versions of clang will require significant modifications to the
# IWYU section
ARG GCC_VER="12"
# Add gcc-${GCC_VER}
RUN apt-get install -y --no-install-recommends gcc-${GCC_VER} g++-${GCC_VER} gdb

# Set gcc-${GCC_VER} as default gcc
RUN update-alternatives --install /usr/bin/gcc gcc $(which gcc-${GCC_VER}) 100
RUN update-alternatives --install /usr/bin/g++ g++ $(which g++-${GCC_VER}) 100

ARG LLVM_VER="15"
# Add clang-${LLVM_VER}
RUN apt-get install -y --no-install-recommends \
    clang-${LLVM_VER} lldb-${LLVM_VER} lld-${LLVM_VER} clangd-${LLVM_VER} \
    llvm-${LLVM_VER}-dev libclang-${LLVM_VER}-dev clang-tidy-${LLVM_VER} \
    clang-format-${LLVM_VER} clang-tools-${LLVM_VER}

# Set the default clang-tidy, so CMake can find it
RUN update-alternatives --install /usr/bin/clang-tidy clang-tidy $(which clang-tidy-${LLVM_VER}) 1

# Set clang-${LLVM_VER} as default clang
RUN update-alternatives --install /usr/bin/clang clang $(which clang-${LLVM_VER}) 100
RUN update-alternatives --install /usr/bin/clang++ clang++ $(which clang++-${LLVM_VER}) 100

# Add clang-format
RUN apt-get install -y --no-install-recommends clang-format

# Add cmake/ccmake
RUN apt-get update -qq && export DEBIAN_FRONTEND=noninteractive && \
    apt-get install -y --no-install-recommends cmake cmake-curses-gui

# Install optional dependecies
RUN apt-get update -qq && export DEBIAN_FRONTEND=noninteractive && \
    apt-get install -y --no-install-recommends \
        doxygen graphviz ccache cppcheck

# Install include-what-you-use
ENV IWYU /home/iwyu
ENV IWYU_BUILD ${IWYU}/build
ENV IWYU_SRC ${IWYU}/include-what-you-use
RUN mkdir -p ${IWYU_BUILD} && \
    git clone --branch clang_${LLVM_VER} \
        https://github.com/include-what-you-use/include-what-you-use.git \
        ${IWYU_SRC}
RUN CC=clang-${LLVM_VER} CXX=clang++-${LLVM_VER} cmake -S ${IWYU_SRC} \
        -B ${IWYU_BUILD} \
        -G "Unix Makefiles" -DCMAKE_PREFIX_PATH=/usr/lib/llvm-${LLVM_VER} && \
    cmake --build ${IWYU_BUILD} -j && \
    cmake --install ${IWYU_BUILD}

# Per https://github.com/include-what-you-use/include-what-you-use#how-to-install:
# `You need to copy the Clang include directory to the expected location before
#  running (similarly, use include-what-you-use -print-resource-dir to learn
#  exactly where IWYU wants the headers).`
RUN mkdir -p $(include-what-you-use -print-resource-dir 2>/dev/null)
RUN ln -s $(readlink -f /usr/lib/clang/${LLVM_VER}/include) \
    $(include-what-you-use -print-resource-dir 2>/dev/null)/include

## Cleanup cached apt data we don't need anymore
RUN apt-get autoremove -y && apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# Allow the user to set compiler defaults
ARG USE_CLANG
# if --build-arg USE_CLANG=1, set CC to 'clang' or set to null otherwise.
ENV CC=${USE_CLANG:+"clang"}
ENV CXX=${USE_CLANG:+"clang++"}
# if CC is null, set it to 'gcc' (or leave as is otherwise).
ENV CC=${CC:-"gcc"}
ENV CXX=${CXX:-"g++"}

# Include project
#ADD . /workspaces/cpp_starter_project
#WORKDIR /workspaces/cpp_starter_project

# Create the user
RUN groupadd --gid $USER_GID $USERNAME \
    && useradd --uid $USER_UID --gid $USER_GID -m $USERNAME -s /bin/bash
USER $USERNAME

# Add alias for python3
RUN echo "alias python=python3" >> ~/.bashrc

# Change default cd directory to app
RUN echo "alias cd='HOME=/workspaces/timebox cd'" >> ~/.bashrc

# Add convenient aliases
RUN echo "alias l='ls -a'" >> ~/.bashrc
RUN echo "alias ll='ls -alFh'" >> ~/.bashrc

CMD ["/bin/bash"]
