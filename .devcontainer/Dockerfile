# sid(unstable), bookworm(test), bullseye(11), buster(10)
# available in -slim variants
ARG VARIANT="sid-slim"
FROM debian:${VARIANT}

ARG USERNAME=vscode
ARG USER_UID=1000
ARG USER_GID=$USER_UID
ARG MIRROR=poland

# add root password
RUN echo 'root:docker' | chpasswd

# disable apt interactive dialogs
ENV DEBIAN_FRONTEND=noninteractive
ENV DEBCONF_NOWARNINGS="yes"

# add fastest available mirrors
RUN apt-get update -qq && \
    apt-get install -y --no-install-recommends netselect-apt

RUN netselect-apt -c ${MIRROR} -t 15 -a amd64 -n unstable && \
    mv sources.list /etc/apt/sources.list && \
    apt-get update -qq && export DEBIAN_FRONTEND=noninteractive

# add locale so dpkg doesn't hang on perl missing locale error building image
# NOTE: libc6 gets installed here instead of at gcc step, maybe thats the reason
ENV LANG=en_US.UTF-8
ENV LANGUAGE=$LANG
ENV LC_ALL=$LANG
RUN apt-get install -y --no-install-recommends locales
RUN sed -i -e "s/# $LANG.*/$LANG UTF-8/" /etc/locale.gen && \
    dpkg-reconfigure --frontend=noninteractive locales && \
    update-locale LANG=$LANG

# add necessary packages available from standard repos
RUN apt-get install -y --no-install-recommends \
        software-properties-common wget apt-utils file zip \
        openssh-client gpg-agent socat rsync make ninja-build git

# add python and configure default venv
ENV PYTHONDONTWRITEBYTECODE 1
ENV PYTHONUNBUFFERED 1

RUN apt-get update -qq && export DEBIAN_FRONTEND=noninteractive && \
    apt-get install -y --no-install-recommends \
    python3 python3-pip python3-venv

RUN python3 -m venv /opt/venv
ENV PATH="/opt/venv/bin:$PATH"
RUN python3 -m pip install --upgrade pip

# add OpenGL libraries
RUN apt-get install -y --no-install-recommends \
        libgl1-mesa-dev libxext-dev libglew-dev

# install cmake-format
RUN python3 -m pip install --upgrade pip setuptools --break-system-packages && \
    python3 -m pip install cmake-format --break-system-packages && \
    cmake-format --version

# install conan
RUN python3 -m pip install conan --break-system-packages && \
    conan --version

# by default, anything you run in Docker is done as superuser.
# Conan runs some install commands as superuser, and will prepend `sudo` to
# these commands, unless `CONAN_SYSREQUIRES_SUDO=0` is in your env variables.
ENV CONAN_SYSREQUIRES_SUDO 0

# some packages request that Conan use the system package manager to install
# a few dependencies. This flag allows Conan to proceed with these
# installations;
# leaving this flag undefined can cause some installation failures.
ENV CONAN_SYSREQUIRES_MODE enabled

# old versions of clang (pre 10) will require significant modifications to the
# IWYU section
ARG GCC_VER="14"
# add gcc-${GCC_VER}
RUN apt-get install -y --no-install-recommends gcc-${GCC_VER} g++-${GCC_VER} gdb

# set default gcc
RUN update-alternatives --install /usr/bin/gcc gcc $(which gcc-${GCC_VER}) 100
RUN update-alternatives --install /usr/bin/g++ g++ $(which g++-${GCC_VER}) 100

ARG LLVM_VER="19"
# add clang-${LLVM_VER}
RUN apt-get install -y --no-install-recommends \
        clang-${LLVM_VER} lldb-${LLVM_VER} lld-${LLVM_VER} clangd-${LLVM_VER} \
        llvm-${LLVM_VER}-dev libclang-${LLVM_VER}-dev clang-tidy-${LLVM_VER} \
        clang-format-${LLVM_VER} clang-tools-${LLVM_VER}

# set the default clang-tidy, so CMake can find it
RUN update-alternatives --install /usr/bin/clang-tidy clang-tidy $(which clang-tidy-${LLVM_VER}) 1

# set default clang
RUN update-alternatives --install /usr/bin/clang clang $(which clang-${LLVM_VER}) 100
RUN update-alternatives --install /usr/bin/clang++ clang++ $(which clang++-${LLVM_VER}) 100

# add clang-format
RUN apt-get install -y --no-install-recommends clang-format

# add cmake/ccmake
RUN apt-get install -y --no-install-recommends cmake cmake-curses-gui

# add optional dependecies
RUN apt-get install -y --no-install-recommends \
        doxygen graphviz ccache cppcheck

# add include-what-you-use
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

# https://github.com/include-what-you-use/include-what-you-use#how-to-install:
# You need to copy the Clang include directory to the expected location before
# running (similarly, use include-what-you-use -print-resource-dir to learn
# exactly where IWYU wants the headers).
RUN mkdir -p $(include-what-you-use -print-resource-dir 2>/dev/null)
RUN ln -s $(readlink -f /usr/lib/clang/${LLVM_VER}/include) \
    $(include-what-you-use -print-resource-dir 2>/dev/null)/include

# cleanup cached apt data we don't need anymore
RUN apt-get autoremove -y && apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# allow the user to set compiler defaults
ARG USE_CLANG
# if --build-arg USE_CLANG=1, set CC to 'clang' or set to null otherwise.
ENV CC=${USE_CLANG:+"clang"}
ENV CXX=${USE_CLANG:+"clang++"}
# if CC is null, set it to 'gcc' (or leave as is otherwise).
ENV CC=${CC:-"gcc"}
ENV CXX=${CXX:-"g++"}

# create the user and switch to it
RUN groupadd --gid $USER_GID $USERNAME \
    && useradd --uid $USER_UID --gid $USER_GID -m $USERNAME -s /bin/bash
USER $USERNAME

# add alias for python3
RUN echo "alias python=python3" >> ~/.bashrc

# change default cd directory to app
RUN echo "alias cd='HOME=/workspaces/molviz cd'" >> ~/.bashrc

# add convenient aliases
RUN echo "alias l='ls -a'" >> ~/.bashrc
RUN echo "alias ll='ls -alFh'" >> ~/.bashrc

# add gdb settings
RUN echo "set confirm off" >> ~/.gdbinit

CMD ["/bin/bash"]
