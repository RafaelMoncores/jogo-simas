# Jogo de Plataforma - C++ e SFML

Este projeto é um jogo de plataforma 2D desenvolvido em C++ como parte de um trabalho acadêmico da matéria de Técnicas de Programação com o professor Dr. Jean Marcelo Simão. O jogo utiliza a biblioteca gráfica SFML na sua versão mais recente (3.0+).

## 📋 Pré-requisitos

Para compilar e rodar este projeto, você precisará das seguintes ferramentas instaladas:

* **Compilador C++**: Suporte a **C++17** ou superior.
* **SFML**: Versão **3.0.0** ou superior.
    * *Atenção:* O código não é compatível com SFML 2.x devido a mudanças na API.
* **VS Code**: Editor de código recomendado com a extensão "C/C++" (Microsoft) instalada.

---

## ⚙️ Configuração do Ambiente (VS Code)

Para rodar o projeto pelo VS Code, você deve configurar a pasta `.vscode` de acordo com o seu sistema operacional. Copie os códigos abaixo para os arquivos correspondentes dentro da pasta `.vscode`.

### 🍎 MacOS (Apple Silicon/Intel)

Certifique-se de ter instalado o SFML via Homebrew (`brew install sfml`).

c_cpp_properties.json
```json
{
    "configurations": [
        {
            "name": "Mac",
            "includePath": [
                "${workspaceFolder}/**",
                "/opt/homebrew/opt/sfml/include"
            ],
            "defines": [],
            "macFrameworkPath": [
                "/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/System/Library/Frameworks"
            ],
            "compilerPath": "/usr/bin/clang",
            "cStandard": "c17",
            "cppStandard": "c++17",
            "intelliSenseMode": "macos-clang-arm64"
        }
    ],
    "version": 4
}
```
launch.json
```json
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "(lldb) Launch Jogo (main)",
            "type": "cppdbg",
            "request": "launch",
            "program": "${workspaceFolder}/main",
            "args": [],
            "stopAtEntry": false,
            "cwd": "${workspaceFolder}",
            "environment": [],
            "externalConsole": false,
            "MIMode": "lldb",
            "preLaunchTask": "Build Jogo (*.cpp)" 
        }
    ]
}
```
tasks.json
```json
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "Build Jogo (*.cpp)",
            "type": "shell",
            "command": "clang++",
            "args": [
                "-std=c++17",
                "-g",
                "${workspaceFolder}/code/**/*.cpp",
                "-o",
                "${workspaceFolder}/main",
                "-I/opt/homebrew/opt/sfml/include",
                "-L/opt/homebrew/opt/sfml/lib",
                "-lsfml-graphics",
                "-lsfml-window",
                "-lsfml-system"
            ],
            "group": {
                "kind": "build",
                "isDefault": true
            },
            "problemMatcher": [
                "$gcc"
            ]
        }
    ]
}
```
🪟 Windows (MinGW64)
Atenção: Verifique se os caminhos do SFML (D:/Usuario/...) e do MinGW (C:/mingw64/...) correspondem exatamente à instalação na sua máquina.

