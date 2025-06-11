#include "../runner.hpp"
#include "../utils.hpp"
#include <string>
#include <optional>
#include <vector>
#include <ranges>
#include <numeric>
#include <unordered_map>
#include <cassert>

class DamagedSprings{
    using key_type = uint64_t;
    key_type max_key_part_value = UINT32_MAX;
    int key_shift = 32;
    std::string springs{};
    std::vector<int> groups{};
    std::unordered_map<key_type, long long int> cache{};
public:
    DamagedSprings(std::string_view record)
    {
        std::string_view springs_folded {split(record, ' ')[0]};
        std::vector<int> groups_folded {std::ranges::to<std::vector<int>>(split(split(record, ' ')[1], ',') | std::views::transform(svtoi))};
        for (int i{}; i<5; i++){
            springs += split(record, ' ')[0];
            springs += "?";
            groups.insert(groups.end(), groups_folded.begin(), groups_folded.end());
        }
        springs.erase(springs.end()-1);
    }
    long long int number_of_possible_arrangements(){
        return number_of_possible_arrangements_aux(springs, groups);
    }
private:
    long long int number_of_possible_arrangements_aux(std::string_view springs_slice, std::span<int> groups_slice){
        unsigned long long int key{pair_keys_to_key(springs_slice.length(), groups_slice.size())};

        if (cache.contains(key))
            return cache.at(key);

        if (static_cast<int>(springs_slice.length()) < std::accumulate(groups_slice.begin(), groups_slice.end(), 0) +  static_cast<int>(groups_slice.size()) - 1)
            return 0;

        if (groups_slice.empty()){
            if (springs_slice.contains('#'))
                return 0;
            else
                return 1;
        }else if (springs_slice.length()==0)
            return 0;

        long long int result_skip = 0;
        long long int result_fit = 0;
        size_t group_size = static_cast<size_t>(groups_slice[0]);
        if (group_size > springs_slice.length())
            return 0;
        
        if (group_size == springs_slice.length()) {
            if(groups_slice.size()==1 &&  !springs_slice.substr(0, group_size).contains('.'))
                return  1;
            else
                return 0;
        }
        if (!springs_slice.substr(0, group_size).contains('.') && std::string(".?").contains(springs_slice[group_size]))
            result_fit = number_of_possible_arrangements_aux(
                springs_slice.substr(group_size + 1, springs_slice.length()-group_size-1), 
                groups_slice | std::views::drop(1));
        
        if (springs_slice[0] != '#')
            result_skip = number_of_possible_arrangements_aux(
                springs_slice.substr(1, springs_slice.length()-1), 
                groups_slice);
        long long int result = result_skip + result_fit;
        cache.emplace(key, result);
        return cache.at(key);
    }
    key_type pair_keys_to_key(size_t a, size_t b){
        //Consider creating a custom class implementing a hash function.
        //The hash function should return a xor of two hashes.
        //This would result in fewer collisions.
        assert(a < max_key_part_value && b < max_key_part_value && "Sub-keys too large");
        return (a << key_shift) ^ b;
    }
};

std::optional<std::string> solve(std::string_view input){
    auto condition_records = split_lines(input) | std::views::transform([](auto x){return DamagedSprings(x).number_of_possible_arrangements();});
    long long int sum{};
    for (const long long int& x : condition_records) 
        sum += x;

    return std::to_string(sum);
}

int main(int argc, [[maybe_unused]] char *argv[]) {
    Solution solution = Solution(12, 2, solve);
    if (argc > 1){
        return solution.test_run();
    }else{
        return solution.run();
    }
}
