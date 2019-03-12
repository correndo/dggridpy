#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

#include <signal.h>
#include <stdio.h>
//#include <unistd.h>

#include <iostream>
using namespace std;

#include "DgParamList.h"
#include "DgGeoSphRF.h"
#include "DgConstants.h"


#include "DgDiscRFS.h"
#include "DgRFBase.h"
#include "DgGeoSphRF.h"
#include "DgEllipsoidRF.h"
//#include "DgIDGG.cpp"
#include "DgLocation.h"
#include "DgIDGG.h"
#include "DgIDGGS.h"
//DGGS integrated

#include "DgIDGGS4H.h"
#include "DgIDGGS4D.h"
#include "DgIDGGS4T.h"
#include "DgIDGGS43H.h"

//wrapper methods
#include "dglib.h"

namespace py = pybind11;

enum TopologyType
{
  HEXAGON  = 0,
  DIAMOND  = 1,
  TRIANGLE = 2
};

std::string TopologyType_str[] = {
  "HEXAGON",
  "DIAMOND",
  "TRIANGLE"
};

enum ProjectionType
{
  ISEA = 0,
  FULLER = 1
};

std::string ProjectionType_str[] = {
  "ISEA",
  "FULLER"
};

struct DGGSParam {

public:
     long double pole_lon_deg;
     long double pole_lat_deg;
     long double azimuth_deg;
     unsigned int aperture;
     int res;
     TopologyType topology;
     ProjectionType projection;

     DGGSParam(
       const long double pole_lon_deg,
       const long double pole_lat_deg,
       const long double azimuth_deg,
       const unsigned int aperture,
       const int res,
       const TopologyType topology,
       const ProjectionType projection)
       : pole_lon_deg(pole_lon_deg),
        pole_lat_deg(pole_lat_deg),
        azimuth_deg(azimuth_deg),
        aperture(aperture),
        res(res),
        topology(topology),
        projection(projection) {}

    //GETTER and SETTERS

    void setPoleLonDeg(const long double d ) { pole_lon_deg = d;}
    long double getPoleLonDeg(){ return pole_lon_deg;}

    void setPoleLatDeg(const long double d ) { pole_lat_deg = d;}
    long double getPoleLatDeg(){ return pole_lat_deg;}

    void setAzimuthDeg(const long double d ) { azimuth_deg = d;}
    long double getAzimuthDeg(){ return azimuth_deg;}

    void setAperture(const unsigned int d ) { aperture = d;}
    unsigned int getAperture(){ return aperture;}

    void setRes(const int d ) { res = d;}
    int getRes(){ return res;}

    void setTopology(const TopologyType t ) { topology = t;}
    TopologyType getTopology(){ return topology;}

    void setProjection(const ProjectionType p ) { projection = p;}
    ProjectionType getProjection(){ return projection;}

    static DGGSParam ISEA3H(){
      return DGGSParam(0.0,0.0,0.0,3,0,HEXAGON,ISEA);
    }

    static DGGSParam ISEA4H(){
      return DGGSParam(0.0,0.0,0.0,4,0,HEXAGON,ISEA);
    }

    static DGGSParam ISEA4T(){
      return DGGSParam(0.0,0.0,0.0,3,0,TRIANGLE,ISEA);
    }

};


long double M_TO_KM = 1.60934;

/**
 * dgetres_res
 * Returns the information for a DGGS of given topology, projection, topology, and resolution.
 *
 */
std::map<std::string,double> dgetres_res (
  const unsigned int aperture,
  const std::string topology,
  const std::string projection,
  const unsigned int res
){
  dglib::GridThing gt(0,0,0,aperture,res,topology,projection);
  std::map<std::string,double> result;
  result.insert(std::pair<std::string,double>("res" , res));
  result.insert(std::pair<std::string,double>("cells" , gt.nCells(res)));
  result.insert(std::pair<std::string,double>("area_km" , gt.cellAreaKM(res)));
  result.insert(std::pair<std::string,double>("spacing_km" , gt.cellDistKM(res)));
  result.insert(std::pair<std::string,double>("cls_km" , gt.cls(res)));
  return result;
};

