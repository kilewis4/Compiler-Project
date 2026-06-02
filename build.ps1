# PowerShell build script for the UNN Compiler
# This script compiles the C source files

param(
    [string]$Target = "build",
    [switch]$Clean,
    [switch]$Run
)

$ProjectRoot = Split-Path -Parent $MyInvocation.MyCommand.Definition
$SrcDir = Join-Path $ProjectRoot "src"
$IncludeDir = Join-Path $ProjectRoot "include"
$BuildDir = Join-Path $ProjectRoot "build"

# Check for GCC
$GCC = Get-Command gcc -ErrorAction SilentlyContinue
if (-not $GCC) {
    Write-Host "ERROR: GCC compiler not found in PATH" -ForegroundColor Red
    Write-Host ""
    Write-Host "To compile this C project, you need to install MinGW or another C compiler:"
    Write-Host ""
    Write-Host "Option 1: MinGW (Recommended)"
    Write-Host "  - Download from: https://www.mingw-w64.org/"
    Write-Host "  - Add MinGW bin folder to PATH"
    Write-Host ""
    Write-Host "Option 2: TDM-GCC"
    Write-Host "  - Download from: https://jmeubank.github.io/tdm-gcc/"
    Write-Host ""
    Write-Host "Option 3: Install 'make' and use the Makefile:"
    Write-Host "  - Use Chocolatey: choco install make"
    Write-Host "  - Or download from: http://gnuwin32.sourceforge.net/packages/make.htm"
    Write-Host ""
    exit 1
}

Write-Host "Found GCC: $($GCC.Source)" -ForegroundColor Green

# Clean
if ($Clean) {
    Write-Host "Cleaning build directory..." -ForegroundColor Cyan
    if (Test-Path $BuildDir) {
        Remove-Item -Path $BuildDir -Recurse -Force
    }
    Write-Host "Clean complete"
}

# Create build directory
if (-not (Test-Path $BuildDir)) {
    New-Item -ItemType Directory -Path $BuildDir | Out-Null
}

# Compile
Write-Host "Compiling..." -ForegroundColor Cyan
$SourceFiles = @(
    "src/token.c"
    "src/lexer.c"
    "src/parser.c"
    "src/main.c"
)

$CompileCmd = @(
    "gcc"
    "-Wall"
    "-Wextra"
    "-I", "include"
) + $SourceFiles + @(
    "-o", "build/compiler"
)

& $CompileCmd

if ($LASTEXITCODE -eq 0) {
    Write-Host "✓ Build successful!" -ForegroundColor Green
    Write-Host "  Output: build/compiler.exe"
} else {
    Write-Host "✗ Build failed!" -ForegroundColor Red
    exit 1
}

# Run
if ($Run) {
    Write-Host ""
    Write-Host "Running compiler..." -ForegroundColor Cyan
    & "build/compiler" test.unn
}
