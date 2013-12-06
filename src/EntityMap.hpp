/*
 * EntityMap.hpp
 */

#ifndef ENTITYMAP_HPP_
#define ENTITYMAP_HPP_

/*
 * Entity - a DrawableAABB which has a position on an EntityMap.
 */
class Entity
{
public:
    DrawableAABB *d;
    xy pos;

    Entity(DrawableAABB *d, xy pos) : d(d), pos(pos) {}
    inline const Rect getRect() { return Rect(pos, d->getSize()); }

    virtual ~Entity() {}
};

/*
 * Matrix - a generic matrix class.
 */
template<class T>
class Matrix
{
    xy sz;
    vector< vector<T> > m;

public:
    Matrix() : sz(xy(0,0)) {}

    void resize(const xy &new_size)
    {
        sz = new_size;

        if(sz.x != m.size())
            m.resize( static_cast<unsigned int>(sz.x) );

        for(auto col = m.begin(); col != m.end(); col++)
            if(sz.y != col->size())
                col->resize( static_cast<unsigned int>(sz.y) );
    }

    const xy size() { return size; }

    vector<T>& operator[] (const unsigned int x) { return m.at(x); }
};

class EntityMap
{
public:
    inline EntityMap(xy worldSize, float optimizationFactor) : optmat(worldSize, optimizationFactor) {}
    inline const xy& getMapSize() { return optmat.getMapSize(); }
    inline const set<Entity *> & getEntities() { return entities; }

    bool place(Entity *e, set<Entity *> &collidingEntities);
    void remove(Entity *e);

    bool move(Entity *e, xy newPos,  set<Entity *> &collidingEntities);
    inline bool moveBy(Entity *e, xy distance,  set<Entity *> &collidingEntities) {
        return move(e, e->pos + distance, collidingEntities);
    }

private:
    set<Entity *> entities;

    inline bool isInsideMap(const Entity &e) {
        return Rect(xy(0,0), optmat.getMapSize()).isInside( Rect(e.pos, e.d->getSize()) );
    }

    bool computeEntityCollisions(const Entity *e, set<Entity *> &collidingEntities);

    /*
     * OptimizationMatrix - used to efficiently track neighbouring entities.
     */
    class OptimizationMatrix
    {
    public:
        inline OptimizationMatrix(xy mapSize, const float optimizationFactor) :
        mapSize(mapSize), optimizationFactor(optimizationFactor) { matrix.resize( ceil( mapSize/optimizationFactor ) ); }

        inline void resizeMap(xy newMapSize) { matrix.resize( ceil( (mapSize = newMapSize)/optimizationFactor ) ); }
        inline const xy& getMapSize() { return mapSize; }

        void insert(Entity *e);
        void erase(Entity *e);
        set<Entity *> getEntities(Rect region);

    private:
        Matrix< set<Entity *> > matrix;

        xy mapSize;
        const float optimizationFactor;

        inline xy_int matrixPos(const xy &pos) { return floor_int( pos / optimizationFactor ); }
    }
    optmat;
};

#endif /* ENTITYMAP_HPP_ */
