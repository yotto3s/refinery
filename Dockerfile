# Multi-stage build for refinery — C++26 refinement types library
#
# Usage:
#   docker build -t refinery .
#   docker build -t refinery --build-arg GCC_COMMIT=abc123 .
#   docker build -t refinery --build-arg GCC_JOBS=4 .
#   docker build -t refinery --build-arg REFINERY_BUILD_EXAMPLES=ON .
#   docker run refinery

# ---------- Stage 1: Build GCC with reflection support ----------
FROM ubuntu:24.04 AS gcc-builder

ARG GCC_REPO=https://github.com/gcc-mirror/gcc.git
ARG GCC_BRANCH=master
ARG GCC_COMMIT=
ARG GCC_JOBS=

RUN apt-get update && apt-get install -y --no-install-recommends \
        build-essential git flex bison texinfo \
        libgmp-dev libmpfr-dev libmpc-dev libisl-dev zlib1g-dev \
        ca-certificates \
    && rm -rf /var/lib/apt/lists/*

COPY scripts/build_gcc.sh /tmp/build_gcc.sh

RUN bash /tmp/build_gcc.sh \
        --gcc-repo "$GCC_REPO" \
        --gcc-branch "$GCC_BRANCH" \
        ${GCC_COMMIT:+--gcc-commit "$GCC_COMMIT"} \
        ${GCC_JOBS:+--jobs "$GCC_JOBS"} \
        --install-prefix /opt/gcc \
    && rm -rf /opt/gcc-build /opt/gcc-src

# ---------- Stage 2: Build and test refinery ----------
FROM ubuntu:24.04 AS refinery

RUN apt-get update && apt-get install -y --no-install-recommends \
        make git ca-certificates gpg wget \
        libc6-dev libgmp10 libmpfr6 libmpc3 libisl23 zlib1g \
        binutils \
    && wget -qO- https://apt.kitware.com/keys/kitware-archive-latest.asc \
        | gpg --dearmor -o /usr/share/keyrings/kitware-archive-keyring.gpg \
    && echo "deb [signed-by=/usr/share/keyrings/kitware-archive-keyring.gpg] https://apt.kitware.com/ubuntu/ noble main" \
        > /etc/apt/sources.list.d/kitware.list \
    && apt-get update && apt-get install -y --no-install-recommends cmake \
    && rm -rf /var/lib/apt/lists/*

COPY --from=gcc-builder /opt/gcc /opt/gcc
ENV PATH="/opt/gcc/bin:$PATH"
ENV LD_LIBRARY_PATH="/opt/gcc/lib64"

ARG REFINERY_BUILD_EXAMPLES=OFF

WORKDIR /refinery
COPY . .

RUN cmake -B build \
        -DCMAKE_CXX_COMPILER=/opt/gcc/bin/g++ \
        -DREFINERY_BUILD_EXAMPLES=${REFINERY_BUILD_EXAMPLES} \
    && cmake --build build \
    && ctest --test-dir build --output-on-failure

# If examples are enabled, run asm-compare
RUN if [ "${REFINERY_BUILD_EXAMPLES}" = "ON" ]; then \
        cmake --build build --target asm-compare; \
    fi

CMD ["ctest", "--test-dir", "build", "--output-on-failure"]
