/*
 * EntityMap.cpp
 */

#include "EntityMap.hpp"

void EntityMap::setMapSize(Dim newMapSize)
{
    mapSize = newMapSize;
    Dim matrixSize(ceil(mapSize.w / optimizationFactor),
                   ceil(mapSize.h / optimizationFactor));
    mat.resize(matrixSize);
}

bool EntityMap::isInsideMap(const EntityAABB &e)
{
    return Rect(Pt(0,0), mapSize).isInside(e.rect);
}

PtI EntityMap::matrixBottomLeft(const Rect &rt)
{
    return PtI( static_cast<int>( floor( rt.pos.x / optimizationFactor ) ) ,
                static_cast<int>( floor( rt.pos.y / optimizationFactor ) ) );
}

PtI EntityMap::matrixTopRight(const Rect &rt)
{
    return PtI( static_cast<int>( floor( (rt.pos.x + rt.sz.w) / optimizationFactor ) ) ,
                static_cast<int>( floor( (rt.pos.y + rt.sz.h) / optimizationFactor ) ) );
}

/*
 * Computes which entities in the map intersect `EntityAABB *e`
 *
 * Returns `false` if there is a collision.
 */
bool EntityMap::computeEntityCollisions(const EntityAABB *e, set<EntityAABB *> &collidingEntities)
{
    if( collidingEntities.size() != 0 )
        throw logic_error("Non-empty EntityCollision collidingEntities set passed to EntityMap::computeEntityCollisions(...)");

    if( !isInsideMap(*e) )
        return false;

    PtI bl = matrixBottomLeft(e->rect), tr = matrixBottomLeft(e->rect);
    for(int x = bl.x ; x < tr.x ; x++)
        for(int y = bl.y ; y < tr.y ; y++)
            if(!mat.at(x,y).empty())
                for(auto m_e : mat.at(x,y)) // iterate through the entities in each non-empty cell
                    if( e->rect.doesIntersect((m_e->rect)) )
                        collidingEntities.insert(m_e);

    if(collidingEntities.empty())
        return true;

    return false;
}

/*
 * Place an entity on the map
 */
bool EntityMap::place(EntityAABB *e,  set<EntityAABB *> &collidingEntities)
{
    if( entities.find(e) != entities.end() )
        throw logic_error("EntityMap::place -- attempt to place an existing entity on the map");

    if( !computeEntityCollisions(e, collidingEntities) )
        return false;

    PtI bl = matrixBottomLeft(e->rect), tr = matrixBottomLeft(e->rect);
    for(int x = bl.x ; x < tr.x ; x++)
        for(int y = bl.y ; y < tr.y ; y++)
            mat.at(x,y).insert(e);

    entities.insert(e);
    return true;
}

/*
 * Remove an entity from the map
 */
void EntityMap::remove(EntityAABB *e)
{
    if( entities.find(e) == entities.end() )
        throw logic_error("EntityMap::remove -- attempt to remove an entity that does not exist on the map");

    PtI bl = matrixBottomLeft(e->rect), tr = matrixBottomLeft(e->rect);
    for(int x = bl.x ; x < tr.x ; x++)
        for(int y = bl.y ; y < tr.y ; y++)
            mat.at(x,y).erase(e);

    entities.erase(e);
}

/*
 * Move an existing entity `e` to a new position
 */
bool EntityMap::move(EntityAABB *e, Pt newPos,  set<EntityAABB *> &collidingEntities)
{
    if(e->rect.pos == newPos)
        return true;

    remove(e);

    Pt oldPos = e->rect.pos;
    e->rect.pos = newPos;
    if( !place(e, collidingEntities) ) {
        e->rect.pos = oldPos;
        set<EntityAABB *> temp;
        if( !place(e, temp) ) // place it back
            throw logic_error("this shouldn't happen");
        return false;
    }

    return true;
}

/*
 * Same as `move` until there is a collision.
 *
 * When there's a collision, `moveBy` moves the entity
 * as close as possible to the colliding entities by
 * reducing by reducing the step at which it moves.
 */
bool EntityMap::moveBy(EntityAABB *e, Pt distance,  set<EntityAABB *> &collidingEntities)
{
    if( !move(e, e->rect.pos + distance, collidingEntities) )
    {
        collidingEntities.clear();

        float divisor = min( abs(distance.x), abs(distance.y) );
        if(divisor == 0) // then get the other value:
            divisor = max( abs(distance.x), abs(distance.y) );

        Pt short_distance =  distance / divisor;
        while( move(e, e->rect.pos + short_distance, collidingEntities) ) {}

        return false;
    }

    return true;
}

void EntityMap::step()
{
    for(auto e : entities)
        e->step();
}