c_cpp_properties.json
```json

{
  "configurations": [
    {
      "name": "Win64",
      "includePath": [
        "${workspaceFolder}/**",
        "D:/Usuario/Documents/SFML/SFML-3.0.2/include"
      ],
      "compilerPath": "C:/mingw64/bin/g++.exe",
      "cStandard": "c17",
      "cppStandard": "c++20",
      "intelliSenseMode": "gcc-x64"
    }
  ],
  "version": 4
}
```
launch.json
```json

{
  "version": "0.2.0",
  "configurations": [
    {
      "name": "Debug SFML",
      "type": "cppdbg",
      "request": "launch",
      "program": "${workspaceFolder}/output/projeto_simas.exe",
      "args": [],
      "stopAtEntry": false,
      "cwd": "${workspaceFolder}",
      "externalConsole": true,
      "preLaunchTask": "build with SFML",
      "MIMode": "gdb",
      "miDebuggerPath": "C:/mingw64/bin/gdb.exe"
    }
  ]
}
```
tasks.json
```json

{
  "version": "2.0.0",
  "tasks": [
    {
      "label": "build with SFML",
      "type": "shell",
      "command": "C:/mingw64/bin/g++.exe",
      "args": [
        "-g",
        "-std=c++17",
        "${workspaceFolder}/code/main.cpp",
        "${workspaceFolder}/code/Jogo.cpp",
        "${workspaceFolder}/code/Listas/ListaEntidades.cpp",
        "${workspaceFolder}/code/Gerenciadores/GerenciadorGrafico.cpp",
        "${workspaceFolder}/code/Gerenciadores/GerenciadorColisoes.cpp",
        "${workspaceFolder}/code/Gerenciadores/GerenciadorEventos.cpp",
        "${workspaceFolder}/code/Gerenciadores/GerenciadorRecursos.cpp",
        "${workspaceFolder}/code/Estados/Menu.cpp",
        "${workspaceFolder}/code/Entidades/Ente.cpp",
        "${workspaceFolder}/code/Entidades/Entidade.cpp",
        "${workspaceFolder}/code/Entidades/BolaDeFogo.cpp",
        "${workspaceFolder}/code/Entidades/Personagens/Jogador.cpp",
        "${workspaceFolder}/code/Entidades/Personagens/Personagem.cpp",
        "${workspaceFolder}/code/Entidades/Personagens/Inimigo.cpp",
        "${workspaceFolder}/code/Entidades/Personagens/Gosma.cpp",
        "${workspaceFolder}/code/Entidades/Personagens/Vampiro.cpp",
        "${workspaceFolder}/code/Entidades/Personagens/Dragao.cpp",
        "${workspaceFolder}/code/Entidades/Obstaculos/Obstaculo.cpp",
        "${workspaceFolder}/code/Entidades/Obstaculos/Plataforma.cpp",
        "${workspaceFolder}/code/Entidades/Obstaculos/Trampolim.cpp",
        "${workspaceFolder}/code/Entidades/Obstaculos/Parede.cpp",
        "${workspaceFolder}/code/Fases/Fase.cpp",
        "${workspaceFolder}/code/Fases/FaseUm.cpp",
        "${workspaceFolder}/code/Fases/FaseDois.cpp",

        "-I", "${workspaceFolder}/code",
        "-I", "D:/Usuario/Documents/SFML/SFML-3.0.2/include",
        "-L", "D:/Usuario/Documents/SFML/SFML-3.0.2/lib",
        "-o", "${workspaceFolder}/output/projeto_simas.exe",
        "-lsfml-graphics",
        "-lsfml-window",
        "-lsfml-system"
      ],
      "options": {
        "cwd": "${workspaceFolder}"
      },
      "group": {
        "kind": "build",
        "isDefault": true
      },
      "problemMatcher": ["$gcc"],
      "presentation": {
        "reveal": "always",
        "panel": "shared"
      }
    }
  ]
}
```
📝 settings.json (Comum)
Use este arquivo para configurar as associações de arquivos C++.

```json

{
    "files.associations": {
        "__bit_reference": "cpp",
        "__hash_table": "cpp",
        "__locale": "cpp",
        "__node_handle": "cpp",
        "__split_buffer": "cpp",
        "__verbose_abort": "cpp",
        "array": "cpp",
        "atomic": "cpp",
        "bit": "cpp",
        "cctype": "cpp",
        "charconv": "cpp",
        "chrono": "cpp",
        "clocale": "cpp",
        "cmath": "cpp",
        "codecvt": "cpp",
        "compare": "cpp",
        "concepts": "cpp",
        "condition_variable": "cpp",
        "cstdarg": "cpp",
        "cstddef": "cpp",
        "cstdint": "cpp",
        "cstdio": "cpp",
        "cstdlib": "cpp",
        "cstring": "cpp",
        "ctime": "cpp",
        "cwchar": "cpp",
        "cwctype": "cpp",
        "deque": "cpp",
        "exception": "cpp",
        "algorithm": "cpp",
        "functional": "cpp",
        "iterator": "cpp",
        "list": "cpp",
        "map": "cpp",
        "memory": "cpp",
        "memory_resource": "cpp",
        "numeric": "cpp",
        "optional": "cpp",
        "random": "cpp",
        "ratio": "cpp",
        "string": "cpp",
        "string_view": "cpp",
        "system_error": "cpp",
        "tuple": "cpp",
        "type_traits": "cpp",
        "unordered_map": "cpp",
        "unordered_set": "cpp",
        "utility": "cpp",
        "vector": "cpp",
        "format": "cpp",
        "initializer_list": "cpp",
        "iomanip": "cpp",
        "ios": "cpp",
        "iosfwd": "cpp",
        "iostream": "cpp",
        "istream": "cpp",
        "limits": "cpp",
        "locale": "cpp",
        "mutex": "cpp",
        "new": "cpp",
        "numbers": "cpp",
        "ostream": "cpp",
        "semaphore": "cpp",
        "set": "cpp",
        "span": "cpp",
        "sstream": "cpp",
        "stdexcept": "cpp",
        "stop_token": "cpp",
        "streambuf": "cpp",
        "text_encoding": "cpp",
        "thread": "cpp",
        "typeinfo": "cpp",
        "variant": "cpp",
        "fstream": "cpp",
        "execution": "cpp"
    }
}
```
