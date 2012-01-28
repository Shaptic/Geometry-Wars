#include <string>
#include <sstream>
#include <vector>

/* Get the number of numbers in a number */
int num_len(const int number);

/* Convert a string to its uppercase equivalent.
 * Comes in two flavors, one modifies the original
 * string, whereas the other creates a new one.
 */
void        toupper(std::string& str);
std::string toupper_ret(const std::string& str);

/* Add a string an a C-string together into one string */
std::string combine(const std::string& str1, const char* str2);

/* Split a string by a certain character, returns
 * a vector of all of the split strings.
 */
std::vector<std::string> split(const std::string& str, char token);