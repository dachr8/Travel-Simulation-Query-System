FROM ubuntu:18.04
MAINTAINER FredericDT <frederic.dt.twh@gmail.com>

RUN sed -i "s/archive.ubuntu.com/mirrors.byrio.org/g" /etc/apt/sources.list && apt update && apt install -y libboost-system-dev libboost-thread-dev libboost-coroutine-dev libboost-context-dev libssl-dev cmake g++ nginx

COPY ./ /opt/
COPY Travel-Simulation-Query-System/map.txt /opt/Travel-Simulation-Query-System/build/map.txt
COPY FDTsFrontend/pack.html /var/www/html/index.html
COPY default /etc/nginx/sites-available/default

RUN mkdir -p /opt/Travel-Simulation-Query-System/build && cd /opt/Travel-Simulation-Query-System/build && cmake .. && cmake --build .

WORKDIR /opt/Travel-Simulation-Query-System/build

CMD nginx && ./main

EXPOSE 80
