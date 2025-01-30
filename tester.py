import subprocess
import os
from dataclasses import dataclass
from typing import Optional, List, Tuple

@dataclass
class TestCase:
    name: str
    pipex_cmd: List[str]
    bash_cmd: str
    input_data: Optional[str] = None
    custom_infile: Optional[str] = None

class Colors:
    GREEN = '\033[92m'
    RED = '\033[91m'
    YELLOW = '\033[93m'
    BLUE = '\033[94m'
    BOLD = '\033[1m'
    RESET = '\033[0m'

def print_colored(text: str, color: str) -> None:
    print(f"{color}{text}{Colors.RESET}")

def cleanup_files():
    """Remove test files if they exist and ensure they're gone"""
    files_to_remove = ['infile', 'outfile', 'outfile_bash']

    # First attempt to remove files
    for f in files_to_remove:
        try:
            if os.path.exists(f):
                os.remove(f)
        except Exception as e:
            print(f"{Colors.RED}Error removing {f}: {str(e)}{Colors.RESET}")

    # Verify files are actually gone
    remaining_files = [f for f in files_to_remove if os.path.exists(f)]
    if remaining_files:
        raise RuntimeError(f"Failed to remove files: {remaining_files}")

def create_test_files(content: str = None):
    """Create necessary test files with specified or default content"""
    # Ensure clean state first
    cleanup_files()
    default_content = """Hello World
This is a test file
Some random content
With multiple lines
Testing special chars: $#@!
Numbers: 123456789
Empty line follows

End of file"""

    with open('infile', 'w') as f:
        f.write(content if content else default_content)

def run_command(cmd: str, input_data: Optional[str] = None) -> Tuple[int, str, str]:
    """Execute command and return status, stdout, stderr"""
    try:
        if input_data:
            proc = subprocess.Popen(['bash', '-c', cmd],
                                stdin=subprocess.PIPE,
                                stdout=subprocess.PIPE,
                                stderr=subprocess.PIPE,
                                text=True)
            stdout, stderr = proc.communicate(input=input_data)
            return proc.returncode, stdout, stderr
        else:
            result = subprocess.run(['bash', '-c', cmd],
                                capture_output=True,
                                text=True)
            return result.returncode, result.stdout, result.stderr
    except Exception as e:
        return -1, "", str(e)

def read_file_content(filename: str) -> Tuple[bool, str]:
    """Read file content and return success status and content"""
    try:
        with open(filename, 'r') as f:
            return True, f.read()
    except Exception as e:
        return False, str(e)

def print_test_results(test: TestCase, pipex_result: Tuple[int, str, str],
                        bash_result: Tuple[int, str, str]) -> bool:
    """Print detailed test results and return success status"""
    def normalize(s: str) -> str:
        return s.replace('\r\n', '\n').strip()

    # Compare bash results
    status_match = pipex_result[0] == bash_result[0]
    stderr_match = normalize(pipex_result[2]) == normalize(bash_result[2])
    stdout_match = normalize(pipex_result[1]) == normalize(bash_result[1])

    # Compare output files
    pipex_file_success, pipex_file_content = read_file_content('outfile')
    bash_file_success, bash_file_content = read_file_content('outfile_bash')

    file_read_success = pipex_file_success and bash_file_success
    file_content_match = normalize(pipex_file_content) == normalize(bash_file_content) if file_read_success else False

    print(f"\n{Colors.BOLD}=== Results for: {test.name} ==={Colors.RESET}")
    print("\nPipex output:")
    print(f"Status: {pipex_result[0]}")
    print(f"Stdout: {pipex_result[1].strip()}")
    print(f"Stderr: {pipex_result[2].strip()}")

    print("\nBash output:")
    print(f"Status: {bash_result[0]}")
    print(f"Stdout: {bash_result[1].strip()}")
    print(f"Stderr: {bash_result[2].strip()}")

    print("\nOutput File Comparison:")
    if file_read_success:
        print("Pipex output file content:")
        print(pipex_file_content.strip())
        print("\nBash output file content:")
        print(bash_file_content.strip())
    else:
        print(f"{Colors.RED}Failed to read output files:{Colors.RESET}")
        if not pipex_file_success:
            print(f"Pipex file error: {pipex_file_content}")
        if not bash_file_success:
            print(f"Bash file error: {bash_file_content}")

    print(f"\n{Colors.BOLD}Match Results:{Colors.RESET}")
    print(f"Status match: {Colors.GREEN if status_match else Colors.RED}{status_match}{Colors.RESET}")
    print(f"Stderr match: {Colors.GREEN if stderr_match else Colors.RED}{stderr_match}{Colors.RESET}")
    print(f"Stdout match: {Colors.GREEN if stdout_match else Colors.RED}{stdout_match}{Colors.RESET}")
    print(f"File content match: {Colors.GREEN if file_content_match else Colors.RED}{file_content_match}{Colors.RESET}")

    success = status_match and stderr_match and stdout_match and file_content_match
    print(f"\n{Colors.GREEN if success else Colors.RED}Test {'PASSED' if success else 'FAILED'}{Colors.RESET}")
    return success

