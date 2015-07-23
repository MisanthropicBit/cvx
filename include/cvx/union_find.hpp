#ifndef CVX_UNION_FIND_HPP
#define CVX_UNION_FIND_HPP

#include "cvx/export.hpp"
#include <type_traits>
#include <vector>

namespace cvx {
    //////////////////////////////////////////////////////////////////////
    /// An implementation of a disjoint set data structure for integral
    /// types
    //////////////////////////////////////////////////////////////////////
    template<typename T>
    class CVX_EXPORT union_find final {
        // Ensure that T is an integral type
        static_assert(std::is_integral<T>::value, "T must be an integral type");

        public:
            //////////////////////////////////////////////////////////////////////
            /// Construct an empty unionfind
            //////////////////////////////////////////////////////////////////////
            union_find()
                : _label_count(0) {
                // Labels start at 1
                labels.push_back(0);
            }

            //////////////////////////////////////////////////////////////////////
            /// Construct an empty unionfind with an initial capacity
            //////////////////////////////////////////////////////////////////////
            union_find(std::size_t capacity)
                : _label_count(0) {
                labels.reserve(capacity);
                
                // Labels start at 1
                labels.push_back(0);
            }

            //////////////////////////////////////////////////////////////////////
            /// Returns the root currently pointed to by a label
            ///
            /// \param label Query label
            /// \return Root of the label
            //////////////////////////////////////////////////////////////////////
            T root(T label) const {
                while (labels[label] < label) {
                    label = labels[label];
                }

                return label;
            }

            //////////////////////////////////////////////////////////////////////
            /// Push a new label into the union find
            ///
            /// \param value The value to push
            //////////////////////////////////////////////////////////////////////
            void push_back(const T& value) {
                labels.push_back(value);
            }

            //////////////////////////////////////////////////////////////////////
            /// Compresses the path from a label to its root, and set its root
            ///
            /// \param label Query label
            /// \param root  Root of label to set
            //////////////////////////////////////////////////////////////////////
            void compress(T label, T root) {
                while (labels[label] < label) {
                    T temp = labels[label];
                    labels[label] = root;
                    label = temp;
                }

                labels[label] = root;
            }

            //////////////////////////////////////////////////////////////////////
            /// Merge the paths of label1 and label2
            ///
            /// \param label1 First label
            /// \param label2 Second label
            /// \return The common root of label1 and label2
            //////////////////////////////////////////////////////////////////////
            T merge(T label1, T label2) {
                T root1 = root(label1);

                if (label1 != label2) {
                    T root2 = root(label2);

                    if (root1 > root2) {
                        root1 = root2;
                    }

                    compress(label2, root1);
                }

                compress(label1, root1);
                return root1;
            }

            //////////////////////////////////////////////////////////////////////
            /// Ensure that all labels point to their roots, that roots have
            /// consecutive labels and compute the number of unique labels
            //////////////////////////////////////////////////////////////////////
            void flatten() {
                T k = T(1);

                for (std::size_t i = 1; i < labels.size(); ++i) {
                    if (labels[i] < i) {
                        labels[i] = labels[labels[i]];
                    } else {
                        labels[i] = k++;
                    }
                }

                _label_count = k - 1;
            }

            //////////////////////////////////////////////////////////////////////
            /// \param i The label to query
            /// \return The current root of label i
            //////////////////////////////////////////////////////////////////////
            T get(std::size_t i) const {
                return labels[i];
            }

            //////////////////////////////////////////////////////////////////////
            /// \return True if the union_find is empty
            //////////////////////////////////////////////////////////////////////
            bool empty() const noexcept {
                return labels.size() == 0;
            }

            //////////////////////////////////////////////////////////////////////
            /// \return The size of the union-find
            //////////////////////////////////////////////////////////////////////
            std::size_t size() const {
                return labels.size();
            }

            //////////////////////////////////////////////////////////////////////
            /// \return The number of unique labels
            //////////////////////////////////////////////////////////////////////
            std::size_t label_count() const {
                return _label_count;
            }

        private:
            std::size_t _label_count;
            std::vector<int> labels;
    };
} // cvx

#endif // CVX_UNION_FIND_HPP
