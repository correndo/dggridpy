# dggridpy API

**dggridpy** exposes to Python the high level APIs of the [dggridR](https://github.com/r-barnes/dggridR) R package (Richard Barnes, 2016) and some of the more low level DGGRID C++ functionalities (Kevin Sahr, 2018). For both set of APIs an effort has been made to not change the naming of classes and functions/methods in order to make the adoption of the python APIs as straightforward as possible.


## High level APIs
The functionalities exposed from the **dggridR** package, and subsequently wrapped up in Python, are intended for the conversion of latitude/longitude coordinates into DGGS' cells and back, and to the generation of DGGS cells' polygons for visualization.

### DGGSParam
Parameters for the creation of a new DGGS.
 - longitude of the pole in degrees
 - latitude of the pole in degrees
 - azimuth in degrees
 - aperture of the DGGS
 - resolution of the DGGS (coarser resolution is `0`)
 - topology type, one of:
  -  `TopologyType.HEXAGON`
  -  `TopologyType.DIAMOND`
  -  `TopologyType.TRIANGLE`
 - projection type, one of:
  -  `ProjectionType.ISEA`  
  -  `ProjectionType.FULLER`  

An initial set of DGGS are already included as factory methods of the DGGSParam class:
 - ISEA3H
 - ISEA4H
 - ISEA4T

The parameters of the DGGSParam created can then be set using the available setters/getters for the class.

### dggetres

Returns a dictionary with the properties (dictionary values) of the input DGGS for the first 'target_res' resolution levels organised by resolution level (dictionary key).

The information returned are:
 - `ncells` number of cells;
 - `area_km` area of each cell in this resolution expressed in squared kilometers;
 - `spacing_km` the spacing between the center of adjacent cells expressed in kilometers;
 - `cls` the diameter of a spherical cap of the same area as a cell of the specified resolution in the input dggs.

```
import dggridpy as dg
dg.dggetres(3,dg.TopologyType.HEXAGON, dgProjectionType.ISEA, 3)
{
  0: {
    'area_km': 51006562.172408864,
    'cells': 12.0,
    'cls_km': 8199.50037010203,
    'res': 0.0,
    'spacing_km': 7053.652431410832
  },
  1: {
    'area_km': 17002187.390802953,
    'cells': 32.0,
    'cls_km': 4678.96987172968,
    'res': 1.0,
    'spacing_km': 4072.4281300451025
  },
  2: {
    'area_km': 5667395.796934318,
    'cells': 92.0,
    'cls_km': 2691.2520709128753,
    'res': 2.0,
    'spacing_km': 2351.217477136944
  }
}
```

### dgconstruct

Construct a discrete global grid system (dggs) using the input parameters as constraints. The input parameters are:

 - `aperture` of the DGGS
 - `topology` of the DGGS (`HEXAGON` by default)
 - `projection` of the DGGS (`ISEA` by default)
 - `pole_lon_deg` default is 0.0
 - `pole_lat_deg` default is 0.0
 - `azimuth_deg` default is 0.0
 - `area` desired area of the DGGS' cells
 - `spacing` desired spacing between the center of adjacent cells
 - `cls` desired characteristic length scale (CLS) of the cells (e.g. the diameter of a spherical cap of the same area as a cell of the specified resolution)
 - `resround` direction of DGGS search; one of 'nearest', 'up', or 'down'
 - `metric` input are to be considered as 'metric' or 'imperial'

```
dgconstruct(aperture: int = 3,
            topology: dggridpy.TopologyType = TopologyType.HEXAGON,
            projection: dggridpy.ProjectionType = ProjectionType.ISEA,
            pole_lon_deg: float = 0.0,
            pole_lat_deg: float = 0.0,
            azimuth_deg: float = 0.0,
            area: float = 0.0,
            spacing: float = 0.0,
            cls: float = 0.0,
            resround: str = 'nearest',
            metric: bool = True) -> dggridpy.DGGSParam
```

Example
 ```
dg.dgconstruct( 3,
                dg.TopologyType.HEXAGON ,
                dg.ProjectionType.ISEA,
                area = 500000,
                resround='nearest')
DGGS:
        pole longitude: 0
        pole latitude: 0
        azimuth: 0
        aperture: 3
        resolution: 4
        topology: HEXAGON
        projection: ISEA
dg.dggetres( 3,
             dg.TopologyType.HEXAGON,
             dg.ProjectionType.ISEA,
             4 )
{
  ...
    3: {
      'area_km': 1889131.9323114392,
      'cells': 272.0,
      'cls_km': 1551.8675487722837,
      'res': 3.0,
      'spacing_km': 1357.4760433483675
    },
    4: {
      'area_km': 629710.6441038131,
      'cells': 812.0,
      'cls_km': 895.6018416483499,
      'res': 4.0,
      'spacing_km': 783.7391590456482
    },
    5: {
      'area_km': 209903.54803460438,
      'cells': 2432.0,
      'cls_km': 517.0049969030842,
      'res': 5.0,
      'spacing_km': 452.49201444945595
    }
}
```

### GEO_to_Q2DI
Uses a DGGS to convert between GEO (latitude,longitude) and Q2DI (quad number and (i, j) coordinates on that quad).
```
GEO_to_Q2DI( dggs: dggridpy.DGGSParam,
             in_lon_deg: List[float],
             in_lat_deg: List[float]) -> List[List[int]]
```

### GEO_to_SEQNUM

This function uses a discrete global grid system to convert between (longitude, latitude) to sequence numbers (linear address 1:size-of-DGG).

```
GEO_to_SEQNUM( dggs: dggridpy.DGGSParam,
               in_lon_deg: List[float],
               in_lat_deg: List[float]) -> List[float]
```

Example (see included [notebook](notebooks) `Use case of DGGS for spatial statistics (Earthquackes frequency).ipynb`.


> Richard Barnes (2016). dggridR: Discrete Global Grids for R. https://github.com/r-barnes/dggridR.

> Kevin Sahr  (2018). DGGRID version 6.4: User documentation for discrete global grid software. http://webpages.sou.edu/~sahrk/docs/dggridManualV64.pdf.
