/*
 * EntityMap.hpp
 *
 *  Created on: 08-Feb-2013
 *      Author: arjun
 */

#include <vector>

#include "BaseFramework.hpp"

#ifndef ENTITYMAP_HPP_
#define ENTITYMAP_HPP_

/*
 * A row-major matrix template.
 */
template<class T>
class matrix
{
    Dim size;
    vector< vector<T> > m;

    void set_size(const Dim size)
    {
        this->size = size;

        if(this->size.h != size.h)
            m.resize(size.h);

        if(this->size.w != size.w)
            for(auto it = m.begin(); it != m.end(); it++)
                it->resize(size.h);
    }
};

class EntityMap : public Entity
{
public:
    EntityMap();
    ~EntityMap();

    void step();
};

#endif /* ENTITYMAP_HPP_ */
