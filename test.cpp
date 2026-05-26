#include <unordered_map>

using namespace std;

int main(int argc, char *argv[]) {
    // Test whether iterator does not iterate over elements erased after its creation
    unordered_map alph = unordered_map<int, char>{{2, 'b'}, {5, 'e'}};
    auto it = alph.begin();
    alph.erase(++alph.begin());
    return ++it != alph.end();  // Return 0 if passed
}
