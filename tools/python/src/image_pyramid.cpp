// Copyright (C) 2015 Davis E. King (davis@dlib.net)
// License: Boost Software License   See LICENSE.txt for the full license.

#include <dlib/python.h>
#include <pybind11/stl_bind.h>
#include "opaque_types.h"
#include <dlib/image_transforms/image_pyramid.h>

using namespace dlib;
using namespace std;

namespace py = pybind11;


// ----------------------------------------------------------------------------------------

struct tiled_image_pyramid_details
{
    std::vector<rectangle> rects;
    long pyramid_image_nr;
    long pyramid_image_nc;
};

tiled_image_pyramid_details compute_tiled_image_pyramid_details (
    const unsigned long nr,
    const unsigned long nc,
    const unsigned long padding,
    const unsigned long outer_padding
)
{
    pyramid_down<5> pyr;
    tiled_image_pyramid_details result;
    impl::compute_tiled_image_pyramid_details(pyr, nr, nc, padding, outer_padding, result.rects,
                                              result.pyramid_image_nr, result.pyramid_image_nc);
    return result;
}

// ----------------------------------------------------------------------------------------

void bind_image_pyramid(py::module& m)
{
    {
        typedef tiled_image_pyramid_details type;
        py::class_<type>(m, "tiled_image_pyramid_details")
            .def_readwrite("rects", &type::rects)
            .def_readwrite("pyramid_image_nr", &type::pyramid_image_nr)
            .def_readwrite("pyramid_image_nc", &type::pyramid_image_nc);
    }
    m.def("compute_tiled_image_pyramid_details",
        [](const unsigned long nr, const unsigned long nc, const unsigned long padding, const unsigned long outer_padding) { return compute_tiled_image_pyramid_details(nr, nc, padding, outer_padding); },
        py::arg("nr"),
        py::arg("nc"),
        py::arg("padding")=10,
        py::arg("outer_padding")=0,
"requires \n\
    - nr > 0 \n\
    - nc > 0 \n\
    - padding >= 0 \n\
    - outer_padding >= 0 \n\
ensures \n\
    - Computes a tiled image pyramid details from the input image dimensions. The pyramid is made using \n\
      scale factor 4/5.  The highest resolution image is the input image and then all further \n\
      pyramid levels are generated from downsampling via scale factor 4/5."
    /*!
        requires
            - nr > 0
            - nc > 0
            - padding >= 0
            - outer_padding >= 0
        ensures
            - Computes a tiled image pyramid details from the input image dimensions. The pyramid is made using
              scale factor 4/5.  The highest resolution image is the input image and then all further
              pyramid levels are generated from downsampling via scale factor 4/5.
    !*/
    );
    m.def("image_to_tiled_pyramid",
          [](const std::vector<rectangle> &rects, double scale, drectangle r) { return image_to_tiled_pyramid<pyramid_down<5>>(rects, scale, r); },
          py::arg("rects"),
          py::arg("scale"),
          py::arg("r"),
          "requires \n\
      - 0 < scale <= 1 \n\
      - rects.size() > 0"
        /*!
            requires
                - 0 < scale <= 1
                - rects.size() > 0
        !*/
    );
    m.def("tiled_pyramid_to_image",
          [](const std::vector<rectangle> &rects, drectangle r) { return tiled_pyramid_to_image<pyramid_down<5>>(rects, r); },
          py::arg("rects"),
          py::arg("r"),
          "requires \n\
        - rects.size() > 0"
        /*!
            requires
                - rects.size() > 0
        !*/
    );
}

// ----------------------------------------------------------------------------------------
