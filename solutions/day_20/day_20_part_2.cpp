#include "../runner.hpp"
#include "../utils.hpp"
#include <string>
#include <optional>
#include <algorithm>
#include <ranges>
#include <deque>
#include <numeric>

//The comments below contain major spoilers for this problem.

//This problem requires looking at the structure of the input.
//As far as I know the solutions are not expected to be vaiable for any input.
//Only at best for the class of inputs that are expected basing on the input got.
//A major assuption is that there is a conjunction connected only to rx, and only this conjunction is connected to it, name it A
//There are a few conjunctions connected to A and only those, name them B
//There is one conjunction connected to each in B, call them C
//Those conjunctions are connected to a bunch of flipflops which are on their own connected in a chain
//The flipflops work as a counter. When it reaches a certain number C fires, then B (with appropriate pulses).
//As they fire the counter is reset to 0
//When all C fire at once, only then rx gets low input
//So the answer is the lcm of all of the counters reset/target values

class Signal{
public:
    std::string sender;
    std::string receiver;
    bool pulse;
    Signal(std::string_view sender, std::string_view receiver, bool pulse) 
        : sender{sender}
        , receiver{receiver}
        , pulse{pulse}
    {}
    friend std::ostream& operator<<(std::ostream& os, const Signal& signal);
};
std::ostream& operator<<(std::ostream& os, const Signal& signal) {
    os<<signal.sender<<" "<< (signal.pulse ? "-high" : "-low") << "-> "<<signal.receiver;
    return os;
}



class Module{
public:
    virtual ~Module() noexcept = default;
    virtual void add_input(std::string input_name) = 0;
    virtual std::vector<Signal> receive(Signal p) = 0;
    virtual std::vector<std::string> get_receivers() const = 0;
    virtual std::string get_name() const = 0;
    virtual void print() const =0;
    virtual char get_type() const = 0;
};

class Broadcaster : public Module{
    std::vector<std::string> receivers{};
public:
    std::string name{"broadcaster"};
    Broadcaster(){};
    Broadcaster(std::string_view text){
        for (const auto& receiver : split(split(text, "-> ")[1], ", ")){
            receivers.push_back(std::string(receiver));
        }
    }
    std::vector<Signal> receive([[maybe_unused]] Signal p) override {
        std::vector<Signal> result{};
        for (const auto& receiver : receivers){
            result.push_back(Signal(name, receiver, false));
        }
        return result;
    }
    void add_input([[maybe_unused]] std::string input_name) override{
    }
    std::vector<std::string> get_receivers() const override {
        return receivers;
    }
    std::string get_name() const override {
        return name;
    }
    void print() const override{
    };
    char get_type() const{
        return 'b';
    };
};

class Conjunction : public Module{
    std::string name{};
    std::vector<std::string> receivers{};
    std::unordered_map<std::string, bool> received_signal{};
public:
    Conjunction(std::string_view text){
        auto parts = split(text.substr(1, text.length()-1), " -> ");
        name = std::string(parts[0]);
        for(const auto& receiver : split(parts[1], ", "))
            receivers.push_back(std::string(receiver));
    };
    void add_input(std::string input_name) override {
        received_signal.emplace(input_name, false);
    }
    std::vector<Signal> receive(Signal p) override {
        received_signal[p.sender] = p.pulse;
        std::vector<Signal> result{};
        bool output_pulse = !std::all_of(received_signal.begin(), received_signal.end(), [](auto x){return x.second;});
        
        for (const auto& receiver : receivers)
            result.push_back(Signal(name, receiver, output_pulse));
        return result;
    }
    std::string get_name() const override {
        return name;
    }
    std::vector<std::string> get_receivers() const override {
        return receivers;
    }
    void print() const override{
        for(const auto& [_, s] : received_signal){
            std::cout<<s<<" ";
        }
    };
    char get_type() const{
        return '&';
    };
};

