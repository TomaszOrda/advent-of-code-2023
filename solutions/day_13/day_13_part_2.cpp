#include "../runner.hpp"
#include "../utils.hpp"
#include <string>
#include <optional>
#include <numeric>
#include <ranges>

class MirroredMap{
    Grid<char> map_grid;
public:
    enum class Orientation {horizontal, vertical};
    MirroredMap(std::string_view text) : map_grid{text}
    {}
    int possible_reflection_distances_smudge_corrected(Orientation o) const{
        bool vertical_reflections {o == Orientation::vertical};
        int initial_reflections = vertical_reflections ? map_grid.get_width()-1 : map_grid.get_height()-1;

        std::vector<int> possible_reflections (static_cast<size_t>(initial_reflections));
        std::iota(possible_reflections.begin(), possible_reflections.end(), 0);

        for (int reflection{}; reflection < initial_reflections; reflection++){
            if (is_reflection(o, reflection, true) && !is_reflection(o, reflection, false)){
                return reflection + 1;
            }
        }
        return 0;
    }
    int possible_reflection_distances(Orientation o) const{
        bool vertical_reflections {o == Orientation::vertical};
        int initial_reflections = vertical_reflections ? map_grid.get_width()-1 : map_grid.get_height()-1;

        std::vector<int> possible_reflections (static_cast<size_t>(initial_reflections));
        std::iota(possible_reflections.begin(), possible_reflections.end(), 0);

        for (int reflection{}; reflection < initial_reflections; reflection++){
            if (is_reflection(o, reflection, false)){
                return reflection + 1;
            }
        }
        return 0;
    }
private:
    bool is_reflection(Orientation o, int x, bool smudge_corrected = true) const{
        if (o == Orientation::vertical)
            return is_vertical_reflection(x, smudge_corrected);
        else
            return is_horizontal_reflection(x, smudge_corrected);
    }
    bool is_horizontal_reflection(int row_above, bool smudge_corrected = true) const{
        int corrections_left = smudge_corrected;
        for (int column{}; column < map_grid.get_width(); column++){
            int above_image_id = row_above;
            int below_image_id = row_above+1;
            while (0<=above_image_id && below_image_id < map_grid.get_height())
            {
                if (map_grid.at(column, above_image_id).value() != map_grid.at(column, below_image_id).value())
                    if(--corrections_left < 0)
                        return false;
                above_image_id--;
                below_image_id++;
            }
        }
        return corrections_left==0;
    }
    bool is_vertical_reflection(int column_to_the_left, bool with_smudge_correction = false) const{
        int corrections_left = with_smudge_correction;
        for (int row{}; row < map_grid.get_height(); row++){
            int left_image_id = column_to_the_left;
            int right_image_id = column_to_the_left+1;
            while (0<=left_image_id && right_image_id < map_grid.get_width())
            {
                if (map_grid.at(left_image_id, row).value() != map_grid.at(right_image_id, row).value())
                    if(--corrections_left < 0)
                        return false;
                left_image_id--;
                right_image_id++;
            }
        }
        return corrections_left==0;
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
    Solution solution = Solution(13, 2, solve);
    if (argc > 1){
        return solution.test_run();
    }else{
        return solution.run();
    }
}
