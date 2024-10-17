import re
from itertools import product

# Function to parse and evaluate the boolean function
def parse_boolean_function(level, target, boolean_expression, input_values):
    # Convert the input string into a dictionary of boolean variables
    variables = {}
    for i in range(1, level + 1):
        variables[f'x{i}'] = bool(int(input_values[i - 1]))

    # Replace variable names with their corresponding values in the boolean expression
    for i in range(1, level + 1):
        boolean_expression = re.sub(rf'\bx{i}\b', str(variables[f'x{i}']), boolean_expression)

    # Replace boolean operators with Python syntax
    boolean_expression = boolean_expression.replace('\/', ' or ').replace('/\\', ' and ').replace('~', ' not ')

    # Evaluate the boolean expression
    boolean_result = eval(boolean_expression)

    # XOR result with the value of the target (e.g., f1 corresponds to x1)
    target_var = f'x{target[1:]}'
    xor_result = boolean_result ^ variables[target_var]

    return boolean_result, xor_result

# Function to try all possible combinations of input values
def try_all_combinations(level, target, boolean_expression):
    # Generate all possible binary combinations for the given level
    combinations = list(product([0, 1], repeat=level))

    print(f"Trying all {2**level} possible combinations for {level} variables...\n")

    changed_pairs = set()  # To store unique pairs of input and output that have changed

    for combination in combinations:
        # Convert tuple to a string of binary values (e.g., (0,1,1) -> "011")
        input_values = ''.join(map(str, combination))

        # Parse and evaluate the boolean function for this combination
        boolean_result, xor_result = parse_boolean_function(level, target, boolean_expression, input_values)

        # Format the output result by updating only the target bit
        output_values = list(input_values)
        target_index = int(target[1:]) - 1  # Target bit index (e.g., f1 -> x1 at index 0)
        output_values[target_index] = str(int(xor_result))  # Update only the target bit
        output_values = ''.join(output_values)

        # Display the result, aligned with proper formatting
        print(f"Input: {input_values} -> Boolean result: {int(boolean_result)}, XOR with {target}: {int(xor_result)} -> Output: {output_values}")

        # Check if the input and output differ; if so, record the pair in sorted order
        if input_values != output_values:
            pair = tuple(sorted([input_values, output_values]))
            changed_pairs.add(pair)

    # Print the unique pairs that have changed at the end of the output, converting to integers
    if changed_pairs:
        print("\nChanged pairs (input, output) as integers (each incremented by 1):")
        sorted_pairs = sorted((int(pair[0], 2) + 1, int(pair[1], 2) + 1) for pair in changed_pairs)
        for pair in sorted_pairs:
            print(f"({pair[0]}, {pair[1]})")

# Main function
def main():
    # Get the level of the boolean function
    level = int(input("Enter the level of the boolean function (number of variables): "))

    # Get the combined target and boolean expression input
    target_and_expression = input("Enter the boolean function with the target (e.g., f1 = x2 \\/ x3): ")

    # Split the input into target and boolean expression
    target, boolean_expression = target_and_expression.split(' = ')

    # Try all possible combinations of input values
    try_all_combinations(level, target, boolean_expression)

if __name__ == "__main__":
    main()
