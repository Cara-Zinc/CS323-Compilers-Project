import os 
import argparse

run_parser_cmd_base = "./structura_parser < "
argparser = argparse.ArgumentParser()
argparser.add_argument("--tests_dir", default="~/CS323-Compilers-Project/tests/ext_test_cases/", help="Directory containing test files")
args = argparser.parse_args()
tests_dir = args.tests_dir
tests_dir = os.path.expanduser(tests_dir)

def run_parser_on_test(test_name):
    if not os.path.exists("results_ext"):
        os.system("mkdir results_ext")
    run_parser_cmd = f"{run_parser_cmd_base}{tests_dir}{test_name}.stra > results_ext/my_{test_name}.out"
    os.system(run_parser_cmd)

def is_the_same_ast(my_lines, expected_lines):
    if len(my_lines) != len(expected_lines):
        print("Different number of lines")
        print(f"Expected: {len(expected_lines)}")
        print(f"Got: {len(my_lines)}")
        return False
    i = 0
    for my_line, expected_line in zip(my_lines, expected_lines):
        i += 1
        if my_line != expected_line:
            print(f"Line {i} of AST is different:")
            print(f"Expected: {expected_line}")
            print(f"Got: {my_line}")
            return False
    return True

def run_parser_on_all_tests():
    for file in os.listdir(tests_dir):
        if file.endswith(".stra"):
            test_name = file.split(".")[0]
            print("=====================================================")
            print(f"Source code for {test_name}:")
            with open(f"{tests_dir}{test_name}.stra", "r") as f:
                for i, line in enumerate(f):
                    print(f"[{i+1}] {line}", end="")
            print('\n')
            print(f"=========== Running parser on {test_name} ===========")
            print("Your output:")
            run_parser_on_test(test_name)
            print(f"=========== Done running parser on {test_name} ===========")
            print('\n')
            print("=========== Expected output: ===========")
            os.system(f"cat {tests_dir}{test_name}_out.txt")
            print("=====================================================")
            try:
                flag = input("Print AST? (y/n): ")
            except EOFError:
                flag = "y"
            if flag == "y":
                print("=========== AST: ===========")
                os.system(f"cat results_ext/my_{test_name}.out")
            print("========= Comparing ASTs =========")
            try:
                with open(f"results_ext/my_{test_name}.out", "r", errors="ignore") as my_out, open(f"{tests_dir}{test_name}_out.txt", "r", errors="ignore") as expected_out:
                    my_lines = my_out.readlines()
                    expected_lines = expected_out.readlines()
                    if expected_lines[0].strip().startswith("Error"):
                        print("Aborting comparison because expected output is an error message")
                    elif is_the_same_ast(my_lines, expected_lines):
                        print("ASTs are the same!")
                    else:
                        print("ASTs are different. However, this does not necessarily mean that your parser is incorrect. Please check the ASTs manually.")
            except FileNotFoundError:
                print("Expected output file not found")
            try:
                input("Press Enter to continue...")
            except EOFError:
                pass

run_parser_on_all_tests()