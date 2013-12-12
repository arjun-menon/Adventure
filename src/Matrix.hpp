/*
 * Matrix.hpp
 *
 *  Created on: Dec 12, 2013
 *      Author: arjun
 */

#ifndef MATRIX_HPP_
#define MATRIX_HPP_

/*
 * Matrix - a generic matrix class with an iterator.
 */
template<typename T>
class Matrix
{
    vector< vector<T> > m;
    xy m_size;

public:
    Matrix() : m_size(xy(0,0)) {}

    void resize(const xy &new_size)
    {
        m_size = new_size;

        if(m_size.x != m.size())
            m.resize( static_cast<unsigned int>(m_size.x) );

        for(auto row = m.begin(); row != m.end(); row++)
            if(m_size.y != row->size())
                row->resize( static_cast<unsigned int>(m_size.y) );
    }
    void resize(const unsigned int n, const unsigned int m) {
        resize(xy(n, m));
    }

    const xy size() const {
        return m_size;
    }

    vector<T>& operator[] (const unsigned int x) { return m[x]; }
    vector<T>& at(const unsigned int x) { return m.at(x); }

    T& at(const unsigned int x, const unsigned int y) { return m.at(x).at(y); }
    T at(const unsigned int x, const unsigned int y) const { return m.at(x).at(y); }

    class iterator
    {
    public:
        Matrix &matrix;
        unsigned int i, j;

        iterator(Matrix &matrix, unsigned int i, unsigned int j) : matrix(matrix), i(i), j(j) {}

        iterator(const iterator& it) : matrix(it.matrix), i(it.i), j(it.j) {}

        bool operator==(const iterator& rhs) const {
            return i == rhs.i && j == rhs.j;
        }
        bool operator!=(const iterator& rhs) const {
            return i != rhs.i || j != rhs.j;
        }

        void operator++() {
            j++;
            if(j == matrix.m_size.y) {
                i++;
                j = 0;
            }
        }
        void operator++(int) { operator++(); }

        T& operator*() {
            return matrix.at(i).at(j);
        }
    };

    iterator begin() {
        return iterator(*this, 0, 0);
    }

    iterator end() {
        return iterator(*this, m_size.x, 0);
    }
};

template <typename T>
inline std::ostream& operator<<(std::ostream &strm, const Matrix<T> &matrix) {
    xy sz = matrix.size();
    strm << "[";
    for(int i = 0; i < sz.x; i++) {
        if(i != 0)
            strm << " ";
        strm << "[";
        for(int j = 0; j < sz.y; j++) {
            int val = matrix.at(i, j);
            strm << val;
            if(j != (sz.y - 1))
                strm << ", ";
        }
        strm << "]";
        if(i != (sz.x - 1))
            strm<<"\n";
    }
    strm<<"]\n";
    return strm;
}

template<typename T>
class SubMatrixRef
{
public:
    Matrix<T> &matrix;
    const Rect rect;

    SubMatrixRef(Matrix<T> &matrix, Rect rect) :
        matrix(matrix), rect(rect) {}

    class iterator
    {
    public:
        SubMatrixRef &smr;
        unsigned int i, j;
        const unsigned int max_row_len;

        iterator(SubMatrixRef &smr, unsigned int i, unsigned int j) : smr(smr),
                i(i), j(j), max_row_len(smr.rect.pos.y + smr.rect.size.y) {}

        iterator(const iterator& it) : smr(it.smr), i(it.i), j(it.j), max_row_len(it.max_row_len) {}

        bool operator==(const iterator& rhs) const {
            return i == rhs.i && j == rhs.j;
        }
        bool operator!=(const iterator& rhs) const {
            return i != rhs.i || j != rhs.j;
        }

        void operator++() {
            j++;
            if(j == max_row_len) {
                i++;
                j = smr.rect.pos.y;
            }
        }
        void operator++(int) { operator++(); }

        T& operator*() {
            return smr.matrix.at(i).at(j);
        }
    };

    iterator begin() {
        return iterator(*this, rect.pos.x, rect.pos.y);
    }

    iterator end() {
        return iterator(*this, rect.pos.x + rect.size.x, rect.pos.y);
    }
};

template <typename T>
inline std::ostream& operator<<(std::ostream &strm, const SubMatrixRef<T> &smr) {
    xy start = smr.rect.pos, end = smr.rect.pos + smr.rect.size;
    strm << "[";
    for(int i = start.x; i < end.x; i++) {
        if(i != start.x)
            strm << " ";
        strm << "[";
        for(int j = start.y; j < end.y; j++) {
            int val = smr.matrix.at(i, j);
            strm << val;
            if(j != (end.y - 1))
                strm << ", ";
        }
        strm << "]";
        if(i != (end.x - 1))
            strm<<"\n";
    }
    strm<<"]\n";
    return strm;
}

#endif /* MATRIX_HPP_ */
