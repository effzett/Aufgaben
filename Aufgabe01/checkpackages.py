#!/Users/internet/anaconda3/bin/python3
import re
import os

folder_name = os.path.basename(os.getcwd())[:10]
filename = folder_name + "_packages.txt"


# Pfade zu den Dateien
latex_file = folder_name + ".tex"
dep_file = folder_name + ".dep"


# Pakete aus der LaTeX-Datei extrahieren
def extract_latex_packages(file_path):
    with open(file_path, "r") as f:
        content = f.read()
    # Suche nach \usepackage-Anweisungen
    packages = re.findall(r"\\usepackage(?:\[[^\]]*\])?\{([^\}]+)\}", content)
    return set(pkg.strip() for pkg_list in packages for pkg in pkg_list.split(","))

# Pakete aus der .dep-Datei extrahieren
def extract_dep_packages(file_path):
    with open(file_path, "r") as f:
        content = f.read()
    # Suche nach Paketen mit *{package}{...}
    packages = re.findall(r"\*\{package\}\{([^\}]+)\}", content)
    return set(packages)

# Pakete vergleichen
def compare_packages(latex_file, dep_file):
    latex_packages = extract_latex_packages(latex_file)
    dep_packages = extract_dep_packages(dep_file)

    # Unbenutzte und tatsächlich verwendete Pakete
    unused_packages = latex_packages - dep_packages
    used_packages = latex_packages & dep_packages

# Datei im aktuellen Verzeichnis schreiben
    with open(filename, "w") as f:

        print("\nVerwendete Pakete:",file=f)
        for pkg in sorted(used_packages):
            print(f"- {pkg}",file=f)

        print("\nNicht verwendete Pakete:",file=f)
        for pkg in sorted(unused_packages):
            print(f"- {pkg}",file=f)

# Hauptprogramm
if __name__ == "__main__":
    try:
        compare_packages(latex_file, dep_file)
    except FileNotFoundError as e:
        print(f"Fehler: {e}")
