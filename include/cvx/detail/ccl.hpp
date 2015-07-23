#ifndef CVX_CCL_DETAIL_HPP
#define CVX_CCL_DETAIL_HPP

#include "cvx/array_view.hpp"
#include "cvx/connected_component.hpp"
#include "cvx/exception.hpp"
#include "cvx/union_find.hpp"
#include "cvx/utils.hpp"
#include "cvx/detail/contour.hpp"
#include "cvx/detail/extractor.hpp"
#include <algorithm>
#include <iterator>
#include <type_traits>

#include <iostream>

namespace cvx {
    template<typename Iterator>
    using iterator_value_type = typename std::iterator_traits<Iterator>::value_type;

    namespace detail {
        //////////////////////////////////////////////////////////////////////
        /// Scan labels using 4-connectivity
        ///
        /// \param view       A view of some image data
        /// \param labels     Label equivalences
        /// \param background Value of background elements
        //////////////////////////////////////////////////////////////////////
        template<typename RandomAccessIterator>
        void scan_labels4(array_view<RandomAccessIterator>& view,
                          union_find<iterator_value_type<RandomAccessIterator>>& labels,
                          iterator_value_type<RandomAccessIterator> background) {
            using U = iterator_value_type<RandomAccessIterator>;
            U label_count = 1;

            //////////////////////////////////////////////////////////////////////
            // NOTE: We do some loop unrolling below which increases the
            //       complexity of the code, but removes a lot of unnessary
            //       boundary checks.
            //////////////////////////////////////////////////////////////////////
            // Examine the first element separately
            U& e = view(0, 0);

            if (e == background) {
                e = 0;
            } else {
                e = label_count;
                labels.push_back(label_count++);
            }

            // Scan the first line separately to avoid bounds checks in the remaining lines
            for (std::size_t x = 1; x < view.width(); ++x) {
                U& e = view(0, x);

                if (e == background) {
                    e = 0;
                } else {
                    U d = view(0, x - 1);

                    if (d) {
                        e = d;
                    } else {
                        e = label_count;
                        labels.push_back(label_count++);
                    }
                }
            }

            // Scan the rest of the lines
            for (std::size_t y = 1; y < view.height(); ++y) {
                // Check the left-most element of each row manually to reduce total boundary checks
                U& e = view(y, 0);

                if (e == background) {
                    e = 0;
                } else {
                    U b = view(y - 1, 0);

                    if (b) {
                        e = b;
                    } else {
                        e = label_count;
                        labels.push_back(label_count++);
                    }
                }

                for (std::size_t x = 1; x < view.width(); ++x) {
                    U& e = view(y, x);

                    if (e == background) {
                        e = 0;
                    } else {
                        U b = view(y - 1, x);

                        if (b) {
                            U d = view(y, x - 1);

                            if (d) {
                                e = labels.merge(b, d);
                            } else {
                                e = b;
                            }
                        } else {
                            U d = view(y, x - 1);

                            if (d) {
                                e = d;
                            } else {
                                e = label_count;
                                labels.push_back(label_count++);
                            }
                        }
                    }
                }
            }
        }

