#include "../runner.hpp"
#include "../utils.hpp"
#include <string>
#include <optional>

#define EXPANSION_RATE 1000000

class CelestialMap{
    Grid<char> celestial_grid;
    std::vector<int> expanded_columns_partial_sum{};
    std::vector<int> expanded_rows_partial_sum{};
    std::vector<Coord> galaxies{};
public:
    CelestialMap(std::string_view text) : celestial_grid{text} {
        expanded_columns_partial_sum.push_back(0);
        for(int x{1}; x<celestial_grid.get_width(); x++)
            expanded_columns_partial_sum.push_back(expanded_columns_partial_sum[static_cast<size_t>(x-1)] + is_empty_column(x));
        expanded_rows_partial_sum.push_back(0);
        for(int y{1}; y<celestial_grid.get_height(); y++)
            expanded_rows_partial_sum.push_back(expanded_rows_partial_sum[static_cast<size_t>(y-1)] + is_empty_row(y));

        for (int x{0}; x<celestial_grid.get_width(); x++)
            for (int y{0}; y<celestial_grid.get_height(); y++)
                if (celestial_grid.at(x,y).value() == '#')
                    galaxies.push_back({x,y});
    }
    long long int sum_of_distances_between_galaxies(){
        long long int sum = 0;
        for (size_t a{}; a<galaxies.size(); a++)
            for (size_t b{a}; b<galaxies.size(); b++){
                Coord galaxy_a = galaxies[a];
                Coord galaxy_b = galaxies[b];
                sum += expanded_distance(galaxy_a, galaxy_b);
            }
        return sum;
    }
private:
    long long int expanded_distance(Coord galaxy_1, Coord galaxy_2){
        int standard_distance = (galaxy_2 - galaxy_1).manhattan_distance();
        long long int vertical_expansion = 
            expanded_rows_partial_sum[static_cast<size_t>(std::max(galaxy_1.second, galaxy_2.second))] - 
            expanded_rows_partial_sum[static_cast<size_t>(std::min(galaxy_1.second, galaxy_2.second))];
        long long int horizontal_expansion = 
            expanded_columns_partial_sum[static_cast<size_t>(std::max(galaxy_1.first, galaxy_2.first))] - 
            expanded_columns_partial_sum[static_cast<size_t>(std::min(galaxy_1.first, galaxy_2.first))];
        return standard_distance + (EXPANSION_RATE-1) * (vertical_expansion + horizontal_expansion);
    }
    bool is_empty_column(int x){
        for (int y{}; y<celestial_grid.get_height();y++)
            if (celestial_grid.at(x,y).value()!='.')
                return false;
        return true;
    }
    bool is_empty_row(int y){
        for (int x{}; x<celestial_grid.get_height();x++)
            if (celestial_grid.at(x,y).value()!='.')
                return false;
        return true;
    }
};

std::optional<std::string> solve(std::string_view input){
    return std::to_string(CelestialMap(input).sum_of_distances_between_galaxies());
}

int main(int argc, [[maybe_unused]] char *argv[]) {
    Solution solution = Solution(11, 2, solve);
    if (argc > 1){
        return solution.test_run();
    }else{
        return solution.run();
    }
}