class FlipFlop : public Module{
    std::string name{};
    std::vector<std::string> receivers{};
public:
    bool is_on{false};
    FlipFlop(std::string_view text) {
        auto parts = split(text.substr(1, text.length()-1), " -> ");
        name = std::string(parts[0]);
        for(const auto& receiver : split(parts[1], ", "))
            receivers.push_back(std::string(receiver));
    };
    void add_input([[maybe_unused]] std::string input_name) override {
    }
    std::vector<Signal> receive(Signal p) override {
        if (p.pulse){
            return {};
        }else{
            is_on = !is_on;
            std::vector<Signal> result{};
            for (const auto& receiver : receivers)
                result.push_back(Signal(name, receiver, is_on));
            return result;
        }
    }
    std::string get_name() const override {
        return name;
    }
    std::vector<std::string> get_receivers() const override {
        return receivers;
    }
    void print() const override{
        std::cout<<is_on<<" ";
    };
    char get_type() const{
        return '%';
    };
};

class FlipFlopCycle{
public:
    uint64_t length{};
    FlipFlopCycle(uint64_t target) 
     : length{target}
    {}
    FlipFlopCycle(std::string_view start, std::unordered_map<std::string, std::unique_ptr<Module>>& modules){
        std::string conjunction{};
        std::string current {start};
        while (true){
            for (const auto& m_name : modules[current]->get_receivers()){
                if (modules[m_name]->get_type() == '&'){
                    conjunction = m_name;
                }
            }
            if (conjunction.length()==0){
                current = modules[current]->get_receivers()[0];
            }else{
                break;
            }
        }

        current = start;
        int bit_pos = -1;
        while (true){
            bit_pos++;

            std::vector<std::string> fp_receivers {modules[current]->get_receivers()};  
            if (std::find(fp_receivers.begin(), fp_receivers.end(), conjunction) != fp_receivers.end())
                length |= 1 << bit_pos;
            
            if (fp_receivers.size() == 1 && fp_receivers[0] == conjunction)
                break;
            else{
                if(fp_receivers[0] != conjunction){
                    current = modules[current]->get_receivers()[0];
                }else{
                    current = modules[current]->get_receivers()[1];
                }
            }
            
        }
    }
    FlipFlopCycle combine(FlipFlopCycle& other){
        return FlipFlopCycle(static_cast<uint64_t>(std::lcm(length, other.length)));
    }
};

class Cables{
    std::unordered_map<std::string, std::unique_ptr<Module>> modules{};
public:
    Cables(std::string_view text){
        for (const auto& line : split_lines(text)){
            if (line[0] == 'b'){
                Broadcaster broadcaster{line};
                modules.emplace(broadcaster.get_name(),  std::make_unique<Broadcaster>(broadcaster));
            }else if (line[0] == '%'){
                FlipFlop flipflop{line};
                modules.emplace(flipflop.get_name(),  std::make_unique<FlipFlop>(flipflop));
            }else if (line[0] == '&'){
                Conjunction conjuction{line};
                modules.emplace(conjuction.get_name(), std::make_unique<Conjunction>(conjuction));
            }else {
                throw std::runtime_error("Unknown module type");
            }
        }   
        for (auto& [_, m] : modules){
            for (const auto& receiver : m->get_receivers()){
                if (modules.contains(receiver))
                    modules[receiver]->add_input(m->get_name());
            }
        }
    }
    
    unsigned long long int button_pushes_to_turn_machine_on(){
        std::vector<FlipFlopCycle> relays{};
        for (const auto& relay : modules["broadcaster"]->get_receivers())
            relays.push_back(FlipFlopCycle(relay, modules));
        
        FlipFlopCycle result_relay{relays[0]};
        for (size_t i{1}; i<relays.size(); i++)
            result_relay = result_relay.combine(relays[i]);
        
        return static_cast<unsigned long long int>(result_relay.length);
    }
};

std::optional<std::string> solve(std::string_view input){
    Cables cables{input};
    return std::to_string(cables.button_pushes_to_turn_machine_on());
}

int main(int argc, [[maybe_unused]] char *argv[]) {
    Solution solution = Solution(20, 2, solve);
    if (argc > 1){
        return 0; //No test input for this part
    }else{
        return solution.run();
    }
}