def run_test(test: TestCase) -> bool:
    """Run a single test case"""
    print(f"\n{Colors.BOLD}=== Running Test: {test.name} ==={Colors.RESET}")

    try:
        # Ensure clean state and setup test files
        create_test_files(test.custom_infile)

        # Verify test files are in correct state
        if not os.path.exists('infile'):
            raise RuntimeError("Failed to create infile for test")
        if os.path.exists('outfile') or os.path.exists('outfile_bash'):
            raise RuntimeError("Output files exist before test execution")

        # Format and execute commands
        formatted_pipex_cmd = ' '.join([test.pipex_cmd[0]] + [f'"{arg}"' for arg in test.pipex_cmd[1:]])
        print(f"{Colors.BLUE}Pipex command: {formatted_pipex_cmd}{Colors.RESET}")
        print(f"{Colors.BLUE}Bash command:  {test.bash_cmd}{Colors.RESET}")

        if test.input_data:
            print(f"{Colors.YELLOW}Input data: {test.input_data}{Colors.RESET}")

        pipex_result = run_command(formatted_pipex_cmd, test.input_data)
        bash_result = run_command(test.bash_cmd, test.input_data)

        return print_test_results(test, pipex_result, bash_result)

    except Exception as e:
        print(f"{Colors.RED}Test setup failed: {str(e)}{Colors.RESET}")
        return False

def get_test_cases() -> List[TestCase]:
    return [
        TestCase("Basic test - cat and wc",
                ["./pipex", "infile", "cat", "wc -l", "outfile"],
                "< infile cat | wc -l > outfile_bash"),

        TestCase("Multiple commands",
                ["./pipex", "infile", "cat", "grep test", "wc -l", "outfile"],
                "< infile cat | grep test | wc -l > outfile_bash"),

        # Error cases
        TestCase("Non-existent input file",
                ["./pipex", "nonexistent", "ls", "wc -l", "outfile"],
                "< nonexistent ls | wc -l > outfile_bash"),

        TestCase("Empty command",
                ["./pipex", "infile", "", "wc -l", "outfile"],
                "< infile \"\" | wc -l > outfile_bash"),

        TestCase("Invalid command",
                ["./pipex", "infile", "invalidcmd", "wc -l", "outfile"],
                "< infile invalidcmd | wc -l > outfile_bash"),

        # Permission tests
        TestCase("No read permission infile",
                ["./pipex", "infile", "cat", "wc -l", "outfile"],
                "< infile cat | wc -l > outfile_bash"),

        TestCase("No write permission outfile",
                ["./pipex", "infile", "cat", "wc -l", "outfile"],
                "< infile cat | wc -l > outfile_bash"),

        # Advanced cases
        TestCase("Absolute path commands",
                ["./pipex", "infile", "/bin/cat", "/usr/bin/wc", "outfile"],
                "< infile /bin/cat | /usr/bin/wc > outfile_bash"),
    ]

def main():
    tests = get_test_cases()
    success = 0
    total = len(tests)

    # Initial cleanup
    cleanup_files()

    for test in tests:
        try:
            if run_test(test):
                success += 1
        except Exception as e:
            print(f"{Colors.RED}Test failed with error: {str(e)}{Colors.RESET}")
        finally:
            # Cleanup after each test, regardless of success/failure
            cleanup_files()

    print(f"\n{Colors.BOLD}Test Summary:{Colors.RESET}")
    print(f"Total tests: {total}")
    print(f"{Colors.GREEN}Passed: {success}{Colors.RESET}")
    print(f"{Colors.RED}Failed: {total - success}{Colors.RESET}")

    # Final cleanup
    cleanup_files()

if __name__ == "__main__":
    main()
