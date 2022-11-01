FROM --platform=linux/amd64 ubuntu:22.04 as builder

RUN apt-get update
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y build-essential cmake clang

COPY . /repo
WORKDIR /repo/build
RUN cmake .. -DCMAKE_INSTALL_PREFIX=/install -DAFF3CT_COMPILE_STATIC_LIB=1
RUN make -j8
WORKDIR /repo/mayhem
RUN clang++ /repo/mayhem/fuzz_esn0_to_sigma.cpp -L/repo/build/lib -laff3ct-GIT-NOTFOUND -I/repo/include -o /fuzz -fsanitize=address,fuzzer

FROM ubuntu:22.04 as package
COPY --from=builder /fuzz /fuzz_esn0_to_sigma
