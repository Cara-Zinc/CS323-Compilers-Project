import os 

run_parser_cmd_base = "./structura_parser < "
tests_dir = "~/CS323-2024F/project/phase1/"
tests_dir = os.path.expanduser(tests_dir)

def run_parser_on_test(test_name):
    run_parser_cmd = f"{run_parser_cmd_base}{tests_dir}{test_name}.spl > results/my_{test_name}.out"
    os.system(run_parser_cmd)

def run_parser_on_all_tests():
    for file in os.listdir(tests_dir):
        if file.endswith(".spl"):
            test_name = file.split(".")[0]
            print("=====================================================")
            print(f"Source code for {test_name}:")
            # os.system(f"cat {tests_dir}{test_name}.spl")
            with open(f"{tests_dir}{test_name}.spl", "r") as f:
                for i, line in enumerate(f):
                    print(f"[{i+1}] {line}", end="")
            print('\n')
            print(f"=========== Running parser on {test_name} ===========")
            print("Your output:")
            run_parser_on_test(test_name)
            print(f"=========== Done running parser on {test_name} ===========")
            # os.system(f"cat results/my_{test_name}.out")
            print("Expected output:")
            os.system(f"cat {tests_dir}{test_name}.out")
            print("=====================================================")
            input("Press Enter to continue...")

run_parser_on_all_tests()