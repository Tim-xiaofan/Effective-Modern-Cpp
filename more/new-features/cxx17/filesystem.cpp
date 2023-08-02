#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

static void exists()
{ // Checking if a file exists
	fs::path filePath = "example.txt";
	if(fs::exists(filePath))
	{
		std::cout << "File exists\n";
	}
	else
	{
		std::cout << "File does not exist.\n";
	}
}

static void iteratingDirectory()
{
	fs::path dirPath = "../";
	if (fs::exists(dirPath) && fs::is_directory(dirPath))
	{
		for (const fs::directory_entry& entry : fs::directory_iterator(dirPath)) 
		{
            std::cout << "path: " << entry.path() << ", file_size: " 
				<< (entry.is_directory() ? 4096: entry.file_size()) << std::endl;
        }
	}
	else
	{
		std::cout << "Directory does not exist or is not a directory.\n";
	}
}

static void copyFile()
{
	fs::path sourcePath = "source.txt";
    fs::path destinationPath = "destination.txt";

    try {
        fs::copy(sourcePath, destinationPath, fs::copy_options::recursive);
        std::cout << "File copied successfully.\n";
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error: " << e.what() << '\n';
    }
}

int main(void)
{
	exists();
	iteratingDirectory();
	copyFile();
	return 0;
}
