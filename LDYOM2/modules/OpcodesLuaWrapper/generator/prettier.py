import subprocess


def format_code(input_code):
    try:
        style_options = "{'ColumnLimit': 120, 'BasedOnStyle': 'llvm'}"

        process = subprocess.Popen(
            ['clang-format', f'-style={style_options}'],
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            universal_newlines=True
        )

        formatted_code, error = process.communicate(input_code)

        if process.returncode != 0:
            raise Exception(f"clang-format failed with error: {error}")

        return formatted_code

    except Exception as e:
        return f"An error occurred: {str(e)}"
