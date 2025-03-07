# apply cmake-format
find /workspaces/molviz -type f -name "*.cmake" -not -path
"/workspaces/molviz/out/*" -exec cmake-format -i {} \;
find /workspaces/molviz -type f -name "CMakeLists.txt" -exec cmake-format -i {} \;

# apply clang-format
find /workspaces/molviz -type f -name "*.*pp" -not -path "/workspaces/molviz/out/*" -exec clang-format -i {} \;