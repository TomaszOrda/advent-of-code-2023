#include "../runner.hpp"
#include "../utils.hpp"
#include <string>
#include <optional>
#include <unordered_map>
#include <ranges>

class Part{
    std::unordered_map<char, std::pair<int, int>> parameter{};
public:
    Part(){
        for (const auto& p : "xmas"){
            parameter.emplace(p, std::make_pair(1, 4000));
        }
    }
    long long int total_combinations() const{
        long long int total = 1;
        for (const char& p : std::string("xmas")){
            total *= parameter.at(p).second - parameter.at(p).first + 1;
        }
        return total;
    }
    std::pair<Part, Part> split(char cat, int value) const{
        Part p_left(*this);
        Part p_right(*this);
        p_left.set_category_max(cat, value-1);
        p_right.set_category_min(cat, value);
        return {p_left, p_right};
    }
    void set_category_min(char cat, int value){
        parameter[cat].first = value;
    }
    void set_category_max(char cat, int value){
        parameter[cat].second = value;
    }
    int get_category_min(char cat) const{
        return parameter.at(cat).first;
    }
    int get_category_max(char cat) const{
        return parameter.at(cat).second;
    }
};

class Rule{
    bool always_true{false};
    bool smaller{};
    int value{};
    char category{};
    std::string result{};
public:
    Rule(std::string_view text){
        if (!text.contains(':')){
            always_true = true;
            result = text;
        }else{
            category = text[0];
            smaller = text[1] == '<';
            auto text_parts = split(text, ':');
            value = svtoi(text_parts[0].substr(2, text_parts[0].length()-2));
            result = std::string(text_parts[1]);
        }
    }
    std::tuple<Part, std::string, Part> split_combinations(Part p) const{
        if (always_true){
            Part e{};
            e.set_category_max('x', 0);
            return {p, result, e};
        }
        Part p_false, p_true;
        if (smaller){
            std::tie(p_true, p_false) = p.split(category, value);
        }else{
            std::tie(p_false, p_true) = p.split(category, value+1);
        }
        return {p_true, result, p_false};
    }
};



class Workflow{
    std::string name{};
    std::vector<Rule> rules{};
public:
    Workflow(){};
    Workflow(std::string_view text){
        size_t rules_start = static_cast<size_t>(std::find(text.begin(), text.end(), '{') - text.begin());
        name = text.substr(0, rules_start);
        for (const auto& rule_record : split(text.substr(rules_start+1, text.length() - rules_start - 2), ',')){
            rules.push_back(Rule(rule_record));
        }
    }
    std::string_view get_name() const{
        return name;
    }
    std::vector<std::pair<Part, std::string>> split_combinations(Part p) const {
        std::vector<std::pair<Part, std::string>> result{};
        for (const auto& rule : rules){
            std::string rule_name;
            Part splitted_pattern;
            std::tie(splitted_pattern, rule_name, p) = rule.split_combinations(p);
            result.push_back({splitted_pattern, rule_name});
        }
        return result;
    }
};

class System{
    std::unordered_map<std::string, Workflow> workflows{};
public:
    System(std::string_view text){
        for (const auto& workflow_record : split_lines(text)){
            Workflow workflow = Workflow(workflow_record);
            workflows.emplace(workflow.get_name(), workflow);
        }
    }
    long long int number_of_accepted_combinations(){
        long long int accepted_combinations = 0;
        std::vector<std::pair<Part, std::string>> workflows_to_run = {{ {Part(), "in"} }};
        while(!workflows_to_run.empty()){
            std::string workflow_name;
            Part p;
            std::tie(p, workflow_name) = workflows_to_run.back();
            workflows_to_run.pop_back();

            if (p.total_combinations()==0)
                continue;
            
            if (workflow_name=="R")
                continue;
            
            if (workflow_name=="A"){
                accepted_combinations += p.total_combinations();
                continue;
            }
            
            auto workflow_result = workflows.at(workflow_name).split_combinations(p);
            workflows_to_run.insert(workflows_to_run.end(), workflow_result.begin(), workflow_result.end());
        }
        return accepted_combinations;
    }
};

std::optional<std::string> solve(std::string_view input){
    System system(split(input, "\n\n")[0]);
    return std::to_string(system.number_of_accepted_combinations());
}

int main(int argc, [[maybe_unused]] char *argv[]) {
    Solution solution = Solution(19, 2, solve);
    if (argc > 1){
        return solution.test_run();
    }else{
        return solution.run();
    }
}
