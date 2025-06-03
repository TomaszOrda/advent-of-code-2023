#include <string_view>
#include <vector>
#include <optional>

std::vector<std::string_view> split_lines(std::string_view text);
std::vector<std::string_view> split(std::string_view text, char delimiter);
std::vector<std::string_view> split(std::string_view text, std::string_view delimiter);
int svtoi(std::string_view s);

template<typename T>
class Grid{
public:
    Grid(int width, int height)
        : width {width}
        , height {height}
        , data {std::vector<T>(width*height)}
    {
    }
    Grid(std::string_view) requires(std::is_same_v<T, char>);
    std::optional<T> at(int x, int y) const;
    void replace(int x, int y, T value);
    int get_width() const{
        return height;
    }
    int get_height() const{
        return width;
    }
private:
    int width{};
    int height{};
    std::vector<T> data{};
};

template<typename T>
inline std::optional<T> Grid<T>::at(int x, int y) const {
    if (0<=y && y<height && 0<=y && y<height){
        return data[static_cast<size_t>(y*width + x)];
    }else{
        return std::nullopt;
    }
}
template<typename T>
inline void Grid<T>::replace(int x, int y, T value){
    data[y*width + x] = value;
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