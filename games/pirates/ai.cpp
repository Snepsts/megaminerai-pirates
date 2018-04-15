// AI
// This is where you build your AI

#include "ai.hpp"

#include<map>
#include<string>
// <<-- Creer-Merge: includes -->> - Code you add between this comment and the end comment will be preserved between Creer re-runs.
// You can add #includes here for your AI.
// <<-- /Creer-Merge: includes -->>


namespace cpp_client
{

namespace pirates
{

bool is_ship(Unit u);

/// <summary>
/// This returns your AI's name to the game server.
/// Replace the string name.
/// </summary>
/// <returns>The name of your AI.</returns>
std::string AI::get_name() const
{
    // <<-- Creer-Merge: get-name -->> - Code you add between this comment and the end comment will be preserved between Creer re-runs.
    // REPLACE WITH YOUR TEAM NAME!
    return "Arrays Start at Zero";
    // <<-- /Creer-Merge: get-name -->>
}

/// <summary>
/// This is automatically called when the game first starts, once the game objects are created
/// </summary>
void AI::start()
{
    // <<-- Creer-Merge: start -->> - Code you add between this comment and the end comment will be preserved between Creer re-runs.
    // This is a good place to initialize any variables
    // <<-- /Creer-Merge: start -->>
}

/// <summary>
/// This is automatically called the game (or anything in it) updates
/// </summary>
void AI::game_updated()
{
    // <<-- Creer-Merge: game-updated -->> - Code you add between this comment and the end comment will be preserved between Creer re-runs.
    // If a function you call triggers an update this will be called before it returns.
    // <<-- /Creer-Merge: game-updated -->>
}

/// <summary>
/// This is automatically called when the game ends.
/// </summary>
/// <param name="won">true if you won, false otherwise</param>
/// <param name="reason">An explanation for why you either won or lost</param>
void AI::ended(bool won, const std::string& reason)
{
    //<<-- Creer-Merge: ended -->> - Code you add between this comment and the end comment will be preserved between Creer re-runs.
    // You can do any cleanup of your AI here.  The program ends when this function returns.
    //<<-- /Creer-Merge: ended -->>
}

void AI::spawner()
{
    std::vector<Unit> ships;
    std::vector<Unit> crew;
    for(auto u : this->player->units)
    {
        if(u->ship_health > 0)
            ships.push_back(u);
        else
            crew.push_back(u);
    }
    if(crew.size() == 0)
    {
        this->player->port->spawn("crew");
    }
    else if(ships.size() == 0)
    {
        this->player->port->spawn("ship");
    }
    else
    {
        float ratio = crew.size() / ships.size();
        if(ratio < 3)
            this->player->port->spawn("crew");
        else
            this->player->port->spawn("ship");
    }
}

std::vector<Unit> AI::get_enemy_crew()
{
    std::vector<Unit> crew;
    for(auto u : this->player->opponent->units)
    {
        if(u->ship_health == 0)
            crew.push_back(u);
    }
    return crew;
}

std::vector<Unit> AI::get_enemy_ships()
{
    std::vector<Unit> ships;
    for(auto u : this->player->opponent->units)
    {
        if(u->ship_health != 0)
            ships.push_back(u);
    }
    return ships;
}

void AI::get_action()
{
    for(auto u : this->player->units)
    {

    }
}

bool AI::is_ship(Unit u)
{
    return u->ship_health > 0;
}

bool AI::deposit_treasure_in_home(Unit u)
{
    auto path = this->find_path(u->tile, this->player->port->tile, u);
    if(path.size() == 0)
    {
        u->deposit();
        return u->gold == 0;
    } else {
        u->move(path[0]);
        return false;
    }
}

bool AI::attack_enemy_ship(Unit u)
{
    auto enemy_ships = get_enemy_ships();
    if(enemy_ships.size() > 0)
    {
        Unit closest_ship = enemy_ships[0];
        size_t distance = this->find_path(u->tile, enemy_ships[0]->tile, u).size();
        for(auto es : enemy_ships)
        {
            auto current_distance = this->find_path(u->tile, es->tile, u).size();
            if(current_distance < distance)
            {
                closest_ship = es;
                distance = current_distance;
            }
        }

        auto path = this->find_path(u->tile, closest_ship->tile, u);
        if(path.size() <= 3)
        {
            u->attack(closest_ship->tile, "ship");
            return closest_ship->ship_health == 0;
        }
        else{
            
        }
    }
}

/// <summary>
/// This is called every time it is this AI.player's turn.
/// </summary>
/// <returns>Represents if you want to end your turn. True means end your turn, False means to keep your turn going and re-call this function.</returns>
bool AI::run_turn()
{
    spawner();

    auto my_units = player->units;

    for (auto unit : my_units) {
        if (is_ship(unit)) {
            run_ship_turn(unit);
        } else {
            run_crew_turn(unit);
        }
    }
    
    return true;
}

bool AI::run_ship_turn(Unit u)
{
    return true;
}

bool AI::run_crew_turn(Unit u)
{
    return true;
}
//action functions
bool AI::steal_enemy_treasure(Unit un)
//Trys to steal enemy treasure by moving to it and digging
//will return false in any case that is not the unit digging treasure
{
    //check if possible
    Tile closest_enemy_treasure = get_closest_enemy_treasure(un);
    if(closest_enemy_treasure == NULL) 
    {
        return false; //no treasure was found
    }
    if(un->tile == closest_enemy_treasure)
    {
        if(!un->acted)
        {
            un->dig(-1); //already on tile, DIG
        }
        else
        {
            return false; //on tile but couldn't steal
        }
    }
    else
    {
        if(move_towards_enemy_treasure(un))
        {
            if(!un->acted)
            {
                un->dig(-1);
                return true;
            }
            else
            {
                return false; //on tile but out of energy
            }
        }
    }
    return false; //error
}
//helper functions
std::vector<Tile> AI::build_list_of_enemy_treasure()
{
    bool ours = false;
    std::vector<Tile> enemy_treasure_tiles;
    std::vector<Tile> our_treasure_tiles;
    for(Tile tile : this->game->tiles)
    {
        if(tile->gold > 0)
        {
            for(Tile ourTreasure : our_treasure_tiles)
            {
                if(ourTreasure == tile)
                {
                    ours = true;
                    break;
                }
            }
            if(!ours)
            {
                enemy_treasure_tiles.push_back(tile);
            }
            ours = false;
        }
    }
    return enemy_treasure_tiles;
}

Tile AI::get_closest_enemy_treasure(Unit un)
{
    std::vector<Tile> enemy_treasures = build_list_of_enemy_treasure();
    unsigned largest = 999;
    Tile closest_tile = NULL;
    if(enemy_treasures.empty())
    {
        return closest_tile;
    }
    for(Tile enemy_treasure_tile : enemy_treasures)
    {
        if(un->tile == enemy_treasure_tile)
        {
            return un->tile; //your dumb
        }
        std::vector<Tile> test_path = this->find_path(un->tile, enemy_treasure_tile, un);
        if(test_path.size() != 0 && test_path.size() < largest)
        {
            largest = test_path.size();
            closest_tile = enemy_treasure_tile;
        }
    }
    if(closest_tile == NULL)
    {
        return NULL;
    }
    return closest_tile;
}

bool AI::move_towards_enemy_treasure(Unit un)
{
    Tile closest_tile = get_closest_enemy_treasure(un);
    if(closest_tile == NULL)
        return false; // no treasure
    std::vector<Tile> path = this->find_path(un->tile, closest_tile, un);
    while(un->moves > 0)
    {
        if(path.empty())
            return true; //reached the tile
        un->move(path[0]);
        path.erase(path.begin());
    }
    return false;
}

bool AI::move_to_tile(Unit u, Tile t)
{
    while(u->moves > 0)
    {
        auto path = this->find_path(u->tile, t, u);
        if(path.size() > 0)
            u->move(path[0]);
    }
}

/// A very basic path finding algorithm (Breadth First Search) that when given a starting Tile, will return a valid path to the goal Tile.
/// <param name="start">the starting Tile</param>
/// <param name="goal">the goal Tile</param>
/// <param name="unit">the Unit that will move</param>
/// <return>A List of Tiles representing the path, the the first element being a valid adjacent Tile to the start, and the last element being the goal. Or an empty list if no path found.</return>
std::vector<Tile> AI::find_path(const Tile& start, const Tile& goal, const Unit& unit)
{
    // no need to make a path to here...
    if(start == goal)
    {
        return {};
    }

    // the tiles that will have their neighbors searched for 'goal'
    std::queue<Tile> fringe;

    // How we got to each tile that went into the fringe.
    std::unordered_map<Tile,Tile> came_from;

    // Enqueue start as the first tile to have its neighbors searched.
    fringe.push(start);

    // keep exploring neighbors of neighbors... until there are no more.
    while(fringe.size() > 0)
    {
        // the tile we are currently exploring.
        Tile inspect = fringe.front();
        fringe.pop();

        // Note, we are using the `auto` keyword here
        //   The compiler can discern that this is a `std::vector<Tile>` based on the return type of `getNeighbors()`
        auto neighbors = inspect->get_neighbors();

        // cycle through the tile's neighbors.
        for(unsigned i = 0; i < neighbors.size(); i++)
        {
            Tile neighbor = neighbors[i];

            // If we found the goal we've found the path!
            if(neighbor == goal)
            {
                // Follow the path backward starting at the goal and return it.
                std::deque<Tile> path;
                path.push_front(goal);

                // Starting at the tile we are currently at, insert them retracing our steps till we get to the starting tile
                for(Tile step = inspect; step != start; step = came_from[step])
                {
                    path.push_front(step);
                }

                // we want to return a vector as that's what we use for all containers in C++
                // (and they don't have push_front like we need)
                // So construct the vector-ized path here
                std::vector<Tile> vector_path;
                for(auto& tile : path)
                {
                    vector_path.push_back(tile);
                }
                return vector_path;
            }

            // if the tile exists, has not been explored or added to the fringe yet, and it is pathable
            if(neighbor && came_from.count(neighbor) == 0 && neighbor->is_pathable(unit))
            {
                // add it to the tiles to be explored and add where it came from.
                fringe.push(neighbor);
                came_from[neighbor] = inspect;
            }

        } // for each neighbor

    } // while fringe not empty

    // if you're here, that means that there was not a path to get to where you want to go.
    //   in that case, we'll just return an empty path.
    return {};
}

//<<-- Creer-Merge: methods -->> - Code you add between this comment and the end comment will be preserved between Creer re-runs.

bool is_ship(Unit u)
{
    return u->ship_health > 0;
}

//<<-- /Creer-Merge: methods -->>

} // pirates

} // cpp_client
