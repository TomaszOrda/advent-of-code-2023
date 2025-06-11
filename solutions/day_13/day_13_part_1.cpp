#include "../runner.hpp"
#include "../utils.hpp"
#include <string>
#include <optional>
#include <numeric>
#include <ranges>

//Part 2 implements this solution a little bit cleaner
//Thus I won't make effort changing this file.

class MirroredMap{
    Grid<char> map_grid;
public:
    enum class Orientation {horizontal, vertical};
    MirroredMap(std::string_view text) : map_grid{text}
    {}
    int possible_reflection_distances_smudge_corrected(Orientation o) const{
        bool vertical_reflections {o == Orientation::vertical};
        int initial_reflections = vertical_reflections ? map_grid.get_width()-1 : map_grid.get_height()-1;
        int reflection_parts = vertical_reflections ? map_grid.get_height() : map_grid.get_width();

        std::vector<int> possible_reflections (static_cast<size_t>(initial_reflections));
        std::iota(possible_reflections.begin(), possible_reflections.end(), 0);
        for (int part{0}; part<reflection_parts; part++)
            possible_reflections.erase(
                std::remove_if(
                    possible_reflections.begin(), 
                    possible_reflections.end(), 
                    [this, o, part](int reflection_neighbour){return !is_reflection(o, reflection_neighbour, part);}),
                possible_reflections.end()
                );
        return static_cast<int>(std::accumulate(possible_reflections.begin(), possible_reflections.end(), possible_reflections.size()));
    }
private:
    bool is_reflection(Orientation o, int i, int c) const{
        if (o == Orientation::vertical)
            return is_vertical_reflection(i, c);
        else
            return is_horizontal_reflection(i, c);
    }
    bool is_vertical_reflection(int column_to_the_left, int row) const{
        int left_image_id = column_to_the_left;
        int right_image_id = column_to_the_left+1;
        while (0<=left_image_id && right_image_id < map_grid.get_width())
        {
            if (map_grid.at(left_image_id, row).value() != map_grid.at(right_image_id, row).value())
                return false;
            left_image_id--;
            right_image_id++;
        }
        return true;
    }
    bool is_horizontal_reflection(int row_above, int column) const{
        int above_image_id = row_above;
        int below_image_id = row_above+1;
        while (0<=above_image_id && below_image_id < map_grid.get_height())
        {
            if (map_grid.at(column, above_image_id).value() != map_grid.at(column, below_image_id).value())
                return false;
            above_image_id--;
            below_image_id++;
        }
        return true;
    }
};

std::optional<std::string> solve(std::string_view input){
    auto mirrored_maps = split(input, "\n\n") | std::views::transform([](auto x){return MirroredMap(x);});
    int summarized_notes = 0;
    for (const auto& mirrored_map : mirrored_maps)
        summarized_notes += mirrored_map.possible_reflection_distances_smudge_corrected(MirroredMap::Orientation::vertical) + mirrored_map.possible_reflection_distances_smudge_corrected(MirroredMap::Orientation::horizontal) * 100;
    return std::to_string(summarized_notes);
}

int main(int argc, [[maybe_unused]] char *argv[]) {
    Solution solution = Solution(13, 1, solve);
    if (argc > 1){
        return solution.test_run();
    }else{
        return solution.run();
    }
}
