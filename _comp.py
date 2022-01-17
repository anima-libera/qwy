""" Python script _comp.py - Call g++ to compile the project """


#### OPTIONS

# Binary output options
bin_output_name = "Qwy"
executable = True

# Compiling options
debug_macro = False
warnings = True
execute_after_compiling = False

# Directories options
bin_directory = "bin"
src_directory = "src"

# Link options
lib_links = []
sfml = {
	"sfml-system":   True,
	"sfml-window":   True,
	"sfml-graphics": True,
	"sfml-audio":    False,
	"sfml-network":  False,
}
opengl = True
experimental_filesystem = False


#### PYTHON COMMAND LINE OPTIONS

# Check command line of the call of this
import sys
for arg in sys.argv[1:]:
	if arg in ("debug", "dbg", "d", "-debug", "-dgb", "-d"):
		debug_macro = True


#### COMPILING SCRIPT

# Compute some file path things
bin_file = bin_output_name
bin_file_path = bin_directory+"/"+bin_file

# Compute the compiler options list compiler_options
compiler_options = []
if executable:
	compiler_options.append("-no-pie")
if warnings:
	compiler_options.append("-Wextra -Wall")
if debug_macro:
	compiler_options.append("-DDEBUG")

# Import os for system interaction and file linsting
import os

# List source file paths in src_file_paths
src_file_paths = []
for current_dir, dirs, files in os.walk(src_directory):
	for file in files:
		if file.split(".")[-1] == "cpp":
			src_file_paths.append(current_dir+"/"+file)

# Create compile command parts
str_src_input_files = " ".join(src_file_paths)
str_bin_output_file = "-o "+bin_file_path
str_compiler_options = " ".join(compiler_options)
sfml_modules = [sfml_module for sfml_module in sfml.keys() if sfml[sfml_module]]
str_sfml = " ".join(["-l"+lib for lib in sfml_modules])
if opengl: lib_links.extend(["GL", "GLEW"])
if experimental_filesystem: lib_links.append("stdc++fs")
str_links = " ".join(["-l"+lib for lib in lib_links])

# Compute the compile command and execute it
command = " ".join((
	"g++", "-std=c++17",
	str_src_input_files,
	str_bin_output_file,
	str_compiler_options,
	str_sfml,
	str_links,
))
print(command)
os.system(command)


#### EXECUTE SCRIPT

# Execute the binary output if asked
if execute_after_compiling:
	os.system("./"+bin_file_path)
