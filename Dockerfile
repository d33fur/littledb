FROM ubuntu:22.04

RUN apt-get update && \
    apt-get install -y \
    g++ \
    make \
    libssl-dev \
    libjsoncpp-dev \
    curl

RUN curl https://sh.rustup.rs -sSf | sh -s -- -y
ENV PATH=/root/.cargo/bin:$PATH

WORKDIR /app
COPY . .

RUN make all

# RUN systemctl enable littledb && \
#     systemctl start littledb

EXPOSE 6378

CMD ["/usr/local/bin/littledb-service.sh", "start"]
