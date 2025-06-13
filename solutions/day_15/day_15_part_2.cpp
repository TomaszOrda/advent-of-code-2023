#include "../runner.hpp"
#include "../utils.hpp"
#include <string>
#include <optional>
#include <typeinfo>

#define HASH_MULTIPLIER 17
#define NUMBER_OF_BOXES 256

struct Lens{
    std::string label;
    int focal_length;
};

struct LensLabel{
    std::string label;
};

template<>
struct std::hash<LensLabel>{
    std::size_t operator()(const LensLabel& lens_label){
        size_t current_hash = 0;
        std::string_view data = lens_label.label;
        for (size_t i{}; i< data.length(); i++){
            current_hash += static_cast<size_t>(data[i]);
            current_hash *= HASH_MULTIPLIER;
            current_hash %= NUMBER_OF_BOXES;
        }
        return current_hash;
    }
};

class LightBoxes{
    std::array<std::vector<Lens>,NUMBER_OF_BOXES> boxes{};
public:
    LightBoxes(){};
    void remove_lens(LensLabel lens_label){
        size_t box_id = std::hash<LensLabel>{}(lens_label);
        boxes[box_id].erase(
            std::remove_if(boxes[box_id].begin(), 
                           boxes[box_id].end(), 
                           [lens_label](Lens x){return x.label == lens_label.label;}), 
            boxes[box_id].end());
    }
    void replace_lens(LensLabel lens_label, int focal_length){
        size_t box_id = std::hash<LensLabel>{}(lens_label);
        auto lens_it = std::find_if(boxes[box_id].begin(), boxes[box_id].end(), [lens_label](Lens x){return x.label == lens_label.label;});
        if (lens_it == boxes[box_id].end()){
            boxes[box_id].push_back(Lens{lens_label.label, focal_length});
        }else{
            (*lens_it).focal_length = focal_length;
        }
    }
    int get_focusing_power(){
        int focusing_power = 0;
        for (size_t box_id{}; box_id < boxes.size(); box_id++)
            for (size_t lens_id{}; lens_id < boxes[box_id].size(); lens_id++)
                focusing_power += (static_cast<int>(box_id) + 1) * (static_cast<int>(lens_id) + 1) * boxes[box_id][lens_id].focal_length; 
        
        return focusing_power;
    }
};



std::optional<std::string> solve(std::string_view input){
    LightBoxes light_boxes{};
    auto init_steps = split(input, ',');
    for (const auto& step : init_steps){
        std::string label {split(split(step, '-')[0], '=')[0]};
        if (step.contains('-')){
            light_boxes.remove_lens(LensLabel{label});
        }else if (step.contains('=')){
            int focal_length = svtoi(split(step, '=')[1]);
            light_boxes.replace_lens(LensLabel{label}, focal_length);
        }else {
            std::string command{step};
            throw std::runtime_error("Unknown command " + command);
        } 
    }
    return std::to_string(light_boxes.get_focusing_power());
}

int main(int argc, [[maybe_unused]] char *argv[]) {
    Solution solution = Solution(15, 2, solve);
    if (argc > 1){
        return solution.test_run();
    }else{
        return solution.run();
    }
}
