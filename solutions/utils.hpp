#include <string_view>
#include <vector>
#include <optional>
#include <algorithm>
#include <iostream>

std::vector<std::string_view> split_lines(std::string_view text, bool only_nonempty=false);
std::vector<std::string_view> split(std::string_view text, char delimiter, bool only_nonempty=false);
std::vector<std::string_view> split(std::string_view text, std::string_view delimiter, bool only_nonempty=false);
int svtoi(std::string_view s);
long long int svtoll(std::string_view s);

template<typename T>
class Grid{
public:
    Grid(int width, int height)
        : width {width}
        , height {height}
        , data {std::vector<T>(width*height)}
    {
    }
    Grid(int width, int height, T value)
        : width {width}
        , height {height}
        , data {std::vector<T>(static_cast<size_t>(width*height), value)}
    {
    }
    Grid(std::string_view) requires(std::is_same_v<T, char>);
    std::optional<T> at(int x, int y) const;
    void replace(int x, int y, T value);
    int get_width() const{
        return width;
    }
    int get_height() const{
        return height;
    }
    std::pair<int, int> find(T value) const{
        long long int flat_coordinate {std::find(data.begin(), data.end(), value) - data.begin()};
        return flat_to_x_y(flat_coordinate);
    }
    void print() requires(std::is_same_v<T, char>){
        for (int y{0}; y<get_height(); y++){
            for (int x{0}; x<get_width(); x++)
                std::cout<<at(x,y).value();
            std::cout<<'\n';
        }
    }
private:
    std::pair<int, int> flat_to_x_y(long long int flat) const{
        return {static_cast<int>(flat)%width, static_cast<int>(flat)/width};
    }
    size_t x_y_to_flat(int x, int y) const{
        return static_cast<size_t>(x + width*y);
    }
    int width{};
    int height{};
    std::vector<T> data{};
};

template<typename T>
inline std::optional<T> Grid<T>::at(int x, int y) const {
    if (0<=y && y<height && 0<=x && x<width){
        return data[x_y_to_flat(x,y)];
    }else{
        return std::nullopt;
    }
}
template<typename T>
inline void Grid<T>::replace(int x, int y, T value){
    data[x_y_to_flat(x, y)] = value;
}
template<>
inline Grid<char>::Grid(std::string_view text){
    //Assume the text is a valid grid
    std::vector<std::string_view> lines = split_lines(text);
    width = static_cast<int>(lines[0].length());
    height = static_cast<int>(lines.size());
    for (const std::string_view& line : lines){
        //This is expensive
        //try data.insert(data.end(), line.begin(), line.end());
        for (const char& c : line) {
            data.push_back(c);
        }
    }
}