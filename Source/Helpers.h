#include <string>
#include <vector>

/* Get the number of numbers in a number */
int num_len(const int number);

/* Convert a string to its uppercase equivalent.
 * Comes in two flavors, one modifies the original
 * string, whereas the other creates a new one.
 */
void        toupper(std::string& str);
std::string toupper_ret(const std::string& str);

/* Split a string by a certain character, returns
 * a vector of all of the split strings.
 */
std::vector<std::string> split(const std::string& str, char token);