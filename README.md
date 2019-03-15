# dggridpy: Discrete Global Grids for Python


<img src="notebooks/image/ISEA3H.png" width="40%"/> <img src="notebooks/image/ISEA4H.png" width="40%"/>

<img src="notebooks/image/ISEA4D.png" width="40%"/> <img src="notebooks/image/ISEA4T.png" width="40%"/>



Discrete Global Grid Systems ([DGGS](http://www.opengeospatial.org/projects/groups/dggsswg) for short) allow to subdivide the Earth's sphere in equal size regions that can be used to index or bin geospatial data into homogeneous bins.

![image](notebooks/image/earthquackes.png)

`dggridpy` offers a python binding to some of the API implemented in the [DGGRID library][dggrid] and the [dggridR package][dggridr] to transform latitude/longitude coordinates in DGGS cells, and back from DGGS cells into the tiles' shapes to visualise results in a map.

[dggrid]: http://www.discreteglobalgrids.org/software/
[dggridr]: https://github.com/r-barnes/dggridR

## Install
To install the package follow the instructions in [INSTALL.md](INSTALL.md).

## API
The initial set of APIs wrapped in Python are described in [API.md](API.md).

## Credits

The code in the [src](src) directory is the DGGRIDv6.2b developed by [Kevin Sahr](https://discreteglobalgrids.org/) and includes also modifications from [Richard Barnes](https://github.com/r-barnes/dggridR)

## Licensing

This package includes several libraries adopting the following licenses:

 - clipper: The clipper library has been released under the Boostv1 license by Angus Johnson

 - dggrid: Kevin Sahr has released dggrid as a "public domain software program"

 - proj4lib: Gerald Evenden has released this code into the public domain. More recent versions of the library, not used here, are under an MIT-style license.

 - shapelib: Frank Warmerdam has released shapelib under an MIT-style license. There is also an LGPL licensing option which I have chosen not to use.

This package, and all code and documentation not otherwise mentioned above are released under the [MIT license](https://tldrlegal.com/license/mit-license), as stated in the LICENSE file.
