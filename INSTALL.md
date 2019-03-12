# Building and installing dggridpy

**IMPORTANT** this software is still in developing stage therefore it may not install in all python systems. The software has been successfully tested on:
 - Windows (on Python v3.7.1 installed from https://www.python.org/)
 - Linux (Ubuntu subsystem for Windows, on Python v3.6.7 installed from apt)
 - Docker (image ubuntu:19.04 and apt Python3 packages)  

There are two ways to install *dggridpy* into your system: [compiling](#compile-dggridpy) and installing the software, or install a [precompiled](#install-precompiled) package for your system.  

## Compile dggridpy
In order to compile and install **dggridpy** from source, [pybind11](https://github.com/pybind/pybind11) and a compliant C++ compiler is required. Download the latest version of pybind11 (version 2.2.4 was used) and then edit the ``CMakeLists.txt`` file to point to the folder where pybind11 have been downloaded.

```
add_subdirectory(../pybind11 build)
```

Once the file ``CMakeLists.txt`` has been updated, create the build folder on the root of the project and compile and install the C++ sources:
 - ``python setup.py build`` to build the package
 - ``python setup.py install`` to build and install the package

## Install precompiled
Some precompiled python [wheels](https://www.python.org/dev/peps/pep-0427/) are provided for Windows and Linux OS under the */dist* folder. To install the dggridpy choose the right wheel and invoke the pip command:

```
pip install dggridpy-0.0.1-{python version}-{OS}.whl
```
