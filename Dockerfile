FROM ubuntu:16.04

RUN apt-get update
# Standard build tools
RUN apt-get -y install g++ gdb make dos2unix git
# Compiler build tools
#RUN apt-get -y install bison flex
# MIPS cross-compiler stuff
RUN apt-get -y install g++-mips-linux-gnu gdb-multiarch
# QEMU run-time emulator
RUN apt-get -y install qemu

RUN apt-get -y install openjdk-8-jre-headless cmake curl uuid-dev pkg-config

RUN cd /usr/local/lib && \
     curl -O https://www.antlr.org/download/antlr-4.9.1-complete.jar && \
     curl -sL https://github.com/antlr/antlr4/archive/4.9.1.tar.gz | tar xz && \
     cd antlr4-4.9.1/runtime/Cpp && \
     mkdir build && mkdir run && cd build && \
     cmake .. && \
     make
    # DESTDIR=/usr/local/lib/antlr4-4.9.1/runtime/Cpp/run make install
    

COPY ./.github/entrypoint.sh /usr/bin/entrypoint.sh

RUN chmod +x /usr/bin/entrypoint.sh

ENTRYPOINT ["/usr/bin/entrypoint.sh"]