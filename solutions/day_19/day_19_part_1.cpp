#include "../runner.hpp"
#include "../utils.hpp"
#include <string>
#include <optional>
#include <unordered_map>
#include <ranges>

class Part{
    int x{};
    int m{};
    int a{};
    int s{};
public:
    Part(std::string_view text){
        auto text_parts = split(text.substr(1, text.length()-2), ',');
        x = svtoi(text_parts[0].substr(2, text_parts[0].length()-2));
        m = svtoi(text_parts[1].substr(2, text_parts[1].length()-2));
        a = svtoi(text_parts[2].substr(2, text_parts[2].length()-2));
        s = svtoi(text_parts[3].substr(2, text_parts[3].length()-2));
    }
    int get_category(char c) const{
        switch (c)
        {
        case 'x':
            return x;
        case 'm':
            return m;
        case 'a':
            return a;
        case 's':
            return s;
        default:
            throw std::runtime_error("Unknown category");
        }
    }
    int sum_of_parameters() const{
        return x + m + a + s;
    }
};

class Rule{
    std::function<bool(int, int)> cmp{};
    int value{};
    char category{};
    std::string result{};
public:
    Rule(std::string_view text){
        // std::cout<<text<<'\n';
        if (!text.contains(':')){
            cmp = [](int, int){return true;};
            result = text;
            category = 'x';
        }else{
            category = text[0];
            cmp = text[1] == '<' ? [](int a, int b){return a<b;} : [](int a, int b){return a>b;};
            auto text_parts = split(text, ':');
            value = svtoi(text_parts[0].substr(2, text_parts[0].length()-2));
            result = std::string(text_parts[1]);
        }
    }
    std::optional<std::string> apply(Part p) const{
        if (cmp(p.get_category(category),value)){
            return result;
        }else{
            return std::nullopt;
        }
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
    std::string apply(Part part){
        for (const auto& rule : rules){
            auto rule_result = rule.apply(part);
            if (rule_result.has_value())
                return rule_result.value();
            
        }
        throw std::runtime_error("No fitting rule found");
    }
    std::string_view get_name() const{
        return name;
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
    bool is_accepted(Part part){
        std::string next_workflow = "in";
        while(true){
            if (next_workflow == "A"){
                return true;
            }
            if (next_workflow == "R"){
                return false;
            }
            next_workflow = workflows[next_workflow].apply(part);
        }
    }
};

std::optional<std::string> solve(std::string_view input){
    auto input_parts = split(input, "\n\n");
    System system(input_parts[0]);
    std::vector<Part> parts{};
    for (const auto& part_record : split_lines(input_parts[1])){
        parts.push_back(Part(part_record));
    }
    int sum_of_accepted_parts_parameters = 0;
    for (const auto& part : parts){
        sum_of_accepted_parts_parameters += system.is_accepted(part) ? part.sum_of_parameters() : 0;
    }
    return std::to_string(sum_of_accepted_parts_parameters);
}

int main(int argc, [[maybe_unused]] char *argv[]) {
    Solution solution = Solution(19, 1, solve);
    if (argc > 1){
        return solution.test_run();
    }else{
        return solution.run();
    }
}
