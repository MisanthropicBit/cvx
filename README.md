cvx: A Generic Connected Component Labelling Library
====================================================

**This project is currently under development**

``cvx`` is a C++ library that aims to provide a generic, STL-like, and image-agnostic interface to fast **c**onnected **c**omponent **l**abelling (CCL) and feature extraction.

Due to its STL-like interface using iterators, ``cvx`` works with any image data that can be described by iterators (see the ``examples/`` folder).

### Feature Extraction
At the moment, ``cvx`` can currently extract the following component features:

* Area
* Centroid
* Points
* Bounding box
* Extent
* Convex hull
* Outer contours
* Inner contours
