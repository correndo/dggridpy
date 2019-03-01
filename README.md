# dggridpy: Discrete Global Grids for Python


<img src="notebooks/image/ISEA3H.png" width="400"/> <img src="notebooks/image/ISEA4H.png" width="400"/>

<img src="notebooks/image/ISEA4D.png" width="400"/> <img src="notebooks/image/ISEA4T.png" width="400"/>



Discrete Global Grid Systems ([DGGS](http://www.opengeospatial.org/projects/groups/dggsswg) for short) allow to subdivide the Earth's sphere in equal size regions that can be used to index or bin geospatial data into homogeneous bins.

`dggridpy` offers a python binding to some of the API implemented in the [DGGRID library][dggrid] and the [dggridR package][dggridr] to transform latitude/longitude coordinates in DGGS cells, and back from DGGS cells into the tiles' shapes to visualise results in a map.

[dggrid]: http://www.discreteglobalgrids.org/software/
[dggridr]: https://github.com/r-barnes/dggridR
