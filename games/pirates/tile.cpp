// Tile
// A Tile in the game that makes up the 2D map grid.

// DO NOT MODIFY THIS FILE
// Never try to directly create an instance of this class, or modify its member variables.
// Instead, you should only be reading its variables and calling its functions.



#include "tile.hpp"
#include "../../joueur/src/base_ai.hpp"
#include "../../joueur/src/any.hpp"
#include "../../joueur/src/exceptions.hpp"
#include "../../joueur/src/delta.hpp"
#include "game_object.hpp"
#include "player.hpp"
#include "port.hpp"
#include "tile.hpp"
#include "unit.hpp"
#include "impl/pirates.hpp"

#include <type_traits>

namespace cpp_client
{

namespace pirates
{

const std::vector<std::string> Tile_::directions({"North", "East", "South", "West"});

/// <summary>
/// Gets the neighbors of this Tile
/// </summary>
/// <returns>The neighboring (adjacent) Tiles to this tile</returns>
std::vector<Tile> Tile_::get_neighbors()
{
    std::vector<Tile> neighbors;

    if(tile_north)
    {
        neighbors.push_back(tile_north);
    }

    if(tile_east)
    {
        neighbors.push_back(tile_east);
    }

    if(tile_south)
    {
        neighbors.push_back(tile_south);
    }

    if(tile_west)
    {
        neighbors.push_back(tile_west);
    }

    return neighbors;
}

/// <summary>
/// Checks if a Tile is pathable to units
/// </summary>
/// <param name="unit">the Unit that is checking</param>
/// <return>true if pathable, false otherwise</return>
bool Tile_::is_pathable(const Unit& unit)
{
    // <<-- Creer-Merge: is_pathable_builtin -->> - Code you add between this comment and the end comment will be preserved between Creer re-runs.
    
    // Don't try to path through units
    if (this->unit != nullptr) 
    {
        return false;
    }

    // Ships can only be on water
    if (unit->ship_health > 0) 
    {
        return type == "water" && (!port || (port->owner == unit->owner));
    }

    // Crew that aren't on ships can only be on land or ports owned by their owner
    return type == "land" || (port != nullptr && port->owner == unit->owner);
    
    // <<-- /Creer-Merge: is_pathable_builtin -->>
}

/// <summary>
/// Checks if this Tile has a specific neighboring Tile
/// </summary>
/// <param name="tile">Tile to check against</param>
/// <return>if the tile is a neighbor of this Tile, false otherwise</return>
bool Tile_::has_neighbor(const Tile& tile)
{
    return (tile && (tile_north == tile || tile_east == tile || tile_south == tile || tile_west == tile));
}

} // pirates

} // cpp_client
