#include "../runner.hpp"
#include "../utils.hpp"
#include <string>
#include <optional>
#include <unordered_map>
#include <unordered_set>
#include <cstdlib>
#include <ctime>


class Edge{
public:
    std::string_view first{};
    std::string_view second{};
    Edge(std::string_view node_a, std::string_view node_b){
        if (node_a < node_b){
            first = node_a;
            second = node_b;
        }else{
            first = node_b;
            second = node_a;
        }
    }
    bool operator==(const Edge& other) const{
        return first == other.first && second == other.second;
    }
};

template <>
struct std::hash<Edge> {
    std::size_t operator()(const Edge& p) const {
        return std::hash<std::string_view>()(p.first) ^ (std::hash<std::string_view>()(p.second));
    }
};

class Wiring{
    std::unordered_set<std::string> all_nodes{};
    
    std::unordered_map<std::string_view, std::unordered_set<std::string_view>> connected_wires{};
    std::unordered_map<std::string_view, std::vector<std::string_view>> connected_wires_vec{};
    std::vector<std::string_view> all_nodes_vec{};
    std::unordered_map<Edge, int> edges{};
public:
    Wiring(std::string_view text){
        for (const auto& line : split_lines(text)){
            const auto parts = split(line, ": ");
            auto [component_it, _] = all_nodes.emplace(parts[0]);
            for (const auto& connected_component : split(parts[1], ' ')){
                all_nodes.emplace(connected_component);
                auto [connected_it, _] = all_nodes.emplace(std::string(connected_component));
                connected_wires[*component_it].emplace(*connected_it);
                connected_wires[*connected_it].emplace(*component_it);
                edges.emplace(Edge(*component_it, *connected_it), 0);
            }
        }
        all_nodes_vec.insert(all_nodes_vec.end(), all_nodes.begin(), all_nodes.end());
        for (const auto& [key, value] : connected_wires){
            connected_wires_vec.emplace(key, std::vector<std::string_view>{});
            connected_wires_vec[key].insert(connected_wires_vec[key].end(), value.begin(), value.end());
        }
    }
    long long unsigned int product_of_three_cut_parts(){
        long long unsigned int left_side = all_nodes.size();
        std::vector<Edge> edges_vec{};
        edges_vec.reserve(edges.size());
        for (const auto& [e, _] : edges)
            edges_vec.push_back(e);
        while(true){
            for (long long unsigned int i{}; i<18; i++){
                increase_random_spanning_tree();
            }
            std::partial_sort(edges_vec.begin(), edges_vec.begin()+3, edges_vec.end(), [this](auto a, auto b){return edges[a] > edges[b];});

            remove_top_three_edges(edges_vec);
            if(is_cut(edges_vec[0].first, edges_vec[0].second)){
                break;
            }
            add_top_three_edges(edges_vec);
        }
        left_side = one_part_connected_nodes();
        return left_side * (all_nodes.size() - left_side);
    }
    void remove_top_three_edges(std::vector<Edge> to_remove){
        for(size_t i{}; i<3; i++){
            connected_wires[to_remove[i].first].erase(to_remove[i].second);
            connected_wires[to_remove[i].second].erase(to_remove[i].first);
        }
    }
    void add_top_three_edges(std::vector<Edge> to_add){
        for(size_t i{}; i<3; i++){
            connected_wires[to_add[i].first].emplace(to_add[i].second);
            connected_wires[to_add[i].second].emplace(to_add[i].first);
        }
    }
    void increase_random_spanning_tree(){
        std::string_view start = all_nodes_vec[static_cast<size_t>(rand()) % all_nodes.size()];
        std::unordered_set<std::string_view> visited{start};
        std::vector<Edge> neighbours{};
        for (const auto& neighbour : connected_wires[start])
            neighbours.push_back(Edge(start, neighbour));
        neighbours.reserve(all_nodes.size());
        while(visited.size()!=all_nodes.size()){
            size_t id = static_cast<size_t>(rand()) % neighbours.size();
            Edge current = neighbours[id];
            neighbours.erase(neighbours.begin() + static_cast<std::vector<Edge>::difference_type>(id));
            if (visited.contains(current.first) && visited.contains(current.second)){
                continue;
            }
            if(!visited.contains(current.first)){
                for (const auto& neighbour : connected_wires[current.first])
                    neighbours.push_back(Edge(current.first, neighbour));
                visited.emplace(current.first);
            }else{
                for (const auto& neighbour : connected_wires[current.second])
                    neighbours.push_back(Edge(current.second, neighbour));
                visited.emplace(current.second);
            }
            edges[current]++;
        }
    }
    long long unsigned int one_part_connected_nodes(){
        std::unordered_set<std::string_view> visited{*all_nodes.begin()};
        std::vector<std::string_view> stack {*all_nodes.begin()};
        while(!stack.empty()){
            std::string_view current = stack.back();
            stack.pop_back();
            for(const auto& neighbour : connected_wires[current]){
                if (visited.contains(neighbour))
                    continue;
                visited.emplace(neighbour);
                stack.push_back(neighbour);
            }
        }
        return visited.size();
    }
    int is_cut(std::string_view node_a, std::string_view node_b){
        std::unordered_set<std::string_view> visited{node_a};
        std::vector<std::string_view> stack {node_a};
        while(!stack.empty()){
            std::string_view current = stack.back();
            stack.pop_back();
            for(const auto& neighbour : connected_wires[current]){
                if (neighbour == node_b){
                    return false;
                }
                if (visited.contains(neighbour))
                    continue;
                visited.emplace(neighbour);
                stack.push_back(neighbour);
            }
        }
        return true;
    }
};

std::optional<std::string> solve(std::string_view input){
    return std::to_string(Wiring(input).product_of_three_cut_parts());
}

int main(int argc, [[maybe_unused]] char *argv[]) {
    Solution solution = Solution(25, 1, solve);
    if (argc > 1){
        return solution.test_run();
    }else{
        return solution.run();
    }
}
