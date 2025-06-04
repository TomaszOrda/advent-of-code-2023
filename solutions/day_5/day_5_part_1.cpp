#include "../runner.hpp"
#include "../utils.hpp"
#include <string>
#include <optional>
#include <vector>
#include <map>
#include <tuple>

class CustomRange{
public:
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
private:
    long long int input_start{};
    long long int output_start{};
    long long int output_offset{};
    long long int length{};
};

class RangesMap{
public:
    std::string_view name{};
    RangesMap(){
    }
    RangesMap(std::string_view map_record){
        auto ranges_records = split_lines(map_record);
        name = split(ranges_records[0], ' ')[0];
        ranges_records.erase(ranges_records.begin());
        for (const std::string_view& range_record : ranges_records){
            ranges.push_back(CustomRange(range_record));
        }
    }
    long long int apply(long long int x) const{
        for (const auto& range : ranges){
            if (range.is_in_range(x)){
                return range.convert(x);
            }
        }
        return x;
    }
private:
    std::vector<CustomRange> ranges{};
};

class Almanac{
public:
    std::vector<long long int> seeds{};
    Almanac(std::string_view records){
        auto map_records = split(records, "\n\n");
        for (const std::string_view& seed : split(split(map_records[0], ": ")[1], ' ')){
            seeds.push_back(svtoll(seed));
        };
        map_records.erase(map_records.begin());
        for (const std::string_view& map_record : map_records){
            RangesMap map(map_record);
            maps.insert(std::pair<std::string_view, RangesMap>(map.name, map));
        }
    }
    long long int seed_to_location(long long int seed){
        long long int soil {maps["seed-to-soil"].apply(seed)};
        long long int fertilizer {maps["soil-to-fertilizer"].apply(soil)};
        long long int water {maps["fertilizer-to-water"].apply(fertilizer)};
        long long int light {maps["water-to-light"].apply(water)};
        long long int temperature {maps["light-to-temperature"].apply(light)};
        long long int humidity {maps["temperature-to-humidity"].apply(temperature)};
        long long int location {maps["humidity-to-location"].apply(humidity)};
        return location;
    }
private:
    std::map<std::string_view, RangesMap> maps{};
};

std::optional<std::string> solve(std::string_view almanac_record){
    Almanac almanac(almanac_record);
    std::vector<long long int> locations;
    for (const auto& seed : almanac.seeds){
        locations.push_back(almanac.seed_to_location(seed));
    }
    return std::to_string(*std::min_element(locations.begin(), locations.end()));
}

int main(int argc, [[maybe_unused]] char *argv[]) {
    Solution solution = Solution(5, 1, solve);
    if (argc > 1){
        return solution.test_run();
    }else{
        return solution.run();
    }
}
