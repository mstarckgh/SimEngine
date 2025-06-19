# SimEngine

SimEngine ist eine einfache C++-Engine mit OpenGL-basierter Grafik und einer eigenen, minimalen Physik-Implementierung. Die Engine nutzt GLFW zur Fensterein- und -ausgabe, GLAD zum Laden der OpenGL-Funktionen sowie GLM und stb_image.

## Build

Zum Kompilieren des Projekts wird CMake verwendet. Die notwendigen Abhängigkeiten sind bereits im Repository enthalten.

```bash
cmake -S . -B build
cmake --build build
```

## Abhängigkeiten

Die folgenden Bibliotheken liegen bereits im Projekt bei und müssen nicht separat installiert werden:

- GLFW
- GLAD
- GLM
- stb_image

## Programmstart

Nach erfolgreichem Build befindet sich die ausführbare Datei im `build`-Verzeichnis. Das Programm kann dort direkt gestartet werden:

```bash
./build/SimEngine
```