        //////////////////////////////////////////////////////////////////////
        /// Scan labels using 8-connectivity
        ///
        /// \param view       A view of some image data
        /// \param labels     Label equivalences
        /// \param background Value of background elements
        //////////////////////////////////////////////////////////////////////
        template<typename RandomAccessIterator>
        void scan_labels8(array_view<RandomAccessIterator>& view,
                          union_find<iterator_value_type<RandomAccessIterator>>& labels,
                          iterator_value_type<RandomAccessIterator> background) {
            using U = iterator_value_type<RandomAccessIterator>;
            U label_count = 1;

            //////////////////////////////////////////////////////////////////////
            // NOTE: We do some loop unrolling below which increases the
            //       complexity of the code, but removes a lot of boundary checks
            //////////////////////////////////////////////////////////////////////
            // Examine the first element separately
            U& e = view(0, 0);

            if (e == background) {
                e = 0;
            } else {
                e = label_count;
                labels.push_back(label_count++);
            }

            // Scan the first line separately to avoid bounds checks in the remaining lines
            for (std::size_t x = 1; x < view.width(); ++x) {
                U& e = view(0, x);

                if (e == background) {
                    e = 0;
                } else {
                    U d = view(0, x - 1);

                    if (d) {
                        e = d;
                    } else {
                        e = label_count;
                        labels.push_back(label_count++);
                    }
                }
            }

            for (std::size_t y = 1; y < view.height(); ++y) {
                // Check the left-most element of each row manually to reduce total boundary checks
                U& e = view(y, 0);

                if (e == background) {
                    e = 0;
                } else {
                    U b = view(y - 1, 0);
                    
                    if (b) {
                        e = b;
                    } else {
                        U c = view(y - 1, 1);

                        if (c) {
                            e = c;
                        } else {
                            e = label_count;
                            labels.push_back(label_count++);
                        }
                    }
                }
                
                for (std::size_t x = 1; x < view.width() - 1; ++x) {
                    U& e = view(y, x);

                    if (e == background) {
                        e = 0;
                    } else {
                        U b = view(y - 1, x);

                        if (b) {
                            e = b;
                        } else {
                            U c = view(y - 1, x + 1);
                            U a = view(y - 1, x - 1);

                            if (c) {
                                if (a) {
                                    e = labels.merge(a, c);
                                } else {
                                    U d = view(y, x - 1);

                                    if (d) {
                                        e = labels.merge(c, d);
                                    } else {
                                        e = c;
                                    }
                                }
                            } else {
                                if (a) {
                                    e = a;
                                } else {
                                    U d = view(y, x - 1);

                                    if (d) {
                                        e = d;
                                    } else {
                                        e = label_count;
                                        labels.push_back(label_count++);
                                    }
                                }
                            }
                        }
                    }
                }

                // Check the right-most element of each row manually to reduce total boundary checks
                // (Note: Not necessary for 4-connectivity)
                std::size_t x = view.width() - 1;
                U& f = view(y, x);
                U b = view(y - 1, x);

                if (f == background) {
                    f = 0;
                } else {
                    if (b) {
                        f = b;
                    } else {
                        U a = view(y - 1, x - 1);

                        if (a) {
                            f = a;
                        } else {
                            U d = view(y, x - 1);

                            if (d) {
                                f = d;
                            } else {
                                f = label_count;
                                labels.push_back(label_count++);
                            }
                        }
                    }
                }
            }
        }

        //////////////////////////////////////////////////////////////////////
        /// Relabel all connected components from a set of label equivalences
        ///
        /// \param view   A view of some image data
        /// \param labels Flattened label equivalences
        /// \param background Value of background elements
        //////////////////////////////////////////////////////////////////////
        template<typename RandomAccessIterator>
        void relabel(array_view<RandomAccessIterator>& view,
                     const union_find<iterator_value_type<RandomAccessIterator>>& labels,
                     iterator_value_type<RandomAccessIterator> background) {
            if (!view.valid() || labels.empty()) {
                throw exception("No data");
            }

            for (auto& p : view) {
                if (p) {
                    p = labels.get(p);
                }
            }
        }

        //////////////////////////////////////////////////////////////////////
        /// Relabel all connected components from a set of label equivalences
        ///
        /// \param view       A view of some image data
        /// \param labels     Flattened label equivalences
        /// \param extractors Vector of feature extractors
        /// \param components Vector of connected components
        /// \param background Value of background elements
        //////////////////////////////////////////////////////////////////////
        template<typename RandomAccessIterator, typename Container>
        void relabel(array_view<RandomAccessIterator>& view,
                     const union_find<iterator_value_type<RandomAccessIterator>>& labels,
                     iterator_value_type<RandomAccessIterator> background,
                     const Container& extractors,
                     std::vector<connected_component>& components) {
            using T = iterator_value_type<RandomAccessIterator>;

            if (!view.valid()) {
                throw exception("View is empty");
            }

            for (std::size_t y = 0; y < view.height(); ++y) {
                for (std::size_t x = 0; x < view.width(); ++x) {
                    T& e = view(y, x);

                    if (e) {
                        e = labels.get(e);

                        for (auto& ex : extractors) {
                            ex->update(x, y, components[e - 1]);
                        }
                    }
                }
            }
        }

