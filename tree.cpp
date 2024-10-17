#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <sstream>
#include <stack>

using namespace std;

// Leaf swap pairs for each function f1 = xN
vector<pair<int, int> > f1_x2 = {{9, 25}, {10, 26}, {11, 27}, {12, 28}, {13, 29}, {14, 30}, {15, 31}, {16, 32}};
vector<pair<int, int> > f1_x3 = {{5, 21}, {6, 22}, {7, 23}, {8, 24}, {13, 29}, {14, 30}, {15, 31}, {16, 32}};
vector<pair<int, int> > f1_x4 = {{3, 19}, {4, 20}, {7, 23}, {8, 24}, {11, 27}, {12, 28}, {15, 31}, {16, 32}};
vector<pair<int, int> > f1_x5 = {{2, 18}, {4, 20}, {6, 22}, {8, 24}, {10, 26}, {12, 28}, {14, 30}, {16, 32}};

// Function to convert vector of pairs to set for easy union/intersection
set<pair<int, int> > toSet(const vector<pair<int, int> >& vec) {
    return set<pair<int, int> >(vec.begin(), vec.end());
}

// Function to compute the union of two sets
set<pair<int, int> > setUnion(const set<pair<int, int> >& a, const set<pair<int, int> >& b) {
    set<pair<int, int> > result;
    set_union(a.begin(), a.end(), b.begin(), b.end(), inserter(result, result.begin()));
    return result;
}

// Function to compute the intersection of two sets
set<pair<int, int> > setIntersection(const set<pair<int, int> >& a, const set<pair<int, int> >& b) {
    set<pair<int, int> > result;
    set_intersection(a.begin(), a.end(), b.begin(), b.end(), inserter(result, result.begin()));
    return result;
}

// Apply the swap operation on the leaf nodes
vector<int> applySwaps(const vector<int>& leaves, const set<pair<int, int> >& swaps) {
    vector<int> result = leaves;
    for (const auto& p : swaps) {
        swap(result[p.first - 1], result[p.second - 1]);
    }
    return result;
}

// Parse the input expression and compute the result set
set<pair<int, int>> parseAndCompute(string expr) {
    // Initializing with sets for f1_x2, f1_x3, f1_x4, f1_x5
    set<pair<int, int>> set_x2 = toSet(f1_x2);
    set<pair<int, int>> set_x3 = toSet(f1_x3);
    set<pair<int, int>> set_x4 = toSet(f1_x4);
    set<pair<int, int>> set_x5 = toSet(f1_x5);

    stack<set<pair<int, int>>> operands;  // Stack to hold operands
    stack<string> operators;  // Stack to hold operators
    
    // Tokenize the input expression
    stringstream ss(expr);
    string token;

    while (ss >> token) {
    // cout << token << "\n";
        if (token == "x2") {
            operands.push(set_x2);
        } else if (token == "x3") {
            operands.push(set_x3);
        } else if (token == "x4") {
            operands.push(set_x4);
        } else if (token == "x5") {
            operands.push(set_x5);
        } else if (token == "or" || token == "and") {
            operators.push(token);
        } else if (token == "(") {
            // Ignore opening parenthesis
        } else if (token == ")") {
            // Process the operation when closing parenthesis is encountered
            while (!operators.empty() && operators.top() != "(") {
                string op = operators.top();
                // cout << op << "\n";
                operators.pop();
                set<pair<int, int>> b = operands.top();
                operands.pop();
                set<pair<int, int>> a = operands.top();
                operands.pop();
                
                if (op == "or") {
                    operands.push(setUnion(a, b));
                } else if (op == "and") {
                    operands.push(setIntersection(a, b));
                }
            }
            if (!operators.empty() && operators.top() == "(") {
                operators.pop();  // Pop the matching opening parenthesis
            }
        }
    }

    // If there are still operators left, apply them
    while (!operators.empty()) {
        string op = operators.top();
        operators.pop();
        set<pair<int, int>> b = operands.top();
        operands.pop();
        set<pair<int, int>> a = operands.top();
        operands.pop();
        
        if (op == "or") {
            operands.push(setUnion(a, b));
        } else if (op == "and") {
            operands.push(setIntersection(a, b));
        }
    }

    // The final result set
    return operands.top();
}

int main() {
    // Initial leaf nodes 1 to 32
    vector<int> leaves(32);
    for (int i = 0; i < 32; ++i) {
        leaves[i] = i + 1;
    }

    // Example expression to parse and compute
    // string expr = "( x2 and x4 ) or ( x2 and x3 ) or ( x3 and x5 )";
    string expr = "( x2 and x3 ) or ( x3 and x4 and x5 )";
    
    // Compute the final set of swaps based on the expression
    set<pair<int, int> > finalSwaps = parseAndCompute(expr);

    // Apply the swaps to the leaf nodes
    vector<int> result = applySwaps(leaves, finalSwaps);

    // Output the final leaf nodes order
    cout << expr << endl;
    cout << "Final leaf nodes order: " << endl;
    for (int leaf : result) {
        cout << leaf << " ";
    }
    cout << endl;

    return 0;
}