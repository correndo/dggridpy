FROM ubuntu:19.04

RUN apt-get update

RUN apt-get -y install python3 python3-dev python3-pip git

RUN apt-get -y install cmake

RUN apt-get -y install libgeos++-dev proj-bin proj-data libproj-dev

WORKDIR /opt

RUN git clone https://github.com/pybind/pybind11.git

WORKDIR /opt/pybind11

RUN python3 setup.py install

WORKDIR /opt/dggridpy

ADD . /opt/dggridpy

RUN pip3 install -r requirements.txt

RUN pip3 install https://github.com/matplotlib/basemap/archive/v1.2.0rel.tar.gz

RUN python3 setup.py install

WORKDIR /opt/dggridpy/notebooks

EXPOSE 8888

ENTRYPOINT jupyter notebook --allow-root --port 8888 --ip=0.0.0.0