std::map<uint64_t, py::array_t<double>> SeqNumGrid (
  const long double pole_lon_deg,
  const long double pole_lat_deg,
  const long double azimuth_deg,
  const unsigned int aperture,
  const int res,
  const std::string topology,
  const std::string projection,
  std::vector<uint64_t> cnum
){

  dglib::SeqNumGridGenerator sngg(pole_lon_deg, pole_lat_deg, azimuth_deg, aperture, res, topology, projection, cnum);
  std::map<uint64_t, py::array_t<double>> result;
  std::vector<long double> x;
  std::vector<long double> y;
  while(sngg.good()){
   const auto sn = sngg(x,y);
    double* aresult = new double [x.size() * 2];

    for(int i=0; i<x.size() ; i++){
      aresult[i*2] = x[i];
      aresult[i*2+1] = y[i];
    }
    result[sn] = py::array_t<double>(std::vector<ptrdiff_t>{ static_cast<pybind11::ssize_t>(x.size()) , 2}, &aresult[0]);
  }
  return result;
};


PYBIND11_MODULE(dggridpy, m) {

  m.doc() = "Python wrapping of the DGGRID API for creating and manipulating Discrete Global Grids (http://www.discreteglobalgrids.org/software/). Code modified to be wrapped in R. ";
    py::class_<DGGSParam>(m, "DGGSParam")
      .def(py::init<
         long double ,
         long double ,
         long double ,
         unsigned int ,
         int ,
         TopologyType ,
         ProjectionType
        >(), "Constructor for DGGSParam class")
      .def("setPoleLonDeg" , &DGGSParam::setPoleLonDeg)
      .def("getPoleLonDeg" , &DGGSParam::getPoleLonDeg)
      .def("setPoleLatDeg" , &DGGSParam::setPoleLatDeg)
      .def("getPoleLatDeg" , &DGGSParam::getPoleLatDeg)
      .def("setAzimuthDeg" , &DGGSParam::setAzimuthDeg)
      .def("getAzimuthDeg" , &DGGSParam::getAzimuthDeg)
      .def("setAperture" , &DGGSParam::setAperture)
      .def("getAperture" , &DGGSParam::getAperture)
      .def("setResolution" , &DGGSParam::setRes)
      .def("getResolution" , &DGGSParam::getRes)
      .def("setTopology" , &DGGSParam::setTopology)
      .def("getTopology" , &DGGSParam::getTopology)
      .def("setProjection" , &DGGSParam::setProjection)
      .def("getProjection" , &DGGSParam::getProjection)
      .def("ISEA3H" , &DGGSParam::ISEA3H)
      .def("ISEA4H" , &DGGSParam::ISEA4H)
      .def("ISEA4T" , &DGGSParam::ISEA4T)
      .def("__repr__", [](const DGGSParam &dggs){
          std::ostringstream sink;
          sink << "DGGS:\n\tpole longitude: " << dggs.pole_lon_deg << endl
              <<  "\tpole latitude: " << dggs.pole_lat_deg << endl
              <<  "\tazimuth: " << dggs.azimuth_deg << endl
              <<  "\taperture: " << dggs.aperture << endl
              <<  "\tresolution: " << dggs.res << endl
              <<  "\ttopology: " << TopologyType_str[dggs.topology] << endl
              <<  "\tprojection: " << ProjectionType_str[dggs.projection] << endl;
          return sink.str();
      });

  py::enum_<TopologyType>(m, "TopologyType", py::arithmetic(), "Enumeration of topology types")
          .value("HEXAGON",  HEXAGON,  "HEXAGON topology")
          .value("TRIANGLE", TRIANGLE, "TRIANGLE topology")
          .value("DIAMOND",  DIAMOND,  "DIAMOND topology")
          .export_values();

  py::enum_<ProjectionType>(m, "ProjectionType", py::arithmetic(), "Enumeration of projection types")
                  .value("ISEA",  ISEA,  "ISEA projection")
                  .value("FULLER", FULLER, "FULLER projection")
                  .export_values();

  m.def("dggetres" , [](
    const unsigned int aperture,
    const TopologyType topology,
    const ProjectionType projection,
    const int target_res = 30
  ){
    std::map<int,std::map<std::string,double>> result;
    for (int res =0 ; res<target_res ; res++){
      result.insert(std::pair<int,std::map<std::string,double>>(
        res,
        dgetres_res(aperture,TopologyType_str[topology],ProjectionType_str[projection],res))
      );
    }
    return result;
  } , "Returns a dictionary with the properties of the input DGGS for the first 'target_res' resolution levels organised by resolution level. \nThe information returned are:\n\t'ncells' number of cells;\n\t'area_km' area of each cell in this resolution expressed in squared kilometers;\n\t'spacing_km' the spacing between the center of adjacent cells expressed in kilometers;\n\t'cls' the diameter of a spherical cap of the same area as a cell of the specified resolution in the input dggs.",
  py::arg("aperture"),
  py::arg("topology"),
  py::arg("projection"),
  py::arg("target_res"));

  m.def("dgconstruct" , [](
    const unsigned int aperture,
    const TopologyType topology,
    const ProjectionType projection,
    const long double pole_lon_deg,
    const long double pole_lat_deg,
    const long double azimuth_deg,
    const long double area ,
    const long double spacing ,
    const long double cls ,
    const std::string resround ,
    const bool metric
  ){
     if ((area + spacing + cls) == 0 ||
         (area*spacing != 0) ||
         (spacing*cls != 0) ||
         (cls*area != 0)){
       throw std::invalid_argument("No criteria (or multiple criteria) specified to construct dggs");
     } else if (resround != "nearest" && resround != "down"&& resround != "up") {
       throw std::invalid_argument("Invalid resolution criteria used");
     }
     else {
       std::string key = area >0 ? "area_km" : (spacing > 0 ? "spacing_km" : "cls_km");
       long double value;
       if (key == "area_km"){
         value = metric ? area : area*M_TO_KM*M_TO_KM;
       } else if (key == "spacing_km"){
         value = metric ? spacing : spacing*M_TO_KM;
       } else if (key == "cls_km"){
         value = metric ? cls : cls*M_TO_KM;
       }

       //look for the wanted DGGS
       std::map<int,double> indexes;
       for (int res =0 ; res<30 ; res++){
         auto cvalue = dgetres_res(aperture,TopologyType_str[topology],ProjectionType_str[projection],res)[key];
         indexes.insert(std::pair<int,double>( res , cvalue));
       }
       int down_index , up_index , index;
       long double down_diff , up_diff;

       for(auto element : indexes){
         up_index = element.first;
         if (element.second <= value){
           if (element.second == value){
             up_index   = element.first;
             down_index = element.first;
           } else {
             up_index = element.first-1;
             down_index = element.first;
           }
           up_diff = indexes[up_index]-value;
           down_diff = value - indexes[down_index];
           break;
         }
       }
       if (resround == "down"){
         index = down_index;
       } else if (resround == "up"){
         index = up_index;
       } else {
         index = up_diff<down_diff ? up_index : down_index;
       }
       return DGGSParam(pole_lon_deg,
                        pole_lat_deg,
                        azimuth_deg,
                        aperture,
                        index,
                        topology,
                        projection);
     }
  } , "Construct a discrete global grid system (dggs) using the input parameters as constraints.",
  py::arg("aperture") = 3,
  py::arg("topology") = HEXAGON ,
  py::arg("projection") = ISEA,
  py::arg("pole_lon_deg") = 0.0,
  py::arg("pole_lat_deg") = 0.0,
  py::arg("azimuth_deg")  = 0.0,
  py::arg("area") = 0.0,
  py::arg("spacing") = 0.0,
  py::arg("cls") = 0.0,
  py::arg("resround") = "nearest",
  py::arg("metric")   = true  );

  m.def("GEO_to_Q2DI" , [](
    DGGSParam dggs,
    std::vector<long double> in_lon_deg,
    std::vector<long double> in_lat_deg
  ){
      if (in_lon_deg.size() != in_lat_deg.size())
        throw std::runtime_error("Input coordinates collections' size must match");

      dglib::Transformer dgt(
        dggs.pole_lon_deg,
        dggs.pole_lat_deg,
        dggs.azimuth_deg,
        dggs.aperture,
        dggs.res,
        TopologyType_str[dggs.topology],
        ProjectionType_str[dggs.projection]);

      std::vector< std::vector< long int > >  result (in_lon_deg.size(), std::vector< long int >(3));

      for(unsigned int i=0 ; i< in_lon_deg.size() ; i++){
           const long double tin_lon_deg = in_lon_deg[i];
           const long double tin_lat_deg = in_lat_deg[i];

           long double tout_i = result[0][i];
           long double tout_j = result[1][i];
           uint64_t tout_quad = result[2][i];
           auto in = dgt.inGEO(tin_lon_deg, tin_lat_deg);
           dgt.outQ2DI(in, tout_quad, tout_i, tout_j);
           result[i][0] = tout_i;
           result[i][1] = tout_j;
           result[i][2] = tout_quad;
       }
      return result;
  } , "Uses a discrete global grid system to convert between GEO and Q2DI",
  py::arg("dggs"),
  py::arg("in_lon_deg"),
  py::arg("in_lat_deg"));


  m.def("GEO_to_SEQNUM" , [](
    DGGSParam dggs,
    std::vector<long double> in_lon_deg,
    std::vector<long double> in_lat_deg
    ){
       dglib::Transformer dgt(
         dggs.pole_lon_deg,
         dggs.pole_lat_deg,
         dggs.azimuth_deg,
         dggs.aperture,
         dggs.res,
         TopologyType_str[dggs.topology],
         ProjectionType_str[dggs.projection]);

       std::vector<long double>  out_seqnum (in_lon_deg.size());
       for(unsigned int i=0 ; i< in_lon_deg.size() ; i++){
            const long double tin_lon_deg = in_lon_deg[i];
            const long double tin_lat_deg = in_lat_deg[i];
            uint64_t tout_seqnum = out_seqnum[i];
            auto in = dgt.inGEO(tin_lon_deg, tin_lat_deg);
            dgt.outSEQNUM(in, tout_seqnum);
            out_seqnum[i] = tout_seqnum;
        }
      return out_seqnum;
  } , "This function uses a discrete global grid system to convert between (longitude,latitude) to sequence numbers.",
  py::arg("dggs"),
  py::arg("in_lon_deg"),
  py::arg("in_lat_deg"));

  m.def("SEQNUM_to_GEO" , [](
    DGGSParam dggs,
    std::vector<long double> in_seqnum
    ){
       dglib::Transformer dgt(dggs.pole_lon_deg,
                              dggs.pole_lat_deg,
                              dggs.azimuth_deg,
                              dggs.aperture,
                              dggs.res,
                              TopologyType_str[dggs.topology],
                              ProjectionType_str[dggs.projection]);

       long double*  out_deg = new long double[in_seqnum.size()*2];
       long double* out_lon_deg = new long double[in_seqnum.size()];
       long double* out_lat_deg = new long double[in_seqnum.size()];

       for(unsigned int i=0;i<in_seqnum.size();i++){
         const uint64_t tin_seqnum = in_seqnum[i];
         long double tout_lon_deg = out_lon_deg[i];
         long double tout_lat_deg = out_lat_deg[i];
         auto in = dgt.inSEQNUM(tin_seqnum);
         dgt.outGEO(in, tout_lon_deg, tout_lat_deg);
         out_deg[i*2] = tout_lon_deg;
         out_deg[i*2 + 1] = tout_lat_deg;
       }
       return py::array_t<long double>(std::vector<ptrdiff_t>{ static_cast<pybind11::ssize_t>(in_seqnum.size()) , 2}, out_deg);
    } , "This function uses a discrete global grid system to convert between a list of sequence numbers to (longitude,latitude).\nThe returned object is an array of shape len(in_seqnum)X2.",
    py::arg("dggs"),
    py::arg("in_seqnum"));


  m.def("SeqNumGrid" , [](
    DGGSParam dggs,
    uint64_t cnum
  ){
    std::vector<uint64_t> cseqnums = { cnum };
    return SeqNumGrid( dggs.pole_lon_deg,
                       dggs.pole_lat_deg,
                       dggs.azimuth_deg,
                       dggs.aperture,
                       dggs.res,
                       TopologyType_str[dggs.topology],
                       ProjectionType_str[dggs.projection],
                      cseqnums);
   } , "This function returns a dictionary where for each sequence number in input (key) an array of coordinates is associated (value)",
   py::arg("dggs"),
   py::arg("cnum"));

   m.def("SeqNumGrid" , [](
       DGGSParam dggs,
       std::vector<uint64_t> cseqnums
   ){
     return SeqNumGrid( dggs.pole_lon_deg,
                        dggs.pole_lat_deg,
                        dggs.azimuth_deg,
                        dggs.aperture,
                        dggs.res,
                        TopologyType_str[dggs.topology],
                        ProjectionType_str[dggs.projection],
                        cseqnums);
    }, "This function returns a dictionary where for each sequence number in input (key) an array of coordinates is associated (value)",
    py::arg("dggs"),
    py::arg("cseqnums"));


  //Low level API
  py::bind_vector<std::vector<DgAddressBase*>>(m, "VectorDgAddressBase");

  py::class_<DgIDGG>(m, "DgIDGG")
    .def("convert",
        [](DgIDGG &idgg, DgLocation *location) {
              return idgg.convert(location);
        }
      , "Converts a location within the current dggs.",
      py::arg("location")
    ).def("getVertices",
        [](DgIDGG &idgg, const DgLocation& loc, int densify) {
            DgPolygon vertices;
            idgg.setVertices(loc, vertices, densify);
            auto geoRF = idgg.geoRF();
            auto addresses = vertices.addressVec();
            double* aresult = new double [vertices.size() * 2];
            int index = 0;
            for (std::vector<DgAddressBase*>::iterator it = addresses.begin() ; it != addresses.end(); ++it){
              auto va = geoRF.getVecAddress(**it);
              aresult[index++] = va.x();
              aresult[index++] = va.y();
            }
            return py::array_t<double>(std::vector<ptrdiff_t>{ vertices.size() , 2}, &aresult[0]);
        }
    ).def("getVertices",
        [](DgIDGG &idgg, const uint64_t sn, int densify) {
            DgLocation *loc = idgg.bndRF().locFromSeqNum(sn);
            DgPolygon vertices;
            idgg.setVertices(*loc, vertices, densify);
            auto geoRF = idgg.geoRF();
            auto addresses = vertices.addressVec();
            double* aresult = new double [vertices.size() * 2];
            int index = 0;
            for (std::vector<DgAddressBase*>::iterator it = addresses.begin() ; it != addresses.end(); ++it){
              auto va = geoRF.getVecAddress(**it);
              aresult[index++] = va.x();
              aresult[index++] = va.y();
            }
            return py::array_t<double>(std::vector<ptrdiff_t>{ vertices.size() , 2}, &aresult[0]);
        }
    );

  py::class_<DgIDGGS4H> iddgs4h(m, "DgIDGGS4H");
  iddgs4h.def(py::init<
              DgRFNetwork & ,
              const DgGeoSphRF & ,
              const DgGeoCoord & ,
              double ,
              int ,
              const std::string &,
              const std::string &
          >() , "Constructor for the ISEA4H DGGS" ,
          py::arg("networkIn"),
          py::arg("backFrameIn"),
          py::arg("vert0"),
          py::arg("azDegs"),
          py::arg("nResIn") = 1,
          py::arg("nameIn") = "ISEA4H",
          py::arg("projType") = "ISEA" );
    iddgs4h.def("frequency" , &DgIDGGS4H::frequency);
    iddgs4h.def("idgg" , &DgIDGGS4H::idgg);

    py::class_<DgIDGGS4D> iddgs4d(m, "DgIDGGS4D");
    iddgs4d.def(py::init<
                DgRFNetwork & ,
                const DgGeoSphRF & ,
                const DgGeoCoord & ,
                double ,
                int ,
                const std::string &,
                const std::string &
            >() , "Constructor for the DgIDGGS4D DGGS" ,
            py::arg("networkIn"),
            py::arg("backFrameIn"),
            py::arg("vert0"),
            py::arg("azDegs"),
            py::arg("nResIn") = 1,
            py::arg("nameIn") = "ISEA4D",
            py::arg("projType") = "ISEA" );
      iddgs4d.def("idgg" , &DgIDGGS4D::idgg);

    py::class_<DgIDGGS4T> iddgs4t(m, "DgIDGGS4T");
    iddgs4t.def(py::init<
                  DgRFNetwork & ,
                  const DgGeoSphRF & ,
                  const DgGeoCoord & ,
                  double ,
                  int ,
                  const std::string &,
                  const std::string &
              >() , "Constructor for the DgIDGGS4T DGGS" ,
              py::arg("networkIn"),
              py::arg("backFrameIn"),
              py::arg("vert0"),
              py::arg("azDegs"),
              py::arg("nResIn") = 1,
              py::arg("nameIn") = "ISEA4T",
              py::arg("projType") = "ISEA" );
        iddgs4t.def("idgg" , &DgIDGGS4T::idgg);

    py::class_<DgIDGGS43H> iddgs43h(m, "DgIDGGS43H");
        iddgs43h.def(py::init<
                      DgRFNetwork & ,
                      const DgGeoSphRF & ,
                      const DgGeoCoord & ,
                      double ,
                      int ,
                      const std::string &,
                      const std::string &
                  >() , "Constructor for the DgIDGGS43H DGGS" ,
                  py::arg("networkIn"),
                  py::arg("backFrameIn"),
                  py::arg("vert0"),
                  py::arg("azDegs"),
                  py::arg("nResIn") = 1,
                  py::arg("nameIn") = "ISEA43H",
                  py::arg("projType") = "ISEA" );
        iddgs43h.def("idgg" , &DgIDGGS43H::idgg);


  py::class_<DgGeoSphRF>(m, "DgGeoSphRF")
    .def(py::init<DgRFNetwork & , const std::string & , double>() , "Constructor for the DgGeoSphRF class (network, name, radius)",
          py::arg("networkIn"),
          py::arg("nameIn")="GeodeticSph" ,
          py::arg("earthRadiusKMin")=DEFAULT_RADIUS_KM)
    .def("makeLocation", &DgGeoSphRF::makeLocation)
    .def("getAddress",
        [](DgGeoSphRF gshprf, DgLocation loc) {
            return gshprf.getAddress(loc);
        }
     );

  py::class_<DgRFNetwork>(m, "DgRFNetwork")
    .def(py::init<int>(), "Constructor for the DgRFNetwork class (chunksize)",
            py::arg("chunkSize")=10)
    .def(py::init<const DgRFNetwork &>() , "Copy constructor for the DgRFNetwork class")
    .def("size" , &DgRFNetwork::size)
    .def("update" , &DgRFNetwork::update)
    .def("capacity" , &DgRFNetwork::capacity);

	py::class_<DgGeoCoord>(m, "DgGeoCoord")
		.def(py::init<>(), "Empty constructor for the DgGeoCoord class")
		.def(py::init<const DgGeoCoord &>() , "Copy constructor for the DgGeoCoord class")
		.def(py::init<double , double, bool >() , "Constructor for the DgGeoCoord class using coordinated (radians or non radians)");


  py::class_<DgLocation>(m, "DgLocation")
      .def(py::init<>(), "Constructor")
      .def("isUndefined" , &DgLocation::isUndefined )
      .def("asAddressString" , [](const DgLocation &l) {
          return l.asAddressString();
      })
      .def("__repr__",
          [](const DgLocation &l) {
              return l.asString();
          }
      );


	py::class_<DgStringParam>(m, "DgStringParam")
		.def(py::init<const std::string &>() , "Basic constructor for DgStringParam using parameter name as input")
		.def(py::init<const std::string & , const std::string &>(), "Basic constructor for DgStringParam using parameter name and value as input")
		.def("__str__" , &DgStringParam::valToStr)
		.def("valToStr" , &DgStringParam::valToStr)
		.def("strToVal" , &DgStringParam::strToVal);


	py::class_<DgBoolParam>(m, "DgBoolParam")
		.def(py::init<const std::string &>(), "Basic constructor for DgBoolParam using parameter name as input")
		.def(py::init<const std::string & , bool, bool>(), "Basic constructor for DgBoolParam using parameter name and value as input")
		.def("__str__" , &DgBoolParam::valToStr)
		.def("valToStr" , &DgBoolParam::valToStr)
		.def("strToVal" , &DgBoolParam::strToVal)
		.def("validate" , &DgBoolParam::validate);

    py::class_<DgParamList>(m, "DgParamList")
    	.def(py::init<>(), "Empty constructor for the DgParamList class")
    	.def("loadParams" , &DgParamList::loadParams)
    	.def("setParam" , &DgParamList::setParam)
    	.def("insertParam" ,
    		[](DgParamList &pl , DgStringParam &sp) {
			   pl.insertParam(static_cast<DgAssoc*>(&sp));
           })
    	.def("insertParam" ,
    		[](DgParamList &pl , DgBoolParam &bp) {
			   pl.insertParam(static_cast<DgAssoc*>(&bp));
           });

}
