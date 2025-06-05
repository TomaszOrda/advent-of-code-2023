#include "../runner.hpp"
#include "../utils.hpp"
#include <string>
#include <optional>
#include <vector>
#include <map>
#include <algorithm>

class CustomRange{
public:
    CustomRange(long long int i_start, long long int i_end, long long int offset)
        : input_start{i_start}
        , output_start{i_start + offset}
        , output_offset{offset}
        , length{i_end - i_start}
    {}
    CustomRange(std::string_view range_record){
        auto range_numbers = split(range_record, ' ');
        output_start = svtoll(range_numbers[0]);
        input_start = svtoll(range_numbers[1]);
        output_offset = svtoll(range_numbers[0]) - input_start;
        length = svtoll(range_numbers[2]);
    }
    bool is_in_range(long long int x) const{
        return input_start <= x && x < input_start + length;
    }
    long long int convert(long long int x) const{
        return x + -input_start + output_start;
    }
    bool operator<(const CustomRange& b) const{
        return input_start < b.input_start;
    }
    std::optional<CustomRange> combine(const CustomRange& other) const{
        if (this->overlaps(other)){
            return CustomRange(
                std::max(input_start, other.get_input_start() - output_offset),
                std::min(get_input_end(), other.get_input_end()- output_offset),
                output_offset + other.output_offset
            );
        }
        return std::nullopt;
    }
    bool overlaps(const CustomRange& other) const{
        return get_output_end() >other.get_input_start() && output_start < other.get_input_end();
    }
    long long int get_input_start() const{
        return input_start;
    }
    long long int get_input_end() const{
        return input_start+length;
    }
    long long int get_output_start() const{
        return output_start;
    }
    long long int get_output_end() const{
        return output_start+length;
    }
private:
    long long int input_start{};
    long long int output_start{};
    long long int output_offset{};
    long long int length{};
};

class RangesMap{
public:
    RangesMap(){}
    RangesMap(std::vector<CustomRange> input_ranges): ranges {input_ranges} {}
    RangesMap(std::string_view map_record){
        auto ranges_records = split_lines(map_record);
        name = split(ranges_records[0], ' ')[0];
        ranges_records.erase(ranges_records.begin());
        std::vector<CustomRange> ranges_from_text{};
        for (size_t i{0}; i<ranges_records.size();i++){
            auto& range_record = ranges_records[i];
            ranges_from_text.push_back(CustomRange(range_record));
        }
        std::sort(ranges_from_text.begin(), ranges_from_text.end());

        if (ranges_from_text[0].get_input_start()!=0){
            ranges.push_back(CustomRange(0, ranges_from_text[0].get_input_start(), 0));
        }
        for (size_t i{0}; i<ranges_from_text.size();i++){
            auto& range = ranges_from_text[i];
            ranges.push_back(range);
            if (i!= ranges_from_text.size()-1 && range.get_input_end() != ranges_from_text[i+1].get_input_start()){
                ranges.push_back(CustomRange(range.get_input_end(), ranges_from_text[i+1].get_input_start(), 0));
            }
        }
        ranges.push_back(CustomRange(ranges.back().get_input_end(), LONG_LONG_MAX, 0));
    }
    RangesMap operator>>(RangesMap& other) const{
        std::vector<CustomRange> result_ranges{};

        for (const auto& range : ranges){
            for (const auto& other_range : other.ranges){
                auto combined_ranges = range.combine(other_range);
                if (combined_ranges){
                    result_ranges.push_back(combined_ranges.value());
                }
            };
        };
        return RangesMap(result_ranges);
    };
    std::string_view get_name(){
        return name;
    }
    std::vector<CustomRange> get_ranges(){
        return ranges;
    }
private:
    std::string_view name{};
    std::vector<CustomRange> ranges{};
};

class Almanac{
public:
    Almanac(std::string_view records){
        auto map_records = split(records, "\n\n");
        auto seed_ranges = split(split(map_records[0], ": ")[1], ' ');
        for(size_t i{0}; i<seed_ranges.size(); i+=2){
            seeds.push_back(
                CustomRange(svtoll(seed_ranges[i]), svtoll(seed_ranges[i]) + svtoll(seed_ranges[i+1]),0)
            );
        }
        seeds_map = RangesMap(seeds);
        map_records.erase(map_records.begin());
        for (const std::string_view& map_record : map_records){
            RangesMap map(map_record);
            maps.insert(std::pair<std::string_view, RangesMap>(map.get_name(), map));
        }
    }
    long long int minimal_location(){
        long long int minimal_location = LONG_LONG_MAX;
        for (const auto& range : (
                seeds_map>>
                maps["seed-to-soil"]>>
                maps["soil-to-fertilizer"]>>
                maps["fertilizer-to-water"]>>
                maps["water-to-light"]>>
                maps["light-to-temperature"]>>
                maps["temperature-to-humidity"]>>
                maps["humidity-to-location"]
                ).get_ranges()){
            minimal_location = std::min(minimal_location, range.convert(range.get_input_start()));
        }
        return minimal_location;
    }
private:
    std::map<std::string_view, RangesMap> maps{};
    std::vector<CustomRange> seeds{};
    RangesMap seeds_map{};
};

std::optional<std::string> solve(std::string_view almanac_record){
    Almanac almanac(almanac_record);
    return std::to_string(almanac.minimal_location());
}

int main(int argc, [[maybe_unused]] char *argv[]) {
    Solution solution = Solution(5, 2, solve);
    if (argc > 1){
        return solution.test_run();
    }else{
        return solution.run();
    }
}
