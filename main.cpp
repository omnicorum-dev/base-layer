#include <filesystem>
#include <__filesystem/filesystem_error.h>

#include "base.h"

using namespace omni;
namespace fs = std::filesystem;
using std::string;

fs::path find_tasks_directory(const fs::path& currentPath, const int depth = 0) {
    if (depth > 4) {
        return {};
    }

    fs::path tasksPath = currentPath / "TASKS";

    if (fs::exists(tasksPath)) {
        return tasksPath;
    }

    return find_tasks_directory(currentPath.parent_path(), depth + 1);
}

void init_tasks_folder() {
    const fs::path tasksPath = fs::current_path() / "TASKS";

    if (fs::exists(tasksPath)) {
        LOG_WARN("TASKS folder already exists in current directory. Ignoring");
        return;
    }

    try {
        fs::create_directory(tasksPath);
    } catch (const fs::filesystem_error& e) {
        LOG_ERROR("Error creating tasks folder: {}", e.what());
    }
}

int main(int argc, char* argv[]) {

    if (argc < 2) {
        println("Usage: tama [init] | [find]");
        return 1;
    }

    if (argv[1] == string("init")) {
        println("Initializing tasks folder...");
        init_tasks_folder();
    }

    if (argv[1] == string("find")) {
        println("Finding tasks folder...");
        const fs::path tasksPath = find_tasks_directory(fs::current_path());
        if (tasksPath.empty()) {
            LOG_ERROR("Tasks folder not found.");
        } else {
            println("Tasks folder: {}", tasksPath);
        }
    }

    return 0;
}