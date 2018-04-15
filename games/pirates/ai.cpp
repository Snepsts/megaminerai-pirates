// AI
// This is where you build your AI

#include "ai.hpp"

#include<map>
#include<string>
#include<algorithm>
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

/// <summary>
/// This is called every time it is this AI.player's turn.
/// </summary>
/// <returns>Represents if you want to end your turn. True means end your turn, False means to keep your turn going and re-call this function.</returns>
bool AI::run_turn()
{
    spawn_units();

    auto my_units = player->units;

    std::vector<Unit> available_units; //in case we rerun the function

    for (auto unit : my_units) {
        if (!unit->acted)
            available_units.push_back(unit);
    }

    for (auto unit : available_units) {
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
    float decision = get_ship_aggressiveness(u);

    if (decision > 0.5f) {
        destroy_enemy_ship(u);
    } else {
        //not sure
    }

    return true;
}

bool AI::run_crew_turn(Unit u)
{
    return true;
}

bool AI::run_ship_attack(Unit u)
{
    float decision = 0.0f;
    Tile enemy_ship = get_closest_enemy_ship(u);

    decision += (1.0f - get_enemy_ship_health_value(u, enemy_ship));
    decision += get_enemy_ship_crew_value(u, enemy_ship); 
    decision /= 2;

    if (decision > 0.5f) //higher value means ranged attack
        return destroy_enemy_ship(u);
    else
        return steal_enemy_ship(u);

}

float AI::get_enemy_ship_health_value(Unit u, Tile enemy_ship)
{
    return (float)(enemy_ship->unit->ship_health / game->ship_health);
}

float AI::get_enemy_ship_crew_value(Unit u, Tile enemy_ship)
{ //favor them more so if we get a higher value we know our crew is definitely better
    return 1.0f - (float)(u->crew_health / 1.3f) / enemy_ship->unit->crew_health;
}

float AI::get_ship_aggressiveness(Unit u)
{
    float ret = 0.0f;

    ret += (1 - get_ship_danger_level(u));
    ret += get_ship_health_value(u);

    return ret / 2;
}

//******************************************************************************************
//Fuzzy functions
//******************************************************************************************

float AI::get_ship_danger_level(Unit u)
{
    float ret = 0.0f;

    int enemy_ships = get_close_enemy_ships(u);
    switch (enemy_ships) {
        case 0:
            ret += 0.0f;
            break;
        case 1:
            ret += 0.33f;
            break;
        case 2:
            ret += 0.66f;
            break;
        case 3:
            ret += 1.0f;
            break;
        default:
            ret += 1.0f;
            break;
    }

    return ret;
}

float AI::get_ship_health_value(Unit u)
{
    return (float)(u->ship_health / game->ship_health);
}

float AI::get_crew_dig_fuzzy(Unit u, Tile t)
{
    float fuzzy = 0.0;


}

//**************************************************************************************************
//action functions
//**************************************************************************************************
bool AI::heal_ship(Unit u)
{
    Tile closest_port = get_closest_port(u);
    if(closest_port == NULL) {
        return false;
    }
    else {
        if(move_next_to_tile(u, closest_port))
        {
            u->rest();
            return true;
        }
        return false;
    }
    return false;
}
bool AI::steal_enemy_ship(Unit u)
{
    Tile closest_enemy_ship = get_closest_enemy_ship(u);
    if(closest_enemy_ship == NULL) {
        return false; //no enemy ship
    }
    std::vector<Tile> path_to_enemy_ship = find_path(u->tile, closest_enemy_ship, u);
    if(path_to_enemy_ship.size() == 0) {
        return false; //no valid path
    }
    if(path_to_enemy_ship.size() == 1) {
        if(u->attack(closest_enemy_ship, "crew")) {
            return true; //attacked enemy ship
        }
    }
    else {
        if(move_next_to_tile(u, path_to_enemy_ship.back())) {
            if(u->attack(closest_enemy_ship, "crew")) {
                return true;
            }
        }
        else
        {
            if(u->attack(closest_enemy_ship, "ship"))
            {
                return true;
            }
        }
    }
    return false;
}

bool AI::board_empty_ship(Unit u)
{
    Tile closest_empty_ship = get_closest_empty_ship(u);
    int current_crew = u->crew;
    if(move_next_to_tile(u, closest_empty_ship))
    {
        if(u->split(closest_empty_ship, current_crew/2))
        {
            return true;
        }
    }
    return false;
}

bool AI::steal_enemy_treasure(Unit u)
//Trys to steal enemy treasure by moving to it and digging
//will return false in any case that is not the unit digging treasure
{
    //check if possible
    Tile closest_enemy_treasure = get_closest_enemy_treasure(u);
    if(closest_enemy_treasure == NULL) 
    {
        return false; //no treasure was found
    }
    if(u->tile == closest_enemy_treasure)
    {
        if(!u->acted)
        {
            u->dig(-1); //already on tile, DIG
        }
        else
        {
            return false; //on tile but couldn't steal
        }
    }
    else
    {
        if(move_towards_enemy_treasure(u))
        {
            if(!u->acted)
            {
                u->dig(-1);
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

bool AI::destroy_enemy_ship(Unit u)
//Moves unit towards nearest enemy ship and attacks it if it is within 3 spaces
//Returns true only if the enemy ship is destroyed
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
            move_to_tile(u, closest_ship->tile);
            return false;
        }
        return false;
    }
    return false;
}

bool AI::destroy_merchant_ship(Unit u)
{
    Tile closest_ship_tile = get_closest_enemy_ship(u);
    auto path = this->find_path(u->tile, closest_ship_tile, u);
    if(path.size() > 3)
    {
        move_next_to_tile(u, closest_ship_tile);
        path = this->find_path(u->tile, closest_ship_tile, u);
        if(path.size() <= 3)
            return u->attack(closest_ship_tile, "ship");
        return false;
    } 
    else
    {
        return u->attack(closest_ship_tile, "ship");
    }
}

bool AI::unit_retreat_and_rest(Unit u)
//Moves unit towards the home port and rests once it is within 3 spaces of it
{
    auto distance_to_port = this->find_path(u->tile, this->player->port->tile, u).size();
    move_to_tile(u, this->player->port->tile);
    if(distance_to_port <= 3)
    {
        u->rest();
        return true;
    }
    return false;
}

bool AI::crew_bury_treasure(Unit u)
//Checks if the unit has gold and buries it there.  If the tile doesnt have gold in it previously
//it is added to the buried treasure vector
{
    if(u->gold > 0)
    {
        if(std::find(buried_treasure_vec.begin(), buried_treasure_vec.end(), u->tile) == buried_treasure_vec.end())
            buried_treasure_vec.push_back(u->tile);

        return u->bury(u->gold);
    }
    return false;
}

bool AI::crew_dig_treasure(Unit u, Tile t)
{
    move_to_tile(u, t);
    if(u->tile == t)
    {
        return u->dig(-1);
    }
    return false;
}

//**************************************************************************************************
//helper functions
//**************************************************************************************************
Tile AI::get_closest_empty_ship(Unit u)
{
    std::vector<std::vector<Tile>> possible_paths;
    std::vector<Tile> empty_ship_tiles;
    for(Unit possible_empty_ship : game->units) {
        if(possible_empty_ship->ship_health > 0 && possible_empty_ship->crew_health <= 0) {
            empty_ship_tiles.push_back(possible_empty_ship->tile);
        }
    }
    if(empty_ship_tiles.empty()) {
        return NULL;
    }
    return get_closest_tile_from_options(u, empty_ship_tiles);
}

Tile AI::get_closest_tile_from_options(Unit u, std::vector<Tile> tile_options)
{
    unsigned smallest = 999;
    Tile closest_tile = NULL;
    std::vector<Tile> smallest_path;
    std::vector<std::vector<Tile>> possible_paths;
    for(Tile tile_option : tile_options)
    {
        std::vector<Tile> temp = find_path(u->tile, tile_option, u);
        if(temp.size() > 0)
        {
            possible_paths.push_back(temp);
        }
    }
    for(std::vector<Tile> possible_path : possible_paths)
    {
        if(possible_path.size() != 0 && possible_path.size() < smallest)
        {
            smallest = possible_path.size();
            smallest_path = possible_path;
            closest_tile = possible_path.back();
        }
    }
    return closest_tile;
}

Tile AI::get_closest_port(Unit u)
{
    std::vector<std::vector<Tile>> possible_paths;
    std::vector<Tile> possible_ports;
    for(Tile possible_port : game->tiles)
    {
        if(possible_port->port != NULL)
        {
            if(possible_port->port->owner == NULL)
            {
                possible_ports.push_back(possible_port);
            }
            else if(possible_port->port->owner != player->opponent)
            {
                possible_ports.push_back(possible_port);
            }
        }
    }
    for(Tile port : possible_ports) {
        std::vector<Tile> path = find_path(u->tile, port, u);
        possible_paths.push_back(path);
    }
    unsigned smallest = 999;
    Tile closest_tile = NULL;
    std::vector<Tile> smallest_path;
    for(std::vector<Tile> possible_path : possible_paths)
    {
        if(possible_path.size() != 0 && possible_path.size() < smallest)
        {
            smallest = possible_path.size();
            smallest_path = possible_path;
            closest_tile = possible_path.back();
        }
    }
    return closest_tile;
}

Tile AI::get_closest_enemy_ship(Unit u)
{
    std::vector<std::vector<Tile>> possible_paths;
    std::vector<Unit> enemy_ships = get_enemy_ships();
    for(Unit enemy_ship : enemy_ships) {
        std::vector<Tile> path = find_path(u->tile, enemy_ship->tile, u);
        possible_paths.push_back(path);
    }
    unsigned smallest = 999;
    Tile closest_tile = NULL;
    std::vector<Tile> smallest_path;
    for(std::vector<Tile> possible_path : possible_paths)
    {
        if(possible_path.size() != 0 && possible_path.size() < smallest)
        {
            smallest = possible_path.size();
            smallest_path = possible_path;
            closest_tile = possible_path.back();
        }
    }
    return closest_tile;
}

std::vector<Tile> AI::get_list_of_enemy_treasure()
{
    bool ours = false;
    std::vector<Tile> enemy_treasure_tiles;
    for(Tile tile : this->game->tiles)
    {
        if(tile->gold > 0)
        {
            if(buried_treasure_vec.size() != 0)
            {
                for(Tile ourTreasure : buried_treasure_vec)
                {
                    if(ourTreasure == tile)
                    {
                        ours = true;
                        break;
                    }
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

Tile AI::get_closest_enemy_treasure(Unit u)
{
    std::vector<Tile> enemy_treasures = get_list_of_enemy_treasure();
    unsigned largest = 999;
    Tile closest_tile = NULL;
    if(enemy_treasures.empty())
    {
        return closest_tile;
    }
    for(Tile enemy_treasure_tile : enemy_treasures)
    {
        if(u->tile == enemy_treasure_tile)
        {
            return u->tile; //your dumb
        }
        std::vector<Tile> test_path = this->find_path(u->tile, enemy_treasure_tile, u);
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

bool AI::move_towards_enemy_treasure(Unit u)
{
    Tile closest_tile = get_closest_enemy_treasure(u);
    if(closest_tile == NULL)
        return false; // no treasure
    std::vector<Tile> path = this->find_path(u->tile, closest_tile, u);
    while(u->moves > 0)
    {
        if(path.empty())
            return true; //reached the tile
        u->move(path[0]);
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
    if (u->tile == t)
        return true;
    else
        return false;
}

bool AI::move_next_to_tile(Unit u, Tile t)
{
    while(u->moves > 0)
    {
        auto path = this->find_path(u->tile, t, u);
        if(path.size() > 1)
            u->move(path[0]);
    }
    if (find_path(u->tile, t, u).size() == 1)
        return true;
    else
        return false;
}

int AI::get_close_enemy_ships(Unit u)
{
    int count = 0;
    auto enemy_ships = get_enemy_ships();
    for(auto es : enemy_ships)
    {
        auto distance = this->find_path(u->tile, es->tile, u).size();
        if(distance < 4)
            ++count;
    }

    return count;
}

void AI::spawn_units()
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

Tile AI::get_nearest_port(Unit u)
{
    std::vector<Tile> docks;
    for(auto t : this->game->tiles)
    {
        if(t->port != nullptr)
            docks.push_back(t);
    }
    AI* temp = this;
    
    Tile nearest;
    if(docks.size() > 0)
    {
        nearest = docks[0];
        size_t distance = this->find_path(u->tile, nearest, u).size();
        for(auto d : docks)
        {
            size_t current_distance = this->find_path(u->tile, d, u).size();
            if(current_distance < distance)
            {
                distance = current_distance;
                nearest = d;
            }
        }
    }

    return nearest;
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
