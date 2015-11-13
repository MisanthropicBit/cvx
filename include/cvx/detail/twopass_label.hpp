#ifndef CVX_TWOPASS_LABEL_HPP
#define CVX_TWOPASS_LABEL_HPP

#include "cvx/array_view.hpp"
#include "cvx/connected_component.hpp"
#include "cvx/exception.hpp"
#include "cvx/union_find.hpp"
#include "cvx/utils.hpp"
#include "cvx/detail/extractor.hpp"
#include <algorithm>
#include <iterator>

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
            RandomAccessIterator row = view.begin();

            // Examine the first element separately
            U& e = *row++;

            if (e == background) {
                e = 0;
            } else {
                e = label_count;
                labels.push_back(label_count++);
            }

            // Scan the first line separately to avoid bounds checks in the remaining lines
            for (std::size_t x = 1; x < view.width(); ++x, ++row) {
                U& e = *row;

                if (e == background) {
                    e = 0;
                } else {
                    U d = *(row - 1);

                    if (d != background) {
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
                U& e = *row;

                if (e == background) {
                    e = 0;
                } else {
                    U b = *(row - view.width());

                    if (b != background) {
                        e = b;
                    } else {
                        e = label_count;
                        labels.push_back(label_count++);
                    }
                }

                ++row;

                for (std::size_t x = 1; x < view.width(); ++x, ++row) {
                    U& e = *row;

                    if (e == background) {
                        e = 0;
                    } else {
                        U b = *(row - view.width());

                        if (b != background) {
                            U d = *(row - 1);

                            if (d != background) {
                                e = labels.merge(b, d);
                            } else {
                                e = b;
                            }
                        } else {
                            U d = *(row - 1);

                            if (d != background) {
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
            
            std::vector<connected_component> components;
            components.reserve(labels.label_count());

            // Set labels
            for (size_t i = 0; i < labels.label_count(); ++i) {
                components.emplace_back(i + 1);
            }

            for (auto& ex : extractors) {
                for (auto& cc : components) {
                    ex->initialise(cc);
                }
            }

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
    } // detail
} // cvx

#endif // CVX_TWOPASS_LABEL_HPP
