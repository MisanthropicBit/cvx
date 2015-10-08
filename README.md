cvx
===

[![Build Status](https://travis-ci.org/MisanthropicBit/cvx.svg?branch=master)](https://travis-ci.org/MisanthropicBit/cvx)

**This project is currently under development**

``cvx`` is a C++ library that aims to provide a generic, STL-like, and image-agnostic interface to fast **c**onnected **c**omponent **l**abelling (CCL) and feature extraction.

Due to its STL-like interface, ``cvx`` works with any image data that can be described by random access iterators (see the ``examples/`` folder).

### Feature Extraction
At the moment, ``cvx`` supports 4- and 8-connectedness and can currently extract the following component features:

* Areas
* Centroids
* Points
* Bounding boxes
* Extents

``cvx`` will support the following features in the future:

* Convex hulls (including their areas and additional properties)
* Outer contours
* Inner contours
* Ellipses
* Circularity

any hopefully many more...

### Building ``cvx``

Run the following:

```
mkdir cxv
cd cvx
git clone https://github.com/MisanthropicBit/cvx.git
mkdir build
cd build
cmake <options> ..
```

``cvx`` can be built as a static or shared library. Below is a list of all the available options:

* **``CVX_STATIC_LIBRARY``**: Build ``cvx`` as a static library
* **``CVX_SHARED_LIBRARY``**: Build ``cvx`` as a shared library (default)
* **``CVX_BUILD_EXAMPLES``**: Build all examples
* **``CVX_BUILD_TESTS``**   : Build all tests
* **``CVX_GEN_DOCS``**      : Build all documentation locally
* **``CVX_WITH_OPENCV``**   : Also build examples that require OpenCV, and add display support to ``cvx``