        //////////////////////////////////////////////////////////////////////
        ///
        //////////////////////////////////////////////////////////////////////
        template<typename RandomAccessIterator>
        std::size_t two_pass_label(array_view<RandomAccessIterator>& view,
                                   unsigned char connectivity,
                                   iterator_value_type<RandomAccessIterator> background) { 
            using T = iterator_value_type<RandomAccessIterator>;
            union_find<T> labels;

            // 1. Do initial scan of connected components
            if (connectivity == 4) {
                scan_labels4(view, labels, background);
            } else {
                scan_labels8(view, labels, background);
            }

            // 2. Compress all labels so they point to their root
            labels.flatten();

            // 3. Relabel all connected components with final labels
            relabel(view, labels, background);

            return labels.label_count();
        }
        
        //////////////////////////////////////////////////////////////////////
        ///
        //////////////////////////////////////////////////////////////////////
        template<typename RandomAccessIterator,
                 typename OutputIterator>
        std::size_t two_pass_label(array_view<RandomAccessIterator>& view,
                                   OutputIterator out,
                                   unsigned char connectivity,
                                   iterator_value_type<RandomAccessIterator> background,
                                   const feature_flag& flags) { 
            std::vector<std::shared_ptr<extractor>> extractors;
            make_extractors_from_flags(flags,
                                       std::back_inserter(extractors));

            std::cout << "BEG: " << extractors.size() << std::endl;

            for (auto& ex : extractors) {
                ex->initialise();
            }

            using T = iterator_value_type<RandomAccessIterator>;
            union_find<T> labels;

            // 1. Do initial scan of connected components
            if (connectivity == 4) {
                scan_labels4(view, labels, background);
            } else {
                scan_labels8(view, labels, background);
            }

            // 2. Compress all labels so they point to their root
            labels.flatten();
            
            std::vector<connected_component> components(labels.label_count());

            // 3. Relabel all connected components with final labels
            relabel(view, labels, background, extractors, components);

            for (auto& ex : extractors) {
                for (auto& cc : components) {
                    ex->finalise(cc);
                }
            }

            std::move(components.begin(),
                      components.end(),
                      out);

            return labels.label_count();
        }

        //////////////////////////////////////////////////////////////////////
        ///
        //////////////////////////////////////////////////////////////////////
        template<typename RandomAccessIterator,
                 typename OutputIterator>
        std::size_t label_connected_components_view(array_view<RandomAccessIterator>& view,
                                                    OutputIterator out,
                                                    unsigned char connectivity,
                                                    iterator_value_type<RandomAccessIterator> foreground,
                                                    iterator_value_type<RandomAccessIterator> background,
                                                    const feature_flag& flags) {
            std::size_t label_count = 0;

            if (any_flags(flags & feature_flag::all_contours) > 0) {
                // If contours need to be extracted, call the labelling algorithm
                // that labels by finding contours
                label_count = contour_label(view,
                                            out,
                                            connectivity,
                                            foreground,
                                            background,
                                            flags);
            } else {
                label_count = two_pass_label(view,
                                             out,
                                             connectivity,
                                             background,
                                             flags);
            }

            return label_count;
        }

        //////////////////////////////////////////////////////////////////////
        ///
        //////////////////////////////////////////////////////////////////////
        template<typename T>
        void validate_arguments(unsigned char connectivity,
                                const T& foreground,
                                const T& background) {
            if (connectivity != 4 && connectivity != 8) {
                throw exception("Connectivity must be 4 or 8");
            }

            if (foreground == background) {
                throw exception("Foreground and background values cannot be identical");
            }
        }

