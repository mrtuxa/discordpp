#include <string>

std::string uwufy(std::string text) {
    std::string result;
    char prev_char = ' ';
    for (char c: text) {
        switch (c) {
            case 'l':
            case 'r':
                // Replace 'l' and 'r' with 'w'
                result.push_back('w');
                prev_char = c;
                break;
            case 'L':
            case 'R':
                // Replace 'L' and 'R' with 'W'
                result.push_back('W');
                prev_char = c;
                break;
            case 'n':
                // Replace 'n' with 'ny'
                result.push_back('n');
                result.push_back('y');
                prev_char = c;
                break;
            case 'N':
                // Replace 'N' with 'Ny'
                result.push_back('N');
                result.push_back('y');
                prev_char = c;
                break;
            default:
                // Otherwise, just add the character to the result
                result.push_back(c);
                prev_char = c;
                break;
        }
    }
    return result;
}