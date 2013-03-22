/*
 * EntityMap.cpp
 */

#include "EntityMap.hpp"

void EntityMap::setMapSize(Dim newMapSize)
{
    mapSize = newMapSize;
    mat.resize( Dim(ceil(mapSize.h) / optimizationFactor,
                    ceil(mapSize.w) / optimizationFactor) );
}

bool EntityMap::isInsideMap(const EntityAABB &e)
{
    return Rect(Pt(0,0), mapSize).isInside(e.rect);
}

/*
 * Computes which entities in the map intersect `EntityAABB *e`
 */
set<EntityAABB *> EntityMap::computeIntersectingEntities(const EntityAABB *e)
{
    set<EntityAABB *> intersectingEntities;

    if( isInsideMap(*e) )
    {
        Pt bottomLeft(e->rect.pos.x / optimizationFactor,
                      e->rect.pos.y / optimizationFactor ),
           topRight( (e->rect.pos.x + e->rect.sz.w) / optimizationFactor ,
                     (e->rect.pos.y + e->rect.sz.h) / optimizationFactor );

        for(int x = static_cast<int>(bottomLeft.x) ; x <= static_cast<int>(topRight.x) ; x++) {
        for(int y = static_cast<int>(bottomLeft.y) ; y <= static_cast<int>(topRight.y) ; y++) {
            if(!mat.at(x,y).empty()) {
                for(auto m_e : mat.at(x,y)) { // iterate through the entities in each non-empty cell
                    if( e->rect.doesIntersect((m_e->rect)) ) {
                        intersectingEntities.insert(m_e);
                    }
                }
            }
        }
        }
    }

    return intersectingEntities;
}

/*
 * Place an entity on the map
 */
void EntityMap::place(EntityAABB *e) // throws `set<EntityAABB *> intersectingEntities` when there is a collision
{
    if( entities.find(e) != entities.end() )
        throw logic_error("EntityMap::place -- attempt to place an existing entity on the map");

    set<EntityAABB *> intersectingEntities = computeIntersectingEntities(e);

    if(!intersectingEntities.empty())
        throw intersectingEntities;

    entities.insert(e);

    Pt bottomLeft( floor( e->rect.pos.x / optimizationFactor ) ,
                   floor( e->rect.pos.y / optimizationFactor ) ),
       topRight( ceil( (e->rect.pos.x + e->rect.sz.w) / optimizationFactor ) ,
                 ceil( (e->rect.pos.y + e->rect.sz.h) / optimizationFactor ) );

    for(int x = (int) bottomLeft.x ; x <= (int) topRight.x ; x++)
        for(int y = (int) bottomLeft.y ; y <= (int) topRight.y ; y++)
            mat.at(x,y).insert(e);
}

/*
 * Remove an entity from the map
 */
void EntityMap::remove(EntityAABB *e)
{
    if( entities.find(e) == entities.end() )
        throw logic_error("EntityMap::place -- attempt to remove an entity that does not exist on the map");

    Pt bottomLeft( floor( e->rect.pos.x / optimizationFactor ) ,
                   floor( e->rect.pos.y / optimizationFactor ) ),
       topRight( ceil( (e->rect.pos.x + e->rect.sz.w) / optimizationFactor ) ,
                 ceil( (e->rect.pos.y + e->rect.sz.h) / optimizationFactor ) );

    for(int x = (int) bottomLeft.x ; x <= (int) topRight.x ; x++)
        for(int y = (int) bottomLeft.y ; y <= (int) topRight.y ; y++)
            mat.at(x,y).erase(e);

    entities.erase(e);
}

/*
 * Move an existing entity `e` to a new position
 */
void EntityMap::move(EntityAABB *e, Pt newPos) // throws `set<EntityAABB *> intersectingEntities` when there is a collision
{
    Pt oldPos = e->rect.pos;
    e->rect.pos = newPos;

    remove(e);

    try {
        place(e);
    }
    catch(set<EntityAABB *> &intersectingEntities) {
        e->rect.pos = oldPos;
        place(e); // place it back
        throw intersectingEntities;
    }
}

void EntityMap::step()
{
    for(auto e : entities)
        e->step();
}
