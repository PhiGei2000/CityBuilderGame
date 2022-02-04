$currentDir = Get-Location

$sourceDir = Join-Path -Path $currentDir -ChildPath "src"

$sourceFiles = Get-ChildItem $sourceDir -Recurse -Filter *.cpp

$output = 'SET(SOURCES ${SOURCES}'+ "`n"

foreach ($file in $sourceFiles) {
    $relativePath = "." + $file.FullName.Replace($currentDir, "").Replace("\", "/")

    $output += $relativePath + "`n"
}

$output += ")"

[System.IO.File]::WriteAllText("./sourcelist.cmake", $output)
