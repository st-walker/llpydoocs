// Author: Frank Mayet
// Creation Date: 11.03.2024

#include <iostream>
#include <eq_client.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

namespace py = pybind11;

py::dict read(std::string address) {
    py::gil_scoped_release release;

    EqCall eq;      // Equipment function call object
    EqAdr  ea;      // Equipment address object
    EqData src;     // Equipment data object to be sent to server
    EqData dst;     // Equipment data object returned data is stored to

    // Set the address of a DOOCS channel
    ea.adr(address);

    // Make the call
    int return_code = eq.get(&ea, &src, &dst);
    result["address"] = address;
    result["timestamp"]  = std::to_string(dst.time());
    result["macropulse"] = dst.get_event_id().to_int();
    
    if (return_code) {
      result["data"] = "";
      result["macropulse"] = -1;
      result["error_code"] = std::to_string(dst.error());
    }

    switch (dst.type()) {
    case DATA_STRING:
      result["data"] = dst.get_string();
      result["type"] = "STRING";
      break;
    case DATA_INT:
      result["data"] = dst.get_int();
      result["data"] = "INT"
      break;
    case DATA_FLOAT:
      result["data"] = dst.get_float();
      result["type"] = "FLOAT";
      break;
    case DATA_IMAGE:
      parse_image(dst, result);
      break;
    }

    py::gil_scoped_acquire acquire;
    return result;
}

template <typename T> void write(std::string address, T value) {
  gil_scoped_release release;
  EqCall eq;      // Equipment function call object
  EqAdr  ea;      // Equipment address object
  EqData data_in;     // Equipment data object to be sent to server
  EqData data_out;     // Equipment data object returned data is stored to

  // Set the address of a DOOCS channel
  ea.adr(address);

  data_in.set(value);

  // Set the value to be sent to the server
  src.set_int(value);

  // Make the call
  eq.set(&ea, &src, &dst);
  py::gil_scoped_acquire acquire;
}
  

void parse_image(EqData const &dst, py::dict rdict) {
      assert(dst.type() == DATA_IMAGE);s

  IMH header;
  dst.get_image_header(&header);

  size_t size = header.aoi_width * header.aoi_height * header.bpp;
  uint8_t *buffer = new uint8_t[size];

  int size_dummy = 0;
  dst.get_image(&buffer, &size_dummy, &header);

  switch (header.bpp) {
  case 1:
    py::array_t<uint8_t> image({header.aoi_height, header.aoi_width},
			       {header.aoi_width*header.bpp, header.bpp},
			       buffer
			       );
    rdict["data"] = image;
    break;
  case 2:
    py::array_t<uint16_t> image({header.aoi_height, header.aoi_width},
				{header.aoi_width*header.bpp, header.bpp},
				(uint16_t*)buffer
				);
    rdict["data"] = image;
    break;
  case 4:
    py::array_t<uint32_t> image({header.aoi_height, header.aoi_width},
				{header.aoi_width*header.bpp, header.bpp},
				(uint32_t*)buffer
				);
    rdict["data"] = image;
    break;
  default:
    rdict["data"] = "UNSUPPORTED IMAGE FORMAT";
  }
}



PYBIND11_MODULE(lldoocs, m) {
    m.doc() = R"pbdoc(
        LockLess DOOCS 
        -----------------------

        .. currentmodule:: lldoocs

        .. autosummary::
           :toctree: _generate

           read
           write
    )pbdoc";

    m.def("read", &read, "Get image data from a DOOCS server");
    m.def("write", [](const std::string &s, const int &i) {
      return write(s, i);
    });
    m.def("write", [](const std::string &s, const float &i) {
      return write(s, i);
    });

#ifdef VERSION_INFO
      m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}