        //////////////////////////////////////////////////////////////////////
        /// Implementation of the connected component algorithm based on:
        /// "Optimizing two-pass connected-component labeling algorithms" by
        /// Kesheng Wu, Ekow Otoo and Kenji Suzuki. Does not extract components
        ///
        /// \param first              Iterator to the beginning of the image
        ///                           data source
        /// \param last               Iterator to the end of the image data
        ///                           source
        /// \param first2             Iterator to the beginning of the image
        ///                           data target
        /// \param last2              Iterator to the end of the image data
        ///                           target
        /// \param width              Width of the image data
        /// \param height             Height of the image data
        /// \param pitch              Pitch of the image data
        /// \param connectivity       Neighbourhood connectivity (4 or 8)
        /// \param pred               Unary predicate that identifies a
        ///                           background element
        /// \return The number of connected components found
        //////////////////////////////////////////////////////////////////////
        template<typename RandomAccessIterator>
        std::size_t label_connected_components(RandomAccessIterator first,
                                               RandomAccessIterator last,
                                               std::size_t width,
                                               std::size_t height,
                                               std::size_t pitch,
                                               unsigned char connectivity,
                                               iterator_value_type<RandomAccessIterator> foreground,
                                               iterator_value_type<RandomAccessIterator> background) {
            validate_arguments(connectivity, foreground, background);

            array_view<RandomAccessIterator> view(first,
                                                  last,
                                                  width,
                                                  height,
                                                  pitch);

            return two_pass_label(view, connectivity, background);
        }

        //////////////////////////////////////////////////////////////////////
        /// Implementation of the connected component algorithm based on:
        /// "Optimizing two-pass connected-component labeling algorithms" by
        /// Kesheng Wu, Ekow Otoo and Kenji Suzuki
        ///
        /// \param first              Iterator to the beginning of the image
        ///                           data source
        /// \param last               Iterator to the end of the image data
        ///                           source
        /// \param out                Output iterator for storing connected
        ///                           components, e.g. a std::vector<>
        /// \param width              Width of the image data
        /// \param height             Height of the image data
        /// \param pitch              Pitch of the image data
        /// \param connectivity       Neighbourhood connectivity (4 or 8)
        /// \param foreground         Value of foreground elements
        /// \param background         Value of background elements
        /// \param flags              Bitflag of the component features to
        ///                           extract
        /// \return The number of connected components found
        //////////////////////////////////////////////////////////////////////
        template<typename RandomAccessIterator,
                 typename OutputIterator>
        std::size_t label_connected_components(RandomAccessIterator first,
                                               RandomAccessIterator last,
                                               OutputIterator out,
                                               std::size_t width,
                                               std::size_t height,
                                               std::size_t pitch,
                                               unsigned char connectivity,
                                               iterator_value_type<RandomAccessIterator> foreground,
                                               iterator_value_type<RandomAccessIterator> background,
                                               const feature_flag& flags = feature_flag::none) {
            // TODO: Necessary?
            //if (first == last) {
            //    return 0;
            //}

            if (any_flags(flags)) {
                validate_arguments(connectivity, foreground, background);

                array_view<RandomAccessIterator> view(first,
                                                      last,
                                                      width,
                                                      height,
                                                      pitch);

                return label_connected_components_view(view,
                                                       out,
                                                       connectivity,
                                                       foreground,
                                                       background,
                                                       flags);
            }

            // If no features need to be extracted, call the function
            // that does no extraction instead
            return label_connected_components(first,
                                              last,
                                              width,
                                              height,
                                              pitch,
                                              connectivity,
                                              foreground,
                                              background);
        }
    } // detail
} // cvx

#endif // CVX_CCL_DETAIL_HPP
