#include <string_view>
#include <vector>
#include <optional>
#include <algorithm>
#include <iostream>
#include <functional>

std::vector<std::string_view> split_lines(std::string_view text, bool only_nonempty=false);
std::vector<std::string_view> split(std::string_view text, char delimiter, bool only_nonempty=false);
std::vector<std::string_view> split(std::string_view text, std::string_view delimiter, bool only_nonempty=false);
int svtoi(std::string_view s);
long long int svtoll(std::string_view s);
double svtod(std::string_view s);
int is_digit(char c);
int digit_to_int(char c);
int hex_digit_to_int(char c);
int hex_to_int(std::string_view hex);

class Coord{
public:
    int first;
    int second;
    constexpr Coord(): first{0}, second{0} {};
    constexpr Coord(std::pair<int, int> pair) : first{pair.first}, second{pair.second} {};
    constexpr Coord(int x, int y) : first{x}, second{y}{}
    Coord operator+(const Coord& other) const;
    Coord operator*(const int& scalar) const;
    Coord operator-(const Coord& other) const;
    Coord operator-() const;
    bool operator==(const Coord& other) const;
    bool operator<(const Coord& other) const;
    Coord rotate_left() const;
    Coord rotate_right() const;
    int manhattan_distance() const;
    operator std::pair<int, int>() const{
        return {first, second};
    };
    static const Coord North;
    static const Coord South;
    static const Coord East;
    static const Coord West;
};
Coord operator*(int scalar, const Coord& obj);

template<>
struct std::hash<Coord>{
    std::size_t operator()(const Coord& coord) const{
        //hash conflicts when second is exactly UINT16_MAX bigger than first
        return std::hash<int>{}(coord.first)^std::hash<int>{}(coord.second + UINT16_MAX);
    }
};

template<typename T>
class Grid{
public:
    Grid(int width, int height)
        : width {width}
        , height {height}
        , data {std::vector<T>(static_cast<size_t>(width*height))}
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
    std::optional<T> at(Coord c) const;
    void replace(int x, int y, T value);
    void replace(Coord c, T value);
    void update(int x, int y, std::function<void(T&)> func);
    void update(Coord c, std::function<void(T&)> func);
    int get_width() const{
        return width;
    }
    int get_height() const{
        return height;
    }
    Coord find(T value) const{
        long long int flat_coordinate {std::find(data.begin(), data.end(), value) - data.begin()};
        return flat_to_coord(flat_coordinate);
    }
    void print() requires(std::is_same_v<T, char>){
        for (int y{0}; y<get_height(); y++){
            for (int x{0}; x<get_width(); x++)
                std::cout<<at(x,y).value();
            std::cout<<'\n';
        }
    }
private:
    Coord flat_to_coord(long long int flat) const{
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
std::optional<T> Grid<T>::at(Coord c) const{
    return at(c.first, c.second);
}
template<typename T>
inline void Grid<T>::replace(int x, int y, T value){
    data[x_y_to_flat(x, y)] = value;
}
template<typename T>
inline void Grid<T>::replace(Coord c, T value){
    replace(c.first, c.second, value);
}
template<typename T>
void Grid<T>::update(int x, int y, std::function<void(T&)> func){
    func(data[x_y_to_flat(x,y)]);
}
template<typename T>
void Grid<T>::update(Coord c, std::function<void(T&)> func){
    update(c.first, c.second, func);
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