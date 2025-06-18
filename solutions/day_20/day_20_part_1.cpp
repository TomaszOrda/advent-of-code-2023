#include "../runner.hpp"
#include "../utils.hpp"
#include <string>
#include <optional>
#include <algorithm>
#include <ranges>
#include <deque>

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
};

class Conjuction : public Module{
    std::string name{};
    std::vector<std::string> receivers{};
    std::unordered_map<std::string, bool> received_signal{};
public:
    Conjuction(std::string_view text){
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
};

class FlipFlop : public Module{
    std::string name{};
    std::vector<std::string> receivers{};
    bool is_on{false};
public:
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
};

class Cables{
    std::unordered_map<std::string, std::unique_ptr<Module>> modules{}; 
    int low_pulses{};
    int high_pulses{};
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
                Conjuction conjuction{line};
                modules.emplace(conjuction.get_name(), std::make_unique<Conjuction>(conjuction));
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
    void push_button(){
        std::deque<Signal> signals {};
        signals.push_back(Signal("button", "broadcaster", false));
        while(!signals.empty()){
            Signal signal = signals.front();
            signals.pop_front();

            if (signal.pulse){
                high_pulses+=1;
            }else{
                low_pulses+=1;
            }

            // std::cout<<signal<<'\n';
            if (!modules.contains(signal.receiver)){
                continue;
            }
            std::vector<Signal> signal_result = modules[signal.receiver]->receive(signal);
            signals.insert(signals.end(), signal_result.begin(), signal_result.end());
        }
    }
    int get_high_pulses(){
        return high_pulses;
    }
    int get_low_pulses(){
        return low_pulses;
    }
};

std::optional<std::string> solve(std::string_view input){
    Cables cables{input};
    for(int i{}; i<1000; i++)
        cables.push_button();
    return std::to_string(cables.get_high_pulses() * cables.get_low_pulses());
}

int main(int argc, [[maybe_unused]] char *argv[]) {
    Solution solution = Solution(20, 1, solve);
    if (argc > 1){
        solution.test_run();
        std::cout<<'\n';
        return solution.run("day_20_test_2.txt");
    }else{
        return solution.run();
    }
}
