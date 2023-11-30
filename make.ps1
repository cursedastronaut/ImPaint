Invoke-Expression "g++ main.cpp nbgc.cpp menu_files.cpp -o main.exe"
if ($args -contains "-run") {
    if ($LASTEXITCODE -ne 0) { Exit }
    Invoke-Expression "./main.exe"
}