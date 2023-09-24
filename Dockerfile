FROM alpine:latest

RUN apk update && \
    apk add --no-cache g++ make git

RUN git clone https://github.com/Tencent/rapidjson.git && \
    cp -r rapidjson/include/rapidjson /usr/include/

WORKDIR /app

COPY . /app
RUN make -C /app

COPY source.rinha.json /var/rinha/source.rinha.json

ENTRYPOINT ["./interpreter", "/var/rinha/source.rinha.json"]
